package update;

import java.util.*;

import network.message.*;
import network.message.fromrobot.*;

public class RobotUpdate implements Iterable<RemoteMessageFromRobot> {

  public RobotUpdate(boolean isBlue) {
    this.isBlue = isBlue; 
    
    messages = new Vector<RemoteMessageFromRobot>();
  }
  
  public void addMessage(RemoteMessageFromRobot message) {
    if (message instanceof TeamColorMessage) {
      isBlue = ((TeamColorMessage)message).isBlue();
    }
    else {
      messages.add(message);
    }
  }
  
  public boolean isBlue() {
    return isBlue;
  }
  
  public Iterator<RemoteMessageFromRobot> iterator() {
    return messages.iterator();
  }
  
  List<RemoteMessageFromRobot> messages;
  
  private boolean isBlue;
}
