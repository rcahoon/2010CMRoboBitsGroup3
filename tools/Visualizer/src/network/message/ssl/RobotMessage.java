package network.message.ssl;

import network.message.SSLMessage;

public class RobotMessage extends SSLMessage {

  public enum RobotColor {
    yellow,
    blue
  }
  
  public RobotMessage(RobotColor color, int robotID, float x, float y, float theta, float confidence) {
    this.color      = color;
    this.robotID    = robotID;
    this.x          = x;
    this.y          = y;
    this.theta      = theta;
    this.confidence = confidence;
  }
  
  public RobotColor getColor() {
    return color;
  }
  
  public int getRobotID() {
    return robotID;
  }
  
  public float getX() {
    return x;
  }
  
  public float getY() {
    return y;
  }
  
  public float getTheta() {
    return theta;
  }

  public float getConfidence() {
    return confidence;
  }

  private RobotColor color;
  private int        robotID;
  private float      x, y, theta;
  private float      confidence;
}
