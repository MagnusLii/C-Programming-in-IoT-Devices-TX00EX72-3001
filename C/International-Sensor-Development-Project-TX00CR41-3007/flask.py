from flask import Flask, request, jsonify
from flask_mqtt import Mqtt
from flask_sqlalchemy import SQLAlchemy

app = Flask(__name__)
mqtt = Mqtt(app)
db = SQLAlchemy(app)

class Vote(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    voter_id = db.Column(db.String(50), unique=True, nullable=False)
    vote = db.Column(db.String(50), nullable=False)

@app.route('/vote', methods=['POST'])
def handle_vote():
    data = request.get_json()
    vote = Vote(voter_id=data['voter_id'], vote=data['vote'])
    db.session.add(vote)
    db.session.commit()
    mqtt.publish('votes', jsonify(data))
    return jsonify({'message': 'Vote received'}), 200

if __name__ == '__main__':
    db.create_all()
    app.run(debug=True)
