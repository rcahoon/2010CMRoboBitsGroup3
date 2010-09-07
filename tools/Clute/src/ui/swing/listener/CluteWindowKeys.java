package ui.swing.listener;

import java.awt.event.*;

import ui.swing.*;

public class CluteWindowKeys implements KeyListener {

  public CluteWindowKeys(CluteWindow cluteWindow) {
    this.cluteWindow = cluteWindow;
  }
  
  private CluteWindow cluteWindow;
  
  @Override
  public void keyPressed(KeyEvent e) {
    processKey(e.getKeyCode());
  }

  @Override
  public void keyReleased(KeyEvent arg0) {
  }

  @Override
  public void keyTyped(KeyEvent arg0) {
  }

  private void processKey(int keyCode) {
    switch (keyCode) {
    case KeyEvent.VK_C:
      cluteWindow.setSegmentedAlpha(false);
      break;
    case KeyEvent.VK_V:
      cluteWindow.setSegmentedAlpha(true);
      break;
    case KeyEvent.VK_SPACE:
      cluteWindow.toggleImagePaused();
      break;
    case KeyEvent.VK_R:
      cluteWindow.toggleAutoSegment();
    }
  }
}
