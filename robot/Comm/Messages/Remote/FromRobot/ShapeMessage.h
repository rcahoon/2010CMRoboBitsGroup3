#ifndef SHAPEMESSAGE_H_
#define SHAPEMESSAGE_H_

#include <vector>

#include "Comm/Messages/Remote/RemoteMessageFromRobot.h"
#include "Log/Log.h"
#include "shared/Shape/Shape.h"
#include "shared/UsefulMacros.h"

class ShapeMessage : public RemoteMessageFromRobot {
public:
  ShapeMessage(Log::Screen _screen, const Shape & shape);

  virtual ~ShapeMessage();

private:
  PREVENT_COPY_AND_ASSIGNMENT(ShapeMessage);

  void createData(Log::Screen _screen, const Shape & shape);

  void createData(Log::Screen _screen, const Shape & shape,
                  const std::vector<int> & ints,
                  const std::vector<float> & floats);

};

#endif /* SHAPEMESSAGE_H_ */
