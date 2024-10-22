#!/usr/bin/env python3

import paho.mqtt.client as mqtt
import time


def mqtt_subscribe_fn():
    broker_address = "192.168.195.137:1883"
    mqtt_username = "ducanh"
    mqtt_password = "ducanh172"
    client = mqtt.Client(mqtt.CallbackAPIVersion.VERSION1, "CLIENT2")
    client.username_pw_set(
        mqtt_username, mqtt_password
    )  # Sets the username and password
    print("creating new instance")
    client.on_message = on_message  # attach function to callback
    print("connecting to broker")
    client.connect(broker_address)  # connect to broker
    print("Subscribing to topic", "RELAY")
    client.subscribe("RELAY")
    client.loop_forever()


def on_message(client, userdata, message):
    print("message received")
    rec_msg = message.payload.decode("utf-8")
    print(message.topic, ": ", rec_msg)


if __name__ == "__main__":
    mqtt_subscribe_fn()
