package network.message;

public class RemoteMessageFromRobot extends RemoteMessage {

  public enum MessageType {
    // This should match Comm/Messages/Remote/RemoteMessageFromRobot.h
    UNKNOWN        (0),
    LOG_BEGIN      (1),
    LOG_END        (2),
    ROBOT_STATE    (3),
    ORIGINAL_IMAGE (4),
    SEGMENTED_IMAGE(5),
    LOG_TEXT       (6),
    SHAPE          (7),
    TEAM_COLOR     (8);
    
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

  public RemoteMessageFromRobot(MessageType type, byte [] data) {
    super(type.getTypeID(), data);
    
    this.type = type;
  }
  
  public MessageType getType() {
    return type;
  }
  
  private MessageType type;
}
