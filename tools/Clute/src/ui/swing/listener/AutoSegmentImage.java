package ui.swing.listener;

import java.awt.event.*;

import clute.*;

public class AutoSegmentImage implements ItemListener {

  public AutoSegmentImage(Clute clute) {
    this.clute = clute;
  }
  
  private Clute clute;
  
  @Override
  public void itemStateChanged(ItemEvent e) {
    clute.setAutoSegment(e.getStateChange() == ItemEvent.SELECTED);
  }

}
