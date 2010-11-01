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

import java.io.FileInputStream;
import java.io.FileOutputStream;

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
    Y_BITS = visionConfig.getInt("vision/thresholdYBits", 4);
    U_BITS = visionConfig.getInt("vision/thresholdUBits", 6);
    V_BITS = visionConfig.getInt("vision/thresholdVBits", 6);

    ConfigFile vclassConfig = new ConfigFile(VISION_CONFIG_PATH, VCLASSES_CONFIG_FILE);
    this.classes = new ColorClass[vclassConfig.getInt("vision/numClasses", 0)];
    for(int i=0; i < classes.length; i++)
    	classes[i] = new ColorClass(vclassConfig, i);
    
    t_map = new byte[1<<Y_BITS][1<<U_BITS][1<<V_BITS];
    reloadThreshold();
    
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
  
  public void reloadThreshold()
  {
	try {
		byte[] temp = new byte[(1<<Y_BITS)*(1<<U_BITS)*(1<<V_BITS)];
		System.out.println("Loading from " + VISION_CONFIG_PATH + "vision/threshold");
		FileInputStream fStream = new FileInputStream(VISION_CONFIG_PATH + "vision/threshold");
		fStream.read(temp);
		fStream.close();
		for(int i=0; i < (1<<Y_BITS); i++)
		for(int j=0; j < (1<<U_BITS); j++)
		for(int k=0; k < (1<<V_BITS); k++)
			t_map[i][j][k] = temp[(i*(1<<U_BITS)+j)*(1<<V_BITS)+k];
	} catch (java.io.IOException e) {
		System.err.println(e);
	}
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
      try {
        Thread.sleep(RECONNECT_SLEEP_TIME);
      }
      catch (Exception e) {
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
      
      segmentImage();
      
      imageCanvas.setOriginalImage(originalbufferedImage);
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
  
  private void fillBlock(YUV c1, YUV c2, int label)
  {
    int y1 = c1.getY();
    int y2 = c2.getY();
    int u1 = c1.getU();
    int u2 = c2.getU();
    int v1 = c1.getV();
    int v2 = c2.getV();
    
    for(int y=Math.min(y1,y2); y <= Math.max(y1,y2); y++)
    for(int u=Math.min(u1,u1); u <= Math.max(u1,u2); u++)
    for(int v=Math.min(v1,v2); v <= Math.max(v1,v2); v++)
    {
  		t_map[y>>(8-Y_BITS)][u>>(8-U_BITS)][v>>(8-V_BITS)] = (byte)label;
  	}
  }
  
  public void applyLabel(YUV color, int classIdx)
  {
    Lab colorL = new Lab(color);
    System.out.println(color.getY() + " " + color.getU() + " " + color.getV());
    System.out.println(colorL.getL() + " " + colorL.getA() + " " + colorL.getB());
    YUV yuvL = colorL.toYUV();
    System.out.println(yuvL.getY() + " " + yuvL.getU() + " " + yuvL.getV());
    
    YUV last = null;
    System.out.println(t_map[color.getY()>>(8-Y_BITS)][color.getU()>>(8-U_BITS)][color.getV()>>(8-V_BITS)]);
    for(double L = colorL.getL() - 5; L <= colorL.getL() + 5; L+=2)
    for(double A = colorL.getA() - 5; A <= colorL.getA() + 5; A+=2)
    for(double B = colorL.getB() - 5; B <= colorL.getB() + 5; B+=2)
    {
    	YUV yuv = new Lab(L, A, B).toYUV();
    	
    	//System.out.println(L + " " + A + " " + B);
    	//System.out.println(yuv.getY() + " " + yuv.getU() + " " + yuv.getV());
    	
    	if (last!=null)
    	{
    		System.out.println("("+last.getY()+","+last.getU()+","+last.getV()+")-("+yuv.getY()+","+yuv.getU()+","+yuv.getV()+")");
    		fillBlock(last, yuv, classIdx);
    	}
    	
    	last = yuv;
    }
    System.out.println(t_map[color.getY()>>(8-Y_BITS)][color.getU()>>(8-U_BITS)][color.getV()>>(8-V_BITS)]);
    System.out.println("updated");
  }
  
  /*public void updateCurrentClass(int yl, int yu, int ul, int uu, int vl, int vu)
  {
    classes[currentClass].yl = yl;
    classes[currentClass].yu = yu;
    classes[currentClass].ul = ul;
    classes[currentClass].uu = uu;
    classes[currentClass].vl = vl;
    classes[currentClass].vu = vu;
    
   	System.out.println(yl + " " + yu + " " + ul + " " + uu + " " + vl + " " + vu);
    
    segmentImage();
  }*/
  
  public YUVImage getOriginalImage()
  {
    return originalImage;
  }
  
  public void segmentImage() {
    if (originalImage == null) {
      return;
    }
    
    byte[] indices = new byte[IMAGE_WIDTH*IMAGE_HEIGHT];
    
    for(int i=0; i < IMAGE_HEIGHT; i++)
    for(int j=0; j < IMAGE_WIDTH; j++)
    {
      /*for(int k=0; k < classes.length; k++)
        if (classes[k].match(originalImage.getYUV(j,i)))
          indices[j+i*IMAGE_WIDTH] = (byte)k;*/
      YUV yuv = originalImage.getYUV(j,i);
      indices[j+i*IMAGE_WIDTH] = t_map[yuv.getY()>>(8-Y_BITS)][yuv.getU()>>(8-U_BITS)][yuv.getV()>>(8-V_BITS)];
    }
    
    try {
      SegmentedImage segmentedImage = new SegmentedImage(indices, IMAGE_WIDTH, IMAGE_HEIGHT, classes);
      imageCanvas.setSegmentedImage(segmentedImage.toBufferedImage());
    }
    catch (Exception e) {
    }
  }
  
  public void saveThreshold() {
    /*try {
      FileWriter fw = new FileWriter(VISION_CONFIG_PATH + VCLASSES_CONFIG_FILE);
      fw.write("vision/numClasses = " + classes.length + "\n");
      fw.write("\n");
      for(ColorClass cc : classes)
        cc.save(fw);
      JOptionPane.showMessageDialog(imageCanvas, "Color class data saved", "Save successful", JOptionPane.INFORMATION_MESSAGE);
    } catch (java.io.IOException e) {
      e.printStackTrace();
    }*/
	byte[] temp = new byte[(1<<Y_BITS)*(1<<U_BITS)*(1<<V_BITS)];
	for(int i=0; i < (1<<Y_BITS); i++)
	for(int j=0; j < (1<<U_BITS); j++)
	for(int k=0; k < (1<<V_BITS); k++)
		temp[(i*(1<<U_BITS)+j)*(1<<V_BITS)+k] = t_map[i][j][k];
	
    try {
    	System.out.println("Saved to " + VISION_CONFIG_PATH + "vision/threshold");
		FileOutputStream fStream = new FileOutputStream(VISION_CONFIG_PATH + "vision/threshold");
		fStream.write(temp);
		fStream.close();
	} catch (java.io.IOException e) {
		System.err.println(e);
	}
  }
  
  public boolean isImagePaused() {
    return imagePaused;
  }
  
  public void setImagePaused(boolean imagePaused) {
    this.imagePaused = imagePaused;
  }
  
  private final int  IMAGE_WIDTH, IMAGE_HEIGHT;
  private final int  Y_BITS, U_BITS, V_BITS;
  
  private byte[][][] t_map;
  
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
