#include "Motion/StaticActionPlayer.h"

void StaticActionPlayer::sideStrongToRightIn()
{
  if ( motionProxy->isRunning(taskID) )
  {
      return;
  }

  motionProxy->killAll();
  motionProxy->killWalk();

  currentAction = StaticAction::actSideStrongToRightIn;

  AL::ALValue names, times, keys;

  #include "sideStrongToRightIn.txt"

  try
  {
    taskID = motionProxy->post.angleInterpolationBezier(names, times, keys);
  }
  catch(ALError& e)
  {
      std::cout << "ERROR!!!!" << std::endl;
  }
}

void StaticActionPlayer::sideStrongToLeftIn()
{
  if ( motionProxy->isRunning(taskID) )
  {
      return;
  }

  motionProxy->killAll();
  motionProxy->killWalk();

  currentAction = StaticAction::actSideStrongToLeftIn;

  AL::ALValue names, times, keys;

  #include "sideStrongToLeftIn.txt"

  try
  {
    taskID = motionProxy->post.angleInterpolationBezier(names, times, keys);
  }
  catch(ALError& e)
  {
      std::cout << "ERROR!!!!" << std::endl;
  }
}
