#include "NaoQiInterface.h"

#include <iostream>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <shared/ConfigFile/ConfigFile.h>
#include <Log/Log.h>
#include <Factory/Factory.h>

NaoQiInterface::NaoQiInterface():
    sharedInfo((SharedInformation*)MAP_FAILED),
    fdShm(-1),
    semaphore(SEM_FAILED),
    configFile(Factory::getNullFactory().getConfigFile()),
    log(Factory::getNullFactory().getLog()),
    WAIT_FOR_NAOQI_TIME(configFile.getMillisecondsFromSeconds("motion/waitForNaoqiTime", 5000)),
    WAIT_FOR_NAOQI_SONAR_TIME(configFile.getMillisecondsFromSeconds("motion/waitForNaoqiSonarTime", 30000)),
    CMURFS_TIMEOUT(configFile.getMillisecondsFromSeconds("motion/cmurfsTimeout", 1000)),
    CMURFS_OFFLINE_UPDATE_ANGLE_PERIOD(configFile.getMillisecondsFromSeconds("motion/cmurfsOfflineUpdateAnglePeriod", 1000)),
    IGNORE_FSR(configFile.getBool("sensorThreshold/ignoreFSR", false)),
    FSR_THRESHOLD(configFile.getFloat("sensorThreshold/FSRSensorThreshold", 0.4)),
    MIN_PAN_DELTA(configFile.getRadiansFromDegrees("motion/minPanDelta", 1)),
    MIN_TILT_DELTA(configFile.getRadiansFromDegrees("motion/minTiltDelta", 1)),
    MAX_HEAD_SPEED(configFile.getFloat("motion/maxHeadSpeed", 1.0f)),
    KEYFRAME_MAX_HEAD_SPEED(configFile.getFloat("motion/keyframeMaxHeadSpeed", 0.5f)),
    FEET_TOGETHER_DIST_X(configFile.getFloat("motion/feetTogetherDistX", 1)),
    FEET_TOGETHER_DIST_Y(configFile.getFloat("motion/feetTogetherDistY", 1)),
    MAX_TRAPEZOID_NORMAL(configFile.getFloat("motion/maxTrapezoidNormal", 4.5f)),
    MAX_TRAPEZOID_WALKTO(configFile.getFloat("motion/maxTrapezoidWalkTo", 7.0f)),
    MAX_THETA_NORMAL(configFile.getFloat("motion/maxThetaNormal", 20.0f)),
    MAX_THETA_WALKTO(configFile.getFloat("motion/maxThetaWalkTo", 45.0f)),
    headAnglesTaskID(-1),
    torsoToNeck(),
    neckToBottomCamera(),
    neckToTopCamera(),
    stiffnessSet(false),
    sitting(true),
    cmurfsOnline(false),
    updateAngleTimestamp(log.getTimestamp()),
    walkToTaskID(-1),
    bodyCommandType(bcNoBodyCommand),
    staticActionType(StaticAction::actNone),
    lastStaticActionType(StaticAction::actNone),
    motionLib(configFile,log),
    initialStartupTime(0),
    initialized(false),
    initializedSonar(false),
    maxTrapezoidValue(MAX_TRAPEZOID_NORMAL),
    maxThetaValue(MAX_THETA_NORMAL)
{
    
    headAngles.resize(2);
    headAngles[0] = 0.0;
    headAngles[1] = 0.0;

    // Set up the transformations
    HMatrix scratch;

    // Torso to neck
    torsoToNeck.initTranslation(0, 0, configFile.getMetersFromCentimeters("physical/distFromTorsoToNeck", 0.1265f));

    // Neck to top camera
    neckToTopCamera.initTranslation(configFile.getMetersFromCentimeters("physical/camera/top/XOffsetFromNeck", 0.0539f),
                                    configFile.getMetersFromCentimeters("physical/camera/top/YOffsetFromNeck", 0),
                                    configFile.getMetersFromCentimeters("physical/camera/top/ZOffsetFromNeck", 0.0679f));
    scratch.initRotY(configFile.getRadiansFromDegrees("physical/camera/top/angleRotation", 0));
    neckToTopCamera.mul(&scratch);

    // Neck to bottom camera
    neckToBottomCamera.initTranslation(configFile.getMetersFromCentimeters("physical/camera/bottom/XOffsetFromNeck", 0.0488f),
                                       configFile.getMetersFromCentimeters("physical/camera/bottom/YOffsetFromNeck", 0),
                                       configFile.getMetersFromCentimeters("physical/camera/bottom/ZOffsetFromNeck", 0.02381f));
    scratch.initRotY(configFile.getRadiansFromDegrees("physical/camera/bottom/angleRotation", 40));
    neckToBottomCamera.mul(&scratch);

}

NaoQiInterface::~NaoQiInterface()
{
    deinitialize();
}

void NaoQiInterface::setBroker(AL::ALPtr<AL::ALBroker> _pBroker) {
  pBroker = _pBroker;
}

