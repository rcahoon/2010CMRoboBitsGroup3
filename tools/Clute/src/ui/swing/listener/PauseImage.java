package ui.swing.listener;

import java.awt.event.*;

import ui.swing.*;

public class PauseImage implements ItemListener {

  public PauseImage(CluteWindow cluteWindow) {
    this.cluteWindow = cluteWindow;
  }
  
  private CluteWindow cluteWindow;
  
  @Override
  public void itemStateChanged(ItemEvent e) {
    cluteWindow.setImagePaused(e.getStateChange() == ItemEvent.SELECTED);
  }

}
