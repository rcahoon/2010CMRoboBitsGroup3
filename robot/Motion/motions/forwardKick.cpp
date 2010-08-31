#include "Motion/StaticActionPlayer.h"

void StaticActionPlayer::forwardKickRight()
{
  if ( motionProxy->isRunning(taskID) )
  {
      return;
  }

  motionProxy->killAll();
  motionProxy->killWalk();

  currentAction = StaticAction::actForwardKickRight;

  AL::ALValue names, times, keys;

  #include "forwardKickRight.txt"

  try
  {
      taskID = motionProxy->post.angleInterpolationBezier(names, times, keys);
  }
  catch(ALError& e)
  {
      std::cout << "ERROR!!!!" << std::endl;
  }
}

void StaticActionPlayer::forwardKickLeft() {
  if ( motionProxy->isRunning(taskID) )
  {
      return;
  }

  motionProxy->killAll();
  motionProxy->killWalk();

  currentAction = StaticAction::actForwardKickLeft;

  AL::ALValue names, times, keys;

  #include "forwardKickLeft.txt"

  try
  {
      taskID = motionProxy->post.angleInterpolationBezier(names, times, keys);
  }
  catch(ALError& e)
  {
      std::cout << "ERROR!!!!" << std::endl;
  }
}
