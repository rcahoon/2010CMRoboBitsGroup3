#ifndef FEEDBACK_H_
#define FEEDBACK_H_

#include "shared/Vector/Vector2D.h"
#include "Localization/Localization.h"

class Factory;
class Log;
class WorldModel;
class Vision;

class Feedback {
public:
  Feedback(Factory & factory);
  virtual ~Feedback();

  /**
   * Runs the feedback mechanism. Feedback will call the relevant functions on
   * the components as necessary.
   */
  bool run();

  void clear();

  void setConnectedToClient(bool connectedToClient);

  void setScanningForGoals(bool scanningForGoals);

  void setScanningForObstacles(bool scanningForObstacles);
  
  void setClearFreeSpaceModel(bool clearFreeSpaceModel);

  void setRobotIsGettingUp();

  void setRobotIsLifted();

  void setKickEffect(const Vector2D & kickEffect);

  void resetFromPenalty(bool isOpponentGoalOnTheLeftSide);

  void resetLocalization(Localization::ResetCase resetCase);

  void invalidateBall();

  void setGoalieLocalization(bool _useGoalieLocalization);

  void disableBallDetectionForNextFrame();

private:

  enum PossibleBool {
    Unknown,
    False,
    True,
  };

  enum ConnectedToClient {
    UnknownConnectedToClient,
    NotConnected,
    Connected,
  };

  Log & log;
  WorldModel & worldModel;
  Localization & localization;
  Vision & vision;

  PossibleBool connectedToClient, scanGoalStatus, scanObstaclesStatus, clearFreeSpaceModelStatus;

  bool gettingUp, lifted;
  bool doResetLocalization;
  Localization::ResetCase resetLocalizationCase;
  Vector2D kickEffect;
  bool haveKickEffect;

  bool ballInvalidated;
  PossibleBool useGoalieLocalization;

  bool disableBallDetection;
};

#endif /* FEEDBACK_H_ */
