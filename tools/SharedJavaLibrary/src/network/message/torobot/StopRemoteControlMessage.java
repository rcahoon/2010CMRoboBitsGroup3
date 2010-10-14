package network.message.torobot;

import network.message.RemoteMessageToRobot;

public class StopRemoteControlMessage extends RemoteMessageToRobot {

  public StopRemoteControlMessage() {
    super(RemoteMessageToRobot.MessageType.STOP_REMOTE_CONTROL, null);
  }
  
}
