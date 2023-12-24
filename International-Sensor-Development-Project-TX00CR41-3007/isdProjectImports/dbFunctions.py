from flask_sqlalchemy import SQLAlchemy
from datetime import datetime

db = SQLAlchemy()

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
    VoteTime = db.Column(db.TIMESTAMP, default=datetime.utcnow())

def create_tables():
    db.create_all()