package swing;

import java.awt.*;
import java.awt.image.*;

import config.*;
import network.message.*;
import network.message.fromrobot.*;
import network.message.ssl.*;
import network.message.ssl.RobotMessage.RobotColor;
import shape.*;
import update.*;
import image.*;

public class DisplayHandler {

  public DisplayHandler(ConfigFile configFile) {
    SCREEN_WIDTH  = configFile.getInt("visualizer/screenWidth",  986);
    SCREEN_HEIGHT = configFile.getInt("visualizer/screenHeight", 486);
    
    FIELD_SCREEN_WIDTH           = configFile.getInt("visualizer/fieldScreenWidth",  666);
    FIELD_SCREEN_HEIGHT          = configFile.getInt("visualizer/fieldScreenHeight", 486);
    FIELD_COLOR                  = configFile.getInt("visualizer/fieldColor", 0x003200);
    FIELD_WIDTH                  = configFile.getInt("field/fieldWidth", 400);
    FIELD_LENGTH                 = configFile.getInt("field/fieldLength", 600);
    FIELD_LINE_WIDTH             = configFile.getInt("visualizer/fieldLineWidth", 5);
    PENALTY_BOX_WIDTH            = configFile.getInt("field/penaltyBoxWidth", 220);
    PENALTY_BOX_LENGTH           = configFile.getInt("field/penaltyBoxLength", 60);
    PENALTY_POINT_LENGTH         = configFile.getInt("visualizer/penaltyPointLength", 10); 
    CENTER_CIRCLE_RADIUS         = configFile.getInt("field/centerCircleRadius", 60);
    DIST_CENTER_TO_PENALTY_POINT = configFile.getInt("field/distCenterToPenaltyPoint", 120); 
    BLUE_GOAL_POST_COLOR         = configFile.getInt("visualizer/blueGoalPostColor",0x0000FF); 
    YELLOW_GOAL_POST_COLOR       = configFile.getInt("visualizer/yellowGoalPostColor",0xFFFF00);   
    GOAL_POST_LENGTH             = configFile.getInt("visualizer/goalPostLength", 20);
    GOAL_POST_WIDTH              = configFile.getInt("visualizer/goalPostWidth", 5);  
    GOAL_WIDTH                   = configFile.getInt("field/goalWidth", 140); 
    
    IMAGE_WIDTH               = configFile.getInt("visualizer/imageWidth",  320);
    IMAGE_HEIGHT              = configFile.getInt("visualizer/imageHeight", 240);
    IMAGE_HORIZONTAL_SEPARATOR= configFile.getInt("visualizer/horizontalImageSep", 6);
    
    MIN_SSL_BALL_CONFIDENCE  = (float)configFile.getDouble("visualizer/minSSLBallConfidence",  0);
    MIN_SSL_ROBOT_CONFIDENCE = (float)configFile.getDouble("visualizer/minSSLRobotConfidence", 0);
    
    SSL_BALL_FILL_COLOR = configFile.getInt("visualizer/sslBallFillColor", 0xFF0000);
    SSL_BALL_RADIUS     = (float)configFile.getDouble("visualizer/sslBallRadius", 10);
    
    SSL_BLUE_ROBOT_FILL_COLOR   = configFile.getInt("visualizer/sslBlueRobotFillColor", 0x0000FF);
    SSL_YELLOW_ROBOT_FILL_COLOR = configFile.getInt("visualizer/sslYellowRobotFillCOlor", 0xFFFF00);
    SSL_ROBOT_RADIUS            = (float)configFile.getDouble("visualizer/sslRobotRadius", 10);

    field = new BufferedImage(FIELD_SCREEN_WIDTH,
                              FIELD_SCREEN_HEIGHT,
                              BufferedImage.TYPE_INT_ARGB);
    
    originalImage  = new BufferedImage(IMAGE_WIDTH,
                                       IMAGE_HEIGHT,
                                       BufferedImage.TYPE_INT_ARGB);
    segmentedImage = new BufferedImage(IMAGE_WIDTH,
                                       IMAGE_HEIGHT,
                                       BufferedImage.TYPE_INT_ARGB);

    fieldShapes = new BufferedImage(FIELD_SCREEN_WIDTH,
                                    FIELD_SCREEN_HEIGHT,
                                    BufferedImage.TYPE_INT_ARGB);

    originalImageShapes  = new BufferedImage(IMAGE_WIDTH,
                                             IMAGE_HEIGHT,
                                             BufferedImage.TYPE_INT_ARGB);
    segmentedImageShapes = new BufferedImage(IMAGE_WIDTH,
                                             IMAGE_HEIGHT,
                                             BufferedImage.TYPE_INT_ARGB);
    
    fieldShapesGraphics          = fieldShapes.createGraphics();
    originalImageShapesGraphics  = originalImageShapes.createGraphics();
    segmentedImageShapesGraphics = segmentedImageShapes.createGraphics();
    
    fieldShapesGraphics.setRenderingHint(RenderingHints.KEY_ANTIALIASING,
                                         RenderingHints.VALUE_ANTIALIAS_ON);
    originalImageShapesGraphics.setRenderingHint(RenderingHints.KEY_ANTIALIASING,
                                                 RenderingHints.VALUE_ANTIALIAS_ON);
    segmentedImageShapesGraphics.setRenderingHint(RenderingHints.KEY_ANTIALIASING,
                                                  RenderingHints.VALUE_ANTIALIAS_ON);
    
    robotCoordinates = new CoordinatesConverter(configFile);
    
    sslCoordinates   = new CoordinatesConverter(configFile);
    sslCoordinates.setBlue(configFile.getBool("sslVision/onBlueTeam", true));
    
    robotUpdate = new RobotUpdate(true);
    sslUpdate   = new SSLUpdate();
    
    frameNumber = 0;
    
    // Draw the empty field
    initializeEmptyField();
  }
  
