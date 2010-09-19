package core;

import java.awt.image.*;
import javax.swing.JOptionPane;
import java.io.FileWriter;

import config.*;
import image.*;
import image.format.*;
import network.*;
import network.message.*;
//import network.message.fromrobot.*;
import ui.*;

public class Threshedit implements Runnable, MessageFromRobotListener {

  public static void main(String[] args) throws Exception {
    if (args.length < 1) {
      System.out.println("./run.sh robot_ip");
      return;
    }
    
    new Threshedit(args[0], 10000);
  }

  private static final long RECONNECT_SLEEP_TIME = 100;
  private static final String VISION_CONFIG_PATH = getCMurfsDir() + "robot/config/";
  private static final String VISION_CONFIG_FILE = "vision/vision.txt";
  private static final String VCLASSES_CONFIG_FILE = "vision/vclasses.txt";

  private static String getCMurfsDir() {
    String CMurfsDir = System.getenv("CMURFS_DIR");
    // Check that CMURFS_DIR is valid
    if (CMurfsDir == null) {
      System.err.println("Cannot find CMURFS_DIR environment variable, assuming ../../");
      return "../../";
    }
    
    if (!CMurfsDir.endsWith("/")) {
      CMurfsDir += "/";
    }
    return CMurfsDir;
  }

  public Threshedit(String robotHost, int port) throws Exception {
  	ConfigFile visionConfig = new ConfigFile(VISION_CONFIG_PATH, VISION_CONFIG_FILE);
    IMAGE_WIDTH = visionConfig.getInt("vision/imageWidth", 320);
    IMAGE_HEIGHT = visionConfig.getInt("vision/imageHeight", 240);

    ConfigFile vclassConfig = new ConfigFile(VISION_CONFIG_PATH, VCLASSES_CONFIG_FILE);
    this.classes = new ColorClass[vclassConfig.getInt("vision/numClasses", 0)];
    for(int i=0; i < classes.length; i++)
    	classes[i] = new ColorClass(vclassConfig, i);
    
    imagePaused = false;
    
    robotConnector = new RobotConnector(visionConfig);
    robotConnector.addListener(this);
    
    thread = null;
    
    originalImage = null;
    currentClass = 0;
    
    MainWindow mainWindow = new MainWindow(this, visionConfig);
    imageCanvas = mainWindow.getImageScreen();
    
    connectTo(robotHost, port);
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
        reconnectRunning = false;
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
  
  public ColorClass[] getClasses() {
    return classes;
  }
  
  public int getCurrentClass() {
    return currentClass;
  }
  public void setCurrentClass(int currentClass) {
    this.currentClass = currentClass;
  }
  
  public void updateCurrentClass(int yl, int yu, int ul, int uu, int vl, int vu)
  {
    classes[currentClass].yl = yl;
    classes[currentClass].yu = yu;
    classes[currentClass].ul = ul;
    classes[currentClass].uu = uu;
    classes[currentClass].vl = vl;
    classes[currentClass].vu = yu;
    
    segmentImage();
  }
  
  public void segmentImage() {
    if (originalImage == null) {
      return;
    }
    
    byte[] indices = new byte[IMAGE_WIDTH*IMAGE_HEIGHT];
    
    for(int i=0; i < IMAGE_HEIGHT; i++)
    for(int j=0; j < IMAGE_WIDTH; j++)
    {
      for(int k=0; k < classes.length; k++)
        if (classes[k].match(originalImage.getYUV(j,i)))
          indices[j+i*IMAGE_WIDTH] = (byte)k;
    }
    
    try {
      SegmentedImage segmentedImage = new SegmentedImage(indices, IMAGE_WIDTH, IMAGE_HEIGHT, classes);
      BufferedImage segmentedBufferedImage = segmentedImage.toBufferedImage();
      imageCanvas.setSegmentedImage(segmentedBufferedImage);
    }
    catch (Exception e) {
    }
  }
  
  public void saveThreshold() {
    try {
      FileWriter fw = new FileWriter(VISION_CONFIG_PATH + VCLASSES_CONFIG_FILE);
      fw.write("vision/numClasses = " + classes.length + "\n");
      fw.write("\n");
      for(ColorClass cc : classes)
        cc.save(fw);
      JOptionPane.showMessageDialog(null, "Color class data saved", "Save successful", JOptionPane.INFORMATION_MESSAGE);
    } catch (java.io.IOException e) {
      e.printStackTrace();
    }
  }
  
  public boolean isImagePaused() {
    return imagePaused;
  }
  
  public void setImagePaused(boolean imagePaused) {
    this.imagePaused = imagePaused;
  }
  
  private final int  IMAGE_WIDTH, IMAGE_HEIGHT;
  
  private ColorClass[] classes;
  
  private ImageCanvas imageCanvas;
  
  private String robotHost;
  private int    robotPort;
  
  private boolean reconnectRunning;
  
  private boolean imagePaused;

  private RobotConnector robotConnector;
  
  private Thread thread;
  
  private YUVImage originalImage;
  private int currentClass;
}
