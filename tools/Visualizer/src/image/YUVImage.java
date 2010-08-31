package image;

import image.format.*;

public abstract class YUVImage extends RGBImage {

  protected YUVImage(int width, int height) {
    super(width, height);
  }

  @Override
  public RGB getRGB(int x, int y) {
    return getYUV(x, y).toRGB();
  }
  
  public abstract YUV getYUV(int x, int y);
}
