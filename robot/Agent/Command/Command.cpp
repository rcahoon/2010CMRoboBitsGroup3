#include "Command.h"

Command::Command()
  : motionCommand(),
    soundCommand(),
    ledCommand(),
    usingBottomCamera(true) {
}

Command::~Command() {
}

void Command::clear() {
  motionCommand.clear();
  soundCommand.clear();
  ledCommand.clear();
  // We shouldn't reset which camera to use
}

MotionCommand & Command::getMotionCommand() {
  return motionCommand;
}

SoundCommand & Command::getSoundCommand() {
  return soundCommand;
}

LEDCommand & Command::getLEDCommand() {
  return ledCommand;
}

void Command::useBottomCamera(bool _usingBottomCamera) {
  usingBottomCamera = _usingBottomCamera;
}

const MotionCommand & Command::getMotionCommand() const {
  return motionCommand;
}

const SoundCommand & Command::getSoundCommand() const {
  return soundCommand;
}

const LEDCommand & Command::getLEDCommand() const {
  return ledCommand;
}

bool Command::shouldUseBottomCamera() const {
  return usingBottomCamera;
}
