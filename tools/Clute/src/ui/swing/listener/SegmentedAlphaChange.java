package ui.swing.listener;

import java.awt.event.*;

import ui.swing.clute.*;

public class SegmentedAlphaChange implements AdjustmentListener {

  public SegmentedAlphaChange(ImageCanvas imageCanvas) {
    this.imageCanvas = imageCanvas;
  }
  
  private ImageCanvas imageCanvas;
  
  @Override
  public void adjustmentValueChanged(AdjustmentEvent e) {
    imageCanvas.setSegmentedAlpha(e.getValue());
  }

}
