#include "Motion/StaticActionPlayer.h"

void StaticActionPlayer::forwardDribbleRight()
{
    if ( motionProxy->isRunning(taskID) )
    {
        return;
    }

    motionProxy->killAll();
    motionProxy->killWalk();

    currentAction = StaticAction::actForwardDribbleRight;

    AL::ALValue names, times, keys;

    #include "forwardDribbleRight.txt"

    try
    {
        taskID = motionProxy->post.angleInterpolationBezier(names, times, keys);
    }
    catch(ALError& e)
    {
        std::cout << "ERROR!!!!" << std::endl;
    }
}

void StaticActionPlayer::forwardDribbleLeft()
{
    if ( motionProxy->isRunning(taskID) )
    {
        return;
    }

    motionProxy->killAll();
    motionProxy->killWalk();

    currentAction = StaticAction::actForwardDribbleLeft;

    AL::ALValue names, times, keys;

    #include "forwardDribbleLeft.txt"

    try
    {
        taskID = motionProxy->post.angleInterpolationBezier(names, times, keys);
    }
    catch(ALError& e)
    {
        std::cout << "ERROR!!!!" << std::endl;
    }
}
