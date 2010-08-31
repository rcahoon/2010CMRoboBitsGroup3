package network.message.fromrobot;

import image.*;
import image.format.*;
import network.message.*;

public class SegmentedImageMessage extends RemoteMessageFromRobot {

  public SegmentedImageMessage(SegmentedImage segmentedImage) {
    super(RemoteMessageFromRobot.MessageType.SEGMENTED_IMAGE, null);
    
    this.segmentedImage = segmentedImage;
  }
  
  public SegmentedImage getSegmentedImage() {
    return segmentedImage;
  }
  
  private SegmentedImage segmentedImage;
  
  public static SegmentedImageMessage create(byte [] data) {
    try {
      int offset = 0;
      
      // Read the image width
      int imageWidth = readInt(data, offset);
      offset += 4;
      
      // Read the image height
      int imageHeight = readInt(data, offset);
      offset += 4;
      
      int imageSize = imageWidth * imageHeight;
      
      // Read the image
      byte [] imageData = new byte[imageSize];
      System.arraycopy(data, offset, imageData, 0, imageSize);
      offset += imageSize;
      
      // Read the number of colors
      int numColors = data[offset];
      offset++;
      
      RGB [] colors = new RGB[numColors];
      for (int i = 0; i < numColors; i++) {
        int red   = (int)data[offset]     & 0xFF;
        int green = (int)data[offset + 1] & 0xFF;
        int blue  = (int)data[offset + 2] & 0xFF;
        offset += 3;
        
        colors[i] = new RGB(red, green, blue);
      }
      
      return new SegmentedImageMessage(new SegmentedImage(imageData, imageWidth, imageHeight, colors));
    }
    catch (Exception e) {
      System.out.println(e.toString());
      return null;
    }
  }
}
