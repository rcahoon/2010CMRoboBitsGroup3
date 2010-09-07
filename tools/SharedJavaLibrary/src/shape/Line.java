package shape;

public class Line extends Shape {
  private float end1X;
  private float end1Y;
  private float end2X;
  private float end2Y;

  public Line(float end1X, float end1Y, float end2X, float end2Y,
              int strokeColor, float strokeThickness) {
    super(strokeColor, strokeThickness);
    this.end1X = end1X;
    this.end1Y = end1Y;
    this.end2X = end2X;
    this.end2Y = end2Y;
  }

  public float getEnd1X() {
    return end1X;
  }

  public float getEnd1Y() {
    return end1Y;
  }

  public float getEnd2X() {
    return end2X;
  }

  public float getEnd2Y() {
    return end2Y;
  }

}
