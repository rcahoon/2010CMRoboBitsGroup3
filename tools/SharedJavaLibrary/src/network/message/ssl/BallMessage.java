package network.message.ssl;

import network.message.SSLMessage;

public class BallMessage extends SSLMessage {

  public BallMessage(float x, float y, float confidence) {
    this.x = x;
    this.y = y;
    this.confidence = confidence;
  }
  
  public float getX() {
    return x;
  }
  
  public float getY() {
    return y;
  }
  
  public float getConfidence() {
    return confidence;
  }
  
  private float x, y;
  private float confidence;
}
