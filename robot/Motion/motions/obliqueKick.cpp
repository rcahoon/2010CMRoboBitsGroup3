#include "Motion/StaticActionPlayer.h"

//void StaticActionPlayer::obliqueKickToRight()
//{
//  if ( motionProxy->isRunning(taskID) )
//  {
//      return;
//  }
//
//  motionProxy->killAll();
//  motionProxy->killWalk();
//
//  currentAction = StaticAction::actObliqueKickToRight;
//
//  AL::ALValue names, times, keys;
//
//  #include "obliqueKickToRight.txt"
//
//  try
//  {
//    taskID = motionProxy->post.angleInterpolationBezier(names, times, keys);
//  }
//  catch(ALError& e)
//  {
//      std::cout << "ERROR!!!!" << std::endl;
//  }
//}
//
//void StaticActionPlayer::obliqueKickToLeft()
//{
//  if ( motionProxy->isRunning(taskID) )
//  {
//      return;
//  }
//
//  motionProxy->killAll();
//  motionProxy->killWalk();
//
//  currentAction = StaticAction::actObliqueKickToLeft;
//
//  AL::ALValue names, times, keys;
//
//  #include "obliqueKickToLeft.txt"
//
//  try
//  {
//    taskID = motionProxy->post.angleInterpolationBezier(names, times, keys);
//  }
//  catch(ALError& e)
//  {
//      std::cout << "ERROR!!!!" << std::endl;
//  }
//}
