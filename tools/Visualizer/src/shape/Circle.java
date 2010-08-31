package shape;

public class Circle extends Shape {
  private float centerX, centerY;
  private float radius;

  public Circle(float centerX, float centerY,
                float radius,
                int fillColor,
                int strokeColor, float strokeThickness) {
    super(fillColor, strokeColor, strokeThickness);
    this.centerX = centerX;
    this.centerY = centerY;
    this.radius = radius;
  }

  public Circle(float centerX, float centerY,
                float radius,
                int strokeColor, float strokeThickness) {
    super(strokeColor, strokeThickness);
    this.centerX = centerX;
    this.centerY = centerY;
    this.radius = radius;
  }

  public Circle(float centerX, float centerY,
                float radius,
                int fillColor) {
    super(fillColor);
    this.centerX = centerX;
    this.centerY = centerY;
    this.radius = radius;
  }

  public float getCenterX() {
    return centerX;
  }

  public float getCenterY() {
    return centerY;
  }

  public float getRadius() {
    return radius;
  }

}
