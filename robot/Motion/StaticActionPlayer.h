#ifndef STATICACTIONPLAYER_H_
#define STATICACTIONPLAYER_H_

#include <alproxy.h>
#include <almotionproxy.h>
#include "Agent/Command/MotionCommand/BodyCommand/StaticAction.h"
using namespace AL;

class StaticActionPlayer 
{
public:

  StaticActionPlayer();

  virtual ~StaticActionPlayer();

  void setMotionProxy( AL::ALPtr<AL::ALMotionProxy> proxy );

  void standUp();
  void sitDown();

  void forwardDribbleRight();
  void forwardDribbleLeft();
  void forwardKickRight();
  void forwardKickLeft();
  void forwardStrongRight();
  void forwardStrongLeft();

  void sideMiniToRightIn();
  void sideMiniToLeftIn();
//  void sideWeakToRightIn();
//  void sideWeakToLeftIn();
//  void sideKickToRightIn();
//  void sideKickToLeftIn();
  void sideStrongToRightIn();
  void sideStrongToLeftIn();
  void sideKickToRightOut();
  void sideKickToLeftOut();

//  void obliqueKickToRight();
//  void obliqueKickToLeft();

  void spreadOut();
  void getUpFromFront();
  void getUpFromBack();

  void testAction();

  bool isActive();
  StaticAction::StaticActionType getCurrentAction();

private:
  ALPtr<ALMotionProxy> motionProxy;

  int taskID;
  StaticAction::StaticActionType currentAction;

};


#endif /* STATICACTIONPLAYER_H_ */
