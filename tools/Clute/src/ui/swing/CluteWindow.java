package ui.swing;

import config.*;
import ui.swing.clute.*;
import ui.swing.listener.*;
import clute.*;

import java.awt.*;
import java.awt.event.KeyEvent;

import javax.swing.*;

public class CluteWindow extends JFrame {
  
  public CluteWindow(String robotHost, int port) {
    this(CluteConfig.getInstance(), robotHost, port);
  }
  
  private CluteWindow(ConfigFile configFile, String robotHost, int port) {
    super(configFile.getString("tools/clute/windowName", "Color Lookup-Table Editor"));
    
    localSettings = new CluteLocalSettings(configFile);
    
    int windowWidth = configFile.getInt("tools/clute/windowWidth", 800);
    int windowHeight = configFile.getInt("tools/clute/windowHeight", 600);
    int windowPosX = localSettings.getWindowPosX();
    int windowPosY = localSettings.getWindowPosY();
    
    int imageScreenWidth  = configFile.getInt("tools/clute/imageScreenWidth",  640);
    int imageScreenHeight = configFile.getInt("tools/clute/imageScreenHeight", 480);
    int imageScreenPosX   = configFile.getInt("tools/clute/imageScreenPosX",   0);
    int imageScreenPosY   = configFile.getInt("tools/clute/imageScreenPosY",   50);

    int thresholdScreenWidth  = configFile.getInt("tools/clute/thresholdScreenWidth",  255);
    int thresholdScreenHeight = configFile.getInt("tools/clute/thresholdScreenHeight", 255);
    int thresholdScreenPosX   = configFile.getInt("tools/clute/thresholdScreenPosX",   650);
    int thresholdScreenPosY   = configFile.getInt("tools/clute/thresholdScreenPosY",   50);

    int colorComboBoxWidth = configFile.getInt("tools/clute/colorComboBoxWidth", 150);
    int colorComboBoxHeight = configFile.getInt("tools/clute/colorComboBoxHeight", 40);
    int colorComboBoxPosX = configFile.getInt("tools/clute/colorComboBoxPosX", 0);
    int colorComboBoxPosY = configFile.getInt("tools/clute/colorComboBoxPosY", 0);

    int saveButtonWidth = configFile.getInt("tools/clute/saveButtonWidth", 200);
    int saveButtonHeight = configFile.getInt("tools/clute/saveButtonHeight", 40);
    int saveButtonPosX = configFile.getInt("tools/clute/saveButtonPosX", 200);
    int saveButtonPosY = configFile.getInt("tools/clute/saveButtonPosY", 0);

    int alphaScrollBarWidth = configFile.getInt("tools/clute/alphaScrollBarWidth", 640);
    int alphaScrollBarHeight = configFile.getInt("tools/clute/alphaScrollBarHeight", 20);
    int alphaScrollBarPosX = configFile.getInt("tools/clute/alphaScrollBarPosX", 0);
    int alphaScrollBarPosY = configFile.getInt("tools/clute/alphaScrollBarPosY", 530);
    int alphaScrollBarThickness = configFile.getInt("tools/clute/alphaScrollBarThickness", 5);
    int segmentedAlpha = localSettings.getSegmentedAlpha();

    int pauseButtonWidth = configFile.getInt("tools/clute/pauseButtonWidth", 200);
    int pauseButtonHeight = configFile.getInt("tools/clute/pauseButtonHeight", 40);
    int pauseButtonPosX = configFile.getInt("tools/clute/pauseButtonPosX", 0);
    int pauseButtonPosY = configFile.getInt("tools/clute/pauseButtonPosY", 550);
    boolean imagePaused = localSettings.isImagePaused();

    int autoSegmentButtonWidth = configFile.getInt("tools/clute/autoSegmentButtonWidth", 200);
    int autoSegmentButtonHeight = configFile.getInt("tools/clute/autoSegmentButtonHeight", 40);
    int autoSegmentButtonPosX = configFile.getInt("tools/clute/autoSegmentButtonPosX", 250);
    int autoSegmentButtonPosY = configFile.getInt("tools/clute/autoSegmentButtonPosY", 550);
    boolean autoSegment = localSettings.shouldAutoSegment();

    setLocationRelativeTo(null);
    setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);
    setFocusable(true);
    
    // Add a key listener
    cluteWindowKeys = new CluteWindowKeys(this);
    setFocusable(true);
    addKeyListener(cluteWindowKeys);
    
    setSize(new Dimension(windowWidth + WIDTH_ADD, windowHeight + HEIGHT_ADD));
    
    imageScreen = new ImageCanvas(configFile, imageScreenWidth, imageScreenHeight);
    imageScreen.setBounds(imageScreenPosX, imageScreenPosY, imageScreenWidth, imageScreenHeight);
    imageScreen.setSegmentedAlpha(segmentedAlpha);
    
    thresholdScreen = new DirectCanvas(thresholdScreenWidth, thresholdScreenHeight);
    thresholdScreen.setBounds(thresholdScreenPosX, thresholdScreenPosY, thresholdScreenWidth, thresholdScreenHeight);
    
    clute = new Clute(configFile, imageScreen);
    clute.setImagePaused(imagePaused);
    clute.setAutoSegment(autoSegment);
    
