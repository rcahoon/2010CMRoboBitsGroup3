package network.message;

public class RemoteMessage {

  public RemoteMessage(int type, byte [] data) {
    this.type = type;
    this.data = data;
  }
  
  public byte [] getData() {
    return data;
  }
  
  protected int getTypeID() {
    return type;
  }
  
  protected byte [] data;
  
  private int type;
  
  public static int readInt(byte [] data, int offset) {
    if (offset + 3 >= data.length) {
      return 0;
    }
    
//    System.out.printf("%x %x %x %x\n", (int)data[offset], (int)data[offset + 1], (int)data[offset + 2], (int)data[offset + 3]);
    return ((int)data[offset] & 0xFF) << 24 |
           ((int)data[offset + 1] & 0xFF) << 16 |
           ((int)data[offset + 2] & 0xFF) << 8  |
           ((int)data[offset + 3] & 0xFF);
  }

  public static float readFloat(byte [] data, int offset) {
    if (offset + 3 >= data.length) {
      return 0;
    }
    
    int intBits = readInt(data, offset);
    
//    System.out.println(Integer.toHexString(intBits));
    return Float.intBitsToFloat(intBits);
  }

}
