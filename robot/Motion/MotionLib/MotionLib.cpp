#include <fstream>
#include <sstream>
#include <math.h>

#include "MotionLib.h"
#include "Agent/Command/MotionCommand/BodyCommand/StaticAction.h"
#include "shared/ConfigFile/ConfigFile.h"
#include "shared/UsefulMacros.h"

// Define the logging constants
#define COMPONENT OTHERS
//#define CLASS_LOG_LEVEL LOG_LEVEL_DEBUG
#include "Log/LogSettings.h"

MotionLib::MotionLib(ConfigFile & configFile, Log & _log)
  : motionLibFileName(configFile.getPath("motion/MotionLib","motion/motionLib.txt")),
    NON_FORWARD_KICK_ORBIT_WEIGHT_MULTIPLIER(configFile.getFloat("motion/motionLib/nonForwardKickOrbitWeightMultiplier", 1.5f)),
    log(_log),
    motionLibrary(),
    motionMask(),
    through(true),
    useAngle(false),
    minAngleValue(0),
    maxAngleValue(0),
    ballPosition(0,0),
    relTarget(0,0){
  loadMotionLib(configFile, motionLibFileName);
}

MotionLib::~MotionLib() {
  std::vector<MotionInfo *>::iterator iter;
  for (iter = motionLibrary.begin(); iter != motionLibrary.end(); iter++) {
    delete (*iter);
  }
  motionLibrary.clear();
}

void MotionLib::getMotionOdometry(StaticAction::StaticActionType type, Vector2D & translation, float & rotation) const {
  if ((type <= StaticAction::actNone) || (type >= StaticAction::numStaticActions) || ((unsigned int)type >= motionLibrary.size())) {
    translation.x = translation.y = 0;
    rotation = 0;
  }
  else {
    motionLibrary[(int)type - 1]->getOdometryChanges(translation, rotation);
  }
}

bool MotionLib::isForwardKick(StaticAction::StaticActionType type) {
  return (type == StaticAction::actForwardDribbleRight)
      || (type == StaticAction::actForwardDribbleLeft)
      || (type == StaticAction::actForwardKickRight)
      || (type == StaticAction::actForwardKickLeft)
      || (type == StaticAction::actForwardStrongRight)
      || (type == StaticAction::actForwardStrongLeft);
}

void MotionLib::clearMask() {
  motionMask.clear();
}

void MotionLib::addAllKicks() {
  clearMask();
  addForwardKicks();
  addSideKicks();
}

void MotionLib::addForwardKicks() {
  //Kick in motionLibrary does not include actNone, therefore -1 in index
  motionMask.push_back(motionLibrary[StaticAction::actForwardDribbleRight-1]);
  motionMask.push_back(motionLibrary[StaticAction::actForwardDribbleLeft-1]);

  motionMask.push_back(motionLibrary[StaticAction::actForwardKickRight-1]);
  motionMask.push_back(motionLibrary[StaticAction::actForwardKickLeft-1]);

  motionMask.push_back(motionLibrary[StaticAction::actForwardStrongRight-1]);
  motionMask.push_back(motionLibrary[StaticAction::actForwardStrongLeft-1]);
}

void MotionLib::addSideKicks() {
  //Kick in motionLibrary does not include actNone, therefore -1 in index
  motionMask.push_back(motionLibrary[StaticAction::actSideMiniToRightIn-1]);
  motionMask.push_back(motionLibrary[StaticAction::actSideMiniToLeftIn-1]);

//  motionMask.push_back(motionLibrary[StaticAction::actSideWeakToRightIn-1]);
//  motionMask.push_back(motionLibrary[StaticAction::actSideWeakToLeftIn-1]);
//
//  motionMask.push_back(motionLibrary[StaticAction::actSideKickToRightIn-1]);
//  motionMask.push_back(motionLibrary[StaticAction::actSideKickToLeftIn-1]);

  motionMask.push_back(motionLibrary[StaticAction::actSideStrongToRightIn-1]);
  motionMask.push_back(motionLibrary[StaticAction::actSideStrongToLeftIn-1]);

  motionMask.push_back(motionLibrary[StaticAction::actSideKickToRightOut-1]);
  motionMask.push_back(motionLibrary[StaticAction::actSideKickToLeftOut-1]);

}

void MotionLib::addObliqueKicks() {
  //Kick in motionLibrary does not include actNone, therefore -1 in index
//  motionMask.push_back(motionLibrary[StaticAction::actObliqueKickToRight-1]);
//  motionMask.push_back(motionLibrary[StaticAction::actObliqueKickToLeft-1]);
}

