#ifndef MOTIONCOMMAND_H_
#define MOTIONCOMMAND_H_

#include <boost/serialization/utility.hpp>

#include "MotionCommandList.h"

#include "shared/Vector/Vector2D.h"
#include "shared/HMatrix/HMatrix.h"
#include "shared/UsefulMacros.h"
//#include "BodyCommand/NoBodyCommand.h"
//class BodyCommand;
//class HeadCommand;

/**
 * The MotionCommand abstracts all possible commands to the physical robot,
 * i.e. how to move its joints.
 *
 * MotionCommands are split into 2 parts: a head command and a body command.
 *
 * Head commands control only the head, and body commands control the arms
 * and/or legs and/or body or the robot. Thus, head commands and body commands
 * can be executed in parallel without interference.
 *
 * Head and Body commands are subclassed into their various types.
 */
class MotionCommand {
public:

  /**
   * Constructor. Clears all member variables.
   */
  MotionCommand();

  /**
   * Destructor.
   */
  ~MotionCommand();

  /**
   * Clears the motion command.
   */
  void clear();

  /**
   * Returns the head command.
   *
   * @return the head command
   */
   int getHeadCommand();

  /**
   * Returns the body command.
   *
   * @return the body command
   */
   int getBodyCommand();

  // Functions to set body commands
  void noBodyCommand();
  void standNeutral(StandNeutral::WhichLeg whichLeg = StandNeutral::Any);
  void stopWalk();
  void walk(float vX, float vY, float vTheta);
  void walkTo(float pX, float pY, float pTheta);
  void staticAction(StaticAction::StaticActionType actionID);

  NoBodyCommand& getNoBodyCommand();
  StandNeutral& getStandNeutral();
  StopWalk& getStopWalk();
  Walk& getWalk();
  WalkTo& getWalkTo();
  StaticAction& getStaticAction();


  // Functions to set head commands
  void noHeadCommand();
  void headAngles(float pan, float tilt, float speed = 0.8f);
  void setHeadStiffness(float stiffness);
  void restoreHeadStiffness();

  NoHeadCommand& getNoHeadCommand();
  HeadAngles& getHeadAngles();
  HeadStiffness& getHeadStiffness();
  RestoreHeadStiffness& getRestoreHeadStiffness();

  // Functions for falling/getting up

private:
  PREVENT_COPY_AND_ASSIGNMENT(MotionCommand);

  NoBodyCommand _noBodyCommand;
  StandNeutral  _standNeutral;
  StopWalk      _stopWalk;
  Walk          _walk;
  WalkTo        _walkTo;
  StaticAction  _staticAction;

  NoHeadCommand        _noHeadCommand;
  HeadAngles           _headAngles;
  HeadStiffness        _headStiffness;
  RestoreHeadStiffness _restoreHeadStiffness;

//  HeadCommand const *headCommand;
//  BodyCommand const *bodyCommand;

  int headCommand;
  int bodyCommand;

  /**
   * Allow access for Boost serialization
   */
  friend class boost::serialization::access;

  BOOST_SERIALIZATION_SPLIT_MEMBER()

  template<class Archive>
  void save(Archive & ar, unsigned int version) const;

  template<class Archive>
  void load(Archive & ar, unsigned int version);
};

// Boost doesn't allow this to be placed in the cpp file for some reason.
template<class Archive>
void MotionCommand::save(Archive & ar, unsigned int version) const {
  /*  
  ar & headCommand;
  ar & bodyCommand;
  */
}

// Boost doesn't allow this to be placed in the cpp file for some reason.
template<class Archive>
void MotionCommand::load(Archive & ar, unsigned int version) {
  /*
  HeadCommand *headCmd;
  BodyCommand *bodyCmd;
  ar & headCmd;
  ar & bodyCmd;

  // Some errors when trying to read a const pointer directly from archive
  headCommand         = headCmd;
  bodyCommand         = bodyCmd;
  shouldDeleteMembers = true;
  */
}

#endif /* MOTIONCOMMAND_H_ */
