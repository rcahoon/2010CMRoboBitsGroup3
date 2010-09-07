package ui.swing.listener;

import ui.swing.clute.*;

import java.awt.*;
import java.awt.event.*;

public class ImageZoom implements MouseWheelListener {

  public ImageZoom(ImageCanvas imageCanvas) {
    this.imageCanvas = imageCanvas;
  }
  
  private ImageCanvas imageCanvas;

  @Override
  public void mouseWheelMoved(MouseWheelEvent e) {
    int notches = e.getWheelRotation();
    Point point = e.getPoint();
    
    int imageX = imageCanvas.getImagePositionX(point.x);
    int imageY = imageCanvas.getImagePositionY(point.y);

    if (notches < 0) {
      // Mouse wheel moved up, zoom in
      if ((imageX < 0) || (imageY < 0)) {
        imageCanvas.increaseZoom();
      }
      else {
        imageCanvas.increaseZoom(imageX, imageY);
      }
    }
    else {
      // Mouse wheel moved down, zoom out
      if ((imageX < 0) || (imageY < 0)) {
        imageCanvas.decreaseZoom();
      }
      else {
        imageCanvas.decreaseZoom(imageX, imageY);
      }
    }
  }

}
