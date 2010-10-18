package network.message.torobot;

import network.message.RemoteMessageToRobot;

public class SetCameraMessage extends RemoteMessageToRobot {
  public SetCameraMessage(boolean controlCamera, boolean useBottomCamera) {
    super(RemoteMessageToRobot.MessageType.SET_CAMERA, createData(controlCamera, useBottomCamera));
  }
 
  private static byte [] createData(boolean controlCamera, boolean useBottomCamera) {
    if (!controlCamera) {
      byte [] data = new byte[1];
      data[0] = 0;
      
      return data;
    }
    else {
      byte [] data = new byte[2];
      data[0] = 1;
      data[1] = (byte)(useBottomCamera ? 1 : 0);
      
      return data;
    }
  }
}
