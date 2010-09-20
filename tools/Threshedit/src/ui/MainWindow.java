package ui;

import config.*;
import image.*;
import image.format.*;

import java.awt.*;
import javax.swing.*;
import java.awt.event.*;

import core.*;

public class MainWindow extends JFrame {

  public class SliderListener implements AdjustmentListener
  {
    public void adjustmentValueChanged(AdjustmentEvent e) {
      threshedit.updateCurrentClass(threshBars[0].getValue(), threshBars[1].getValue(),
                                    threshBars[2].getValue(), threshBars[3].getValue(),
                                    threshBars[4].getValue(), threshBars[5].getValue());
      for(int i=0; i < 6; i++)
        threshLabels[i].setText(""+threshBars[i].getValue());
    }
  }

  public MainWindow(final Threshedit threshedit, ConfigFile visionConfig) {
    super("CMRoboBits Group3 Threshold Editor");

    setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);
    setSize(new Dimension(800, 480));
    
    this.threshedit = threshedit;
    imageScreen = new ImageCanvas(visionConfig.getInt("vision/imageWidth", 320), visionConfig.getInt("vision/imageHeight", 240));
    
    // Add a combo box for all the colors
    JComboBox colorComboBox = new JComboBox(threshedit.getClasses());
    colorComboBox.setEditable(false);
    colorComboBox.setFocusable(false);
    colorComboBox.addActionListener(new ActionListener() {
      public void actionPerformed(ActionEvent e) {
        JComboBox comboBox = (JComboBox)e.getSource();
        int value = comboBox.getSelectedIndex();
        threshedit.setCurrentClass(value);
        
        updateThreshBars();
      }
    });
    
    JButton resetButton = new JButton("Reset threshold");
    resetButton.setFocusable(false);
    resetButton.setMnemonic(KeyEvent.VK_R);
    resetButton.addActionListener(new ActionListener() {
      public void actionPerformed(ActionEvent e) {
        threshedit.getClasses()[threshedit.getCurrentClass()].reload();
        threshedit.segmentImage();
        updateThreshBars();
      }
    });
    
    // Add a button to save the threshold
    JButton saveButton = new JButton("Save threshold");
    saveButton.setFocusable(false);
    saveButton.setMnemonic(KeyEvent.VK_S);
    saveButton.addActionListener(new ActionListener() {
      public void actionPerformed(ActionEvent e) {
        threshedit.saveThreshold();
      }
    });
    
    // Add a scroll bar to adjust the alpha
    alphaScrollBar = new JScrollBar(JScrollBar.HORIZONTAL, imageScreen.getSegmentedAlpha(), 0, 0, 100);
    alphaScrollBar.setUnitIncrement(1);
    alphaScrollBar.addAdjustmentListener(new AdjustmentListener() {
      public void adjustmentValueChanged(AdjustmentEvent e) {
        imageScreen.setSegmentedAlpha(e.getValue());
      }
    });
    
    // Add a button to pause/unpause the image
    pauseButton = new JToggleButton("Pause Image");
    pauseButton.addItemListener(new ItemListener() {
      public void itemStateChanged(ItemEvent e) {
        setImagePaused(e.getStateChange() == ItemEvent.SELECTED);
      }
    });
	
    JPanel topPanel = new JPanel();
    topPanel.add(colorComboBox);
    topPanel.add(resetButton);
    topPanel.add(saveButton);
    topPanel.add(pauseButton);
    
    JPanel threshPanel = new JPanel();
    threshPanel.setPreferredSize(new Dimension(300, 300));
    threshPanel.setLayout(new BoxLayout(threshPanel, BoxLayout.Y_AXIS));
    threshBars = new JScrollBar[6];
    threshLabels = new JLabel[6];
    SliderListener sl = new SliderListener();
    for(int i=0; i < 6; i++)
    {
      threshBars[i] = new JScrollBar(JScrollBar.HORIZONTAL, 0, 0, 0, 256);
      threshBars[i].addAdjustmentListener(sl);
      threshLabels[i] = new JLabel("0");
      threshPanel.add(threshLabels[i]);
      threshPanel.add(threshBars[i]);
      threshPanel.add(Box.createVerticalStrut(30));
    }
    threshBars[0].setUnitIncrement(1 << (8 - visionConfig.getInt("vision/thresholdYBits", 0)));
    threshBars[1].setUnitIncrement(1 << (8 - visionConfig.getInt("vision/thresholdYBits", 0)));
    threshBars[2].setUnitIncrement(1 << (8 - visionConfig.getInt("vision/thresholdUBits", 0)));
    threshBars[3].setUnitIncrement(1 << (8 - visionConfig.getInt("vision/thresholdUBits", 0)));
    threshBars[4].setUnitIncrement(1 << (8 - visionConfig.getInt("vision/thresholdVBits", 0)));
    threshBars[5].setUnitIncrement(1 << (8 - visionConfig.getInt("vision/thresholdVBits", 0)));
    
    updateThreshBars();
    
    final JLabel pointColorLabel = new JLabel("Y:  U:  V:  ");
    threshPanel.add(pointColorLabel);
    imageScreen.addMouseMotionListener(new MouseMotionAdapter() {
      public void mouseMoved(MouseEvent e) {
        Point p = imageScreen.componentToImage(e.getPoint());
        if (p.getX() >= 0)
        {
          YUVImage image = threshedit.getOriginalImage();
          if (image==null) return;
          YUV color = image.getYUV((int)p.getX(), (int)p.getY());
          pointColorLabel.setText("Y:" + color.getY() + "  U:" + color.getU() + "  V:" + color.getV());
        }
        else
        {
          pointColorLabel.setText("Y:  U:  V:  ");
        }
      }
    });

    // Add all the ui components
    getContentPane().setLayout(new BorderLayout());
    getContentPane().add(topPanel, BorderLayout.NORTH);
    getContentPane().add(imageScreen, BorderLayout.CENTER);
    getContentPane().add(alphaScrollBar, BorderLayout.SOUTH);
    getContentPane().add(threshPanel, BorderLayout.EAST);
    
    this.setVisible(true);
  }
  
  protected void updateThreshBars()
  {
    ColorClass cc = new ColorClass(threshedit.getClasses()[threshedit.getCurrentClass()]);
  
    threshBars[0].setValue(cc.yl);
    threshBars[1].setValue(cc.yu);
    threshBars[2].setValue(cc.ul);
    threshBars[3].setValue(cc.uu);
    threshBars[4].setValue(cc.vl);
    threshBars[5].setValue(cc.vu);
  }
  
  public int getSegmentedAlpha() {
    return imageScreen.getSegmentedAlpha();
  }
  
  public void setSegmentedAlpha(boolean fullOn) {
    if (fullOn) {
      alphaScrollBar.setValue(alphaScrollBar.getMaximum());
    }
    else {
      alphaScrollBar.setValue(alphaScrollBar.getMinimum());
    }
  }
  
  public ImageCanvas getImageScreen() {
    return imageScreen;
  }
  
  public void toggleImagePaused() {
    pauseButton.doClick();
  }
  
  public void setImagePaused(boolean imagePaused) {
    threshedit.setImagePaused(imagePaused);
  }
  
  public boolean isImagePaused() {
    return threshedit.isImagePaused();
  }
  
  private Threshedit threshedit;
  
  private JLabel[] threshLabels;
  private JScrollBar[] threshBars;
  
  private ImageCanvas imageScreen;
  
  private JScrollBar alphaScrollBar;
  private JToggleButton pauseButton;
}
