#include "PlayerMessage.h"
#include "shared/ConfigFile/ConfigFile.h"

PlayerMessage::PlayerMessage(ConfigFile & configFile)
  : teamNumber  (configFile.getInt("team/teamNumber",   -1)),
    playerNumber(configFile.getInt("team/playerNumber", -1)),
    currentPlayID(-1),
    currentRoleID(-1),
    senderLost(true),
    senderPenalized(false),
    senderGettingUp(false),
    senderLifted(false),
    ballKnown(false),
    senderGlobalPosition(0, 0),
    ballGlobalPosition(0, 0),
    senderDistToBall(INFINITY),
    ballConfidence(0),
    kickoffDone(false) {
}


PlayerMessage::PlayerMessage(int _teamNumber, int _playerNumber)
  : teamNumber(_teamNumber),
    playerNumber(_playerNumber),
    currentPlayID(-1),
    currentRoleID(-1),
    senderLost(true),
    senderPenalized(false),
    senderGettingUp(false),
    senderLifted(false),
    ballKnown(false),
    senderGlobalPosition(0, 0),
    ballGlobalPosition(0, 0),
    senderDistToBall(INFINITY),
    ballConfidence(0),
    kickoffDone(false) {
}

PlayerMessage::PlayerMessage()
  : teamNumber(-1),
    playerNumber(-1),
    currentPlayID(-1),
    currentRoleID(-1),
    senderLost(true),
    senderPenalized(false),
    senderGettingUp(false),
    senderLifted(false),
    ballKnown(false),
    senderGlobalPosition(0, 0),
    ballGlobalPosition(0, 0),
    senderDistToBall(INFINITY),
    ballConfidence(0),
    kickoffDone(false) {
}

PlayerMessage::~PlayerMessage() {
}

int PlayerMessage::getCurrentPlayID() const {
  return currentPlayID;
}

int PlayerMessage::getCurrentRoleID() const {
  return currentRoleID;
}

int PlayerMessage::getSenderTeamNumber() const {
  return teamNumber;
}

int PlayerMessage::getSenderPlayerNumber() const {
  return playerNumber;
}

bool PlayerMessage::isSenderLost() const {
  return senderLost;
}

bool PlayerMessage::isSenderPenalized() const {
  return senderPenalized;
}

bool PlayerMessage::isSenderGettingUp() const {
  return senderGettingUp;
}

bool PlayerMessage::isSenderLifted() const {
  return senderLifted;
}

bool PlayerMessage::isBallKnown() const {
  return ballKnown;
}

const Vector2D & PlayerMessage::getSenderGlobalPosition() const {
  return senderGlobalPosition;
}

const Vector2D & PlayerMessage::getBallGlobalPosition() const {
  return ballGlobalPosition;
}

float PlayerMessage::getSenderDistToBall() const {
  return senderDistToBall;
}

float PlayerMessage::getBallConfidence() const {
  return ballConfidence;
}

bool PlayerMessage::isKickoffDone() const {
  return kickoffDone;
}

void PlayerMessage::setCurrentPlay(int _currentPlayID) {
  currentPlayID = _currentPlayID;
}

void PlayerMessage::setCurrentRole(int _currentRoleID) {
  currentRoleID = _currentRoleID;
}

void PlayerMessage::setKickoffDone() {
  kickoffDone = true;
}

void PlayerMessage::setSenderAndBallGlobalPosition(const Vector2D & _senderGlobalPosition,
                                                   const Vector2D & _ballGlobalPosition,
                                                   float _ballConfidence) {
  senderLost = false;
  ballKnown  = true;
  senderGlobalPosition = _senderGlobalPosition;
  ballGlobalPosition   = _ballGlobalPosition;
  senderDistToBall     = (_senderGlobalPosition - _ballGlobalPosition).length();
  ballConfidence       = _ballConfidence;
}

void PlayerMessage::setSenderGlobalPosition(const Vector2D & _senderGlobalPosition) {
  senderLost = false;
  ballKnown  = false;
  senderGlobalPosition = _senderGlobalPosition;
  senderDistToBall     = INFINITY;
  ballConfidence       = 0;
}

void PlayerMessage::setBallRelativeDistance(float _senderDistToBall) {
  senderLost = true;
  ballKnown  = true;
  senderDistToBall = _senderDistToBall;
}

void PlayerMessage::setSenderPenalized(bool penalized) {
  senderPenalized = penalized;
}

void PlayerMessage::setSenderGettingUp(bool gettingUp) {
  senderGettingUp = gettingUp;
}

void PlayerMessage::setSenderLifted(bool lifted) {
  senderLifted = lifted;
}
