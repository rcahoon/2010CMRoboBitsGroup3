package network.message.fromrobot;

import network.message.RemoteMessageFromRobot;

public class TeamColorMessage extends RemoteMessageFromRobot {

  public TeamColorMessage(boolean isBlue) {
    super(RemoteMessageFromRobot.MessageType.TEAM_COLOR, null);
    
    this.isBlue = isBlue;
  }
  
  public boolean isBlue() {
    return isBlue;
  }
  
  private boolean isBlue;

  public static TeamColorMessage create(byte [] data) {
    try {
      // The first byte is the boolean
      boolean isBlue = (data[0] != 0);
      
      return new TeamColorMessage(isBlue);
    }
    catch (Exception e) {
      return null;
    }
  }
}
