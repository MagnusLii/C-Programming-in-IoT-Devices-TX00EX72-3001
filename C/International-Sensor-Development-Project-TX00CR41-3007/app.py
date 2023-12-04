from flask import Flask, request, jsonify
from flask_mqtt import Mqtt
import json

app = Flask(__name__)
app.config["MQTT_BROKER_URL"] = "localhost" # Replace with broker IP
#app.config['MQTT_USERNAME'] = ""  # Will be set once we start using authentication
#app.config['MQTT_PASSWORD'] = ""
app.config["MQTT_BROKER_PORT"] = 1883
app.config["MQTT_KEEPALIVE"] = 60
app.config["MQTT_TLS_ENABLED"] = False

# Topics
registration_topic = "/registration"
vote_topic = "/vote"
vote_setup_topic = "/setupVote/setup"
vote_resync_topic = "/setupVote/resync"
all_topics = [registration_topic, vote_topic, vote_setup_topic, vote_resync_topic]

mqtt = Mqtt(app)

# Subscribe to all topics in 'all_topics' list.
@mqtt.on_connect()
def handle_connect(client, userdata, flags, rc):
   if rc == 0:
       print('Connected successfully')
       for topic in all_topics:
           mqtt.subscribe(all_topics)  # subscribe to each topic
           print(f'Subscribed to {topic}')
   else:
       print(f'Connection failed. Code: {rc}')

# Sort incoming messages
@mqtt.on_message()
def handle_message(client, userdata, message):
    received_message = message.payload.decode("utf-8")
    print(f'Received message: {received_message}, on topic: {message.topic}')

    if message.topic == registration_topic:
        print(f'Received registration message: {received_message}')
    elif message.topic == vote_topic:
        print(f'Received vote message: {received_message}')
    elif message.topic == vote_setup_topic:
        print(f'Received vote setup message: {received_message}')
    elif message.topic == vote_resync_topic:
        print(f'Received vote resync message: {received_message}')
    else:
        print(f'Received message on unknown topic: {received_message}')


@app.route("/")
def index():
    return "Flask MQTT Server is running!"


if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000)
