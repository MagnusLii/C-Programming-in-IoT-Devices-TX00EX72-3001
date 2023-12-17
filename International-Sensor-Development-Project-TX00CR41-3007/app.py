from flask import Flask, jsonify
from flask_mqtt import Mqtt
import json
import random
from flask_sqlalchemy import SQLAlchemy
from datetime import datetime
import credentials

mqttBrokerPort = 1883
mqttKeepAliveSec = 10
mqttBrokerIP = 'localhost'  # Replace with broker IP if not running locally.
mqttQoSLevel = 1

# Topics
# Registration topics
registrationIncomingTopic = '/registration/Server/#' # + mac address, ESPs will start registration with this topic.
registrationResponeTopic = '/registration/esp/'  # + mac address, server will respond to ESPs with this topic.
# VoteSetup topics
voteSetupTopic = '/setupVote/Setup'  # Vote information is posted here.
voteResyncTopic = '/setupVote/Resync'  # ESPs will request resync with this topic.
# Vote topics
voteIncomingTopic = '/vote/'  # + votingID, ESPs will send votes to this topic.

# List of all topics to subscribe on server boot.
initialSubscribeTopics = [registrationIncomingTopic, voteResyncTopic]  # List of all topics to subscribe to.

# Flask app setup.
app = Flask(__name__)
app.config['MQTT_BROKER_URL'] = mqttBrokerIP
app.config['MQTT_BROKER_PORT'] = mqttBrokerPort
app.config['MQTT_KEEPALIVE'] = mqttKeepAliveSec
app.config['MQTT_TLS_ENABLED'] = False
app.config['SQLALCHEMY_DATABASE_URI'] = f'mysql+pymysql://{credentials.dbUsername}:{credentials.dbPassword}@{credentials.dbHostname}:{credentials.dbPort}/{credentials.dbName}'
app.config['SQLALCHEMY_TRACK_MODIFICATIONS'] = False

db = SQLAlchemy(app)
mqtt = Mqtt(app)
espList = []  # List of all ESPs that have been registered.

class Esp:

    registredESPs = 0

    def __init__(self, mac_address, registeredUser='NULL',):
        self.mac_address = mac_address
        self.registeredUser = registeredUser

        self.uniqueID = random.randint(1, 10)  # Needs to be changed later.
        self.voteStatus = 'pass'  # default value is pass.

        self.registration_confirmation_topic = registrationResponeTopic + self.mac_address


        Esp.registredESPs += 1

# Database models
class ESPDevice(db.Model):
    DeviceID = db.Column(db.String(17), primary_key=True)
    RegistrationTime = db.Column(db.TIMESTAMP, default=datetime.utcnow)
    LastActiveTime = db.Column(db.TIMESTAMP)

class User(db.Model):
    UserID = db.Column(db.Integer, primary_key=True, autoincrement=True)
    Username = db.Column(db.Text, unique=True)
    DeviceID = db.Column(db.String(17))
    RegistrationDate = db.Column(db.TIMESTAMP, default=datetime.utcnow)

class Topic(db.Model):
    TopicID = db.Column(db.Integer, primary_key=True, autoincrement=True)
    Title = db.Column(db.Text, nullable=False)
    Description = db.Column(db.Text)
    StartTime = db.Column(db.TIMESTAMP)
    EndTime = db.Column(db.TIMESTAMP)

class Vote(db.Model):
    VoteID = db.Column(db.Integer, primary_key=True, autoincrement=True)
    UserID = db.Column(db.Integer, db.ForeignKey('user.UserID'), nullable=False)
    VoteType = db.Column(db.Text, nullable=False)
    TopicID = db.Column(db.Integer, db.ForeignKey('topic.TopicID'), nullable=False)
    VoteTime = db.Column(db.TIMESTAMP, default=datetime.utcnow)


# Subscribe to all topics in 'initialSubscribeTopics' list when server is started.
@mqtt.on_connect()
def handle_connect(client, userdata, flags, rc):
   if rc == 0:
       for topic in initialSubscribeTopics:
           mqtt.subscribe(topic, qos=1)  # subscribe to each topic
   else:
       print(f'Connection failed. Code: {rc}')


@mqtt.on_message()
def handle_message(client, userdata, message):
    return


@app.route('/')
def index():
    return 'Flask MQTT Server is running!'


def decodeStringToJSON(json_string):
    try:
        decodedMessage = json.loads(json_string)
        print('JSON test passed.')
        return decodedMessage
    except json.decoder.JSONDecodeError as error:
        print(f'JSON test failed. Error: {error}')
        return -1


def validateKeywordsInJSON(decodedJSON, keywordList, verifycationLevel):
    jsonKeySet = set(decodedJSON.keys())

    if verifycationLevel == 1:
        for keyword in keywordList:
            if keyword not in jsonKeySet:
                return False
        return True

    elif verifycationLevel == 2:
        for keyword in keywordList:
            if keyword not in jsonKeySet or not decodedJSON[keyword] or decodedJSON[keyword] == "NULL":
                return False
        return True

    else:
        raise ValueError("Invalid verification level. Please provide either 1 or 2.")


def publishJSONtoMQTT(topic, message):
    try:
        mqtt.publish(topic, message, qos=mqttQoSLevel)
    except:
        print(f'Failed to publish to topic: {topic}')
        return False
    else:
        return True

def create_tables():
    db.create_all()

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000, use_reloader=False)