  public synchronized void receiveUpdate(RobotUpdate robotUpdate, Graphics2D graphics) {
    System.out.println("--- FRAME NUMBER " + frameNumber + " ---");
    frameNumber++;
    
    this.robotUpdate = robotUpdate;
    paint(graphics);
  }
  
  public synchronized void receiveUpdate(SSLUpdate sslUpdate, Graphics2D graphics) {
    this.sslUpdate = sslUpdate;
    paint(graphics);
  }
  
  private void paint(Graphics2D graphics) {
    // Clear the shape images
    clearShapes();
    
    // Set the coordinate system for robot shapes
    robotCoordinates.setBlue(robotUpdate.isBlue());
    
    // Loop through the robot updates
    for (RemoteMessageFromRobot message : robotUpdate) {
      if (message instanceof LogTextMessage) {
        LogTextMessage logTextMessage = (LogTextMessage)message;
        System.out.println(logTextMessage.getText());
      }
      else if (message instanceof ShapeMessage) {
        ShapeMessage shapeMessage = (ShapeMessage)message;
        paintShape(robotCoordinates, shapeMessage.getScreen(), shapeMessage.getShape());
      }
      else if (message instanceof SegmentedImageMessage) {
        SegmentedImageMessage segmentedImageMessage = (SegmentedImageMessage)message;
        SegmentedImage image = segmentedImageMessage.getSegmentedImage();
        segmentedImage = image.toBufferedImage();
      }
      else {
        switch (message.getType()) {
        case ORIGINAL_IMAGE:
          byte [] bytes = message.getData();
          YUV422Image image = new YUV422Image(bytes, IMAGE_WIDTH, IMAGE_HEIGHT);
          originalImage = image.toBufferedImage();
          break;
        }
      }
    }
    
    // Loop through the SSL updates
    for (SSLMessage message : sslUpdate) {
      if (message instanceof BallMessage) {
        BallMessage ballMessage = (BallMessage)message;
        
        if (ballMessage.getConfidence() < MIN_SSL_BALL_CONFIDENCE) {
          continue;
        }
        
        shape.Shape shape = new Circle(ballMessage.getX(), ballMessage.getY(),
                                       SSL_BALL_RADIUS,
                                       SSL_BALL_FILL_COLOR);
        paintShape(sslCoordinates, Screen.field, shape);
      }
      else if (message instanceof RobotMessage) {
        RobotMessage robotMessage = (RobotMessage)message;
        
        if (robotMessage.getConfidence() < MIN_SSL_ROBOT_CONFIDENCE) {
          continue;
        }
        
        shape.Shape shape = new Circle(robotMessage.getX(), robotMessage.getY(),
                                       SSL_ROBOT_RADIUS,
                                       robotMessage.getColor() == RobotColor.blue ?
                                                                  SSL_BLUE_ROBOT_FILL_COLOR
                                                                : SSL_YELLOW_ROBOT_FILL_COLOR);
        paintShape(sslCoordinates, Screen.field, shape);          
      }
    }
    
    // Clear the graphics
    graphics.setColor(Color.BLACK);
    graphics.fillRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    
    // Draw the empty field
    graphics.setClip(0, 0, FIELD_SCREEN_WIDTH, FIELD_SCREEN_HEIGHT);
    graphics.drawImage(field, 0, 0, null);
    
    // Draw the shapes on the field
    graphics.drawImage(fieldShapes, 0, 0, null);
    
    // Draw the original image
    graphics.setClip(FIELD_SCREEN_WIDTH, 0, IMAGE_WIDTH, IMAGE_HEIGHT);
    graphics.drawImage(originalImage, FIELD_SCREEN_WIDTH, 0, null);
    
    // Draw the shapes on the original image
    graphics.drawImage(originalImageShapes, FIELD_SCREEN_WIDTH, 0, null);
    
    // Draw the segmented image
    graphics.setClip(FIELD_SCREEN_WIDTH, IMAGE_HEIGHT+IMAGE_HORIZONTAL_SEPARATOR, IMAGE_WIDTH, IMAGE_HEIGHT);
    graphics.drawImage(segmentedImage, FIELD_SCREEN_WIDTH, IMAGE_HEIGHT+IMAGE_HORIZONTAL_SEPARATOR, null);

    // Draw the shapes on the segmented image
    graphics.drawImage(segmentedImageShapes, FIELD_SCREEN_WIDTH, IMAGE_HEIGHT+IMAGE_HORIZONTAL_SEPARATOR, null);
  }
  