void MotionLib::addSideKicksToLeft() {
  //Kick in motionLibrary does not include actNone, therefore -1 in index
//  motionMask.push_back(motionLibrary[StaticAction::actSideWeakToLeftIn-1]);
//  motionMask.push_back(motionLibrary[StaticAction::actSideKickToLeftIn-1]);
  motionMask.push_back(motionLibrary[StaticAction::actSideMiniToLeftIn-1]);
  motionMask.push_back(motionLibrary[StaticAction::actSideStrongToLeftIn-1]);
  motionMask.push_back(motionLibrary[StaticAction::actSideKickToLeftOut-1]);
}

void MotionLib::addSideKicksToRight() {
  //Kick in motionLibrary does not include actNone, therefore -1 in index
//  motionMask.push_back(motionLibrary[StaticAction::actSideWeakToRightIn-1]);
//  motionMask.push_back(motionLibrary[StaticAction::actSideKickToRightIn-1]);
  motionMask.push_back(motionLibrary[StaticAction::actSideMiniToRightIn-1]);
  motionMask.push_back(motionLibrary[StaticAction::actSideStrongToRightIn-1]);
  motionMask.push_back(motionLibrary[StaticAction::actSideKickToRightOut-1]);
}

void MotionLib::addKick(StaticAction::StaticActionType type) {
  //Kick in motionLibrary does not include actNone, therefore -1 in index
  motionMask.push_back(motionLibrary[type-1]);
}

void MotionLib::getKickWalkTo(MotionInfo const *kick, Vector2D & walkTo, float & turnTo, bool separateDirs) {
  walkTo.zero();
  turnTo = 0;
  if (kick == NULL) {
    return;
  }

  if (!kick->isBallInKickRange(ballPosition)) {
    float minX, maxX, minY, maxY;
    kick->getBoundingBox(minX, maxX, minY, maxY);
    if (separateDirs) {
      if (ballPosition.x < minX || ballPosition.x > maxX)
         walkTo.x = ballPosition.x - maxX;
      if (ballPosition.y < minY || ballPosition.y > maxY) {
         if (ballPosition.y < minY) {
           walkTo.y = ballPosition.y - minY;
         } else {
           walkTo.y = ballPosition.y - maxY;
         }
       }
    }
    else
    {
      // walk backwards first if the ball is to our side
      if (ballPosition.x < minX && (ballPosition.y < minY || ballPosition.y > maxY))
      {
        LOG_WARN("Walking backwards for kick.");
        walkTo.x = ballPosition.x - (maxX + 2 * (maxX - minX));
      }
      else
        walkTo.x = ballPosition.x - (maxX - (maxX - minX) / 2.0);

	  walkTo.y = ballPosition.y - (maxY - (maxY - minY) / 2.0);
    }

    turnTo = 0;
  }
  else {
    walkTo.zero();
    turnTo = 0;
  }
}

MotionInfo const * MotionLib::getBestKick(StaticAction::StaticActionType & type, Vector2D & walkTo, float & turnTo, bool separateDirs) {
  MotionInfo * kick(NULL);
  MotionInfo * bestKick(NULL);
  float score, bestScore(-INFINITY);

  for (std::vector<MotionInfo *>::iterator iter=motionMask.begin(); iter!=motionMask.end(); iter++){
    kick = (*iter);

    // Will the kick bring the ball in the right direction?
    if ( (!useAngle) && (!kick->isAngleGoodForKick(relTarget)) ){
      continue;
    } else if ( (useAngle) && (!kick->isAngleGoodForKick(minAngleValue, maxAngleValue))) {
      continue;
    }

    score = kick->getKickScore(relTarget, ballPosition, through, useAngle, minAngleValue, maxAngleValue);
    LOG_DEBUG("Kick %d got score %.1f", kick->getType(), score);
    if ((bestScore == -INFINITY) || (score > bestScore)) {
      bestScore = score;
      bestKick = kick;
    }
  }

  if (bestKick == NULL){
    type = StaticAction::actNone;
    walkTo.zero();
    turnTo = 0;
  } else {
    type = bestKick->getType();
    getKickWalkTo(bestKick, walkTo, turnTo, separateDirs);
  }

  return bestKick;
}

void MotionLib::setKickCriteria(const Vector2D & _relTarget, const Vector2D & _ballPosition, bool _through,
                                bool _useAngle, float _minAngleValue, float _maxAngleValue) {
  relTarget = _relTarget;
  ballPosition = _ballPosition;
  through = _through;
  useAngle = _useAngle;
  minAngleValue = _minAngleValue;
  maxAngleValue = _maxAngleValue;
}

