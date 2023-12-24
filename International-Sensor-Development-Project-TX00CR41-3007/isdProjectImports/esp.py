from isdProjectImports import mqttImports
import random
import uuid

class Esp:

    numRegistredESPs = 0
    regiseredESPs = []

    def __init__(self, mac_address, registeredUser='NULL',):
        self.macAddress = mac_address
        self.registeredUser = registeredUser

        self.uniqueID = str(uuid.uuid4())  # random unique ID for each ESP based on RFC 4122 standards
        self.voteStatus = 'pass'  # default value is pass/abstain.

        self.registration_confirmation_topic = mqttImports.registrationResponeTopic + self.mac_address


        Esp.numRegistredESPs += 1
        Esp.regiseredESPs.append(self)
    
    def updateVoteStatus(self, voteStatus):
        self.voteStatus = voteStatus

    @classmethod
    def getRegistereEsps(cls):
        return cls.regiseredESPs