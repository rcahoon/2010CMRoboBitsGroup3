package ui.swing.clute;

import java.awt.*;
import java.awt.image.*;

import config.*;
import ui.swing.*;

public class ImageCanvas extends DirectCanvas {

  public ImageCanvas(ConfigFile configFile, int width, int height) {
    super(width, height);
    
    MIN_ZOOM = configFile.getInt("tools/clute/minZoom", 100);
    MAX_ZOOM = configFile.getInt("tools/clute/maxZoom", 1000);
    
    ZOOM_MULTIPLIER = configFile.getDouble("tools/clute/zoomMultiplier", 1.1);
    
    WIDTH  = width;
    HEIGHT = height;
    
    zoom = 200;
    zoomCenterX = 0;
    zoomCenterY = 0;
    
    segmentedAlpha = 50;
    
    originalImage = null;
    segmentedImage = null;
    
    getGraphicsToDraw().clearRect(0, 0, WIDTH, HEIGHT);
    flipScreen();
  }
  
  public void setOriginalImage(BufferedImage originalImage) {
    this.originalImage = originalImage;
    
    redrawImage();
  }
  
  public void setSegmentedImage(BufferedImage segmentedImage) {
    this.segmentedImage = segmentedImage;
    
    redrawImage();
  }
  
  public int getZoom() {
    return zoom;
  }
  
  public void setZoom(int zoom) {
    if (zoom < MIN_ZOOM) {
      zoom = MIN_ZOOM;
    }
    else if (zoom > MAX_ZOOM) {
      zoom = MAX_ZOOM;
    }
    
    this.zoom = zoom;
    
    redrawImage();
  }
  
  public void adjustZoomCenter(int deltaX, int deltaY) {
    zoomCenterX += deltaX;
    zoomCenterY += deltaY;
    
    redrawImage();
  }
  
  public void increaseZoom() {
    setZoom((int)(zoom * ZOOM_MULTIPLIER));
  }
  
  public void decreaseZoom() {
    setZoom((int)(zoom / ZOOM_MULTIPLIER));
  }
  
  public void increaseZoom(int invariantCenterX, int invariantCenterY) {
    int zoomCenterX = this.zoomCenterX + (int)((ZOOM_MULTIPLIER - 1) * (invariantCenterX - this.zoomCenterX));
    int zoomCenterY = this.zoomCenterY + (int)((ZOOM_MULTIPLIER - 1) * (invariantCenterY - this.zoomCenterY));
    setZoom((int)(zoom * ZOOM_MULTIPLIER), zoomCenterX, zoomCenterY);
  }
  
  public void decreaseZoom(int invariantCenterX, int invariantCenterY) {
    int zoomCenterX = this.zoomCenterX - (int)((ZOOM_MULTIPLIER - 1) * (invariantCenterX - this.zoomCenterX));
    int zoomCenterY = this.zoomCenterY - (int)((ZOOM_MULTIPLIER - 1) * (invariantCenterY - this.zoomCenterY));
    setZoom((int)(zoom / ZOOM_MULTIPLIER), zoomCenterX, zoomCenterY);
  }
  
  private void setZoom(int zoom, int zoomCenterX, int zoomCenterY) {
    this.zoomCenterX = zoomCenterX;
    this.zoomCenterY = zoomCenterY;
    
    setZoom(zoom);
  }
  
  public int getImagePositionX(int mousePosX) {
    if ((mousePosX < zoomStartX) || (mousePosX >= zoomEndX)) {
      return -1;
    }
    
    float ratio = (float)(mousePosX - zoomStartX) / (zoomEndX - zoomStartX);
    
    int result = imageStartX + (int)(ratio * (imageEndX - imageStartX));
    if (result >= imageEndX) {
      result = imageEndX - 1;
    }
    
    return result;
  }
  
  public int getImagePositionY(int mousePosY) {
    if ((mousePosY < zoomStartY) || (mousePosY >= zoomEndY)) {
      return -1;
    }
    
    float ratio = (float)(mousePosY - zoomStartY) / (zoomEndY - zoomStartY);
    
    int result = imageStartY + (int)(ratio * (imageEndY - imageStartY));
    if (result >= imageEndY) {
      result = imageEndY - 1;
    }
    
    return result;
  }
  
