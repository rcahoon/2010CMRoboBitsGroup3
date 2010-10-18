package network.message.torobot;

import network.message.RemoteMessage;
import network.message.RemoteMessageToRobot;

public class SetHeadStiffnessMessage extends RemoteMessageToRobot {
  
  public SetHeadStiffnessMessage(float stiffness) {
    super(RemoteMessageToRobot.MessageType.SET_HEAD_STIFFNESS, createData(stiffness));
  }

  private static byte [] createData(float stiffness) {
    // We need 4 bytes for the float
    byte [] bytes = new byte[4];
    RemoteMessage.writeFloat(bytes, 0, stiffness);
    
    return bytes;
  }
}
