#ifndef MOTIONLIB_H_
#define MOTIONLIB_H_

#include <string>
#include <vector>

#include "MotionInfo.h"
#include "shared/UsefulMacros.h"

class ConfigFile;
class Log;

class MotionLib {
public:
  MotionLib(ConfigFile & configFile, Log & log);
  virtual ~MotionLib();

  void getMotionOdometry(StaticAction::StaticActionType type, Vector2D & translation, float & rotation) const;

  void clearMask();
  void addAllKicks();
  void addForwardKicks();
  void addSideKicks();
  void addObliqueKicks();
  void addSideKicksToLeft();
  void addSideKicksToRight();
  void addKick(StaticAction::StaticActionType type);
  MotionInfo const * getBestKick(StaticAction::StaticActionType & type, Vector2D & walkTo, float & turnTo,
		  bool separateDirs = false);
  void setKickCriteria(const Vector2D & _relTarget, const Vector2D & _ballPosition, bool _through = true,
                       bool _useAngle = false, float _minAngleValue = 0, float _maxAngleValue = 0);

  bool isForwardKick(StaticAction::StaticActionType type);

  void getKickWalkTo(MotionInfo const *kick, Vector2D & walkTo, float & turnTo, bool separateDirs = false);

  /**
   * Calculates how much we should orbit by to make the next kick available,
   * where positive is an anti-clockwise orbit around the ball
   */
  float getOrbitAngle(const Vector2D & target, const Vector2D & ballPosition);

  float getOrbitAngle(const Vector2D & target, const Vector2D & ballPosition, StaticAction::StaticActionType & staticActionType);

private:
  PREVENT_COPY_AND_ASSIGNMENT(MotionLib);

  void loadMotionLib(ConfigFile & configFile, const std::string & motionLibFileName);

  const std::string motionLibFileName;

  const float NON_FORWARD_KICK_ORBIT_WEIGHT_MULTIPLIER;

  Log & log;

  std::vector<MotionInfo *> motionLibrary;
  std::vector<MotionInfo *> motionMask;
  //Kick criteria
  bool through;
  bool useAngle;
  float minAngleValue;
  float maxAngleValue;
  Vector2D ballPosition;
  Vector2D relTarget;


};

#endif /* MOTIONLIB_H_ */
