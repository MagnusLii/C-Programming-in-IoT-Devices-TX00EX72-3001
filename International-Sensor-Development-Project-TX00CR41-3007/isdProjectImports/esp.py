from isdProjectImports import mqttImports
import random

class Esp:

    registredESPs = 0

    def __init__(self, mac_address, registeredUser='NULL',):
        self.mac_address = mac_address
        self.registeredUser = registeredUser

        self.uniqueID = random.randint(1, 10)  # Needs to be changed later.
        self.voteStatus = 'pass'  # default value is pass.

        self.registration_confirmation_topic = mqttImports.registrationResponeTopic + self.mac_address


        Esp.registredESPs += 1

    
    def registerEsp(self, jsonMessage):
        self.registeredUser = jsonMessage['user']
        print(f'ESP {self.mac_address} registered to user {self.registeredUser}.')