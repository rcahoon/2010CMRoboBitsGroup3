package image.format;

public class RGB {

  public RGB(int red, int green, int blue) {
    set(red, green, blue);
  }
  
  public int getRed() {
    return red;
  }
  
  public int getGreen() {
    return green;
  }
  
  public int getBlue() {
    return blue;
  }
  
  public int toInt() {
    return ((red & 0xFF) << 16) |
           ((green & 0xFF) << 8) |
            (blue & 0xFF);
  }
  
  public void set(int red, int green, int blue) {
    if ((red < 0) || (red > 255)) {
      throw new IllegalArgumentException("Red should be within 0 and 255");
    }
    if ((green < 0) || (green > 255)) {
      throw new IllegalArgumentException("Green should be within 0 and 255");
    }
    if ((blue < 0) || (blue > 255)) {
      throw new IllegalArgumentException("Blue should be within 0 and 255");
    }

    this.red   = red;
    this.green = green;
    this.blue  = blue;
  }
  
  private int red, green, blue;
  
}