void MotionLib::loadMotionLib(ConfigFile & configFile, const std::string & motionLibFileName){
  std::ifstream inputFile(motionLibFileName.c_str());
  std::string line;

  // Check if file was opened successfully
  if (!inputFile.good()) {
    LOG_ERROR("Failed to open motion library file: %s", motionLibFileName.c_str());
    return;
  }

  int lineNum = 0;

  while (getline(inputFile, line)) {

    lineNum++;

    if (inputFile.fail()) {
      LOG_ERROR("Error reading motion library file: %s at line number %d.", motionLibFileName.c_str(), lineNum);
      break;
    }

    // Trim off comments (starting with #) and spaces/tabs
    std::string trimmedLine = ConfigFile::trimString(line);

    // Check if the line is empty
    if (trimmedLine.size() == 0) {
      continue;
    }

    int index;
    StaticAction::StaticActionType type;
    //bounding box
    float minX, maxX, minY, maxY;
    //changes in odometry
    float odometryX, odometryY, odometryTheta;
    //effect from motion
    float resultR, resultTheta, deviationResultR, deviationResultTheta;
    //time to execute
    float timeToExecute;
    // motion type
    int motionType;

    std::istringstream trimmedLineStream(trimmedLine);
    trimmedLineStream >> index >> minX >> maxX >> minY >> maxY
                      >> odometryX >> odometryY >> odometryTheta
                      >> resultR >> resultTheta >> deviationResultR >> deviationResultTheta
                      >> timeToExecute >> motionType;

    if (trimmedLineStream.fail()) {
      LOG_WARN("Error parsing motions library file at line number %d", lineNum);
    }

    LOG_DEBUG("%d %f %f %f %f %f %f %f %f %f %f %f %f %d",
               index, minX, maxX, minY, maxY,
               odometryX, odometryY, odometryTheta,
               resultR, resultTheta, deviationResultR, deviationResultTheta,
               timeToExecute, motionType);

    if ((index <= 0) || (index >= StaticAction::numStaticActions)) {
      LOG_WARN("Error parsing StaticActionType in motions library file at line number %d.", lineNum);
      continue;
    }
    type = (StaticAction::StaticActionType)index;

    odometryTheta = TO_RAD(odometryTheta);
    resultTheta = TO_RAD(resultTheta);
    deviationResultTheta = TO_RAD(deviationResultTheta);

    motionLibrary.push_back(new MotionInfo(configFile,
                                           log,
                                           type,
                                           minX, maxX, minY, maxY,
                                           odometryX, odometryY, odometryTheta,
                                           resultR, resultTheta, deviationResultR, deviationResultTheta,
                                           timeToExecute, motionType));
  }

  LOG_INFO("Loaded motion library.");
}

float MotionLib::getOrbitAngle(const Vector2D & target, const Vector2D & ballPosition) {
  StaticAction::StaticActionType staticActionType;
  return getOrbitAngle(target, ballPosition, staticActionType);
}

float MotionLib::getOrbitAngle(const Vector2D & target, const Vector2D & ballPosition, StaticAction::StaticActionType & staticActionType) {
  float bestOrbitAngle(INFINITY), bestOrbitAngleFabs(INFINITY);
  MotionInfo *kick = NULL, *bestKick = NULL;

  float ballAngle = ballPosition.angle();
  Vector2D ballToTarget = target - ballPosition;

  LOG_DEBUG("ballPosition = (%.1f, %.1f), ballToTarget = (%.1f, %.1f), ballAngle = %.1f deg",
           V2COMP(ballPosition), V2COMP(ballToTarget), TO_DEG(ballAngle));

  for (std::vector<MotionInfo *>::iterator iter=motionMask.begin(); iter!=motionMask.end(); iter++){
    kick = (*iter);
    float kickTheta = kick->getResultTheta();
    Vector2D kickToTarget = ballToTarget.rotate(-kickTheta);
    float orbitAngle = kickToTarget.angle() - ballAngle;
    float orbitAngleFabs = fabs(orbitAngle);
    bool leftFoot;
    if (!kick->isAForwardKick(leftFoot)) {
      orbitAngleFabs *= NON_FORWARD_KICK_ORBIT_WEIGHT_MULTIPLIER;
    }

    LOG_DEBUG("Kick %d, kickTheta = %.1f deg, kickToTarget = (%.1f, %.1f), orbitAngle = %.1f deg",
             kick->getType(), TO_DEG(kickTheta), V2COMP(kickToTarget), TO_DEG(orbitAngle));

    if (orbitAngleFabs < bestOrbitAngleFabs) {
      bestOrbitAngle     = orbitAngle;
      bestOrbitAngleFabs = orbitAngleFabs;
      bestKick = kick;
    }
  }

  if (bestKick != NULL) {
    LOG_INFO("Best kick = %d", bestKick->getType());
    staticActionType = bestKick->getType();
  }
  else {
    staticActionType = StaticAction::actNone;
  }

  return bestOrbitAngle;
}
