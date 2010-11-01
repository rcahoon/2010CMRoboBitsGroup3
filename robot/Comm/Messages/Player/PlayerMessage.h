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

  int getSenderTeamNumber() const;

  int getSenderPlayerNumber() const;

  bool isBallSeen() const;

  void setBallSeen(bool ballSeen);

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

  bool ballSeen;
};

// Boost doesn't allow this to be placed in the cpp file for some reason.
template<class Archive>
void PlayerMessage::serialize(Archive & ar, unsigned int version) {
  ar & teamNumber;
  ar & playerNumber;

  ar & ballSeen;
}

#endif /* PLAYERMESSAGE_H_ */
