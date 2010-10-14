package network.message.torobot;

import network.message.RemoteMessageToRobot;

public class RestoreHeadStiffnessMessage extends RemoteMessageToRobot {

  public RestoreHeadStiffnessMessage() {
    super(RemoteMessageToRobot.MessageType.RESTORE_HEAD_STIFFNESS, null);
  }
}
