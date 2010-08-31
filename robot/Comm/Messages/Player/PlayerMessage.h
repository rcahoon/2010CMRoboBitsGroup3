#ifndef PLAYERMESSAGE_H_
#define PLAYERMESSAGE_H_

#include <boost/serialization/utility.hpp>

#include "shared/Vector/Vector2D.h"
#include "shared/UsefulMacros.h"

class ConfigFile;

class PlayerMessage {
public:
  PlayerMessage(ConfigFile & configFile);

  PlayerMessage(int _teamNumber, int _playerNumber);

  PlayerMessage();

  virtual ~PlayerMessage();

  int getCurrentPlayID() const;

  int getCurrentRoleID() const;

  int getSenderTeamNumber() const;

  int getSenderPlayerNumber() const;

  bool isSenderLost() const;

  bool isSenderPenalized() const;

  bool isSenderGettingUp() const;

  bool isSenderLifted() const;

  bool isBallKnown() const;

  const Vector2D & getSenderGlobalPosition() const;

  const Vector2D & getBallGlobalPosition() const;

  float getSenderDistToBall() const;

  float getBallConfidence() const;

  bool isKickoffDone() const;

  void setCurrentPlay(int currentPlayID);

  void setCurrentRole(int currentRoleID);

  void setKickoffDone();

  /**
   * Called when the sender knows its global position and the ball's global
   * position.
   *
   * @param senderGlobalPosition global position of the sender
   * @param ballGlobalPosition global position of the ball
   * @param ballConfidence confidence on the global ball
   */
  void setSenderAndBallGlobalPosition(const Vector2D & senderGlobalPosition,
                                      const Vector2D & ballGlobalPosition,
                                      float ballConfidence);

  /**
   * Called when the sender knows its global position, but doesn't know where
   * the ball is.
   *
   * @param senderGlobalPosition global position of the sender
   */
  void setSenderGlobalPosition(const Vector2D & senderGlobalPosition);

  /**
   * Called when the sender is lost but knows the ball's relative position.
   *
   * @param senderDistToBall distance from the sender to the ball
   */
  void setBallRelativeDistance(float senderDistToBall);

  /**
   * Called when the sender is penalized.
   *
   * @param penalized whether the sender is penalized
   */
  void setSenderPenalized(bool penalized = true);

  /**
   * Called when the sender is getting up.
   *
   * @param penalized whether the sender is getting up
   */
  void setSenderGettingUp(bool gettingUp = true);

  /**
   * Called when the sender is lifted (usually by a human).
   *
   * @param penalized whether the sender is lifted
   */
  void setSenderLifted(bool lifted = true);

private:
  PREVENT_COPY_AND_ASSIGNMENT(PlayerMessage);

  /**
   * Allow access for Boost serialization
   */
  friend class boost::serialization::access;

  template<class Archive>
  void serialize(Archive & ar, unsigned int version);

  int teamNumber;
  int playerNumber;

  int currentPlayID, currentRoleID;

  bool senderLost, senderPenalized, senderGettingUp, senderLifted, ballKnown;
  Vector2D senderGlobalPosition, ballGlobalPosition;
  float senderDistToBall;
  // Confidence on the ball's global position
  float ballConfidence;

  // For Kickoff
  bool kickoffDone;
};

// Boost doesn't allow this to be placed in the cpp file for some reason.
template<class Archive>
void PlayerMessage::serialize(Archive & ar, unsigned int version) {
  ar & teamNumber;
  ar & playerNumber;

  ar & currentPlayID;
  ar & currentRoleID;

  ar & senderLost;
  ar & senderPenalized;
  ar & senderGettingUp;
  ar & senderLifted;
  ar & ballKnown;

  ar & senderGlobalPosition;
  ar & ballGlobalPosition;

  ar & senderDistToBall;

  ar & ballConfidence;

  ar & kickoffDone;
}

#endif /* PLAYERMESSAGE_H_ */
