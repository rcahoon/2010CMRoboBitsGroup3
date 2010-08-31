#ifndef COMMAND_H_
#define COMMAND_H_

#include "MotionCommand/MotionCommand.h"
#include "SoundCommand/SoundCommand.h"
#include "LEDCommand/LEDCommand.h"
#include "shared/UsefulMacros.h"

class Command {
public:
  Command();
  ~Command();

  /**
   * Clears the command.
   */
  void clear();

  /**
   * Retrieves a reference to the motion command.
   *
   * @return reference to the motion command
   */
  MotionCommand & getMotionCommand();

  /**
   * Retrieves a reference to the sound command.
   *
   * @return reference to the sound command
   */
  SoundCommand & getSoundCommand();

  /**
   * Retrieves a reference to the LED command.
   *
   * @return reference to the LED command
   */
  LEDCommand & getLEDCommand();

  /**
   * Sets which camera to use.
   *
   * @param _useBottomCamera true: use bottom camera, false: use top camera
   */
  void useBottomCamera(bool _usingBottomCamera = true);

  /**
   * Retrieves a const reference to the motion command.
   *
   * @return const reference to the motion command
   */
  const MotionCommand & getMotionCommand() const;

  /**
   * Retrieves a const reference to the speech command.
   *
   * @return const reference to the speech command
   */
  const SoundCommand & getSoundCommand() const;

  /**
   * Retrieves a const reference to the LED command.
   *
   * @return const reference to the LED command
   */
  const LEDCommand & getLEDCommand() const;

  /**
   * Returns which camera should be used.
   *
   * @return true: use bottom camera, false: use top camera
   */
  bool shouldUseBottomCamera() const;

private:
  PREVENT_COPY_AND_ASSIGNMENT(Command);

  MotionCommand motionCommand;
  SoundCommand  soundCommand;
  LEDCommand    ledCommand;

  bool usingBottomCamera;
};

#endif /* COMMAND_H_ */
