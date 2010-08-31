#include "StaticActionPlayer.h"

/*
#define DEFINE_ACTION(actName) \
    void StaticActionPlayer::actName() \
    { \
        if ( motionProxy->isRunning(taskID) ) \
        { \
            return; \
        } \
        motionProxy->killAll(); \
        motionProxy->killWalk(); \
        AL::ALValue names, times, keys; \
        #include #motions/actName.txt \
        try \
        { \
          taskID = motionProxy->post.angleInterpolationBezier(names, times, keys); \
        } \
        catch(ALError& e) \
        { \
            std::cout << "ERROR!!!!" << std::endl; \
        } \
    }
*/


StaticActionPlayer::StaticActionPlayer()
{
    taskID = -1;
    currentAction = StaticAction::actNone;
}

StaticActionPlayer::~StaticActionPlayer()
{
}

void StaticActionPlayer::standUp()
{
    if ( motionProxy->isRunning(taskID) )
    {
        return;
    }

    currentAction = StaticAction::actStandUp;

    motionProxy->killAll();
    motionProxy->killWalk();

    // Special motion for standing up, should only be called by NaoQiModule
    #include "motions/standUp.txt"

    try
    {
        taskID = motionProxy->post.angleInterpolationWithSpeed(names, values, 0.05);
    }
    catch(ALError& e)
    {
        std::cout << "ERROR!!!!" << std::endl;
    }

}

void StaticActionPlayer::sitDown()
{
    if ( motionProxy->isRunning(taskID) )
    {
        return;
    }

    currentAction = StaticAction::actSitDown;

    motionProxy->killAll();
    motionProxy->killWalk();

    AL::ALValue names, times, keys;

    #include "motions/sitDown.txt"

    try
    {
        taskID = motionProxy->post.angleInterpolationBezier(names, times, keys);
    }
    catch(ALError& e)
    {
        std::cout << "ERROR!!!!" << std::endl;
    }

}

void StaticActionPlayer::spreadOut() {
  if ( motionProxy->isRunning(taskID) )
  {
      return;
  }

  motionProxy->killAll();
  motionProxy->killWalk();

  currentAction = StaticAction::actSpreadOut;

  AL::ALValue names, times, keys;

  #include "motions/spreadOut.txt"

  try
  {
    taskID = motionProxy->post.angleInterpolationBezier(names, times, keys);
  }
  catch(ALError& e)
  {
      std::cout << "ERROR!!!!" << std::endl;
  }
}

void StaticActionPlayer::getUpFromFront() {
  if ( motionProxy->isRunning(taskID) )
  {
      return;
  }

  motionProxy->killAll();
  motionProxy->killWalk();

  currentAction = StaticAction::actGetUpFromFront;

  AL::ALValue names, times, keys;

  #include "motions/getUpFromFront.txt"

  try
  {
    taskID = motionProxy->post.angleInterpolationBezier(names, times, keys);
  }
  catch(ALError& e)
  {
      std::cout << "ERROR!!!!" << std::endl;
  }
}

void StaticActionPlayer::getUpFromBack() {
  if ( motionProxy->isRunning(taskID) )
  {
      return;
  }

  motionProxy->killAll();
  motionProxy->killWalk();

  currentAction = StaticAction::actGetUpFromBack;

  AL::ALValue names, times, keys;

  #include "motions/getUpFromBack.txt"

  try
  {
    taskID = motionProxy->post.angleInterpolationBezier(names, times, keys);
  }
  catch(ALError& e)
  {
      std::cout << "ERROR!!!!" << std::endl;
  }
}

void StaticActionPlayer::testAction()
{
    if ( motionProxy->isRunning(taskID) )
    {
        return;
    }

    motionProxy->killAll();
    motionProxy->killWalk();

    currentAction = StaticAction::actTest;

    AL::ALValue names, times, keys;

    #include "motions/testAction.txt"

    try
    {
      taskID = motionProxy->post.angleInterpolationBezier(names, times, keys);
    }
    catch(ALError& e)
    {
        std::cout << "ERROR!!!!" << std::endl;
    }
}

bool StaticActionPlayer::isActive()
{
    if ( motionProxy->isRunning(taskID) )
    {
        //std::cout << "StaticActionPlayer::isActive() - active" << std::endl;
        return true;
    }
    else
    {
        //std::cout << "StaticActionPlayer::isActive() - not active" << std::endl;
        currentAction = StaticAction::actNone;
//        motionProxy->killAll();
        return false;
    }
}

StaticAction::StaticActionType StaticActionPlayer::getCurrentAction()
{
    return currentAction;
}


void StaticActionPlayer::setMotionProxy(ALPtr<ALMotionProxy> proxy )
{
    motionProxy = proxy;
}

