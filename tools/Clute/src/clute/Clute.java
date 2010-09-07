package clute;

import java.awt.image.*;

import config.*;
import image.*;
import image.format.*;
import network.*;
import network.message.*;
//import network.message.fromrobot.*;
import ui.swing.clute.*;

public class Clute implements Runnable, MessageFromRobotListener {

  public Clute(ConfigFile configFile, ImageCanvas imageCanvas) {
    RECONNECT_SLEEP_TIME = configFile.getMillisecondsFromSeconds("tools/clute/reconnectSleepTime", 100);
    
    IMAGE_WIDTH = configFile.getInt("tools/clute/imageWidth", 320);
    IMAGE_HEIGHT = configFile.getInt("tools/clute/imageHeight", 240);
    
    thresholdFilename = configFile.getPath("tools/clute/thresholdPath", "threshold.tmap.gz");
    
    colors = Colors.createFromFile(configFile.getPath("tools/clute/colorsPath", "colors.txt"));
    threshold = Threshold.createFromFile(thresholdFilename);
    
    robotHost = null;
    robotPort = 0;
    
    imagePaused = false;
    autoSegment = true;
    
    robotConnector = new RobotConnector(configFile);
    robotConnector.addListener(this);
    
    this.imageCanvas = imageCanvas;
    
    thread = null;
    
    originalImage = null;
    currentColor = 0;
  }
  
  public Colors getColors() {
    return colors;
  }
  
  public Threshold getThreshold() {
    return threshold;
  }
  
  public void connectTo(String robotHost, int robotPort) {
    this.robotHost = robotHost;
    this.robotPort = robotPort;
    
    startThread();
  }
  
  public void run() {
    if (reconnectRunning) {
      return;
    }
    
    reconnectRunning = true;
    while (reconnectRunning) {
      if ((robotHost == null) || (robotHost.length() == 0)) {
        break;
      }
      if (!robotConnector.isConnected()) {
        if (robotConnector.connect(robotHost, robotPort)) {
          System.out.println("Connected to robot at " + robotHost + ".");
        }
      }
      if (robotConnector.isConnected()) {
        try {
          Thread.sleep(RECONNECT_SLEEP_TIME);
        }
        catch (Exception e) {
        }
      }
    }
  }

  public void startThread() {
    if (thread != null) {
      stopThread();
    }
    thread = new Thread(this);
    thread.start();
  }
  
  public void stopThread() {
    reconnectRunning = false;
    while (thread.isAlive());
  }
  
  public void receiveRemoteMessageFromRobot(RemoteMessageFromRobot message) {
    // If the image is paused, we ignore new images from the robot
    if (imagePaused) {
      return;
    }
    
    if (message.getType() == RemoteMessageFromRobot.MessageType.ORIGINAL_IMAGE) {
      byte [] bytes = message.getData();
      originalImage = new YUV422Image(bytes, IMAGE_WIDTH, IMAGE_HEIGHT);
      BufferedImage originalbufferedImage = originalImage.toBufferedImage();
      
      imageCanvas.setOriginalImage(originalbufferedImage);
      
      segmentImage();
    }
//    else if (message instanceof SegmentedImageMessage) {
//      SegmentedImageMessage segmentedImageMessage = (SegmentedImageMessage)message;
//      SegmentedImage image = segmentedImageMessage.getSegmentedImage();
//      BufferedImage segmentedImage = image.toBufferedImage();
//      
//      imageCanvas.setSegmentedImage(segmentedImage);
//    }
  }
  
  public void setCurrentColor(int currentColor) {
    this.currentColor = currentColor;
  }
  
  public void updateThresholdFromImage(int imageX, int imageY) {
    if (originalImage == null) {
      return;
    }
    if ((imageX < 0) || (imageX >= IMAGE_WIDTH) || (imageY < 0) || (imageY >= IMAGE_HEIGHT)) {
      return;
    }
    
    YUV yuv = originalImage.getYUV(imageX, imageY);
    threshold.setIndex(yuv, currentColor);
    
    if (autoSegment) {
      segmentImage();
    }
  }
  
  public void updateThresholdDirectly(int y, int u, int v) {
    YUV yuv = null;
    try {
      yuv = new YUV(y, u, v);
    }
    catch (Exception e) {
      return;
    }
    
    threshold.setIndex(yuv, currentColor);
    
    if (autoSegment) {
      segmentImage();
    }
  }
  
  public void segmentImage() {
    if (originalImage == null) {
      return;
    }
    try {
      SegmentedImage segmentedImage = new SegmentedImage(threshold.segmentImage(originalImage), IMAGE_WIDTH, IMAGE_HEIGHT, colors.getAllRGB());
      BufferedImage segmentedBufferedImage = segmentedImage.toBufferedImage();
      imageCanvas.setSegmentedImage(segmentedBufferedImage);
    }
    catch (Exception e) {
    }
  }
  
  public void saveThreshold() {
    threshold.saveToFile(thresholdFilename);
  }
  
  public boolean isImagePaused() {
    return imagePaused;
  }
  
  public void setImagePaused(boolean imagePaused) {
    this.imagePaused = imagePaused;
  }
  
  public boolean shouldAutoSegment() {
    return autoSegment;
  }
  
  public void setAutoSegment(boolean autoSegment) {
    this.autoSegment = autoSegment;
    if (autoSegment) {
      segmentImage();
    }
  }
  
  private final long RECONNECT_SLEEP_TIME;
  private final int  IMAGE_WIDTH, IMAGE_HEIGHT;
  
  private String thresholdFilename;
  
  private String robotHost;
  private int    robotPort;
  
  private boolean reconnectRunning;
  
  private boolean imagePaused, autoSegment;

  private RobotConnector robotConnector;
  
  private ImageCanvas imageCanvas;
  
  private Thread thread;
  
  private Colors colors;
  private Threshold threshold;
  
  private YUVImage originalImage;
  private int currentColor;
}
