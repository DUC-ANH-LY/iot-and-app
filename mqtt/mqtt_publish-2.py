#!/usr/bin/env python3
# Modified to timeout on publishing
#
import paho.mqtt.client as mqtt

def main_fn():
   msg = "OFF"
   broker_address = "127.0.0.1"
   mqtt_username = "raman"
   mqtt_password = "Test@2021"
   client = mqtt.Client("CLIENT1")
   client.username_pw_set(mqtt_username, mqtt_password) # Sets the username and password
   print("connecting to broker")
   client.on_publish = on_publish
   client.connect(broker_address) #connect to broker
   print("Publishing message to topic", "RELAY")
   global published
   published = False
   client.publish("RELAY", msg)
   count = 50  # Wait for 5 seconds
   while count > 0:
      client.loop(0.1)  # This loops within it for 0.1 seconds
      count -= 1
      if published:
         break
   if published:
      print("Published")
   else:
      print("Failed")
   #time.sleep(4) # wait
   client.loop_stop()
   client.disconnect()


def on_publish(client, userdata, message):
   global published
   published = True




if __name__ == '__main__':
  main_fn()