  private void paintShape(CoordinatesConverter coordinates, Screen screen, shape.Shape shape) {
    Graphics2D graphics = null;
    
    if (screen == Screen.field) {
      graphics = fieldShapesGraphics;
    }
    else if (screen == Screen.originalImage) {
      graphics = originalImageShapesGraphics;
    }
    else if (screen == Screen.segmentedImage) {
      graphics = segmentedImageShapesGraphics;
    }
    else {
      graphics = fieldShapesGraphics;
    }
    
    if (shape instanceof shape.FieldObject) {
      shape.FieldObject fieldObject = (shape.FieldObject)shape;
      int centerX = coordinates.getX(screen, fieldObject.getCenterX());
      int centerY = coordinates.getY(screen, fieldObject.getCenterY());
      int radius = (int) fieldObject.getRadius();
      float angle = coordinates.getAngle(screen, fieldObject.getAngle());
      float angleDeviation = fieldObject.getAngleDeviation();
      float positionDeviation = fieldObject.getPositionDeviation();
      
      //Position Deviation
      if (fieldObject.havePosition()){
        graphics.setColor(new Color(fieldObject.getPositionColor(), true));
        this.fillCircle(graphics, centerX, centerY, (int)(positionDeviation+radius));
      }
      
      //Object Fill
      graphics.setColor(new Color(fieldObject.getFillColor(), true));
      this.fillCircle(graphics, centerX, centerY, radius);
      //Object Stroke
      graphics.setColor(new Color(fieldObject.getStrokeColor(), true));
      graphics.setStroke(new BasicStroke(fieldObject.getStrokeThickness()));
      this.drawCircle(graphics, centerX, centerY, radius);
      
      
      //Angle Deviation
      if (fieldObject.haveAngle()){
        graphics.setColor(new Color(fieldObject.getAngleColor(), true));
        graphics.fillArc(centerX-radius, centerY-radius, 
                         radius*2, radius*2, 
                         Math.round((float)Math.toDegrees(angle-angleDeviation/2)), 
                         Math.round((float)Math.toDegrees(angleDeviation)));
        graphics.setColor(new Color(fieldObject.getStrokeColor(), true));
        graphics.drawLine(centerX, centerY, 
                          (int)Math.round(centerX+radius*Math.cos(angle-angleDeviation/2)),
                          (int)Math.round(centerY-radius*Math.sin(angle-angleDeviation/2)));
        graphics.drawLine(centerX, centerY, (int)(centerX+radius*Math.cos(angle+angleDeviation/2)), (int)(centerY-radius*Math.sin(angle+angleDeviation/2)));
        graphics.drawArc(centerX-radius, centerY-radius, 
                         radius*2, radius*2, 
                         Math.round((float)Math.toDegrees(angle-angleDeviation/2)), 
                         Math.round((float)Math.toDegrees(angleDeviation)));
      }

    }
    else if (shape instanceof shape.Circle) {
      shape.Circle circle = (shape.Circle)shape;
      int centerX = coordinates.getX(screen, circle.getCenterX());
      int centerY = coordinates.getY(screen, circle.getCenterY());
      int radius = (int) circle.getRadius();
      if (circle.isFilled()) {
        graphics.setColor(new Color(circle.getFillColor(), true));
        this.fillCircle(graphics, centerX, centerY, radius);        
      }
      
      if (circle.isStroked()) {
        graphics.setColor(new Color(circle.getStrokeColor(), true));
        graphics.setStroke(new BasicStroke(circle.getStrokeThickness()));
        this.drawCircle(graphics, centerX, centerY, radius);
      }
    }
    else if (shape instanceof shape.Line) {
      shape.Line line = (shape.Line)shape;
      int x1 = coordinates.getX(screen, line.getEnd1X());
      int y1 = coordinates.getY(screen, line.getEnd1Y());
      int x2 = coordinates.getX(screen, line.getEnd2X());
      int y2 = coordinates.getY(screen, line.getEnd2Y());
      
      if (line.isStroked()) {
        graphics.setColor(new Color(line.getStrokeColor(), true));
        graphics.setStroke(new BasicStroke(line.getStrokeThickness()));
        graphics.drawLine(x1, y1, x2, y2);
      }
      
    } else if (shape instanceof shape.Rectangle) {
      shape.Rectangle rectangle = (shape.Rectangle)shape;
      int x1 = coordinates.getX(screen, rectangle.getTopLeftX());
      int y1 = coordinates.getY(screen, rectangle.getTopLeftY());
      int x2 = coordinates.getX(screen, rectangle.getBottomRightX());
      int y2 = coordinates.getY(screen, rectangle.getBottomRightY());
      
      int topLeftX = x1 <= x2 ? x1 : x2;
      int topLeftY = y1 <= y2 ? y1 : y2;
      int width    = Math.abs(x1 - x2);
      int height   = Math.abs(y1 - y2);
      
      if (rectangle.isFilled()) {
        graphics.setColor(new Color(rectangle.getFillColor(), true));
        graphics.fillRect(topLeftX, topLeftY, width, height);
      }
      
      if (rectangle.isStroked()) {
        graphics.setColor(new Color(rectangle.getStrokeColor(), true));
        graphics.setStroke(new BasicStroke(rectangle.getStrokeThickness()));
        graphics.drawRect(topLeftX, topLeftY, width, height);
      }
    } else if (shape instanceof shape.Triangle) {
      shape.Triangle triangle = (shape.Triangle)shape;
      int [] xPoints = new int[3];
      int [] yPoints = new int[3];
      xPoints[0] = coordinates.getX(screen, triangle.getEnd1X());
      yPoints[0] = coordinates.getY(screen, triangle.getEnd1Y());
      xPoints[1] = coordinates.getX(screen, triangle.getEnd2X());
      yPoints[1] = coordinates.getY(screen, triangle.getEnd2Y());
      xPoints[2] = coordinates.getX(screen, triangle.getEnd3X());
      yPoints[2] = coordinates.getY(screen, triangle.getEnd3Y());
      
      if (triangle.isFilled()) {
        graphics.setColor(new Color(triangle.getFillColor(), true));
        graphics.fillPolygon(xPoints, yPoints, 3);
      }
      
      if (triangle.isStroked()) {
        graphics.setColor(new Color(triangle.getStrokeColor(), true));
        graphics.setStroke(new BasicStroke(triangle.getStrokeThickness()));
        graphics.drawPolygon(xPoints, yPoints, 3);
      }      
    }
  }

