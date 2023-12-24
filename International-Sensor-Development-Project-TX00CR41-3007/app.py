from flask import Flask, jsonify
from flask_mqtt import Mqtt
import json
import random
from flask_sqlalchemy import SQLAlchemy
from datetime import datetime
from isdProjectImports import credentials
from isdProjectImports import mqttImports
from isdProjectImports import dbFunctions
from isdProjectImports import voteHandling
from isdProjectImports import esp

mqttBrokerPort = 1883
mqttKeepAliveSec = 10
mqttBrokerIP = 'localhost'  # Replace with broker IP if not running locally.
mqttQoSLevel = 1


# Flask app setup.
app = Flask(__name__)

# MQTT setup.
app.config['MQTT_BROKER_URL'] = mqttBrokerIP
app.config['MQTT_BROKER_PORT'] = mqttBrokerPort
app.config['MQTT_KEEPALIVE'] = mqttKeepAliveSec
app.config['MQTT_TLS_ENABLED'] = False

# Database setup.
app.config['SQLALCHEMY_DATABASE_URI'] = f'mysql+pymysql://{credentials.dbUsername}:{credentials.dbPassword}@{credentials.dbHostname}:{credentials.dbPort}/{credentials.dbName}'
app.config['SQLALCHEMY_TRACK_MODIFICATIONS'] = False

@app.route('/')
def index():
    return 'Flask MQTT Server is running!'

# Subscribe to all topics in 'initialSubscribeTopics' list when server is started.
@mqttImports.mqtt.on_connect()
def handle_connect(client, userdata, flags, rc):
   if rc == 0:
       for topic in mqttImports.initialSubscribeTopics:
           mqttImports.mqtt.subscribe(topic, qos=1)  # subscribe to each topic
   else:
       print(f'Connection failed. Code: {rc}')


@mqttImports.mqtt.on_message()
def handle_message(client, userdata, message):
    received_message = message.payload.decode("utf-8")
    received_topic = message.topic
    print(f'Received message: {received_message} on topic: {received_topic}')
    return


if __name__ == '__main__':

    globalVoteInformation = voteHandling.VoteInformation()  # Global vote information object.

    # Initialize imported app extensions.
    dbFunctions.db.init_app(app)
    mqttImports.mqtt.init_app(app)

    app.run(host='0.0.0.0', port=5000, use_reloader=False)