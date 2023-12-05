# Light flask server.

This is a light version of the flask server made for the ESP team to enable testing.

# Requirements.

The server requires all of the following installed on the machine running the server.
>-Python 3.x (minimum 3.8+)
>-Flask module  (https://flask.palletsprojects.com/en/3.0.x/installation/)
>-Flask-mqtt module  (https://pypi.org/project/Flask-MQTT/)
>-Mosquitto MQTT broker

## Features.

The server includes a light version of the ESP registration process and a template for custom tests using the **/test/server/receive_message** and **/test/est/send_message topics**.

## Registration process flowchart.
```mermaid
graph LR
A[ESP] -- ESP registration message --> B(/registration/Server/ESP-Mac-address)
A --> C(/registration/esp/UniqueID)
B --> D[SERVER]
C -- VotingID response --> D
```