  private void initializeEmptyField() {
    Graphics2D graphics = field.createGraphics();
    graphics.setRenderingHint(RenderingHints.KEY_ANTIALIASING,
                              RenderingHints.VALUE_ANTIALIAS_ON);

    //Field 
    graphics.setColor(new Color(FIELD_COLOR));
    graphics.fillRect(0, 0, FIELD_SCREEN_WIDTH, FIELD_SCREEN_HEIGHT);
    
    //Field outline    
    graphics.setColor(Color.WHITE);
    graphics.setStroke(new BasicStroke(FIELD_LINE_WIDTH));
    graphics.drawRect((FIELD_SCREEN_WIDTH-FIELD_LENGTH)/2, (FIELD_SCREEN_HEIGHT-FIELD_WIDTH)/2, FIELD_LENGTH, FIELD_WIDTH);

    //Left Penalty Box    
    graphics.setStroke(new BasicStroke(FIELD_LINE_WIDTH));
    graphics.drawRect((FIELD_SCREEN_WIDTH-FIELD_LENGTH)/2, (FIELD_SCREEN_HEIGHT-PENALTY_BOX_WIDTH)/2, PENALTY_BOX_LENGTH, PENALTY_BOX_WIDTH);    
 
    //Right Penalty Box
    graphics.setStroke(new BasicStroke(FIELD_LINE_WIDTH));
    graphics.drawRect((FIELD_SCREEN_WIDTH-FIELD_LENGTH)/2 + FIELD_LENGTH - PENALTY_BOX_LENGTH, (FIELD_SCREEN_HEIGHT-PENALTY_BOX_WIDTH)/2, PENALTY_BOX_LENGTH, PENALTY_BOX_WIDTH);    
    
    //Center Line
    graphics.drawLine(FIELD_SCREEN_WIDTH/2, 
                      (FIELD_SCREEN_HEIGHT-FIELD_WIDTH)/2, 
                      FIELD_SCREEN_WIDTH/2, 
                      (FIELD_SCREEN_HEIGHT-FIELD_WIDTH)/2 + FIELD_WIDTH);
      
    //Center Circle
    this.drawCircle(graphics, FIELD_SCREEN_WIDTH/2, FIELD_SCREEN_HEIGHT/2, CENTER_CIRCLE_RADIUS);
                
    //Center Point
    graphics.drawLine(FIELD_SCREEN_WIDTH/2 - (PENALTY_POINT_LENGTH/2), 
                      FIELD_SCREEN_HEIGHT/2, 
                      FIELD_SCREEN_WIDTH/2 + (PENALTY_POINT_LENGTH/2), 
                      FIELD_SCREEN_HEIGHT/2);

    //Left Penalty Point
    //horizontal line
    graphics.drawLine(FIELD_SCREEN_WIDTH/2 - DIST_CENTER_TO_PENALTY_POINT - (PENALTY_POINT_LENGTH/2),
                      FIELD_SCREEN_HEIGHT/2,
                      FIELD_SCREEN_WIDTH/2 - DIST_CENTER_TO_PENALTY_POINT + (PENALTY_POINT_LENGTH/2),
                      FIELD_SCREEN_HEIGHT/2);
    
    //vertical line
    graphics.drawLine(FIELD_SCREEN_WIDTH/2 - DIST_CENTER_TO_PENALTY_POINT,
                      FIELD_SCREEN_HEIGHT/2 - (PENALTY_POINT_LENGTH/2),
                      FIELD_SCREEN_WIDTH/2 - DIST_CENTER_TO_PENALTY_POINT,
                      FIELD_SCREEN_HEIGHT/2 + (PENALTY_POINT_LENGTH/2));
         
    //Right Penalty Point
    //horizontal line
    graphics.drawLine(FIELD_SCREEN_WIDTH/2 + DIST_CENTER_TO_PENALTY_POINT - (PENALTY_POINT_LENGTH/2),
                      FIELD_SCREEN_HEIGHT/2,
                      FIELD_SCREEN_WIDTH/2 + DIST_CENTER_TO_PENALTY_POINT + (PENALTY_POINT_LENGTH/2),
                      FIELD_SCREEN_HEIGHT/2);
         
    //vertical line
    graphics.drawLine(FIELD_SCREEN_WIDTH/2 + DIST_CENTER_TO_PENALTY_POINT,
                      FIELD_SCREEN_HEIGHT/2 - (PENALTY_POINT_LENGTH/2),
                      FIELD_SCREEN_WIDTH/2 + DIST_CENTER_TO_PENALTY_POINT,
                      FIELD_SCREEN_HEIGHT/2 + (PENALTY_POINT_LENGTH/2));
     
    //Blue Goal Post
    graphics.setColor(new Color(BLUE_GOAL_POST_COLOR));
    //right goal post from the center facing the blue goal post
    graphics.fillRect( (FIELD_SCREEN_WIDTH-FIELD_LENGTH)/2 - GOAL_POST_LENGTH + (FIELD_LINE_WIDTH/2),
                       (FIELD_SCREEN_HEIGHT-GOAL_WIDTH)/2,
                       GOAL_POST_LENGTH,
                       GOAL_POST_WIDTH);
    //left goal post from the center facing the blue goal post
    graphics.fillRect( (FIELD_SCREEN_WIDTH-FIELD_LENGTH)/2 - GOAL_POST_LENGTH + (FIELD_LINE_WIDTH/2),
                       (FIELD_SCREEN_HEIGHT-GOAL_WIDTH)/2 + GOAL_WIDTH,
                       GOAL_POST_LENGTH, GOAL_POST_WIDTH);
     
    //center goal bar from the center facing the blue goal post
    graphics.drawLine( (FIELD_SCREEN_WIDTH-FIELD_LENGTH)/2 - GOAL_POST_LENGTH + (FIELD_LINE_WIDTH/2),
                       (FIELD_SCREEN_HEIGHT-GOAL_WIDTH)/2 + (GOAL_POST_WIDTH/2),
                       (FIELD_SCREEN_WIDTH-FIELD_LENGTH)/2 - GOAL_POST_LENGTH + (FIELD_LINE_WIDTH/2),
                       (FIELD_SCREEN_HEIGHT-GOAL_WIDTH)/2 + GOAL_WIDTH + (GOAL_POST_WIDTH/2));

    
    //Yellow Goal Post
    graphics.setColor(new Color(YELLOW_GOAL_POST_COLOR));
    //left goal post from the center facing the yellow goal post
    graphics.fillRect(FIELD_SCREEN_WIDTH - ((FIELD_SCREEN_WIDTH - FIELD_LENGTH)/2),
                      (FIELD_SCREEN_HEIGHT - GOAL_WIDTH)/2,
                      GOAL_POST_LENGTH,
                      GOAL_POST_WIDTH);
     
    //right goal post from the center facing the yellow goal post
    graphics.fillRect(FIELD_SCREEN_WIDTH - ((FIELD_SCREEN_WIDTH - FIELD_LENGTH)/2),
                     (FIELD_SCREEN_HEIGHT - GOAL_WIDTH)/2 + GOAL_WIDTH,
                      GOAL_POST_LENGTH,
                      GOAL_POST_WIDTH);
        
    //center goal bar from the center facing the yellow goal post
    graphics.drawLine(FIELD_SCREEN_WIDTH - ((FIELD_SCREEN_WIDTH - FIELD_LENGTH)/2) + GOAL_POST_LENGTH,
                      (FIELD_SCREEN_HEIGHT - GOAL_WIDTH)/2 + (GOAL_POST_WIDTH/2),
                      FIELD_SCREEN_WIDTH - ((FIELD_SCREEN_WIDTH - FIELD_LENGTH)/2) + GOAL_POST_LENGTH,
                      (FIELD_SCREEN_HEIGHT - GOAL_WIDTH)/2 + GOAL_WIDTH + (GOAL_POST_WIDTH/2));        
 }
  
