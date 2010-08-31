package image;

import java.io.*;

import image.format.*;

public class YUV422Image extends YUVImage {

  public YUV422Image(String filename, int width, int height) throws IOException {
    super(width, height);
    
    data = new YUV[width][height];
    
    load(filename);
  }
  
  public YUV422Image(byte [] bytes, int width, int height) {
    super(width, height);
    
    data = new YUV[width][height];
    
    convertBytes(bytes);
  }
  
  @Override
  public YUV getYUV(int x, int y) {
    return data[x][y];
  }

  private void load(String filename) throws IOException {
    FileInputStream in = new FileInputStream(new File(filename));
    
    int width = getWidth();
    int height = getHeight();
    
    int numBytes = width * height * 2;
    byte [] bytes = new byte[numBytes];
    // Read the bytes from the file
    if (in.read(bytes) != numBytes) {
      throw new IOException("Error reading YUV422 file: " + filename);
    }
    
    convertBytes(bytes);
  }
  
  private void convertBytes(byte [] bytes) {
    int width  = getWidth();
    int height = getHeight();
    
    // Convert the YUV422 bytes to YUV
    int offset = 0;
    for (int y = 0; y < height; y++) {
      for (int x = 0; x < width; x+= 2) {
        int y1, u, y2, v;
        y1 = byteToInt(bytes[offset]);
        u  = byteToInt(bytes[offset + 1]);
        y2 = byteToInt(bytes[offset + 2]);
        v  = byteToInt(bytes[offset + 3]);
        
        data[x][y]     = new YUV(y1, u, v);
        data[x + 1][y] = new YUV(y2, u, v);
        
        offset += 4;
      }
    }
  }
  
  private int byteToInt(byte b) {
    if (b >= 0) {
      return (int)b;
    }
    else {
      int value = (int)b + 256;
      return value;
    }
  }
  
  private YUV [][] data;
  
}
