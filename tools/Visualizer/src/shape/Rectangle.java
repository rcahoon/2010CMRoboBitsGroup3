package shape;

public class Rectangle extends Shape {
  public Rectangle(float topLeftX, float topLeftY,
                   float bottomRightX, float bottomRightY,
                   int fillColor) {
    super(fillColor);
    this.topLeftX = topLeftX;
    this.topLeftY = topLeftY;
    this.bottomRightX = bottomRightX;
    this.bottomRightY = bottomRightY;
  }

  public Rectangle(float topLeftX, float topLeftY,
                   float bottomRightX, float bottomRightY,
                   int fillColor,
                   int strokeColor, float strokeThickness) {
    super(fillColor, strokeColor, strokeThickness);
    this.topLeftX = topLeftX;
    this.topLeftY = topLeftY;
    this.bottomRightX = bottomRightX;
    this.bottomRightY = bottomRightY;
  }

  public Rectangle(float topLeftX, float topLeftY,
                   float bottomRightX, float bottomRightY,
                   int strokeColor, float strokeThickness) {
    super(strokeColor, strokeThickness);
    this.topLeftX = topLeftX;
    this.topLeftY = topLeftY;
    this.bottomRightX = bottomRightX;
    this.bottomRightY = bottomRightY;
  }

  public float getTopLeftX() {
    return topLeftX;
  }

  public float getTopLeftY() {
    return topLeftY;
  }

  public float getBottomRightX() {
    return bottomRightX;
  }

  public float getBottomRightY() {
    return bottomRightY;
  }

  private float topLeftX;
  private float topLeftY;
  private float bottomRightX;
  private float bottomRightY;
}
