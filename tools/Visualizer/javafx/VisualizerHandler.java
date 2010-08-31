package javafx;

import java.awt.image.*;
import java.util.Iterator;

import config.*;
import image.*;
import logwriter.*;
import network.*;
import network.message.*;
import network.message.fromrobot.*;
import network.message.ssl.*;
import network.message.ssl.RobotMessage.RobotColor;
import shape.*;

public class VisualizerHandler implements MessageFromRobotListener, MessageFromSSLListener {

  private VisualizerHandler() {
    ConfigFile configFile = VisualizerConfig.getInstance();
    
    imageWidth = configFile.getInt("vision/imageWidth", 320);
    imageHeight = configFile.getInt("vision/imageHeight", 240);
    
    minSSLBallConfidence  = (float)configFile.getDouble("visualizer/minSSLBallConfidence",  0.1);
    minSSLRobotConfidence = (float)configFile.getDouble("visualizer/minSSLRobotConfidence", 0.1);
    
    sslOnBlueTeam = configFile.getBool("sslVision/onBlueTeam", true);
    
    sslBallFillColor = configFile.getInt("visualizer/sslBallFillColor", 0xFF0000);
    sslBallRadius    = (float)configFile.getDouble("visualizer/sslBallRadius", 10);
    
    sslBlueRobotFillColor   = configFile.getInt("visualizer/sslBlueRobotFillColor", 0x0000FF);
    sslYellowRobotFillColor = configFile.getInt("visualizer/sslYellowRobotFillColor", 0xFFFF00);
    sslRobotRadius          = (float)configFile.getDouble("visualizer/sslRobotRadius", 10);
    
    robotConnector = new RobotConnector(configFile);
    sslConnector   = new SSLConnector(configFile);
    
    robotCoordinates = new CoordinatesConverter(configFile);
    sslCoordinates   = new CoordinatesConverter(configFile);
    
    haveRobotUpdate = false;
    
    robotUpdateInProgress = new Update(true);
    robotUpdate           = new Update(true);
    
    haveSSLUpdate = false;
    
    sslUpdateInProgress = new Update(sslOnBlueTeam);
    sslUpdate           = new Update(sslOnBlueTeam);
    
    logWriter = LogWriter.create(configFile);
    sslLogWriter = SSLLogWriter.create(configFile);
  }
  
  public void connectToRobot(String robotAddress, int robotPort) {
    robotConnector.addListener(this);
    if (logWriter != null) {
      robotConnector.addListener(logWriter);
    }
    if (sslLogWriter != null){
      robotConnector.addListener(sslLogWriter);
    }
    if (!robotConnector.connect(robotAddress, robotPort)) {
      System.out.println("Error connecting to robot.");
    }
  }
  
  public void connectToSSLVision(String sslAddress, int sslPort) {
    sslConnector.addListener(this);
    if (sslLogWriter != null){
      sslConnector.addListener(sslLogWriter);
    } 
    if (!sslConnector.connect(sslAddress, sslPort)) {
      System.out.println("Error connecting to SSLVision server.");
    }
  }
  
  public void finish() {
    robotConnector.disconnect();
    robotConnector.removeListener(this);
    
    if (logWriter != null) {
      robotConnector.removeListener(logWriter);
    }
    if (sslLogWriter != null){
      robotConnector.removeListener(sslLogWriter);
      sslConnector.removeListener(sslLogWriter);
    }
    sslConnector.disconnect();
    sslConnector.removeListener(this);
  }
  
  @Override
  public synchronized void receiveRemoteMessageFromRobot(RemoteMessageFromRobot message) {
    if (message instanceof LogBeginMessage) {
      robotUpdateInProgress = new Update(robotUpdateInProgress.isBlue());
      System.out.println("LogBegin");
    }
    else if (message instanceof LogEndMessage) {
      haveRobotUpdate = true;
      robotUpdate = robotUpdateInProgress;
      robotUpdateInProgress = new Update(robotUpdateInProgress.isBlue());
      System.out.println("LogEnd");
    }
    else {
      robotUpdateInProgress.addMessage(message);
      System.out.println("LogOther");
    }
  }
  
  @Override
  public synchronized void receiveSSLMessage(SSLMessage message) {
    if (message instanceof TickMessage) {
      haveSSLUpdate = true;
      sslUpdate = sslUpdateInProgress;
      sslUpdateInProgress = new Update(sslOnBlueTeam);
    }
    else {
      sslUpdateInProgress.addSSLMessage(message);
    }
  }
  
