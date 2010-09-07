package network.message.fromrobot;

import network.message.RemoteMessageFromRobot;

public class Factory {

  public static RemoteMessageFromRobot create(int typeID, byte [] data) {
    RemoteMessageFromRobot.MessageType type = RemoteMessageFromRobot.MessageType.create(typeID);
    
    RemoteMessageFromRobot result = null;
    
    switch (type) {
    case LOG_BEGIN:
      result = LogBeginMessage.create(data);
      break;
    case LOG_END:
      result = LogEndMessage.create(data);
      break;
    case LOG_TEXT:
      result = LogTextMessage.create(data);
      break;
    case SHAPE:
      result = ShapeMessage.create(data);
      break;
    case TEAM_COLOR:
      result = TeamColorMessage.create(data);
      break;
    case SEGMENTED_IMAGE:
      result = SegmentedImageMessage.create(data);
      break;
    default:
      result = new RemoteMessageFromRobot(type, data);
    }
    
    // If one of the create methods fail
    if (result == null) {
      result = new RemoteMessageFromRobot(RemoteMessageFromRobot.MessageType.UNKNOWN, data);
    }
    
    return result;
  }
  
}
