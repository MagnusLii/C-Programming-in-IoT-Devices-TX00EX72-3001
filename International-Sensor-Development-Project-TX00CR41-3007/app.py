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


# MQTT message handling
@mqttImports.mqtt.on_message()
def handle_message(client, userdata, message):
    receivedMessage = message.payload.decode("utf-8")
    receivedTopic = message.topic
    print(f'Received message: {receivedMessage} on topic: {receivedTopic}')

    # Decode JSON message.
    decodedMessage = mqttImports.decodeStringToJSON(receivedMessage)
    if decodedMessage == -1:
        print('JSON decode failed.')
        return # TODO: maybe add something to notify ESPs about failed JSON decode.

    # Handle received message based on topic.
    if receivedTopic.startswith("/registration/Server/") == True:
        for i in range(len(esp.Esp.regiseredESPs)):
            if esp.Esp.regiseredESPs[i].macAddress == decodedMessage['Mac']:
                print(f'{decodedMessage["Mac"]} ESP already registered.')
                return
            else:
                esp.Esp(decodedMessage['Mac'])
                print(f'{decodedMessage["Mac"]} ESP registered.')
                pass
    



    print()
    print()
    return

@app.route('/getRegisteredESPs', methods=['GET'])
def getRegisteredESPs():
    return jsonify([esp.Esp.regiseredESPs[i].macAddress for i in range(len(esp.Esp.regiseredESPs))])


if __name__ == '__main__':

    globalVoteInformation = voteHandling.VoteInformation()  # Global vote information object.

    # Initialize imported app extensions.
    dbFunctions.db.init_app(app)
    mqttImports.mqtt.init_app(app)

    app.run(host='0.0.0.0', port=5000, use_reloader=False)