  private void clearShapes() {
    Composite oldComposite;
    
    oldComposite = fieldShapesGraphics.getComposite();
    fieldShapesGraphics.setComposite(AlphaComposite.getInstance(AlphaComposite.CLEAR, 0.0f));
    fieldShapesGraphics.fillRect(0, 0, FIELD_SCREEN_WIDTH, FIELD_SCREEN_HEIGHT);
    fieldShapesGraphics.setComposite(oldComposite);
    
    oldComposite = originalImageShapesGraphics.getComposite();
    originalImageShapesGraphics.setComposite(AlphaComposite.getInstance(AlphaComposite.CLEAR, 0.0f));
    originalImageShapesGraphics.fillRect(0, 0, IMAGE_WIDTH, IMAGE_HEIGHT);
    originalImageShapesGraphics.setComposite(oldComposite);
    
    oldComposite = segmentedImageShapesGraphics.getComposite();
    segmentedImageShapesGraphics.setComposite(AlphaComposite.getInstance(AlphaComposite.CLEAR, 0.0f));
    segmentedImageShapesGraphics.fillRect(0, 0, IMAGE_WIDTH, IMAGE_HEIGHT);
    segmentedImageShapesGraphics.setComposite(oldComposite);
  }
  
  private void drawCircle(Graphics2D graphics, int centerX, int centerY, int radius) {
    graphics.drawOval(centerX-radius, centerY-radius, radius*2, radius*2);
  }
  
