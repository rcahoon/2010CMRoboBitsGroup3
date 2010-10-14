package network.message.torobot;

import network.message.RemoteMessageToRobot;

public class StartRemoteControlMessage extends RemoteMessageToRobot {

  public StartRemoteControlMessage() {
    super(RemoteMessageToRobot.MessageType.START_REMOTE_CONTROL, null);
  }
  
}
