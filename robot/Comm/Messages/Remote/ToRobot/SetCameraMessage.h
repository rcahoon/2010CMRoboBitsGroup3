#ifndef SETCAMERAMESSAGE_H_
#define SETCAMERAMESSAGE_H_

#include "Comm/Messages/Remote/RemoteMessageToRobot.h"

class SetCameraMessage : public RemoteMessageToRobot {
public:
  SetCameraMessage(bool controlCamera, bool useBottomCamera = true);

  virtual ~SetCameraMessage();

  bool shouldControlCamera() const;

  bool shouldUseBottomCamera() const;

  static SetCameraMessage const * create(unsigned int size, void const *data);

private:
  bool controlCamera, useBottomCamera;

};

#endif /* SETCAMERAMESSAGE_H_ */
