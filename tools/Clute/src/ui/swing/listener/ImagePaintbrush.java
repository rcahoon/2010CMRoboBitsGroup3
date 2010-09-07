package ui.swing.listener;

import java.awt.*;
import java.awt.event.*;

import clute.*;
import ui.swing.clute.*;

public class ImagePaintbrush implements MouseListener, MouseMotionListener {

  public ImagePaintbrush(Clute clute, ImageCanvas imageCanvas) {
    this.clute = clute;
    this.imageCanvas = imageCanvas;
  }
  
  private Clute clute;
  private ImageCanvas imageCanvas;
  
  @Override
  public void mousePressed(MouseEvent e) {
    updateThreshold(e);
  }

  @Override
  public void mouseDragged(MouseEvent e) {
    updateThreshold(e);
  }

  @Override
  public void mouseReleased(MouseEvent e) {
  }

  @Override
  public void mouseMoved(MouseEvent e) {
  }

  @Override
  public void mouseClicked(MouseEvent e) {
  }

  @Override
  public void mouseEntered(MouseEvent e) {
  }

  @Override
  public void mouseExited(MouseEvent e) {
  }

  private void updateThreshold(MouseEvent e) {
    Point point = e.getPoint();
    
    int imageX = imageCanvas.getImagePositionX(point.x);
    int imageY = imageCanvas.getImagePositionY(point.y);
    
//    System.out.println("Point (" + point.x + ", " + point.y + "), image (" + imageX + ", " + imageY + ")");

    // Mouse wheel moved up, zoom in
    if ((imageX < 0) || (imageY < 0)) {
      return;
    }
    
    clute.updateThresholdFromImage(imageX, imageY);
  }
}
