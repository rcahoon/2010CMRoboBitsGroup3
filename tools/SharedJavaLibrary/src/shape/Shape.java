package shape;

public class Shape {
  
  // This should match shared/Shape/Shape.h
  public enum Type {
    UNKNOWN  (0),
    CIRCLE   (1),
    RECTANGLE(2),
    TRIANGLE (3),
    LINE     (4),
    ROBOT    (5);

    Type(int typeID) {
      this.typeID = typeID;
    }
    
    private int typeID;
    
    public static Type create(int typeID) {
      for (Type type : Type.values()) {
        if (type.typeID == typeID) {
          return type;
        }
      }
        
      return Type.UNKNOWN;
    }
  }
  
  public Shape(int fillColor,
               int strokeColor, float strokeThickness) {
    this.fillColor = fixAlpha(fillColor);
    this.filled = true;
    this.strokeThickness = strokeThickness;
    this.strokeColor = fixAlpha(strokeColor);
    this.stroked = true;
  }

  public Shape(int fillColor) {
    this.fillColor = fixAlpha(fillColor);
    this.filled = true;
    this.stroked = false;
  }

  public Shape(int strokeColor, float strokeThickness) {
    this.filled = false;
    this.strokeThickness = strokeThickness;
    this.strokeColor = fixAlpha(strokeColor);
    this.stroked = true;
  }

  public int getFillColor() {
    return fillColor;
  }

  public boolean isFilled() {
    return filled;
  }

  public float getStrokeThickness() {
    return strokeThickness;
  }

  public int getStrokeColor() {
    return strokeColor;
  }

  public boolean isStroked() {
    return stroked;
  }
  
  protected static int getAlpha(int color) {
    int alpha = (color >> 24) & 0xFF;
    if (alpha == 0) {
      alpha = 255;
    }
    return alpha;
  }
  
  protected int fixAlpha(int color) {
    return (getAlpha(color) << 24) | (color & 0xFFFFFF);
  }
  
  private int fillColor;
  private int strokeColor;
  private boolean filled, stroked;
  private float strokeThickness;
}