  public boolean run(JavaInterface javaInterface) {
    Update robotUpdate = null;
    Update sslUpdate   = null;
    
    synchronized(this) {
      if (!haveRobotUpdate && !haveSSLUpdate) {
        return false;
      }
      else  {
        haveRobotUpdate = false;
        haveSSLUpdate   = false;
        
        // Get the latest updates
        robotUpdate     = this.robotUpdate;
        sslUpdate       = this.sslUpdate;
      }
    }
    
    // Set the color for the coordinates converter
    robotCoordinates.setBlue(robotUpdate.isBlue());
    
    // Loop through all the remote messages
    Iterator<RemoteMessageFromRobot> remoteIterator = robotUpdate.getRemoteMessageIterator();
    while (remoteIterator.hasNext()) {
      RemoteMessageFromRobot message = remoteIterator.next();
      
      if (message instanceof LogTextMessage) {
        LogTextMessage logTextMessage = (LogTextMessage)message;
        
        System.out.println(logTextMessage.getText());
      }
      else if (message instanceof ShapeMessage) {
        ShapeMessage shapeMessage = (ShapeMessage)message;
        addShape(robotCoordinates, javaInterface, shapeMessage.getScreen(), shapeMessage.getShape());
      }
      else if (message instanceof SegmentedImageMessage) {
        SegmentedImageMessage segmentedImageMessage = (SegmentedImageMessage)message;
        SegmentedImage image = segmentedImageMessage.getSegmentedImage();
        
        BufferedImage bufferedImage = image.toBufferedImage();
        javafx.scene.image.Image fxImage = javafx.ext.swing.SwingUtils.toFXImage(bufferedImage);
        javaInterface.drawImage(Screen.segmentedImage.getID(), fxImage);
      }
      else {
        switch (message.getType()) {
        case ORIGINAL_IMAGE:
          byte [] bytes = message.getData();
          YUV422Image image = new YUV422Image(bytes, imageWidth, imageHeight);
          BufferedImage bufferedImage = image.toBufferedImage();
          javafx.scene.image.Image fxImage = javafx.ext.swing.SwingUtils.toFXImage(bufferedImage);
          javaInterface.drawImage(Screen.originalImage.getID(), fxImage);
          
          break;
        }
      }
      
    }
      
    // Loop through all the SSL messages
    Iterator<SSLMessage> sslIterator = sslUpdate.getSSLMessageIterator();
    while (sslIterator.hasNext()) {
      SSLMessage sslMessage = sslIterator.next();
      
      if (sslMessage instanceof BallMessage) {
        BallMessage ballMessage = (BallMessage)sslMessage;
        
        if (ballMessage.getConfidence() < minSSLBallConfidence) {
          continue;
        }
        
        Shape shape = new Circle(ballMessage.getX(), ballMessage.getY(),
                                 sslBallRadius,
                                 sslBallFillColor);
        addShape(sslCoordinates, javaInterface, Screen.field, shape);
      }
      else if (sslMessage instanceof RobotMessage) {
        RobotMessage robotMessage = (RobotMessage)sslMessage;
        
        if (robotMessage.getConfidence() < minSSLRobotConfidence) {
          continue;
        }
        
        Shape shape = new Circle(robotMessage.getX(), robotMessage.getY(),
                                 sslRobotRadius,
                                 robotMessage.getColor() == RobotColor.blue ?
                                   sslBlueRobotFillColor
                                 : sslYellowRobotFillColor);
        addShape(sslCoordinates, javaInterface, Screen.field, shape);          
      }
    }
    
    return true;
  }
  
