#ifndef GAMECONTROLLERMESSAGE_H_
#define GAMECONTROLLERMESSAGE_H_

#include <boost/serialization/utility.hpp>

#include "GameController/RoboCupGameControlData.h"

class GameControllerMessage {
public:

  /**
   * Constructor.
   *
   * @param _data RoboCupGameControlData received from the packet
   */
  GameControllerMessage(const struct RoboCupGameControlData & _data);

  /**
   * Constructor (for serialization).
   */
  GameControllerMessage();

  /**
   * Destructor.
   */
  ~GameControllerMessage();

  /**
   * Checks whether or not the message is valid, i.e. the header in the struct
   * is correct.
   *
   * @param teamNumber the player's team id
   *
   * @return whether the message is valid
   */
  bool isValid(int teamNumber) const;

  /**
   * Returns a reference to the RoboCupGameControlData.
   *
   * @return a reference to the RoboCupGameControlData
   */
  struct RoboCupGameControlData const & getData() const;

private:
  // Prevent copy constructor, and assignment
  GameControllerMessage(const GameControllerMessage &);
  GameControllerMessage & operator=(const GameControllerMessage &);

  struct RoboCupGameControlData data;

  /**
   * Allow access for Boost serialization
   */
  friend class boost::serialization::access;

  BOOST_SERIALIZATION_SPLIT_MEMBER();

  template<class Archive>
  void save(Archive & ar, unsigned int version) const;

  template<class Archive>
  void load(Archive & ar, unsigned int version);
};

// Boost doesn't allow this to be placed in the cpp file for some reason.
template<class Archive>
void GameControllerMessage::save(Archive & ar, unsigned int version) const {
  // Serialize the RoboCupGameControlData, byte by byte
  unsigned char *ptr = (unsigned char *)&data;
  for (unsigned int i = 0; i < sizeof(struct RoboCupGameControlData); i++) {
    unsigned char value = ptr[i];
    ar & value;
  }
}

// Boost doesn't allow this to be placed in the cpp file for some reason.
template<class Archive>
void GameControllerMessage::load(Archive & ar, unsigned int version) {
  // De-serialize the RoboCupGameControlData, byte by byte
  unsigned char *ptr = (unsigned char *)&data;
  for (unsigned int i = 0; i < sizeof(struct RoboCupGameControlData); i++) {
    unsigned char value;
    ar & value;
    ptr[i] = value;
  }
}

#endif /* GAMECONTROLLERMESSAGE_H_ */
