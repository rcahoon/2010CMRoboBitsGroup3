package network.message;

public class RemoteMessageToRobot extends RemoteMessage {

  public enum MessageType {
    // This should match Comm/Messages/Remote/RemoteMessageToRobot.h
    UNKNOWN(0),
    START_REMOTE_CONTROL(1),
    STOP_REMOTE_CONTROL(2),
    SET_CAMERA(3),
    SET_CONFIG_VALUE(4),
    SET_HEAD_STIFFNESS(5),
    RESTORE_HEAD_STIFFNESS(6);
    
    MessageType(int type) {
      this.type = type;
    }
    
    public int getTypeID() {
      return type;
    }
    
    private int type;
    
    public static MessageType create(int type) {
      for (MessageType messageType : MessageType.values()) {
        if (messageType.type == type) {
          return messageType;
        }
      }
        
      return MessageType.UNKNOWN;
    }
  }
  
  public static final int MESSAGE_HEADER_SIZE = 5;

  public RemoteMessageToRobot(MessageType type, byte [] data) {
    super(type.getTypeID(), data);
    
    this.type = type;
  }
  
  public MessageType getType() {
    return type;
  }
  
  public byte [] convertToBytes() {
    int dataLength = 0;
    if (data != null) {
      dataLength = data.length;
    }
    
    byte [] bytes = new byte[dataLength + MESSAGE_HEADER_SIZE];
    
    // The first byte is the type
    bytes[0] = (byte)(type.getTypeID() & 0xFF);
    // The next 4 bytes are the data length
    if (!writeInt(bytes, 1, dataLength)) {
      System.err.println("Error writing data length.");
      return null;
    }
    
    if (dataLength > 0) {
      System.arraycopy(data, 0, bytes, MESSAGE_HEADER_SIZE, dataLength);
    }
    
    return bytes;
  }
  
  private MessageType type;
}
