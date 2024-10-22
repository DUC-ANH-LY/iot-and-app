#!/usr/bin/env python3
# Modified to timeout on publishing
#
import paho.mqtt.client as mqtt

def main_fn():
   while True:
      msg = None
      while not msg:
         msg = input("Enter command: ")
      msg = msg.upper()
      rcvd = send_msg(msg)
      if rcvd:
         print(rcvd)
      else:
         print("Fail")


def send_msg(msg):
   broker_address = "127.0.0.1"
   mqtt_username = "raman"
   mqtt_password = "Test@2021"
   client = mqtt.Client("CLIENT1")
   client.username_pw_set(mqtt_username, mqtt_password) # Sets the username and password
   client.on_message=on_message #attach function to callback
   client.on_publish = on_publish
   print("connecting to broker")
   client.connect(broker_address) #connect to broker
   client.subscribe('RESPONSE')
   print("Publishing message to topic", "RELAY")
   global published
   published = False
   global rec_msg
   rec_msg = None
   client.publish("RELAY", msg)
   count = 50  # Wait for 5 seconds
   while count > 0:
      client.loop(0.1)  # This loops within it for 0.1 seconds
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


def on_publish(client, userdata, message):
   global published
   published = True

def on_message(client, userdata, message):
    global rec_msg
    rec_msg = message.payload.decode("utf-8")


if __name__ == '__main__':
  main_fn()

