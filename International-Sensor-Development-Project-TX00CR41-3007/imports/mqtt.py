from flask import Flask
from flask_mqtt import Mqtt
import json


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