  private void addShape(CoordinatesConverter coordinates, JavaInterface javaInterface, Screen screen, Shape shape) {
    if (shape instanceof FieldObject) {
      FieldObject fieldObject = (FieldObject)shape;
      if (fieldObject.havePosition() && fieldObject.haveAngle()) {
        javaInterface.drawFieldObject(screen.getID(),
                                      coordinates.getX(screen, fieldObject.getCenterX()), coordinates.getY(screen, fieldObject.getCenterY()),
                                      fieldObject.getRadius(),
                                      fieldObject.getFillAlpha(), fieldObject.getFillR(), fieldObject.getFillG(), fieldObject.getFillB(),
                                      fieldObject.getStrokeAlpha(), fieldObject.getStrokeR(), fieldObject.getStrokeG(), fieldObject.getStrokeB(),
                                      fieldObject.getStrokeThickness(),
                                      fieldObject.getPositionAlpha(), fieldObject.getPositionR(), fieldObject.getPositionG(), fieldObject.getPositionB(),
                                      fieldObject.getPositionDeviation(),
                                      fieldObject.getAngleAlpha(), fieldObject.getAngleR(), fieldObject.getAngleG(), fieldObject.getAngleB(),
                                      coordinates.getAngle(screen, fieldObject.getAngle()), (float)Math.toDegrees(fieldObject.getAngleDeviation()),
                                      "");
      }
      else if (fieldObject.havePosition()) {
        javaInterface.drawFieldObject(screen.getID(),
                                      coordinates.getX(screen, fieldObject.getCenterX()), coordinates.getY(screen, fieldObject.getCenterY()),
                                      fieldObject.getRadius(),
                                      fieldObject.getFillAlpha(), fieldObject.getFillR(), fieldObject.getFillG(), fieldObject.getFillB(),
                                      fieldObject.getStrokeAlpha(), fieldObject.getStrokeR(), fieldObject.getStrokeG(), fieldObject.getStrokeB(),
                                      fieldObject.getStrokeThickness(),
                                      fieldObject.getPositionAlpha(), fieldObject.getPositionR(), fieldObject.getPositionG(), fieldObject.getPositionB(),
                                      fieldObject.getPositionDeviation(),
                                      "");
      }
      else if (fieldObject.haveAngle()) {
        javaInterface.drawFieldObject(screen.getID(),
                                      coordinates.getX(screen, fieldObject.getCenterX()), coordinates.getY(screen, fieldObject.getCenterY()),
                                      fieldObject.getRadius(),
                                      fieldObject.getFillAlpha(), fieldObject.getFillR(), fieldObject.getFillG(), fieldObject.getFillB(),
                                      fieldObject.getStrokeAlpha(), fieldObject.getStrokeR(), fieldObject.getStrokeG(), fieldObject.getStrokeB(),
                                      fieldObject.getStrokeThickness(),
                                      fieldObject.getAngleAlpha(), fieldObject.getAngleR(), fieldObject.getAngleG(), fieldObject.getAngleB(),
                                      coordinates.getAngle(screen, fieldObject.getAngle()), (float)Math.toDegrees(fieldObject.getAngleDeviation()),
                                      "");
      }
      else {
        javaInterface.drawFieldObject(screen.getID(),
                                      coordinates.getX(screen, fieldObject.getCenterX()), coordinates.getY(screen, fieldObject.getCenterY()),
                                      fieldObject.getRadius(),
                                      fieldObject.getFillAlpha(), fieldObject.getFillR(), fieldObject.getFillG(), fieldObject.getFillB(),
                                      fieldObject.getStrokeAlpha(), fieldObject.getStrokeR(), fieldObject.getStrokeG(), fieldObject.getStrokeB(),
                                      fieldObject.getStrokeThickness(),
                                      "");
      }
    }
    else if (shape instanceof Circle) {
      Circle circle = (Circle)shape;
      if (circle.isFilled() && circle.isStroked()) {
        javaInterface.drawCircle(screen.getID(),
                                 coordinates.getX(screen, circle.getCenterX()), coordinates.getY(screen, circle.getCenterY()),
                                 circle.getRadius(),
                                 circle.getFillAlpha(), circle.getFillR(), circle.getFillG(), circle.getFillB(),
                                 circle.getStrokeAlpha(), circle.getStrokeR(), circle.getStrokeG(), circle.getStrokeB(),
                                 circle.getStrokeThickness());
      }
      else if (circle.isFilled()) {
        javaInterface.drawCircle(screen.getID(),
                                 coordinates.getX(screen, circle.getCenterX()), coordinates.getY(screen, circle.getCenterY()),
                                 circle.getRadius(),
                                 circle.getFillAlpha(), circle.getFillR(), circle.getFillG(), circle.getFillB());
      }
      else if (circle.isStroked()) {
        javaInterface.drawCircle(screen.getID(),
                                 coordinates.getX(screen, circle.getCenterX()), coordinates.getY(screen, circle.getCenterY()),
                                 circle.getRadius(),
                                 circle.getStrokeAlpha(), circle.getStrokeR(), circle.getStrokeG(), circle.getStrokeB(),
                                 circle.getStrokeThickness());
      }
    }
    else if (shape instanceof Rectangle) {
      Rectangle rectangle = (Rectangle)shape;
      if (rectangle.isFilled() && rectangle.isStroked()) {
        javaInterface.drawRectangle(screen.getID(),
                                    coordinates.getX(screen, rectangle.getTopLeftX()), coordinates.getY(screen, rectangle.getTopLeftY()),
                                    rectangle.getLength(), rectangle.getWidth(),
                                    rectangle.getFillAlpha(), rectangle.getFillR(), rectangle.getFillG(), rectangle.getFillB(),
                                    rectangle.getStrokeAlpha(), rectangle.getStrokeR(), rectangle.getStrokeG(), rectangle.getStrokeB(),
                                    rectangle.getStrokeThickness());
      }
      else if (rectangle.isFilled()) {
        javaInterface.drawRectangle(screen.getID(),
                                    coordinates.getX(screen, rectangle.getTopLeftX()), coordinates.getY(screen, rectangle.getTopLeftY()),
                                    rectangle.getLength(), rectangle.getWidth(),
                                    rectangle.getFillAlpha(), rectangle.getFillR(), rectangle.getFillG(), rectangle.getFillB());
      }
      else if (rectangle.isStroked()) {
        javaInterface.drawRectangle(screen.getID(),
                                    coordinates.getX(screen, rectangle.getTopLeftX()), coordinates.getY(screen, rectangle.getTopLeftY()),
                                    rectangle.getLength(), rectangle.getWidth(),
                                    rectangle.getStrokeAlpha(), rectangle.getStrokeR(), rectangle.getStrokeG(), rectangle.getStrokeB(),
                                    rectangle.getStrokeThickness());
      }
    }
    else if (shape instanceof Triangle) {
      Triangle triangle = (Triangle)shape;
      if (triangle.isFilled() && triangle.isStroked()) {
        javaInterface.drawTriangle(screen.getID(),
                                   coordinates.getX(screen, triangle.getEnd1X()), coordinates.getY(screen, triangle.getEnd1Y()),
                                   coordinates.getX(screen, triangle.getEnd2X()), coordinates.getY(screen, triangle.getEnd2Y()),
                                   coordinates.getX(screen, triangle.getEnd3X()), coordinates.getY(screen, triangle.getEnd3Y()),
                                   triangle.getFillAlpha(), triangle.getFillR(), triangle.getFillG(), triangle.getFillB(),
                                   triangle.getStrokeAlpha(), triangle.getStrokeR(), triangle.getStrokeG(), triangle.getStrokeB(),
                                   triangle.getStrokeThickness());
      }
      else if (triangle.isFilled()) {
        javaInterface.drawTriangle(screen.getID(),
                                   coordinates.getX(screen, triangle.getEnd1X()), coordinates.getY(screen, triangle.getEnd1Y()),
                                   coordinates.getX(screen, triangle.getEnd2X()), coordinates.getY(screen, triangle.getEnd2Y()),
                                   coordinates.getX(screen, triangle.getEnd3X()), coordinates.getY(screen, triangle.getEnd3Y()),
                                   triangle.getFillAlpha(), triangle.getFillR(), triangle.getFillG(), triangle.getFillB());
      }
      else if (triangle.isStroked()) {
        javaInterface.drawTriangle(screen.getID(),
                                   coordinates.getX(screen, triangle.getEnd1X()), coordinates.getY(screen, triangle.getEnd1Y()),
                                   coordinates.getX(screen, triangle.getEnd2X()), coordinates.getY(screen, triangle.getEnd2Y()),
                                   coordinates.getX(screen, triangle.getEnd3X()), coordinates.getY(screen, triangle.getEnd3Y()),
                                   triangle.getStrokeAlpha(), triangle.getStrokeR(), triangle.getStrokeG(), triangle.getStrokeB(),
                                   triangle.getStrokeThickness());
      }
    }
    else if (shape instanceof Line) {
      Line line = (Line)shape;
      if (line.isStroked()) {
        javaInterface.drawLine(screen.getID(),
                               coordinates.getX(screen, line.getEnd1X()), coordinates.getY(screen, line.getEnd1Y()), 
                               coordinates.getX(screen, line.getEnd2X()), coordinates.getY(screen, line.getEnd2Y()), 
                               line.getStrokeAlpha(), line.getStrokeR(), line.getStrokeG(), line.getStrokeB(),
                               line.getStrokeThickness());
      }
    }
  }
  
  private final int imageWidth;
  private final int imageHeight;
  
  private final float minSSLBallConfidence;
  private final float minSSLRobotConfidence;
  
  private final boolean sslOnBlueTeam;
  
  private final int   sslBallFillColor;
  private final float sslBallRadius;
  
  private final int   sslYellowRobotFillColor, sslBlueRobotFillColor;
  private final float sslRobotRadius;
  
  private RobotConnector robotConnector;
  private SSLConnector   sslConnector;
  
  private CoordinatesConverter robotCoordinates, sslCoordinates;
  
  private boolean haveRobotUpdate;
  private Update  robotUpdateInProgress, robotUpdate;
  
  private boolean haveSSLUpdate;
  private Update  sslUpdateInProgress, sslUpdate;
  
  private LogWriter logWriter;
  private SSLLogWriter sslLogWriter;
  public static VisualizerHandler getInstance() {
    return instance;
  }
  
  private static VisualizerHandler instance = new VisualizerHandler();
}
