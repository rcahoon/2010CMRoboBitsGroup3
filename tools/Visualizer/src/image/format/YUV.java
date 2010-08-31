package image.format;

public class YUV {

  public YUV(int y, int u, int v) {
    if ((y < 0) || (y > 255)) {
      throw new IllegalArgumentException("y should be within 0 and 255");
    }
    if ((u < 0) || (u > 255)) {
      throw new IllegalArgumentException("u should be within 0 and 255");
    }
    if ((v < 0) || (v > 255)) {
      throw new IllegalArgumentException("v should be within 0 and 255");
    }
    
    this.y = y;
    this.u = u;
    this.v = v;
  }
  
  public RGB toRGB() {
    int y_, u_, v_;

    y_ = y;
    u_ = v * 2 - 255;
    v_ = u * 2 - 255;

    int r = bound(y_ + u_);
    int g = bound((int)(y_ - 0.51*u_ - 0.19*v_));
    int b = bound(y_ + v_);

    RGB rgb = new RGB(r, g, b);
    return rgb;
  }
  
  public int getY() {
    return y;
  }
  
  public int getU() {
    return u;
  }
  
  public int getV() {
    return v;
  }
  
  private int bound(int value) {
    if (value < 0) {
      return 0;
    }
    if (value > 255) {
      return 255;
    }
    return value;
  }
  
  private int y, u, v;

}
