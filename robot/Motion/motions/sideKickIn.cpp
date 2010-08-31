#include "Motion/StaticActionPlayer.h"

//void StaticActionPlayer::sideKickToRightIn()
//{
//    if ( motionProxy->isRunning(taskID) )
//    {
//        return;
//    }
//
//    motionProxy->killAll();
//    motionProxy->killWalk();
//
//    currentAction = StaticAction::actSideKickToRightIn;
//
//    AL::ALValue names, times, keys;
//
//    #include "sideKickToRightIn.txt"
//
//    try
//    {
//      taskID = motionProxy->post.angleInterpolationBezier(names, times, keys);
//    }
//    catch(ALError& e)
//    {
//        std::cout << "ERROR!!!!" << std::endl;
//    }
//}
//
//void StaticActionPlayer::sideKickToLeftIn()
//{
//    if ( motionProxy->isRunning(taskID) )
//    {
//        return;
//    }
//
//    motionProxy->killAll();
//    motionProxy->killWalk();
//
//    currentAction = StaticAction::actSideKickToLeftIn;
//
//    AL::ALValue names, times, keys;
//
//    #include "sideKickToLeftIn.txt"
//
//    try
//    {
//      taskID = motionProxy->post.angleInterpolationBezier(names, times, keys);
//    }
//    catch(ALError& e)
//    {
//        std::cout << "ERROR!!!!" << std::endl;
//    }
//}
//
//void StaticActionPlayer::sideWeakToRightIn()
//{
//    if ( motionProxy->isRunning(taskID) )
//    {
//        return;
//    }
//
//    motionProxy->killAll();
//    motionProxy->killWalk();
//
//    currentAction = StaticAction::actSideWeakToRightIn;
//
//    AL::ALValue names, times, keys;
//
//    #include "sideWeakToRightIn.txt"
//
//    try
//    {
////      std::cout << "WEAK RIGHT" << std::endl;
//      taskID = motionProxy->post.angleInterpolationBezier(names, times, keys);
//    }
//    catch(ALError& e)
//    {
//        std::cout << "ERROR!!!!" << std::endl;
//    }
//}
//
//void StaticActionPlayer::sideWeakToLeftIn()
//{
//    if ( motionProxy->isRunning(taskID) )
//    {
//        return;
//    }
//
//    motionProxy->killAll();
//    motionProxy->killWalk();
//
//    currentAction = StaticAction::actSideWeakToLeftIn;
//
//    AL::ALValue names, times, keys;
//
//    #include "sideWeakToLeftIn.txt"
//
//    try
//    {
////      std::cout << "WEAK LEFT" << std::endl;
//      taskID = motionProxy->post.angleInterpolationBezier(names, times, keys);
//    }
//    catch(ALError& e)
//    {
//        std::cout << "ERROR!!!!" << std::endl;
//    }
//}

void StaticActionPlayer::sideMiniToRightIn()
{
    if ( motionProxy->isRunning(taskID) )
    {
        return;
    }

    motionProxy->killAll();
    motionProxy->killWalk();

    currentAction = StaticAction::actSideMiniToRightIn;

    AL::ALValue names, times, keys;

    #include "sideMiniToRightIn.txt"

    try
    {
//      std::cout << "WEAK RIGHT" << std::endl;
      taskID = motionProxy->post.angleInterpolationBezier(names, times, keys);
    }
    catch(ALError& e)
    {
        std::cout << "ERROR!!!!" << std::endl;
    }
}

void StaticActionPlayer::sideMiniToLeftIn()
{
    if ( motionProxy->isRunning(taskID) )
    {
        return;
    }

    motionProxy->killAll();
    motionProxy->killWalk();

    currentAction = StaticAction::actSideMiniToLeftIn;

    AL::ALValue names, times, keys;

    #include "sideMiniToLeftIn.txt"

    try
    {
//      std::cout << "WEAK LEFT" << std::endl;
      taskID = motionProxy->post.angleInterpolationBezier(names, times, keys);
    }
    catch(ALError& e)
    {
        std::cout << "ERROR!!!!" << std::endl;
    }
}
