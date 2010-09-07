package network.message.fromrobot;

import network.message.RemoteMessage;
import network.message.RemoteMessageFromRobot;
import shape.*;

public class ShapeMessage extends RemoteMessageFromRobot {

  public ShapeMessage(Screen screen, Shape shape) {
    super(RemoteMessageFromRobot.MessageType.TEAM_COLOR, null);
    
    this.screen = screen;
    this.shape  = shape;
  }
  
  public Screen getScreen() {
    return screen;
  }
  
  public Shape getShape() {
    return shape;
  }
  
  private Screen screen;
  private Shape  shape;
  
  public static ShapeMessage create(byte [] data) {
    try {
      // Read the header
      int offset = 0;
      
      // The first byte is the shape type
      int shapeTypeID = data[0];
      // The second byte is filled
      boolean filled = (data[1] != 0);
      // The third byte is stroked
      boolean stroked = (data[2] != 0);
      
      offset += 3;
      
      int fillColor = 0;
      // If it's filled, we need to get fillColor
      if (filled) {
        fillColor = RemoteMessage.readInt(data, offset);
        offset += 4;
      }
      
      int strokeColor = 0;
      float strokeThickness = 0;
      // If it's stroked, we need to get strokeColor and strokeThickness
      if (stroked) {
        strokeColor = RemoteMessage.readInt(data, offset);
        offset += 4;
        strokeThickness = RemoteMessage.readFloat(data, offset);
        offset += 4;
      }
      
      // Done reading the header
      
      int screenID = data[offset];
      offset++;
      
      // The following bytes is the number of ints
      int numInts = data[offset];
      offset++;
      
      // The following bytes is the number of floats
      int numFloats = data[offset];
      offset++;
      
      int [] ints = new int[numInts];
      
      for (int i = 0; i < numInts; i++) {
        ints[i] = RemoteMessage.readInt(data, offset);
        offset += 4;
      }
      
      float [] floats = new float[numFloats];
      
      for (int i = 0; i < numFloats; i++) {
        floats[i] = RemoteMessage.readFloat(data, offset);
        offset += 4;
      }
      
      // Convert all the data into a shape
      Shape shape = null;
      
      Shape.Type shapeType = Shape.Type.create(shapeTypeID);
      Screen           screen    = Screen.create(screenID);
      
      if ((shapeType == Shape.Type.CIRCLE) && (numFloats == 3)) {
//        System.out.printf("Got a circle: (%.1f, %.1f) %.1f\n", floats[0], floats[1], floats[2]);
        if (filled && stroked) {
          shape = new Circle(floats[0], floats[1], floats[2],
                                   fillColor,
                                   strokeColor, strokeThickness);
        }
        else if (filled) {
          shape = new Circle(floats[0], floats[1], floats[2],
                                   fillColor);
        }
        else if (stroked) {
          shape = new Circle(floats[0], floats[1], floats[2],
                                   strokeColor, strokeThickness);
        }
      }
      else if ((shapeType == Shape.Type.RECTANGLE) && (numFloats == 4)) {
//        System.out.printf("Got a rectangle: (%.1f, %.1f) %.1f, %.1f\n", floats[0], floats[1], floats[2], floats[3]);
        if (filled && stroked) {
          shape = new Rectangle(floats[0], floats[1], floats[2], floats[3],
                                      fillColor,
                                      strokeColor, strokeThickness);
        }
        else if (filled) {
          shape = new Rectangle(floats[0], floats[1], floats[2], floats[3],
                                      fillColor);
        }
        else if (stroked) {
          shape = new Rectangle(floats[0], floats[1], floats[2], floats[3],
                                      strokeColor, strokeThickness);
        }
      }
      else if ((shapeType == Shape.Type.TRIANGLE) && (numFloats == 6)) {
//        System.out.printf("Got a triangle: (%.1f, %.1f)-(%.1f, %.1f)-(%.1f, %.1f)\n", floats[0], floats[1], floats[2], floats[3], floats[4], floats[5]);
        if (filled && stroked) {
          shape = new Triangle(floats[0], floats[1],
                                     floats[2], floats[3],
                                     floats[4], floats[5],
                                     fillColor,
                                     strokeColor, strokeThickness);
        }
        else if (filled) {
          shape = new Triangle(floats[0], floats[1],
                                     floats[2], floats[3],
                                     floats[4], floats[5],
                                     fillColor);
        }
        else if (stroked) {
          shape = new Triangle(floats[0], floats[1],
                                     floats[2], floats[3],
                                     floats[4], floats[5],
                                     strokeColor, strokeThickness);
        }
      }
      else if ((shapeType == Shape.Type.LINE) && (numFloats == 4)) {
//        System.out.printf("Got a line: (%.1f, %.1f)-(%.1f, %.1f), %f\n", floats[0], floats[1], floats[2], floats[3], strokeThickness);
        if (stroked) {
          shape = new Line(floats[0], floats[1],
                           floats[2], floats[3],
                           strokeColor, strokeThickness);
        }
      }
      else if ((shapeType == Shape.Type.ROBOT) && filled && stroked && (numInts >= 2) && (numFloats >= 3)) {
//        System.out.printf("Got a robot: (%.1f, %.1f)\n", floats[0], floats[1]);
        float positionX = floats[0];
        float positionY = floats[1];
        float radius    = floats[2];
        
        boolean havePosition = (ints[0] != 0);
        boolean haveAngle    = (ints[1] != 0);
        
        if (havePosition && haveAngle && (numInts == 4) && (numFloats == 6)) {
          int positionColor = ints[2];
          int angleColor    = ints[3];
          
          float positionDeviation = floats[3];
          float angle             = floats[4];
          float angleDeviation    = floats[5];
          
          shape = new FieldObject(positionX, positionY, radius,
                            fillColor,
                            strokeColor, strokeThickness,
                            positionColor, positionDeviation,
                            angleColor, angle, angleDeviation);
        }
        else if (havePosition && (numInts == 3) && (numFloats == 4)) {
          int positionColor = ints[2];
          
          float positionDeviation = floats[3];
          
          shape = new FieldObject(positionX, positionY, radius,
                            fillColor,
                            strokeColor, strokeThickness,
                            positionColor, positionDeviation);
        }
        else if (haveAngle && (numInts == 3) && (numFloats == 5)) {
          int angleColor    = ints[2];
          
          float angle             = floats[3];
          float angleDeviation    = floats[4];
          
          shape = new FieldObject(positionX, positionY, radius,
                            fillColor,
                            strokeColor, strokeThickness,
                            angleColor, angle, angleDeviation);
        }
        else if (!havePosition && !haveAngle) {
          shape = new FieldObject(positionX, positionY, radius,
                            fillColor,
                            strokeColor, strokeThickness);
        }
    }
      
      if (shape != null) {
        return new ShapeMessage(screen, shape);
      }
      else {
        return null;
      }
      
    }
    catch (Exception e) {
      return null;
    }
  }
}
