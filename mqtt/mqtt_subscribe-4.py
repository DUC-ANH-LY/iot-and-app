#!/usr/bin/env python3

import paho.mqtt.client as mqtt
import time

def mqtt_subscribe_fn():
   broker_address = "127.0.0.1"
   mqtt_username = "raman"
   mqtt_password = "Test@2021"
   client = mqtt.Client("DEVICE")
   client.username_pw_set(mqtt_username, mqtt_password) # Sets the username and password
   print("creating new instance")
   client.on_message=on_message #attach function to callback
   print("connecting to broker")
   client.connect(broker_address) #connect to broker
   print("Subscribing to topic", "DEVICE")
   client.subscribe("DEVICE")
   client.loop_forever()


def on_message(client, userdata, message):
    print("message received")
    rec_msg = message.payload.decode("utf-8")
    print(message.topic, ': ', rec_msg)
    sensors = "Motion: , Temperature: , Humidity: "
    client.publish("SENSORS", sensors)

if __name__ == '__main__':
  mqtt_subscribe_fn()