void NaoQiInterface::initFastAccess( AL::ALPtr<AL::ALBroker> pBroker )
{
    jointNames.clear();
    jointNames.push_back("HeadYaw");
    jointNames.push_back("HeadPitch");
    jointNames.push_back("LShoulderPitch");
    jointNames.push_back("LShoulderRoll");
    jointNames.push_back("LElbowYaw");
    jointNames.push_back("LElbowRoll");
    jointNames.push_back("LHipYawPitch");
    jointNames.push_back("LHipRoll");
    jointNames.push_back("LHipPitch");
    jointNames.push_back("LKneePitch");
    jointNames.push_back("LAnklePitch");
    jointNames.push_back("LAnkleRoll");
    jointNames.push_back("RHipRoll");
    jointNames.push_back("RHipPitch");
    jointNames.push_back("RKneePitch");
    jointNames.push_back("RAnklePitch");
    jointNames.push_back("RAnkleRoll");
    jointNames.push_back("RShoulderPitch");
    jointNames.push_back("RShoulderRoll");
    jointNames.push_back("RElbowYaw");
    jointNames.push_back("RElbowRoll");

    sensorNames.clear();
    for( unsigned int i = 0; i < jointNames.size(); i++ )
    {
        sensorNames.push_back("Device/SubDeviceList/" + jointNames[ i ] + "/Position/Sensor/Value");
    }

    sensorNames.push_back("Device/SubDeviceList/LFoot/FSR/FrontLeft/Sensor/Value");
    sensorNames.push_back("Device/SubDeviceList/LFoot/FSR/FrontRight/Sensor/Value");
    sensorNames.push_back("Device/SubDeviceList/LFoot/FSR/RearLeft/Sensor/Value");
    sensorNames.push_back("Device/SubDeviceList/LFoot/FSR/RearRight/Sensor/Value");

    sensorNames.push_back("Device/SubDeviceList/RFoot/FSR/FrontLeft/Sensor/Value");
    sensorNames.push_back("Device/SubDeviceList/RFoot/FSR/FrontRight/Sensor/Value");
    sensorNames.push_back("Device/SubDeviceList/RFoot/FSR/RearLeft/Sensor/Value");
    sensorNames.push_back("Device/SubDeviceList/RFoot/FSR/RearRight/Sensor/Value");

    sensorNames.push_back("Device/SubDeviceList/InertialSensor/GyrX/Sensor/Value");
    sensorNames.push_back("Device/SubDeviceList/InertialSensor/GyrY/Sensor/Value");
    sensorNames.push_back("Device/SubDeviceList/InertialSensor/AngleX/Sensor/Value");
    sensorNames.push_back("Device/SubDeviceList/InertialSensor/AngleY/Sensor/Value");

    sensorNames.push_back("Device/SubDeviceList/InertialSensor/AccX/Sensor/Value");
    sensorNames.push_back("Device/SubDeviceList/InertialSensor/AccY/Sensor/Value");
    sensorNames.push_back("Device/SubDeviceList/InertialSensor/AccZ/Sensor/Value");
    
    sensorNames.push_back("Device/SubDeviceList/RFoot/Bumper/Left/Sensor/Value");
    sensorNames.push_back("Device/SubDeviceList/RFoot/Bumper/Right/Sensor/Value");
    sensorNames.push_back("Device/SubDeviceList/LFoot/Bumper/Left/Sensor/Value");
    sensorNames.push_back("Device/SubDeviceList/LFoot/Bumper/Right/Sensor/Value");
    sensorNames.push_back("Device/SubDeviceList/Chestboard/Button/Sensor/Value");

    sensorNames.push_back("Device/SubDeviceList/US/Left/Sensor/Value");
    sensorNames.push_back("Device/SubDeviceList/US/Right/Sensor/Value");

    fastAccess.ConnectToVariables( pBroker,sensorNames,false );

    std::cout << "NaoQiInterface::initFastAccess() - Fast memory access is initialized" << std::endl;

    // Initialize the ALValue used in setting stiffness
    body.arraySetSize(jointNames.size());
    bodyStiffness.arraySetSize(jointNames.size());
    for (unsigned int i = 0; i < jointNames.size(); i++) {
      body[i] = jointNames[i];
    }

    // Assumption: the first 2 elements are the head
    float stiffness = configFile.getFloat("motion/stiffness/head", 0.8f);
    bodyStiffness[0] = stiffness;
    bodyStiffness[1] = stiffness;

    stiffness = configFile.getFloat("motion/stiffness/body", 0.8f);
    for (unsigned int i = 2; i < jointNames.size(); i++) {
      bodyStiffness[i] = stiffness;
    }
}

