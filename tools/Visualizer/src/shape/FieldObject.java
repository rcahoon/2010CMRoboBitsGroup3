package shape;

public class FieldObject extends Circle {

  public FieldObject(float x, float y, float radius,
                     int fillColor,
                     int strokeColor, float strokeThickness) {
    super(x, y, radius, fillColor, strokeColor, strokeThickness);
    
    this.havePosition = false;
    this.haveAngle    = false;
  }

  public FieldObject(float x, float y, float radius,
                     int fillColor,
                     int strokeColor, float strokeThickness,
                     int positionColor, float positionDeviation) {
    super(x, y, radius, fillColor, strokeColor, strokeThickness);
    
    this.havePosition = true;
    this.haveAngle    = false;
    this.positionColor     = fixAlpha(positionColor);
    this.positionDeviation = positionDeviation;
  }
  
  public FieldObject(float x, float y, float radius,
                     int fillColor,
                     int strokeColor, float strokeThickness,
                     int angleColor, float angle, float angleDeviation) {
    super(x, y, radius, fillColor, strokeColor, strokeThickness);
    
    this.havePosition = false;
    this.haveAngle    = true;
    this.angleColor     = fixAlpha(angleColor);
    this.angle          = angle;
    this.angleDeviation = angleDeviation;
  }

  public FieldObject(float x, float y, float radius,
                     int fillColor,
                     int strokeColor, float strokeThickness,
                     int positionColor, float positionDeviation,
                     int angleColor, float angle, float angleDeviation) {
    super(x, y, radius, fillColor, strokeColor, strokeThickness);
    
    this.havePosition = true;
    this.haveAngle    = true;
    this.positionColor     = fixAlpha(positionColor);
    this.positionDeviation = positionDeviation;
    this.angleColor     = fixAlpha(angleColor);
    this.angle          = angle;
    this.angleDeviation = angleDeviation;
  }
  
  public boolean havePosition() {
    return havePosition && (positionDeviation > 0);
  }
  
  public boolean haveAngle() {
    return haveAngle;
  }
  
  public int getPositionColor() {
    return positionColor;
  }
  
  public int getAngleColor() {
    return angleColor;
  }
  
  public float getPositionDeviation() {
    return positionDeviation;
  }
  
  public float getAngleDeviation() {
    return angleDeviation;
  }
  
  public float getAngle() {
    return angle;
  }
  
  private int     positionColor,     angleColor;
  private boolean havePosition,      haveAngle;
  private float   positionDeviation, angleDeviation;
  private float   angle;
}
