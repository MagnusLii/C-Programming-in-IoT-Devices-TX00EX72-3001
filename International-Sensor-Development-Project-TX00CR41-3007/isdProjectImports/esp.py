from isdProjectImports import mqttImports
import random
import uuid

class Esp:

    numRegistredESPs = 0
    regiseredESPs = []

    def __init__(self, macAddress, registeredUser='NULL',):
        self.macAddress = macAddress
        self.registeredUser = registeredUser

        self.uniqueID = str(uuid.uuid4())  # random unique ID for each ESP based on RFC 4122 standards
        self.voteStatus = 'pass'  # default value is pass/abstain.

        self.registrationConfirmationTopic = mqttImports.registrationResponeTopic + self.macAddress


        Esp.numRegistredESPs += 1
        Esp.regiseredESPs.append(self)
    
    def updateVoteStatus(self, voteStatus):
        self.voteStatus = voteStatus
        return

    def returnUniqueIDToEsp(self):
        mqttImports.publishJSONtoMQTT(self.registrationConfirmationTopic, self.uniqueID)
        return

    @classmethod
    def getRegistereEsps(cls):
        return cls.regiseredESPs
    