bool NaoQiInterface::initialize( AL::ALPtr<AL::ALBroker> pBroker )
{
    fdShm = shm_open( SHAREDMEMORYNAME, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR );
    if( fdShm == -1 )
    {
        std::cout << "NaoQiInterface::initialize() - The file descriptor for the shared memory could not be opened." << std::endl;
        deinitialize();
        return false;
    }

    if ( ftruncate( fdShm, sizeof(SharedInformation) ) == -1 )
    {
        std::cout << "NaoQiInterface::initialize() - The size of the shared memory file descriptor is invalid." << std::endl;
        deinitialize();
        return false;
    }

    void* temp = mmap(NULL, sizeof(SharedInformation), PROT_READ | PROT_WRITE, MAP_SHARED, fdShm, 0);
    sharedInfo = new(temp) SharedInformation();
    //sharedInfo = (SharedInformation*) mmap(NULL, sizeof(SharedInformation), PROT_READ | PROT_WRITE, MAP_SHARED, fdShm, 0);
    if ( sharedInfo == MAP_FAILED )
    {
        std::cout << "NaoQiInterface::initialize() - mmap() failed." << std::endl;
        deinitialize();
        return false;
    }
    //memset(sharedInfo, 0, sizeof(SharedInformation));

    semaphore = sem_open(SEMAPHORENAME, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR, 1 );
    if ( semaphore == SEM_FAILED )
    {
        std::cout << "NaoQiInterface::initialize() - The semaphore could not be created." << std::endl;
        deinitialize();
        return false;
    }

    sem_init(semaphore,0,1);

    try
    {
        motionProxy=pBroker->getMotionProxy();
        motionProxy->setWalkArmsEnable(false, false);

        if (IGNORE_FSR) {
          AL::ALValue footContactCommand;
          footContactCommand.arraySetSize(1);
          footContactCommand[0].arraySetSize(2);
          footContactCommand[0][0] = std::string("ENABLE_FOOT_CONTACT_PROTECTION");
          footContactCommand[0][1] = false;
          motionProxy->setMotionConfig(footContactCommand);
        }
        //motionProxy = new AL::ALMotionProxy(pBroker);
    }
    catch(AL::ALError& e)
    {
        std::cout<<"NaoQiInterface::initialize() - Creating a proxy to ALMotion failed with error: " << e.toString() << std::endl;
    }

    staticActionPlayer.setMotionProxy( motionProxy );

    initFastAccess( pBroker );

    ledController = new LEDController(pBroker, configFile, log);
    soundController = new SoundController(pBroker);

    try
    {
        dcmProxy = new AL::DCMProxy(pBroker);
        AL::ALValue delta=0;
        unsigned long sysTime=log.getTimestamp();
        unsigned long dcmTime = dcmProxy->getTime(delta);
        DCMTimeOffset = dcmTime - sysTime;
        std::cout<<"NaoQiInterface::initialize() - A proxy to DCM has been created. The time difference between DCM and system is " << DCMTimeOffset << std::endl;
    }
    catch(AL::ALError& e)
    {
        std::cout << "NaoQiInterface::initialize() - Creating a proxy to DCM failed with error: " << e.toString() << std::endl;
    }

    if ( semaphore != SEM_FAILED )
    {
        sem_trywait(semaphore);
        sharedInfo->command.getMotionCommand().noBodyCommand();
        sharedInfo->command.getLEDCommand().clear();
        sem_post(semaphore);
    }

    cxA  = configFile.getFloat("motion/velocitymapping/x/a",0);
    cxB  = configFile.getFloat("motion/velocitymapping/x/b",0);

    cyA  = configFile.getFloat("motion/velocitymapping/y/a",0);
    cyB  = configFile.getFloat("motion/velocitymapping/y/b",0);

    ctA  = configFile.getFloat("motion/velocitymapping/turn/a",0);
    ctB  = configFile.getFloat("motion/velocitymapping/turn/b",0);
   
    maxX = configFile.getFloat("motion/velocity/max/x",0);
    maxY = configFile.getFloat("motion/velocity/max/y",0);
    maxT = configFile.getFloat("motion/velocity/max/turn",0);
    
    std::vector<float>currPosition = motionProxy->getRobotPosition(false);
    if ( semaphore != SEM_FAILED )
    {
        sem_trywait(semaphore); 
        sharedInfo->lastPosition.setPosition(Vector2D(currPosition[0],currPosition[1]));
        sharedInfo->lastPosition.setAngle(currPosition[2]);
        sem_post(semaphore);
    }
    else
    {
        std::cout << "NaoQiInterface::initialize() - semaphore error" << std::endl;
    }

    maxTrapezoidCommand.arraySetSize(1);
    maxTrapezoidCommand[0].arraySetSize(2);
    maxTrapezoidCommand[0][0] = std::string("WALK_MAX_TRAPEZOID");
    maxTrapezoidCommand[0][1] = MAX_TRAPEZOID_NORMAL;

    maxThetaCommand.arraySetSize(1);
    maxThetaCommand[0].arraySetSize(2);
    maxThetaCommand[0][0] = std::string("WALK_MAX_STEP_THETA");
    maxThetaCommand[0][1] = MAX_THETA_NORMAL;

    std::cout << "NaoQiInterface::initialize() - completed initialization" << std::endl;

    return true;
}

bool NaoQiInterface::initializeSonar() {
  if ( dcmProxy != NULL )
  {
      try
      {
        // Switch on ultrasound
        dcmSonarCommand.arraySetSize(3);
        dcmSonarCommand[0]=std::string("Device/SubDeviceList/US/Actuator/Value");
        dcmSonarCommand[1]=std::string("ClearAll");

        dcmSonarCommand[2].arraySetSize(1);
        dcmSonarCommand[2][0].arraySetSize(2);
        dcmSonarCommand[2][0][0] = 68.0;
        dcmSonarCommand[2][0][1] = dcmProxy->getTime(0);
        dcmProxy->set(dcmSonarCommand);

        std::cout<<"NaoQiInterface::initialize() - DCM aliases for actuators have been created. " << std::endl;
      }
      catch(AL::ALError& e)
      {
          std::cout<<"Creating an alias in DCM failed with error: "<<e.toString()<<std::endl;
          return false;
      }

  }

  return true;
}

