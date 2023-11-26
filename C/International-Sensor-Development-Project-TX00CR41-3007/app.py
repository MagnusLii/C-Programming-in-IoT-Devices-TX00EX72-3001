from flask import Flask, request, jsonify
from flask_mqtt import Mqtt
import json

app = Flask(__name__)
app.config["MQTT_BROKER_URL"] = "192.168.1.118"  # Replace with broker IP
#app.config['MQTT_USERNAME'] = ""  # Will be set once we start using authentication
#app.config['MQTT_PASSWORD'] = ""
app.config["MQTT_BROKER_PORT"] = 1883
app.config["MQTT_KEEPALIVE"] = 60
app.config["MQTT_TLS_ENABLED"] = False
topic = "/test/topic"  # Replace with topic you want to subscribe to
mqtt = Mqtt(app)

@mqtt.on_connect()
def handle_connect(client, userdata, flags, rc):
   if rc == 0:
       print('Connected successfully')
       mqtt.subscribe(topic) # subscribe topic
   else:
       print('Bad connection. Code:', rc)


@mqtt.on_connect()
def handle_connect(client, userdata, flags, rc):
    mqtt.subscribe(topic)  # Subscribe to the topic
    print("Connected to MQTT broker")


@mqtt.on_message()
def handle_message(client, userdata, message):
    received_message = message.payload.decode("utf-8")
    print("Received message:", received_message)
    
    # Process the received message here
    mqtt.publish(topic, f'MQTT message received:"{received_message}".')  # Publish a response message


@app.route("/")
def index():
    return "Flask MQTT Server is running!"


if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000)