    // Add window listener
    addWindowListener(new CluteWindowListener(this, localSettings));
    
    // Add a combo box for all the colors
    JComboBox colorComboBox = new JComboBox(clute.getColors().getAllNames());
    colorComboBox.setEditable(false);
    colorComboBox.setFocusable(false);
    colorComboBox.setBounds(colorComboBoxPosX, colorComboBoxPosY, colorComboBoxWidth, colorComboBoxHeight);
    // Add an action listener for the combo box
    colorComboBox.addActionListener(new ColorChange(clute));
    
    // Add a button to save the threshold
    JButton saveButton = new JButton("Save threshold");
    saveButton.setFocusable(false);
    saveButton.setMnemonic(KeyEvent.VK_S);
    saveButton.setBounds(saveButtonPosX, saveButtonPosY, saveButtonWidth, saveButtonHeight);
    // Add an action listener for the save button
    saveButton.addActionListener(new SaveThreshold(clute));
    
    // Add a scroll bar to adjust the alpha
    alphaScrollBar = new JScrollBar(JScrollBar.HORIZONTAL, segmentedAlpha, alphaScrollBarThickness, 0, 100);
    alphaScrollBar.setFocusable(false);
    alphaScrollBar.setUnitIncrement(1);
    alphaScrollBar.setBounds(alphaScrollBarPosX, alphaScrollBarPosY, alphaScrollBarWidth, alphaScrollBarHeight);
    // Add an adjustment listener
    alphaScrollBar.addAdjustmentListener(new SegmentedAlphaChange(imageScreen));
    
    // Add a button to pause/unpause the image
    pauseButton = new JToggleButton("Pause Image", imagePaused);
    pauseButton.setFocusable(false);
    pauseButton.setBounds(pauseButtonPosX, pauseButtonPosY, pauseButtonWidth, pauseButtonHeight);
    // Add an item state listener for the save button
    pauseButton.addItemListener(new PauseImage(this));
    
    // Add a button to toggle auto-segmenting the image
    autoSegmentButton = new JToggleButton("Auto Segment", autoSegment);
    autoSegmentButton.setEnabled(imagePaused);
    autoSegmentButton.setFocusable(false);
    autoSegmentButton.setBounds(autoSegmentButtonPosX, autoSegmentButtonPosY, autoSegmentButtonWidth, autoSegmentButtonHeight);
    // Add an item state listener for the save button
    autoSegmentButton.addItemListener(new AutoSegmentImage(clute));
    
    // Add default listeners for image screen
    ImageZoom imageZoom = new ImageZoom(imageScreen);
    imageScreen.addMouseWheelListener(imageZoom);
//    ImageDrag imageDrag = new ImageDrag(imageScreen);
//    imageScreen.addMouseMotionListener(imageDrag);
//    imageScreen.setCursor(Cursor.getPredefinedCursor(Cursor.MOVE_CURSOR));
    ImagePaintbrush imagePaintbrush = new ImagePaintbrush(clute, imageScreen);
    imageScreen.addMouseListener(imagePaintbrush);
    imageScreen.addMouseMotionListener(imagePaintbrush);
    
    // Add all the ui components
    getContentPane().add(colorComboBox);
    getContentPane().add(saveButton);
    getContentPane().add(alphaScrollBar);
    getContentPane().add(pauseButton);
    getContentPane().add(autoSegmentButton);
    getContentPane().add(imageScreen);
    getContentPane().add(thresholdScreen);
    
    // Prevent resizing
    setResizable(false);
    
    setLocation(windowPosX, windowPosY);
    
    this.setVisible(true);
    
    clute.connectTo(robotHost, port);
  }
  
  public void flipImageScreen() {
    imageScreen.flipScreen();
  }
  
  public Graphics2D getImageGraphicsToDraw() {
    return imageScreen.getGraphicsToDraw();
  }
  
  public void flipThresholdScreen() {
    thresholdScreen.flipScreen();
  }
  
  public Graphics2D getThresholdGraphicsToDraw() {
    return thresholdScreen.getGraphicsToDraw();
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
  
  public void toggleImagePaused() {
    pauseButton.doClick();
  }
  
  public void setImagePaused(boolean imagePaused) {
    clute.setImagePaused(imagePaused);
    autoSegmentButton.setEnabled(imagePaused);
  }
  
  public boolean isImagePaused() {
    return clute.isImagePaused();
  }
  
  public void toggleAutoSegment() {
    if (autoSegmentButton.isEnabled()) {
      autoSegmentButton.doClick();
    }
  }
  
  public boolean shouldAutoSegment() {
    return clute.shouldAutoSegment();
  }
  
  private CluteLocalSettings localSettings;
  
  private Clute clute;
  
  private CluteWindowKeys cluteWindowKeys;
  
  private ImageCanvas imageScreen;
  private DirectCanvas thresholdScreen;
  
  private JScrollBar alphaScrollBar;
  private JToggleButton pauseButton, autoSegmentButton;
  
  private static final long serialVersionUID = -822508356043237506L;
  
  // Additional width and height to handle the window frame size
  private static final int WIDTH_ADD = 10;
  private static final int HEIGHT_ADD = 33;
}
