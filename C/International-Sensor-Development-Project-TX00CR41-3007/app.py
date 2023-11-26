import eventlet
import json
from flask import Flask, render_template
from flask_mqtt import Mqtt
from flask_socketio import SocketIO
from flask_bootstrap import Bootstrap

eventlet.monkey_patch()

app = Flask(__name__)
app.config['SECRET'] = 'my secret key'
app.config['TEMPLATES_AUTO_RELOAD'] = True
app.config['MQTT_BROKER_URL'] = '192.168.1.113'  # Update with your MQTT broker IP
app.config['MQTT_BROKER_PORT'] = 1883
app.config['MQTT_USERNAME'] = ''
app.config['MQTT_PASSWORD'] = ''
app.config['MQTT_KEEPALIVE'] = 5
app.config['MQTT_TLS_ENABLED'] = False
app.config['MQTT_CLEAN_SESSION'] = True

mqtt = Mqtt(app)
socketio = SocketIO(app)
bootstrap = Bootstrap(app)


@app.route('/')
def index():
    return render_template('index.html')


@mqtt.on_connect()
def handle_connect(client, userdata, flags, rc):
    mqtt.subscribe('/endToSrv')
    mqtt.publish('/srvToEnd', 'Hello flask server')


@mqtt.on_message()
def handle_mqtt_message(client, userdata, message):
    if message.topic == '/endToSrv' and message.payload.decode() == 'Hello end device':
        mqtt.publish('/srvToEnd', 'Message received')
    elif message.topic == '/srvToEnd' and message.payload.decode() == 'Message received':
        mqtt.publish('/endToSrv', 'Hello end device')


@mqtt.on_log()
def handle_logging(client, userdata, level, buf):
    print(level, buf)


if __name__ == '__main__':
    socketio.run(app, host='0.0.0.0', port=5000, use_reloader=False, debug=False)
