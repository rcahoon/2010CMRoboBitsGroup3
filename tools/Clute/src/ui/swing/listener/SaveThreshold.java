package ui.swing.listener;

import java.awt.event.*;

import clute.*;

public class SaveThreshold implements ActionListener {

  public SaveThreshold(Clute clute) {
    this.clute = clute;
  }
  
  private Clute clute;
  
  @Override
  public void actionPerformed(ActionEvent e) {
    clute.saveThreshold();
  }

}
