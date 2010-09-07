package ui.swing.listener;

import ui.swing.clute.*;

import java.awt.event.*;

public class ImageDrag implements MouseMotionListener {

  public ImageDrag(ImageCanvas imageCanvas) {
    this.imageCanvas = imageCanvas;
  }
  
  private ImageCanvas imageCanvas;

  @Override
  public void mouseDragged(MouseEvent e) {
    int deltaX = e.getX() - previousMousePositionX;
    int deltaY = e.getY() - previousMousePositionY;
    
    previousMousePositionX = e.getX();
    previousMousePositionY = e.getY();
    
    // We adjust the center in the opposite direction of the drag
    imageCanvas.adjustZoomCenter(-deltaX, -deltaY);
  }

  @Override
  public void mouseMoved(MouseEvent e) {
    previousMousePositionX = e.getX();
    previousMousePositionY = e.getY();
  }

  private int previousMousePositionX, previousMousePositionY;
}
