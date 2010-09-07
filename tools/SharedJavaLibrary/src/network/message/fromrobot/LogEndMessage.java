package network.message.fromrobot;

import network.message.RemoteMessageFromRobot;

public class LogEndMessage extends RemoteMessageFromRobot {
  
  public LogEndMessage() {
    super(RemoteMessageFromRobot.MessageType.LOG_BEGIN, null);
  }
  
  public static LogEndMessage create(byte [] data) {
    try {
      return new LogEndMessage();
    }
    catch (Exception e) {
      return null;
    }
  }

}
