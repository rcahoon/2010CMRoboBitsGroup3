package ui.swing.listener;

import javax.swing.*;
import java.awt.event.*;

import clute.*;

public class ColorChange implements ActionListener {

  public ColorChange(Clute clute) {
    this.clute = clute;
  }
  
  private Clute clute;
  
  @Override
  public void actionPerformed(ActionEvent e) {
    JComboBox comboBox = (JComboBox)e.getSource();
    int value = comboBox.getSelectedIndex();
    
    clute.setCurrentColor(value);
  }

}
