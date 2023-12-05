from flask import Flask, request, jsonify
from flask_mqtt import Mqtt
import json

app = Flask(__name__)
app.config["MQTT_BROKER_URL"] = "localhost" # Replace with broker IP if not running locally.
app.config["MQTT_BROKER_PORT"] = 1883
app.config["MQTT_KEEPALIVE"] = 10
app.config["MQTT_TLS_ENABLED"] = False


# Topics
registration_topic = "/registration"
test_topic = "/test"
all_topics = [registration_topic, test_topic] # List of all topics to subscribe to.

# Defines
sender = "Server"

mqtt = Mqtt(app)

# Subscribe to all topics in 'all_topics' list when server is started.
@mqtt.on_connect()
def handle_connect(client, userdata, flags, rc):
   if rc == 0:
       print('Connected successfully')
       for topic in all_topics:
           mqtt.subscribe(topic, qos=1)  # subscribe to each topic
           print(f'Subscribed to {topic}')
   else:
       print(f'Connection failed. Code: {rc}')

# Sort incoming messages.
# As we'll have randomly generated topics later on, we'll have handle_message() to sort the messages and send them to functions that handle messages from specific topics.
@mqtt.on_message()
def handle_message(client, userdata, message):
    received_message = message.payload.decode("utf-8")

    print(f'\n\nReceived message on topic {message.topic}: {received_message}')

    # Decode JSON
    decoded_message = decode_json(received_message)


    # Veriify that the message is a JSON object.
    if not test_json(decoded_message):
        print(f'JSON test failed. Message: {received_message}')
        return
    
    # Ignore messages sent by the Server.
    if decoded_message.get('Sender') == 'Server':
        print('Message sent by Server, ignoring.')
        return

    # Example message handling for the registration topic.
    if message.topic == registration_topic:
        registration_message_handler(decoded_message)

    # Simple test topic.
    elif message.topic == test_topic:
        test_message_handler(decoded_message)


    # Message on unknown topic.        
    else:
        print(f'Received message on not handeled topic: {received_message}')


@app.route("/")
def index():
    return 'Flask MQTT Server is running!'


def decode_json(json_string):
    try:
        decoded_message = json.loads(json_string)
        print('JSON test passed.')
        return decoded_message
    except json.decoder.JSONDecodeError as error:
        print(f'JSON test failed. Error: {error}')
        #return f'Error: Invalid JSON format - {error}'
        return


# Test for if the decoded message is a JSON object, intended for use with the decode_json() function.
def test_json(decoded_message):
    if isinstance(decoded_message, str) and decoded_message.startswith('Error: Invalid JSON format'):
        return False
    else:
        return True


# Check if all keywords are present in the JSON keys.
def check_keywords_in_json(decoded_json, keyword_list):
    # Convert the JSON keys to a set for faster keyword checking.
    json_keys_set = set(decoded_json.keys())

    # Check if all keywords are present in the JSON keys.
    for keyword in keyword_list:
        if keyword not in json_keys_set:
            return False
    
    return True


# Handle messages in the registration topic.
def registration_message_handler(decoded_message):

    if check_keywords_in_json(decoded_message, ["UniqueID"]):
        uniqueID = "123ASD456" # Set whatever ID here for test purposes.
        return_message = f'{{"UniqueID": "{uniqueID}", "Sender": "{sender}"}}'
        mqtt.publish(registration_topic, return_message, qos=1)
    else:
        print('Something went wrong.')
    return


# Handle messages in the test topic.
def test_message_handler(decoded_message):
    for key, value in decoded_message.items():
        print(f"Key: {key}, Value: {value}")
    
    '''
    If you wish to send a message back to the topic.

    message = "Your message here" # Message needs Sender key if in JSON format.
    mqtt.publish(test_topic, f"{message}", qos=1)
    '''
    return

if __name__ == "__main__":
    app.run(host='0.0.0.0', port=5000, use_reloader=False)