void NaoQiInterface::deinitialize()
{
  if (!initialized) {
    return;
  }

    if ( sharedInfo != MAP_FAILED )
    {
        munmap( sharedInfo, sizeof(SharedInformation) );
        sharedInfo = (SharedInformation*) MAP_FAILED;
    }

    if ( fdShm != -1 )
    {
        close(fdShm);
        fdShm = -1;
    }

    if ( semaphore != SEM_FAILED )
    {
        sem_close(semaphore);
        semaphore = SEM_FAILED;
    }


}

void NaoQiInterface::sit()
{
    if ( !sitting )
    {
        //do sitting action
        if ( !staticActionPlayer.isActive() )
        {
            staticActionPlayer.sitDown();
            motionProxy->post.stiffnessInterpolation("Body",0.0, 5 );
            stiffnessSet = false;
            sitting = true;
            cmurfsOnline = false;
        }

    }
}

void NaoQiInterface::stand()
{
    if ( sitting )
    {
        //do standing action
        if ( !staticActionPlayer.isActive() )
        {
//            motionProxy->post.setStiffnesses("Body",configFile.getFloat("motion/stiffness/body",0.8f));
            motionProxy->post.setStiffnesses(body, bodyStiffness);
//            motionProxy->post.setStiffnesses("LArm",configFile.getFloat("motion/stiffness/body",0.8f));
//            motionProxy->post.setStiffnesses("RArm",configFile.getFloat("motion/stiffness/body",0.8f));
//            motionProxy->post.setStiffnesses("LLeg",configFile.getFloat("motion/stiffness/body",0.8f));
//            motionProxy->post.setStiffnesses("RLeg",configFile.getFloat("motion/stiffness/body",0.8f));
//            motionProxy->post.setStiffnesses("Head",configFile.getFloat("motion/stiffness/head",0.8f));
            staticActionPlayer.standUp();
            stiffnessSet = true;
            sitting = false;
            cmurfsOnline = true;
        }

    }
}

