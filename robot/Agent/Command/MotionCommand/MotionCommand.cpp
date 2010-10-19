#include "MotionCommand.h"
#include "MotionCommandList.h"

MotionCommand::MotionCommand()
  : headCommand(hcNoHeadCommand),
    bodyCommand(bcNoBodyCommand){
}

MotionCommand::~MotionCommand() {
  clear();
}

void MotionCommand::clear() {
  headCommand = hcNoHeadCommand;
  bodyCommand = bcNoBodyCommand;
}

int MotionCommand::getHeadCommand() {
  return headCommand;
}

int MotionCommand::getBodyCommand() {
  return bodyCommand;
}

void MotionCommand::noBodyCommand() {
  bodyCommand = bcNoBodyCommand;
}

NoBodyCommand& MotionCommand::getNoBodyCommand()
{
    return _noBodyCommand;
}

void MotionCommand::standNeutral(StandNeutral::WhichLeg whichLeg) {
  bodyCommand = bcStandNeutral;
  _standNeutral.setWhichLeg(whichLeg);
}

StandNeutral& MotionCommand::getStandNeutral()
{
    return _standNeutral;
}

void MotionCommand::stopWalk() {
  bodyCommand = bcStopWalk;
}

StopWalk& MotionCommand::getStopWalk()
{
    return _stopWalk;
}

void MotionCommand::walk(float vX, float vY, float vTheta) {
  _walk.setVelocity(vX, vY, vTheta);
  bodyCommand = bcWalk;
}

Walk& MotionCommand::getWalk()
{
    return _walk;
}

void MotionCommand::walkTo(float pX, float pY, float pTheta) {
  _walkTo.setPosition(pX, pY, pTheta);
  bodyCommand = bcWalkTo;
}

WalkTo& MotionCommand::getWalkTo()
{
    return _walkTo;
}

void MotionCommand::staticAction(StaticAction::StaticActionType actionID){
    _staticAction.setActionID( actionID );
    bodyCommand = bcStaticAction;
}

StaticAction& MotionCommand::getStaticAction(){
    return _staticAction;
}

void MotionCommand::noHeadCommand() {
  headCommand = hcNoHeadCommand;
}

NoHeadCommand& MotionCommand::getNoHeadCommand()
{
    return _noHeadCommand;
}

void MotionCommand::headAngles(float pan, float tilt, float speed) {
  _headAngles.setAngles(pan, tilt);
  _headAngles.setSpeed(speed);
  headCommand = hcHeadAngles;
}

HeadAngles& MotionCommand::getHeadAngles()
{
    return _headAngles;
}

void MotionCommand::setHeadStiffness(float stiffness) {
  _headStiffness.setStiffness(stiffness);
  headCommand = hcHeadStiffness;
}

HeadStiffness& MotionCommand::getHeadStiffness() {
  return _headStiffness;
}

void MotionCommand::restoreHeadStiffness() {
  headCommand = hcRestoreHeadStiffness;
}

RestoreHeadStiffness& MotionCommand::getRestoreHeadStiffness() {
  return _restoreHeadStiffness;
}
