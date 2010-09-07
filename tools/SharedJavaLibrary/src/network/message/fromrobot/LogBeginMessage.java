package network.message.fromrobot;

import network.message.RemoteMessage;
import network.message.RemoteMessageFromRobot;

public class LogBeginMessage extends RemoteMessageFromRobot {
  
  public LogBeginMessage(int timestamp) {
    super(RemoteMessageFromRobot.MessageType.LOG_BEGIN, null);
    
    this.timestamp = timestamp;
  }
  
  public int getTimestamp() {
    return timestamp;
  }
  
  private int timestamp;

  public static LogBeginMessage create(byte [] data) {
    try {
      int timestamp = RemoteMessage.readInt(data, 0);
      
      return new LogBeginMessage(timestamp);
    }
    catch (Exception e) {
      return null;
    }
  }

}
