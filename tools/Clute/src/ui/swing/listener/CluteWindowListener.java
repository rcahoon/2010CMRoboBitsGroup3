package ui.swing.listener;

import config.*;
import ui.swing.*;

import java.awt.event.WindowEvent;
import java.awt.event.WindowListener;

public class CluteWindowListener implements WindowListener {

  public CluteWindowListener(CluteWindow window, CluteLocalSettings localSettings) {
    this.window = window;
    this.localSettings = localSettings;
  }
  
  @Override
  public void windowActivated(WindowEvent arg0) {
  }

  @Override
  public void windowClosed(WindowEvent arg0) {
  }

  @Override
  public void windowClosing(WindowEvent arg0) {
    localSettings.setWindowPos(window.getX(), window.getY());
    localSettings.setSegmentedAlpha(window.getSegmentedAlpha());
    localSettings.setImagePaused(window.isImagePaused());
    localSettings.setAutoSegment(window.shouldAutoSegment());
    
    localSettings.save();
  }

  @Override
  public void windowDeactivated(WindowEvent arg0) {
  }

  @Override
  public void windowDeiconified(WindowEvent arg0) {
  }

  @Override
  public void windowIconified(WindowEvent arg0) {
  }

  @Override
  public void windowOpened(WindowEvent arg0) {
  }
  
  private CluteWindow window;
  private CluteLocalSettings localSettings;
  

}
