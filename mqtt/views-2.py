#This incorporates
# This incorporated mqtt communications
# Needs devices table in models.py
#

from django.shortcuts import render

import paho.mqtt.client as mqtt

from .models import devices

# Create your views here.
def index(request):
      #If no record exists in data base, create a record.  This happens only the first time
      # Note:  It is assumed that you have only one device.  If you have more than one device, this code has to be modified.  You will have to do that yourself after completing this course

      try:
         devices_obj = devices.objects.get(device_id=1)
      except:
         devices_obj = devices()
         devices_obj.device_id = 1
         devices_obj.p0_status = "OFF"
         devices_obj.relay_status = "OFF"
         devices_obj.led_status = "OFF"
         devices_obj.save()

      p0_status = devices_obj.p0_status
      relay_status = devices_obj.relay_status
      led_status = devices_obj.led_status

      p0_status_new = request.GET.get("p0_status")
      relay_status_new = request.GET.get("relay_status")
      led_status_new = request.GET.get("led_status")
      buzzer = request.GET.get("buzzer")

      message = ''
      msg = ""
      if p0_status_new == "ON":
         msg = "P0:ON"
      elif p0_status_new == "OFF":
         msg = "P0:OFF"
      if relay_status_new == "ON":
         msg = "RELAY:ON"
      elif relay_status_new == "OFF":
         msg = "RELAY:OFF"
      if led_status_new == "OFF":
         msg = "LED:OFF"
      elif led_status_new == "RED":
         msg = "LED:RED"
      elif led_status_new == "GREEN":
         msg = "LED:GREEN"
      elif led_status_new == "BLUE":
         msg = "LED:BLUE"
      if buzzer == "BUZZER":
         msg = "BUZZER:ON"

      sensors = device_send_rec_msg(msg)
      if sensors:
         message = 'Success'
         if p0_status_new:
            p0_status = p0_status_new
            devices_obj.p0_status = p0_status
         if relay_status_new:
            relay_status = relay_status_new
            devices_obj.relay_status = relay_status
         if led_status_new:
            led_status = led_status_new
            devices_obj.led_status = led_status
         devices_obj.save()

      else:
         message = 'Fail'

      context = {
         'p0_status': p0_status,
         'relay_status': relay_status,
         'led_status': led_status,
         'sensors': sensors,
         'message': message,
      }
      return render(request, 'myapp/ports_status.html', context)


def device_send_rec_msg(msg):
      broker_address = "127.0.0.1"
      mqtt_username = "raman"
      mqtt_password = "Test@2021"
      client = mqtt.Client("SERVER")
      client.username_pw_set(mqtt_username, mqtt_password) # Sets the username and password
      client.on_message=on_message #attach function to callback
      client.on_publish = on_publish
      client.connect(broker_address) #connect to broker
      client.subscribe('SENSORS')  # All sensor value
      global published
      published = None
      global rec_msg
      rec_msg = None
      client.publish("DEVICE", msg)
      count = 50
      while count > 0:
         client.loop(0.1) # This wait for 0.1 seconds
         count -= 1
         if published:
            break
      if published:
         count = 50 # Wait a maximum of 5 seconds.  1 count is 0.1 seconds
         while count > 0:
            client.loop(0.1) # Waits within the loop for 0.1 seconds
            count -= 1
            if rec_msg:
               break
      client.loop_stop()
      client.disconnect()
      return rec_msg

def on_message(client, userdata, message):
    global rec_msg
    rec_msg = message.payload.decode("utf-8")

def on_publish(client, userdata, message):
   global published
   published = True