void NaoQiInterface::updateRobotState()
{
   
    HMatrix neckTransform( motionProxy->getTransform("Torso",2,true) );
    // Move from the torso to the base of the neck
    neckTransform.mul(&torsoToNeck);

    HMatrix camTransform = neckTransform;
    HMatrix scratch;
    // Rotate according to head pan/tilt
    scratch.initRotZ(sensorValues[HeadYaw]);
    camTransform.mul(&scratch);
    scratch.initRotY(sensorValues[HeadPitch]);
    camTransform.mul(&scratch);

    // Transform to the current camera
    if (sharedInfo->state.isUsingBottomCamera()) {
      camTransform.mul(&neckToBottomCamera);
    }
    else {
      camTransform.mul(&neckToTopCamera);
    }

    sharedInfo->state.setTransformationFromCamera( camTransform );
    sharedInfo->state.setTransformationFromNeck  ( neckTransform );

    
    //Buttons and pan&tilt
    sharedInfo->state.setChestButton( (sensorValues[ ChestButton ] > 0 ) );
    sharedInfo->state.setLeftFootButton( ( sensorValues[ LeftBumperLeft ] > 0 || sensorValues[ LeftBumperRight ] > 0 ) );
    sharedInfo->state.setRightFootButton( ( sensorValues[ RightBumperLeft ] > 0 || sensorValues[ RightBumperRight ] > 0 ) );

    sharedInfo->state.setHeadPan( sensorValues[ HeadYaw ] );
    sharedInfo->state.setHeadTilt( sensorValues[ HeadPitch ] );
    


    //Odometry
    std::vector<float>currPosition = motionProxy->getRobotPosition(true);
    Vector2D position = Vector2D(currPosition[0] * 100.0f,currPosition[1] * 100.0f);
    sharedInfo->currPosition.setPosition(position);
    sharedInfo->currPosition.setAngle(currPosition[2]);

    // If CMurfs is not online, we need to discard all odometry changes
    if (!cmurfsOnline) {
      sharedInfo->lastPosition = sharedInfo->currPosition;
    }

    Vector2D trans = sharedInfo->lastPosition.convertGlobalToRelative(position);
    float angle = sharedInfo->lastPosition.convertGlobalAngleToRelative(currPosition[2]);

//    printf("last position = (%.1f, %.1f), %.1f\n",
//           V2COMP(sharedInfo->lastPosition.getPosition()),
//           TO_DEG(sharedInfo->lastPosition.getAngle()));
//    printf("current position = (%.1f, %.1f), %.1f\n",
//           V2COMP(position),
//           TO_DEG(currPosition[2]));
//    printf("odom update = (%.1f, %.1f), %.1f\n",
//           V2COMP(trans),
//           TO_DEG(angle));

    //if a static action has just finished
    if ( lastStaticActionType != StaticAction::actNone && staticActionType == StaticAction::actNone )
    {
        //std::cout << "static action " << lastStaticActionType << " is finished" << std::endl;
        motionLib.getMotionOdometry(lastStaticActionType,trans,angle);
        sharedInfo->state.setOdometryTranslation(trans);
        sharedInfo->state.setOdometryRotation(angle);

        //std::cout << "restoring stiffness" << std::endl;
//        motionProxy->post.setStiffnesses("Body",configFile.getFloat("motion/stiffness/body",0.8f));
        // Restore the stiffness only if the robot is already stiff
        std::vector<float> stiffnesses = motionProxy->getStiffnesses("LHipYawPitch");
        if ((stiffnesses.size() > 0) && (stiffnesses[0] > 0)) {
          motionProxy->post.setStiffnesses(body, bodyStiffness);
        }
    }
    else
//    if ( bodyCommandType == bcWalk || bodyCommandType == bcWalkTo )
    {
        sharedInfo->state.setOdometryTranslation(trans);
        sharedInfo->state.setOdometryRotation(angle);
    }
    
    // Ultrasound readings
    if (initializedSonar) {
      sharedInfo->state.setUltraSound(sensorValues[SonarLeft], sensorValues[SonarRight]);
    }
    else {
      sharedInfo->state.setUltraSound(0, 0);
    }
    // Body angle
    sharedInfo->state.setBodyAngle(sensorValues[AngleX], sensorValues[AngleY]);

    if (IGNORE_FSR) {
      sharedInfo->state.setRobotOnGround(true);
    }
    else {
      if ( ((sensorValues[FSR_LeftFL] + sensorValues[FSR_LeftFR] + sensorValues[FSR_LeftRL] + sensorValues[FSR_LeftRR]) < FSR_THRESHOLD) &&
           ((sensorValues[FSR_RightFL] + sensorValues[FSR_RightFR] + sensorValues[FSR_RightRL] + sensorValues[FSR_RightRR]) < FSR_THRESHOLD) &&
             sharedInfo->state.isRobotOnGround()){
  //      std::cout << "Robot is picked up" << std::endl;
        sharedInfo->state.setRobotOnGround(false);
      }
      if ( ((sensorValues[FSR_LeftFL] + sensorValues[FSR_LeftFR] + sensorValues[FSR_LeftRL] + sensorValues[FSR_LeftRR]) >= FSR_THRESHOLD) &&
           ((sensorValues[FSR_RightFL] + sensorValues[FSR_RightFR] + sensorValues[FSR_RightRL] + sensorValues[FSR_RightRR]) >= FSR_THRESHOLD) &&
           !sharedInfo->state.isRobotOnGround()){
  //      std::cout << "Robot is on the ground" << std::endl;
        sharedInfo->state.setRobotOnGround(true);
      }
    }


    sharedInfo->state.setBodyCommandType( bodyCommandType );
    sharedInfo->state.setStaticActionType( staticActionType );
}

void NaoQiInterface::sense()
{

    if (!initialized) {
      if (initialStartupTime == 0) {
        initialStartupTime = log.getTimestamp();
      }
      else if (log.getTimestamp() - initialStartupTime > WAIT_FOR_NAOQI_TIME) {
        initialized = initialize(pBroker);
      }

      if (!initialized) {
        return;
      }
    }
    if (!initializedSonar) {
      if (log.getTimestamp() - initialStartupTime > WAIT_FOR_NAOQI_SONAR_TIME) {
        initializedSonar = initializeSonar();
      }
    }

    if ( semaphore != SEM_FAILED )
    {
        //read sensors
        sensorValues.clear();
        fastAccess.GetValues(sensorValues);

        sem_trywait(semaphore); 
        //put sensor readings into the shared memory
        updateRobotState();
    
        sem_post(semaphore);

        lastStaticActionType = staticActionType;
//        std::cout << "SonarLeft: " << sensorValues[SonarLeft] << " SonarRight: " << sensorValues[SonarRight] << std::endl;
//        std::cout << "Left Foot: " << sensorValues[FSR_LeftFL] + sensorValues[FSR_LeftFR] + sensorValues[FSR_LeftRL] + sensorValues[FSR_LeftRR] << ", "
//                  << "Right Foot: " << sensorValues[FSR_RightFL] + sensorValues[FSR_RightFR] + sensorValues[FSR_RightRL] + sensorValues[FSR_RightRR]
//                  << std::endl;

    }
    else
    {
        std::cout << "NaoQiInterface::sense() - semaphore error" << std::endl;
    }

}

