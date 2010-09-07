package shape;

public class Triangle extends Shape {
  private float point1X;
  private float point1Y;
  private float point2X;
  private float point2Y;
  private float point3X;
  private float point3Y;

  public Triangle(float point1X, float point1Y,
                  float point2X, float point2Y,
                  float point3X, float point3Y,
                  int fillColor,
                  int strokeColor, float strokeThickness) {
    super(fillColor, strokeColor, strokeThickness);
    this.point1X = point1X;
    this.point1Y = point1Y;
    this.point2X = point2X;
    this.point2Y = point2Y;
    this.point3X = point3X;
    this.point3Y = point3Y;
  }

  public Triangle(float point1X, float point1Y,
                  float point2X, float point2Y,
                  float point3X, float point3Y,
                  int fillColor) {
    super(fillColor);
    this.point1X = point1X;
    this.point1Y = point1Y;
    this.point2X = point2X;
    this.point2Y = point2Y;
    this.point3X = point3X;
    this.point3Y = point3Y;
  }

  public Triangle(float point1X, float point1Y,
                  float point2X, float point2Y,
                  float point3X, float point3Y,
                  int strokeColor, float strokeThickness) {
    super(strokeColor, strokeThickness);
    this.point1X = point1X;
    this.point1Y = point1Y;
    this.point2X = point2X;
    this.point2Y = point2Y;
    this.point3X = point3X;
    this.point3Y = point3Y;
  }

  public float getEnd1X() {
    return point1X;
  }

  public float getEnd1Y() {
    return point1Y;
  }

  public float getEnd2X() {
    return point2X;
  }

  public float getEnd2Y() {
    return point2Y;
  }

  public float getEnd3X() {
    return point3X;
  }

  public float getEnd3Y() {
    return point3Y;
  }

}
