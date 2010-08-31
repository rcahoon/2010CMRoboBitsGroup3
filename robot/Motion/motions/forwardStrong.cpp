#include "Motion/StaticActionPlayer.h"

void StaticActionPlayer::forwardStrongRight()
{
  if ( motionProxy->isRunning(taskID) )
  {
      return;
  }

  motionProxy->killAll();
  motionProxy->killWalk();

  currentAction = StaticAction::actForwardStrongRight;

  AL::ALValue names, times, keys;

  #include "forwardStrongRight.txt"

  try
  {
      taskID = motionProxy->post.angleInterpolationBezier(names, times, keys);
  }
  catch(ALError& e)
  {
      std::cout << "ERROR!!!!" << std::endl;
  }
}

void StaticActionPlayer::forwardStrongLeft()
{
  if ( motionProxy->isRunning(taskID) )
  {
      return;
  }

  motionProxy->killAll();
  motionProxy->killWalk();

  currentAction = StaticAction::actForwardStrongLeft;

  AL::ALValue names, times, keys;

  #include "forwardStrongLeft.txt"

  try
  {
      taskID = motionProxy->post.angleInterpolationBezier(names, times, keys);
  }
  catch(ALError& e)
  {
      std::cout << "ERROR!!!!" << std::endl;
  }
}
