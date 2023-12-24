from flask_sqlalchemy import SQLAlchemy
from datetime import datetime
from flask import jsonify

db = SQLAlchemy()

class ESPDevice(db.Model):
    __tablename__ = 'espdevices'
    DeviceID = db.Column(db.String(17), primary_key=True)
    RegistrationTime = db.Column(db.TIMESTAMP, default=datetime.utcnow)
    LastActiveTime = db.Column(db.TIMESTAMP)

class User(db.Model):
    __tablename__ = 'users'
    UserID = db.Column(db.Integer, primary_key=True, autoincrement=True)
    Username = db.Column(db.Text, unique=True)
    DeviceID = db.Column(db.String(17))
    RegistrationDate = db.Column(db.TIMESTAMP, default=datetime.utcnow)

class Topic(db.Model):
    __tablename__ = 'topics'
    TopicID = db.Column(db.Integer, primary_key=True, autoincrement=True)
    Title = db.Column(db.Text, nullable=False)
    Description = db.Column(db.Text)
    StartTime = db.Column(db.TIMESTAMP)
    EndTime = db.Column(db.TIMESTAMP)

class Vote(db.Model):
    __tablename__ = 'votes'
    VoteID = db.Column(db.Integer, primary_key=True, autoincrement=True)
    UserID = db.Column(db.Integer, db.ForeignKey('user.UserID'), nullable=False)
    VoteType = db.Column(db.Text, nullable=False)
    TopicID = db.Column(db.Integer, db.ForeignKey('topic.TopicID'), nullable=False)
    VoteTime = db.Column(db.TIMESTAMP, default=datetime.utcnow())

def create_tables():
    db.create_all()


# Returns a JSON object containing all the topics (aka votes held) in the database.
def get_all_topics():
    try:
        topics = Topic.query.all()

        # Convert the topics data to a list of dictionaries
        topics_list = []
        for topic in topics:
            topics_list.append({
                'TopicID': topic.TopicID,
                'Title': topic.Title,
                'Description': topic.Description,
                'StartTime': str(topic.StartTime),
                'EndTime': str(topic.EndTime)
            })

        # Return the topics data as a JSON object
        return jsonify({'topics': topics_list}), 200

    except Exception as e:
        return jsonify({'error': str(e)}), 500

