class VoteInformation:

    # Vote object is initialized with NULL values.
    def __init__(self):
        self.voteID = None
        self.topicID = None
        self.voteTitle = None
        self.voteType = None
        self.voteStartTime = None
        self.voteEndTime = None
    
    def updateVoteInformation(self, voteID, voteType, voteTitle, topicID, voteStartTime, voteEndTime):
        self.voteID = voteID
        self.topicID = topicID
        self.voteTitle = voteTitle
        self.voteType = voteType
        self.voteStartTime = voteStartTime
        self.voteEndTime = voteEndTime