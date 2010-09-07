package image;

import image.format.*;

public class SegmentedImage extends RGBImage {

  public SegmentedImage(byte [] bytes, int width, int height, RGB [] colors) throws Exception {
    super(width, height);
    
    if (bytes.length != width * height) {
      throw new Exception("Number of bytes don't match width * height!");
    }
    
    this.colors = colors;
    
    indices = new int[width][height];
    
    int index = 0;
    for (int y = 0; y < height; y++) {
      for (int x = 0; x < width; x++) {
        indices[x][y] = bytes[index];
        index++;
      }
    }
  }
  
  public RGB getRGB(int x, int y) {
    return colors[ indices[x][y] ];
  }
  
  private int [][] indices;
  private RGB [] colors;
}