void NaoQiInterface::act()
{
    if (!initialized) {
      return;
    }

    unsigned long lastUpdateTime;

    if ( semaphore != SEM_FAILED )
    {
        sem_trywait(semaphore);
        //read actuation command
        
        memcpy(&command, &(sharedInfo->command), sizeof(Command));
        lastUpdateTime = sharedInfo->lastUpdateTime;
        sem_post(semaphore);
    
        unsigned long cmurfsElapsed = log.getTimestamp() - lastUpdateTime;
        if (cmurfsElapsed > CMURFS_TIMEOUT) {
          if (cmurfsOnline) {
            if (!sitting) {
              sit();
            }
          }
          else {
            unsigned long timeSinceLastUpdate = log.getTimestamp() - updateAngleTimestamp;
            if (timeSinceLastUpdate > CMURFS_OFFLINE_UPDATE_ANGLE_PERIOD) {
              updateAngleTimestamp = log.getTimestamp();

              std::vector<float> angles(&sensorValues[0], &sensorValues[jointNames.size()]);
              motionProxy->post.setAngles(jointNames, angles, 1.0);
            }
          }
          return;
        }
        if (!cmurfsOnline) {
          if (cmurfsElapsed <= CMURFS_TIMEOUT) {
            if (sitting) {
              stand();
            }
          }
          return;
        }
        
        if ( !stiffnessSet )
        {
            
            motionProxy->post.stiffnessInterpolation("Body",configFile.getFloat("motion/stiffness/body",0.8f), 0.5 );
            motionProxy->post.stiffnessInterpolation("Head",configFile.getFloat("motion/stiffness/head",0.8f), 0.5 );
            stiffnessSet = true;
        }
        
        //actuate
        MotionCommand& motionCommand = command.getMotionCommand();

        int bodyCommand = motionCommand.getBodyCommand();

        float vx;
        float vy;
        float vt;
        Walk& walk = motionCommand.getWalk();
        WalkTo& walkTo = motionCommand.getWalkTo();
        StaticAction& act = motionCommand.getStaticAction();

        bool isWalkToRunning = false;
        if (walkToTaskID >= 0) {
          if (motionProxy->isRunning(walkToTaskID)
           || motionProxy->walkIsActive()) {
            isWalkToRunning = true;
          }
          else {
            walkToTaskID = -1;
          }
        }

        switch ( bodyCommand )
        {
            case bcNoBodyCommand:
                //std::cout << "bcNoBodyCommand" << std::endl;
                bodyCommandType = bcNoBodyCommand;
                break;
            case bcStandNeutral:
              //std::cout << "bcStandNeutral" << std::endl;
              if (!staticActionPlayer.isActive() && !isWalkToRunning) {
                // Make sure we don't have any walk targets
                motionProxy->setWalkTargetVelocity( 0.0f, 0.0f, 0.0f, 1.0f);
                // Check that the walk has completed execution (so the feet
                // are on the ground)
                if (!motionProxy->walkIsActive()) {
                  // Check that the feet are placed together
                  bool moveLeftFoot;
                  if (areFeetTogether(moveLeftFoot)) {
                    bodyCommandType = bcStandNeutral;
                  }
                  else {
                    // Change which leg to move (if specified)
                    StandNeutral::WhichLeg whichLeg = motionCommand.getStandNeutral().getWhichLeg();
                    if (whichLeg != StandNeutral::Any) {
                      moveLeftFoot = (whichLeg == StandNeutral::Left);
                    }
                    // Move the left foot to the right foot
                    if (moveLeftFoot) {
                      motionProxy->post.stepTo("LLeg", 0, 0, 0);
                    }
                    // Move the right foot to the left foot
                    else {
                      motionProxy->post.stepTo("RLeg", 0, 0, 0);
                    }
                    bodyCommandType = bcNoBodyCommand;
                  }
                }
                else {
                  bodyCommandType = bcNoBodyCommand;
                }
              }
              break;

            case bcStopWalk:
                //std::cout << "bcStopWalk" << std::endl;
                if (!staticActionPlayer.isActive() && !isWalkToRunning) {
                  // Check the max trapezoid value
                  if (maxTrapezoidValue != MAX_TRAPEZOID_NORMAL) {
                    setMaxTrapezoid(MAX_TRAPEZOID_NORMAL);
                  }
                  // Check the max theta value
                  if (maxThetaValue != MAX_THETA_NORMAL) {
                    setMaxTheta(MAX_THETA_NORMAL);
                  }
                  // Make sure we don't have any walk targets
                  motionProxy->setWalkTargetVelocity( 0.0f, 0.0f, 0.0f, 1.0f);
                  // Check that the walk has completed execution (so the feet
                  // are on the ground)
                  if (!motionProxy->walkIsActive()) {
                    bodyCommandType = bcStopWalk;
                  }
                  else {
                    bodyCommandType = bcNoBodyCommand;
                  }
                }
                break;
            case bcWalk:
                //std::cout << "bcWalk" << std::endl;
                if (!staticActionPlayer.isActive() && !isWalkToRunning)
                {

                  // Check the max trapezoid value
                  if (maxTrapezoidValue != MAX_TRAPEZOID_NORMAL) {
                    setMaxTrapezoid(MAX_TRAPEZOID_NORMAL);
                  }
                  // Check the max theta value
                  if (maxThetaValue != MAX_THETA_NORMAL) {
                    setMaxTheta(MAX_THETA_NORMAL);
                  }

                  // When we're not calibrating
                    vx = cxA * walk.getVelocityX() + cxB;
                    vy = cyA * walk.getVelocityY() + cyB;
                    vt = ctA * walk.getVelocityTheta() + ctB;

                    if ( fabs(vx) > 1.0f || fabs(vy) > 1.0f )
                    {
                        if ( fabs(vx) > fabs(vy) )
                        {
                            vy /= fabs(vx);
                            vx /= fabs(vx);
                        }
                        else
                        {
                            vx /= fabs(vy);
                            vy /= fabs(vy);
                        }
                    }
                    // Bound vx, vy, vt
                    vx = bound(vx, -1.0f, 1.0f);
                    vy = bound(vy, -1.0f, 1.0f);
                    vt = bound(vt, -1.0f, 1.0f);

                    // ----------------------------------------------

//                    // When we're calibrating
//
//                    // Bound vx, vy, vt
//                    vx = bound(walk.getVelocityX(), -1.0f, 1.0f);
//                    vy = bound(walk.getVelocityY(), -1.0f, 1.0f);
//                    vt = bound(walk.getVelocityTheta(), -1.0f, 1.0f);
//
//                    std::cout << "bcWalk" << vx << " " << vy << " " << vt << std::endl;

                    // ----------------------------------------------

                    motionProxy->setWalkTargetVelocity( vx, vy, vt, 1.0);
                    bodyCommandType = bcWalk;
                }
                break;
            case bcWalkTo:
                //std::cout << "bcWalkTo" << std::endl;
                if ( !staticActionPlayer.isActive() && !isWalkToRunning )
                {
                    float theta = walkTo.getPositionTheta();
                    // Naoqi walkTo takes in theta that must be between -3.1415 to 3.1415
                    theta = bound(theta, -3.1415, 3.1415);

                    // Check the max trapezoid value
                    if (theta == 0) {
                      if (maxTrapezoidValue != MAX_TRAPEZOID_WALKTO) {
                        setMaxTrapezoid(MAX_TRAPEZOID_WALKTO);
                      }
                    }
                    else {
                      if (maxThetaValue != MAX_THETA_WALKTO) {
                        setMaxTheta(MAX_THETA_WALKTO);
                      }
                    }

                    // Convert from cm to m
                    walkToTaskID = motionProxy->post.walkTo(walkTo.getPositionX() / 100.0f, walkTo.getPositionY() / 100.0f,  theta);
                }
                

                break;
            case bcStaticAction:
                //std::cout << "bcStaticAction" << std::endl;
                if ( !isWalkToRunning )
                {
                    // Set the stiffness only if the robot is already stiff
                    std::vector<float> stiffnesses = motionProxy->getStiffnesses("LHipYawPitch");
                    if ((stiffnesses.size() > 0) && (stiffnesses[0] > 0)) {
                      motionProxy->post.setStiffnesses("Body",configFile.getFloat("motion/stiffness/keyframe",1.0f));
                    }
                    //std::cout << "Setting stiffness to 1" << std::endl;

                    //std::cout << "actively playing keyframe: " << staticActionPlayer.isActive() << std::endl;

                    switch(act.getActionID())
                    {
                        case StaticAction::actForwardDribbleRight:
                            staticActionPlayer.forwardDribbleRight();
                            break;
                        case StaticAction::actForwardDribbleLeft:
                            staticActionPlayer.forwardDribbleLeft();
                            break;
                        case StaticAction::actForwardKickRight:
                            staticActionPlayer.forwardKickRight();
                            break;
                        case StaticAction::actForwardKickLeft:
                            staticActionPlayer.forwardKickLeft();
                            break;
                        case StaticAction::actForwardStrongRight:
                            staticActionPlayer.forwardStrongRight();
                            break;
                        case StaticAction::actForwardStrongLeft:
                            staticActionPlayer.forwardStrongLeft();
                            break;

                        case StaticAction::actSideMiniToRightIn:
                            staticActionPlayer.sideMiniToRightIn();
                            break;
                        case StaticAction::actSideMiniToLeftIn:
                            staticActionPlayer.sideMiniToLeftIn();
                            break;
//                        case StaticAction::actSideWeakToRightIn:
//                            staticActionPlayer.sideWeakToRightIn();
//                            break;
//                        case StaticAction::actSideWeakToLeftIn:
//                            staticActionPlayer.sideWeakToLeftIn();
//                            break;
//                        case StaticAction::actSideKickToRightIn:
//                            staticActionPlayer.sideKickToRightIn();
//                            break;
//                        case StaticAction::actSideKickToLeftIn:
//                            staticActionPlayer.sideKickToLeftIn();
//                            break;
                        case StaticAction::actSideStrongToRightIn:
                            staticActionPlayer.sideStrongToRightIn();
                            break;
                        case StaticAction::actSideStrongToLeftIn:
                            staticActionPlayer.sideStrongToLeftIn();
                            break;

                        case StaticAction::actSideKickToRightOut:
                            staticActionPlayer.sideKickToRightOut();
                            break;
                        case StaticAction::actSideKickToLeftOut:
                            staticActionPlayer.sideKickToLeftOut();
                            break;

//                        case StaticAction::actObliqueKickToRight:
//                            staticActionPlayer.obliqueKickToRight();
//                            break;
//                        case StaticAction::actObliqueKickToLeft:
//                            staticActionPlayer.obliqueKickToLeft();
//                            break;

                        case StaticAction::actSpreadOut:
                            staticActionPlayer.spreadOut();
                            break;
                        case StaticAction::actGetUpFromFront:
                            staticActionPlayer.getUpFromFront();
                            break;
                        case StaticAction::actGetUpFromBack:
                            staticActionPlayer.getUpFromBack();
                            break;
                        case StaticAction::actTest:
                            staticActionPlayer.testAction();
                            break;
                        case StaticAction::actNone:
                            break;
                        default:
                            break;
                    }
                }
                break;
        }

        if ( staticActionPlayer.isActive() )
        {
            bodyCommandType = bcStaticAction;
        }
        
        staticActionType = staticActionPlayer.getCurrentAction();


        if ( isWalkToRunning )
        {
            bodyCommandType = bcWalkTo;
        }

        int headCommand = command.getMotionCommand().getHeadCommand();
        if ( headCommand == hcNoHeadCommand )
        {
        }
        else if ( headCommand == hcHeadAngles )
        {
            HeadAngles& angles = motionCommand.getHeadAngles();
            float panDelta = fabs(headAngles[0] - angles.getPan());
            float tiltDelta = fabs(headAngles[1] - angles.getTilt());

            if ((panDelta >= MIN_PAN_DELTA) || (tiltDelta >= MIN_TILT_DELTA)) {
              // Kill the previous task (if any)
              // We can't seem to kill head tasks if a keyframe is playing
              if ((headAnglesTaskID > 0) && !staticActionPlayer.isActive() && motionProxy->isRunning(headAnglesTaskID)) {
                motionProxy->killTask(headAnglesTaskID);
                //std::cout << "Tried to kill " << headAnglesTaskID << std::endl;
              }

              headAngles[0] = angles.getPan();
              headAngles[1] = angles.getTilt();
              headSpeed     = angles.getSpeed();

              if (staticActionPlayer.isActive()) {
                headSpeed = bound(headSpeed, 0.0f, KEYFRAME_MAX_HEAD_SPEED);
              }
              else {
                headSpeed = bound(headSpeed, 0.0f, MAX_HEAD_SPEED);
              }

              headAnglesTaskID = motionProxy->post.setAngles("Head", headAngles, headSpeed);

              // We store the current head angle if a keyframe is being played,
              // instead of the destination angles.
              // In future, check if the static action involves the head
              if (staticActionPlayer.isActive()) {
                headAngles[0] = sensorValues[HeadYaw];
                headAngles[1] = sensorValues[HeadPitch];
              }
            }
        }
        else if (headCommand == hcHeadStiffness) {
          HeadStiffness& headStiffness = motionCommand.getHeadStiffness();
          float stiffness = headStiffness.getStiffness();
          stiffness = bound(stiffness, 0.0f, 1.0f);

          // Assumption: the first 2 joints are the head
          // We set it here so the head stiffness is set correctly after a keyframe
          bodyStiffness[0] = stiffness;
          bodyStiffness[1] = stiffness;

          motionProxy->post.setStiffnesses("Head", stiffness);
        }
        else if (headCommand == hcRestoreHeadStiffness) {
          float stiffness = configFile.getFloat("motion/stiffness/head",0.8f);
          stiffness = bound(stiffness, 0.0f, 1.0f);

          // Assumption: the first 2 joints are the head
          // We set it here so the head stiffness is set correctly after a keyframe
          bodyStiffness[0] = stiffness;
          bodyStiffness[1] = stiffness;

          motionProxy->post.setStiffnesses("Head", stiffness);
        }
        
        
        //leds
        const LEDCommand & ledCommand = command.getLEDCommand();
        ledController->executeLEDCommand(ledCommand);

        //sound/speech
        const SoundCommand& soundCommand = command.getSoundCommand();
        soundController->executeSoundCommand(soundCommand);
    }
    else
    {
        std::cout << "NaoQiInterface::act() - semaphore error" << std::endl;
    }

}

