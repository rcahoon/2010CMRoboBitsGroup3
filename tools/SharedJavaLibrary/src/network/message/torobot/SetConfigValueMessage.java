package network.message.torobot;

import java.nio.charset.Charset;

import network.message.RemoteMessageToRobot;

public class SetConfigValueMessage extends RemoteMessageToRobot {

  public SetConfigValueMessage(String name, String value) {
    super(RemoteMessageToRobot.MessageType.SET_CONFIG_VALUE, createData(name, value));
  }
  
  private static byte [] createData(String name, String value) {
    byte [] bytes = new byte[name.length() + value.length() + 2];
    byte [] nameBytes = name.getBytes(Charset.forName("US-ASCII"));
    byte [] valueBytes = value.getBytes(Charset.forName("US-ASCII"));
    
    System.arraycopy(nameBytes, 0, bytes, 0, nameBytes.length);
    bytes[name.length()] = 0;
    System.arraycopy(valueBytes, 0, bytes, nameBytes.length + 1, valueBytes.length);
    bytes[bytes.length - 1] = 0;
    
    return bytes;
  }
  
}