  public int getSegmentedAlpha() {
    return segmentedAlpha;
  }
  
  public void setSegmentedAlpha(int segmentedAlpha) {
    this.segmentedAlpha = segmentedAlpha;
    
    redrawImage();
  }
  
  private void redrawImage() {
    Graphics2D graphics = getGraphicsToDraw();
    graphics.clearRect(0, 0, WIDTH, HEIGHT);
    graphics.setRenderingHint(RenderingHints.KEY_INTERPOLATION, RenderingHints.VALUE_INTERPOLATION_NEAREST_NEIGHBOR);
    graphics.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_OFF);
    
    if (originalImage != null) {
      adjustZoom(originalImage);
      drawImage(graphics, originalImage, 100);
    }
    if (segmentedImage != null) {
      drawImage(graphics, segmentedImage, segmentedAlpha);
    }
    
    flipScreen();
  }

  private void adjustZoom(BufferedImage image) {
    int imageWidth = image.getWidth();
    int imageHeight = image.getHeight();
    
    imageStartX = 0;
    imageStartY = 0;
    imageEndX = imageWidth;
    imageEndY = imageHeight;
    zoomStartX = 0;
    zoomStartY = 0;
    zoomEndX = imageWidth * zoom / 100;
    zoomEndY = imageHeight * zoom / 100;
    
    // If the width is less than the canvas width, center the image
    if (zoomEndX <= WIDTH) {
      zoomStartX = (WIDTH - zoomEndX) / 2;
      zoomEndX += (WIDTH - zoomEndX) / 2;
    }
    // Otherwise, we have to clip part of the image
    else {
      zoomStartX = 0;
      zoomEndX   = WIDTH;
      
      imageStartX = zoomCenterX - (WIDTH * 100 / zoom / 2);
      imageEndX = zoomCenterX + (WIDTH * 100 / zoom / 2);
      if (imageStartX < 0) {
        imageEndX -= imageStartX;
        imageStartX = 0;
      }
      if (imageEndX > imageWidth) {
        imageStartX -= imageEndX - imageWidth;
        imageEndX = imageWidth;
      }
    }
    // Similar for the height
    if (zoomEndY <= HEIGHT) {
      zoomStartY = (HEIGHT - zoomEndY) / 2;
      zoomEndY += (HEIGHT - zoomEndY) / 2;
    }
    // Otherwise, we have to clip part of the image
    else {
      zoomStartY = 0;
      zoomEndY   = HEIGHT;
      
      imageStartY = zoomCenterY - (HEIGHT * 100 / zoom / 2);
      imageEndY = zoomCenterY + (HEIGHT * 100 / zoom / 2);
      if (imageStartY < 0) {
        imageEndY -= imageStartY;
        imageStartY = 0;
      }
      if (imageEndY > imageHeight) {
        imageStartY -= imageEndY - imageHeight;
        imageEndY = imageHeight;
      }
    }
    
    zoomCenterX = (imageStartX + imageEndX) / 2;
    zoomCenterY = (imageStartY + imageEndY) / 2;
  }
  
  private void drawImage(Graphics2D graphics, BufferedImage image, int alpha) {
    // Full alpha for the original image
    graphics.setComposite(AlphaComposite.getInstance(AlphaComposite.SRC_OVER, (float)alpha / 100));
    graphics.drawImage(image,
                       zoomStartX,  zoomStartY,  zoomEndX,  zoomEndY,
                       imageStartX, imageStartY, imageEndX, imageEndY,
                       null);
  }

  private final int MIN_ZOOM, MAX_ZOOM;
  
  private final double ZOOM_MULTIPLIER;
  
  private final int WIDTH, HEIGHT;
  
  private int zoom, segmentedAlpha;
  private int zoomCenterX, zoomCenterY;
  
  private int imageStartX, imageStartY, imageEndX, imageEndY;
  private int zoomStartX,  zoomStartY,  zoomEndX,  zoomEndY;
  
  private BufferedImage originalImage, segmentedImage;
  
  private static final long serialVersionUID = -6681579750929160156L;
}
