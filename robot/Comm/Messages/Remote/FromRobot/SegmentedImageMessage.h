#ifndef SEGMENTEDIMAGEMESSAGE_H_
#define SEGMENTEDIMAGEMESSAGE_H_

#include "Comm/Messages/Remote/RemoteMessageFromRobot.h"
#include "shared/UsefulMacros.h"

class SegmentedImage;

class SegmentedImageMessage : public RemoteMessageFromRobot {
public:
  SegmentedImageMessage(const SegmentedImage & segmentedImage);

  virtual ~SegmentedImageMessage();

private:
  PREVENT_COPY_AND_ASSIGNMENT(SegmentedImageMessage);

  void createData(const SegmentedImage & segmentedImage);

};

#endif /* SEGMENTEDIMAGEMESSAGE_H_ */
