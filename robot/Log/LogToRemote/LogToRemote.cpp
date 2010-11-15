#include "LogToRemote.h"
#include "Agent/RobotState.h"
#include "Comm/Messages/Messages.h"
#include "Comm/Messages/Remote/RemoteMessageFromRobot.h"
#include "Comm/Messages/Remote/FromRobot/LogBeginMessage.h"
#include "Comm/Messages/Remote/FromRobot/LogEndMessage.h"
#include "Comm/Messages/Remote/FromRobot/LogTextMessage.h"
#include "Comm/Messages/Remote/FromRobot/RobotStateMessage.h"
#include "Comm/Messages/Remote/FromRobot/SegmentedImageMessage.h"
#include "Comm/Messages/Remote/FromRobot/ShapeMessage.h"
#include "Comm/Messages/Remote/FromRobot/TeamColorMessage.h"
#include "shared/ConfigFile/ConfigFile.h"

LogToRemote::LogToRemote(ConfigFile & configFile)
  : Log(configFile),
    LOG_ROBOTSTATE_PERIOD(configFile.getIntReference("log/logRemote/logRobotStatePeriod", 0)),
    LOG_ORIGINAL_IMAGE_PERIOD(configFile.getIntReference("log/logRemote/logOriginalImagePeriod", 0)),
    LOG_SEGMENTED_IMAGE_PERIOD(configFile.getIntReference("log/logRemote/logSegmentedImagePeriod", 0)),
    logRobotStateCounter(0),
    logOriginalImageCounter(0) {
}

LogToRemote::~LogToRemote() {
}

bool LogToRemote::run(Messages & messages) {
  // We don't need to queue messages if we're not connected to a remote client
  if (!isConnectedToClient()) {
    return false;
  }

  // Add all the cached messages
  std::vector<RemoteMessageFromRobot const *>::iterator iter;
  for (iter = remoteMessagesFromRobot.begin();
       iter != remoteMessagesFromRobot.end();
       iter++) {
    messages.addRemoteMessageFromRobot(*iter);
  }
  remoteMessagesFromRobot.clear();

  return false;
}

void LogToRemote::logBegin(unsigned long timestamp) {
  // We don't need to create messages if we're not connected to a remote client
  if (!isConnectedToClient()) {
    return;
  }

  RemoteMessageFromRobot *message = new LogBeginMessage(timestamp);
  remoteMessagesFromRobot.push_back(message);
}

void LogToRemote::logEnd() {
  // We don't need to create messages if we're not connected to a remote client
  if (!isConnectedToClient()) {
    return;
  }

  RemoteMessageFromRobot *message = new LogEndMessage();
  remoteMessagesFromRobot.push_back(message);
}

void LogToRemote::logTextFormatted(int logLevel,
                                   int component,
                                   char const * className,
                                   char const * functionName,
                                   char const * text,
                                   int textLength) {
  // We don't need to create messages if we're not connected to a remote client
  if (!isConnectedToClient()) {
    return;
  }

  // Disabled for now
//  RemoteMessageFromRobot *message = new LogTextMessage(logLevel,
//                                                       component,
//                                                       className,
//                                                       functionName,
//                                                       text);
//  remoteMessagesFromRobot.push_back(message);
}

void LogToRemote::logRobotState(const RobotState & robotState) {
  // We don't need to create messages if we're not connected to a remote client
  if (!isConnectedToClient()) {
    return;
  }

  if (LOG_ROBOTSTATE_PERIOD > 0) {
    // We want to log the RobotState every couple of frames
    logRobotStateCounter++;
    // Is it time to log the robot state?
    if (logRobotStateCounter >= LOG_ROBOTSTATE_PERIOD) {
      logRobotStateCounter = 0;

      RemoteMessageFromRobot *message = new RobotStateMessage(robotState);

      remoteMessagesFromRobot.push_back(message);
    }
  }

  if (LOG_ORIGINAL_IMAGE_PERIOD > 0) {
    // We want to log the original image every couple of frames
    logOriginalImageCounter++;
    // Is it time to log the image?
    if (logOriginalImageCounter >= LOG_ORIGINAL_IMAGE_PERIOD) {
      logOriginalImageCounter = 0;

      RemoteMessageFromRobot *message = new RemoteMessageFromRobot(RemoteMessageFromRobot::originalImage,
                                                                   robotState.getRawImageSize(),
                                                                   robotState.getRawImage());

      remoteMessagesFromRobot.push_back(message);
    }
  }
}

void LogToRemote::logSegmentedImage(const SegmentedImage & segmentedImage) {
  // We don't need to create messages if we're not connected to a remote client
  if (!isConnectedToClient()) {
    return;
  }

  if (LOG_SEGMENTED_IMAGE_PERIOD > 0) {
    // We want to extract the segmented image every couple of frames
    logSegmentedImageCounter++;
    // Is it time to log the image?
    if (logSegmentedImageCounter >= LOG_SEGMENTED_IMAGE_PERIOD) {
      logSegmentedImageCounter = 0;

      RemoteMessageFromRobot *message = new SegmentedImageMessage(segmentedImage);
      remoteMessagesFromRobot.push_back(message);
    }
  }
}

void LogToRemote::logShape(Log::Screen screen, const Shape & shape) {
  // We don't need to create messages if we're not connected to a remote client
  if (!isConnectedToClient()) {
    return;
  }

  RemoteMessageFromRobot *message = new ShapeMessage(screen, shape);
  remoteMessagesFromRobot.push_back(message);
}

void LogToRemote::logTeamColor(bool isBlue) {
  // We don't need to create messages if we're not connected to a remote client
  if (!isConnectedToClient()) {
    return;
  }

  RemoteMessageFromRobot *message = new TeamColorMessage(isBlue);
  remoteMessagesFromRobot.push_back(message);
}

void LogToRemote::logGameState(const GameState & gameState) {
  // We don't log game state remotely for now
}
