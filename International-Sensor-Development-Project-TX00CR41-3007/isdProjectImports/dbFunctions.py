from flask_sqlalchemy import SQLAlchemy
from datetime import datetime
from flask import jsonify
from isdProjectImports import esp

db = SQLAlchemy()

class RegisteredESPs(db.Model):
    __tablename__ = 'registeredesps'
    DeviceIndex = db.Column(db.Integer, primary_key=True, unique=True)
    DeviceID = db.Column(db.String(255), nullable=False)
    RegistrationTime = db.Column(db.TIMESTAMP, server_default=db.func.current_timestamp())
    LastActiveTime = db.Column(db.TIMESTAMP)
    Assigned = db.Column(db.Boolean)
    Registered = db.Column(db.Boolean)
    MacAddress = db.Column(db.String(255))

class Users(db.Model):
    __tablename__ = 'users'
    UserID = db.Column(db.Integer, primary_key=True, autoincrement=True)
    Username = db.Column(db.Text, unique=True)
    DeviceIndex = db.Column(db.Integer, db.ForeignKey('registeredesps.DeviceIndex'))
    RegistrationDate = db.Column(db.TIMESTAMP, server_default=db.func.current_timestamp())
    registered_esp = db.relationship('RegisteredESPs', backref='users')

class Topics(db.Model):
    __tablename__ = 'topics'
    TopicID = db.Column(db.Integer, primary_key=True, autoincrement=True)
    Title = db.Column(db.Text, nullable=False)
    Description = db.Column(db.Text)
    StartTime = db.Column(db.TIMESTAMP)
    EndTime = db.Column(db.TIMESTAMP)

class Votes(db.Model):
    __tablename__ = 'votes'
    VoteID = db.Column(db.Integer, primary_key=True, autoincrement=True)
    UserID = db.Column(db.Integer, db.ForeignKey('users.UserID'), nullable=False)
    VoteType = db.Column(db.Text, nullable=False)
    TopicID = db.Column(db.Integer, db.ForeignKey('topics.TopicID', ondelete='CASCADE'), nullable=False)
    VoteTime = db.Column(db.TIMESTAMP, server_default=db.func.current_timestamp())


def get_registered_esps():
    registered_users = db.session.query(RegisteredESPs, Users).join(Users, RegisteredESPs.DeviceIndex == Users.DeviceIndex).filter(RegisteredESPs.Registered == True).all()
    return registered_users
