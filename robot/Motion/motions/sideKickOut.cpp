#include "Motion/StaticActionPlayer.h"

void StaticActionPlayer::sideKickToRightOut()
{
  if ( motionProxy->isRunning(taskID) )
  {
      return;
  }

  motionProxy->killAll();
  motionProxy->killWalk();

  currentAction = StaticAction::actSideKickToRightOut;

  AL::ALValue names, times, keys;

  #include "sideKickToRightOut.txt"

  try
  {
    taskID = motionProxy->post.angleInterpolationBezier(names, times, keys);
  }
  catch(ALError& e)
  {
      std::cout << "ERROR!!!!" << std::endl;
  }
}

void StaticActionPlayer::sideKickToLeftOut()
{
  if ( motionProxy->isRunning(taskID) )
  {
      return;
  }

  motionProxy->killAll();
  motionProxy->killWalk();

  currentAction = StaticAction::actSideKickToLeftOut;

  AL::ALValue names, times, keys;

  #include "sideKickToLeftOut.txt"

  try
  {
    taskID = motionProxy->post.angleInterpolationBezier(names, times, keys);
  }
  catch(ALError& e)
  {
      std::cout << "ERROR!!!!" << std::endl;
  }
}
