#ifndef _NAOQIINTERFACE_H
#define _NAOQIINTERFACE_H

#include "Definitions.h"
#include "Agent/SharedInformation.h"
#include <semaphore.h>
#include <alproxy.h>
#include <albroker.h>
#include <almemoryfastaccess.h>
#include <dcmproxy.h>
#include <almotionproxy.h>
#include <vector>
#include <Log/Log.h>
#include <shared/ConfigFile/ConfigFile.h>
#include "LEDController.h"
#include "SoundController.h"
#include "Localization/Pose.h"
#include "Motion/StaticActionPlayer.h"
#include "Motion/MotionLib/MotionLib.h"

class NaoQiInterface 
{
public:
    NaoQiInterface();
    virtual ~NaoQiInterface();
    void setBroker( AL::ALPtr<AL::ALBroker> pBroker );
    bool initialize( AL::ALPtr<AL::ALBroker> pBroker );
    bool initializeSonar();
    void initFastAccess( AL::ALPtr<AL::ALBroker> pBroker );
    void deinitialize();
    void sense();
    void act();
    void updateRobotState();
    void sit();
    void stand();

private:
    bool areFeetTogether(bool & moveLeftFoot);

    void setMaxTrapezoid(float value);
    void setMaxTheta(float value);

    SharedInformation* sharedInfo; /**< The object for passing information back and forth among this class and the stand alone process*/
    int fdShm; /**< file descriptor for shared memory */
    sem_t* semaphore; 

    ConfigFile & configFile;
    Log& log;

    // We need to give Naoqi some time to start up all the modules before we
    // subscribe to their data
    const unsigned long WAIT_FOR_NAOQI_TIME;
    const unsigned long WAIT_FOR_NAOQI_SONAR_TIME;

    const unsigned long CMURFS_TIMEOUT, CMURFS_OFFLINE_UPDATE_ANGLE_PERIOD;
    const bool IGNORE_FSR;
    const float FSR_THRESHOLD;

    const float MIN_PAN_DELTA, MIN_TILT_DELTA;

    const float MAX_HEAD_SPEED, KEYFRAME_MAX_HEAD_SPEED;

    const float FEET_TOGETHER_DIST_X, FEET_TOGETHER_DIST_Y;

    const float MAX_TRAPEZOID_NORMAL, MAX_TRAPEZOID_WALKTO;
    const float MAX_THETA_NORMAL,     MAX_THETA_WALKTO;

    AL::ALPtr<AL::ALBroker> pBroker;
    AL::ALMemoryFastAccess fastAccess;
    AL::ALPtr<AL::ALMotionProxy> motionProxy;
    AL::DCMProxy* dcmProxy;
    AL::ALValue dcmSonarCommand, maxTrapezoidCommand, maxThetaCommand;

    StaticActionPlayer staticActionPlayer;

    std::vector<std::string> jointNames;
    std::vector<std::string> sensorNames;
    std::vector<float> sensorValues;

    Command command;

    LEDController *ledController;
    SoundController *soundController;

    unsigned long DCMTimeOffset;

    std::vector<float> headAngles;
    float headSpeed;
    int headAnglesTaskID;

    HMatrix torsoToNeck, neckToBottomCamera, neckToTopCamera;

    Pose lastOdometry;
    bool bottomCam;
    bool stiffnessSet;
    bool sitting;
    bool cmurfsOnline;
    unsigned long updateAngleTimestamp;

    float cxA;
    float cxB;
    float cyA;
    float cyB;
    float ctA;
    float ctB;

    float maxX;
    float maxY;
    float maxT;

    int walkToTaskID;
    BodyCommand bodyCommandType;
    StaticAction::StaticActionType staticActionType;
    StaticAction::StaticActionType lastStaticActionType;
    std::vector<Vector2D> saOdometryTrans;
    std::vector<float> saOdometryRot;
    MotionLib motionLib;

    unsigned long initialStartupTime;
    bool initialized, initializedSonar;

    float maxTrapezoidValue, maxThetaValue;

    AL::ALValue body, bodyStiffness;
};

#endif /*_NAOQIINTERFACE_H*/