  private void fillCircle(Graphics2D graphics, int centerX, int centerY, int radius) {
    graphics.fillOval(centerX-radius, centerY-radius, radius*2, radius*2);
  }
  
  private final int SCREEN_WIDTH, SCREEN_HEIGHT;
  
  private final int FIELD_SCREEN_WIDTH, FIELD_SCREEN_HEIGHT;
  private final int FIELD_COLOR;
  private final int FIELD_LINE_WIDTH;
  private final int FIELD_WIDTH, FIELD_LENGTH;
  private final int PENALTY_BOX_WIDTH, PENALTY_BOX_LENGTH;
  private final int PENALTY_POINT_LENGTH;
  private final int DIST_CENTER_TO_PENALTY_POINT;
  private final int CENTER_CIRCLE_RADIUS;
  private final int BLUE_GOAL_POST_COLOR, YELLOW_GOAL_POST_COLOR;
  private final int GOAL_POST_LENGTH, GOAL_POST_WIDTH;
  private final int GOAL_WIDTH;
  private final int IMAGE_WIDTH, IMAGE_HEIGHT;
  private final int IMAGE_HORIZONTAL_SEPARATOR;
  
  
  private final float MIN_SSL_BALL_CONFIDENCE;
  private final float MIN_SSL_ROBOT_CONFIDENCE;
  
  private final int   SSL_BALL_FILL_COLOR;
  private final float SSL_BALL_RADIUS;
  
  private final int   SSL_YELLOW_ROBOT_FILL_COLOR, SSL_BLUE_ROBOT_FILL_COLOR;
  private final float SSL_ROBOT_RADIUS;

  private BufferedImage field;
  private BufferedImage originalImage, segmentedImage;
  private BufferedImage fieldShapes, originalImageShapes, segmentedImageShapes;
  
  private Graphics2D fieldShapesGraphics,
                     originalImageShapesGraphics,
                     segmentedImageShapesGraphics;
  
  private CoordinatesConverter robotCoordinates, sslCoordinates;
  
  private RobotUpdate robotUpdate;
  private SSLUpdate   sslUpdate;
  
  private int frameNumber;
}
