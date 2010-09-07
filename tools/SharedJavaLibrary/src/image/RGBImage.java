package image;

import java.awt.*;
import java.awt.image.*;
import java.io.*;
import javax.imageio.*;

import image.format.*;

public abstract class RGBImage {

  protected RGBImage(int width, int height) {
    if (width <= 0) {
      throw new IllegalArgumentException("Width has to be > 0");
    }
    if (height <= 0) {
      throw new IllegalArgumentException("Height has to be > 0");
    }
    
    this.width  = width;
    this.height = height;
  }
  
  public int getWidth() {
    return width;
  }
  
  public int getHeight() {
    return height;
  }
  
  public abstract RGB getRGB(int x, int y);
  
  public BufferedImage toBufferedImage() {
    BufferedImage bufferedImage = new BufferedImage(width, height, BufferedImage.TYPE_INT_RGB);
    
    for (int y = 0; y < height; y++) {
      for (int x = 0; x < width; x++) {
        bufferedImage.setRGB(x, y, getRGB(x, y).toInt());
      }
    }
    
    return bufferedImage;
  }
  
  public void saveAsPNG(String filename) throws IOException {
    BufferedImage bufferedImage = new BufferedImage(width, height, BufferedImage.TYPE_INT_RGB);
    Graphics2D graphics = bufferedImage.createGraphics();
    
    for (int y = 0; y < height; y++) {
      for (int x = 0; x < width; x++) {
        RGB color = getRGB(x, y);
        graphics.setColor(new Color(color.getRed(), color.getGreen(), color.getBlue()));
        graphics.fillRect(x, y, 1, 1);
      }
    }
    
    ImageIO.write(bufferedImage, "png", new File(filename));
  }
  
  private int width, height;
}
