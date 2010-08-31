package javafx;

import java.util.*;

import network.message.*;
import network.message.fromrobot.*;

public class Update {

  public Update(boolean isBlue) {
    this.isBlue = isBlue; 
    
    remoteMessages = new Vector<RemoteMessageFromRobot>();
    sslMessages = new Vector<SSLMessage>();
    
  }
  
  public void addMessage(RemoteMessageFromRobot message) {
    if (message instanceof TeamColorMessage) {
      isBlue = ((TeamColorMessage)message).isBlue();
    }
    else {
      remoteMessages.add(message);
    }
  }
  
  public void addSSLMessage(SSLMessage message) {
    sslMessages.add(message);
  }
  
  public boolean isBlue() {
    return isBlue;
  }
  
  public Iterator<RemoteMessageFromRobot> getRemoteMessageIterator() {
    return remoteMessages.iterator();
  }
  
  public Iterator<SSLMessage> getSSLMessageIterator() {
    return sslMessages.iterator();
  }
  
  List<RemoteMessageFromRobot> remoteMessages;
  
  List<SSLMessage> sslMessages;
  
  private boolean isBlue;
}

