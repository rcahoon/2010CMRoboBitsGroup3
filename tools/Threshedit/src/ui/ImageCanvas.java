package ui;

import java.awt.*;
import javax.swing.JPanel;
import java.awt.image.*;

public class ImageCanvas extends JPanel {

  public ImageCanvas(int width, int height) {
    super(true); // Enable double buffering
    
    this.segmentedAlpha = 50;
    this.width = width;
    this.height = height;
    this.originalImage = null;
    this.segmentedImage = null;
  }
  
  public void setOriginalImage(BufferedImage originalImage) {
    this.originalImage = originalImage;
    
    repaint();
  }
  
  public void setSegmentedImage(BufferedImage segmentedImage) {
    this.segmentedImage = segmentedImage;
    
    repaint();
  }
  
  public int getSegmentedAlpha() {
    return segmentedAlpha;
  }
  
  public void setSegmentedAlpha(int segmentedAlpha) {
    this.segmentedAlpha = segmentedAlpha;
    
    repaint();
  }
  
  public Point componentToImage(Point p)
  {
    float r = Math.min(getWidth() / (float)width, getHeight() / (float)height);
    int x = (int)(p.getX() / r);
    int y = (int)(p.getY() / r);
    if ((x > width-1) || (y > height-1)) return new Point(-1,-1);
    return new Point(x, y);
  }
  
  protected void paintComponent(Graphics g)
  {
    Graphics2D graphics = (Graphics2D)g;
    graphics.clearRect(0, 0, width, height);
    graphics.setRenderingHint(RenderingHints.KEY_INTERPOLATION, RenderingHints.VALUE_INTERPOLATION_NEAREST_NEIGHBOR);
    graphics.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_OFF);
    
    if (originalImage != null) {
      drawImage(graphics, originalImage, 100);
    }
    if (segmentedImage != null) {
      drawImage(graphics, segmentedImage, segmentedAlpha);
    }
  }
  
  private void drawImage(Graphics2D graphics, BufferedImage image, int alpha) {
    // Full alpha for the original image
    graphics.setComposite(AlphaComposite.getInstance(AlphaComposite.SRC_OVER, alpha /(float) 100));
    float r = Math.min(getWidth() /(float) width, getHeight() /(float) height);
    graphics.drawImage(image, 0, 0, (int)(r*width), (int)(r*height), null);
  }
  
  private int segmentedAlpha;
  private int width, height;
  
  private BufferedImage originalImage, segmentedImage;
}