bool NaoQiInterface::areFeetTogether(bool & moveLeftFoot) {
  std::vector<float> leftPosition = motionProxy->getPosition("LLeg", 0, true);
  std::vector<float> rightPosition = motionProxy->getPosition("RLeg", 0, true);

  // convert from m to cm
  float feetXDists = (leftPosition[0] - rightPosition[0]) * 100;
  float feetYDists = (leftPosition[1] - rightPosition[1]) * 100;

//  std::cout << "Feet dist x = " << feetXDists << ", y = " << feetYDists << std::endl;

  moveLeftFoot = (feetXDists < 0);

  return (fabs(feetXDists) < FEET_TOGETHER_DIST_X)
      && (fabs(feetYDists) < FEET_TOGETHER_DIST_Y);
}

void NaoQiInterface::setMaxTrapezoid(float value) {
  maxTrapezoidCommand[0][1] = value;
  motionProxy->setMotionConfig(maxTrapezoidCommand);

  maxTrapezoidValue = value;
  //std::cout << "Setting max trapezoid: " << value << std::endl;
}

void NaoQiInterface::setMaxTheta(float value) {
  maxThetaCommand[0][1] = value;
  motionProxy->setMotionConfig(maxThetaCommand);

  maxThetaValue = value;
  //std::cout << "Setting max trapezoid: " << value << std::endl;
}
