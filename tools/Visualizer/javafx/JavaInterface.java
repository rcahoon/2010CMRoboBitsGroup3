package javafx;

public interface JavaInterface {

  public void drawCircle(int screenID,
                         float centerX, float centerY, float radius,
                         float fillAlpha, int fillR, int fillG, int fillB);
  
  public void drawCircle(int screenID,
                         float centerX, float centerY, float radius,
                         float strokeAlpha, int strokeR, int strokeG, int strokeB,
                         float strokeThickness);

  public void drawCircle(int screenID,
                         float centerX, float centerY, float radius,
                         float fillAlpha, int fillR, int fillG, int fillB,
                         float strokeAlpha, int strokeR, int strokeG, int strokeB,
                         float strokeThickness);

  public void drawRectangle(int screenID,
                            float topLeftX, float topLeftY,
                            float length, float breadth,
                            float fillAlpha, int fillR, int fillG, int fillB);
  
  public void drawRectangle(int screenID,
                            float topLeftX, float topLeftY,
                            float length, float breadth,
                            float strokeAlpha, int strokeR, int strokeG, int strokeB,
                            float strokeThickness);

  public void drawRectangle(int screenID,
                            float topLeftX, float topLeftY,
                            float length, float breadth,
                            float fillAlpha, int fillR, int fillG, int fillB,
                            float strokeAlpha, int strokeR, int strokeG, int strokeB,
                            float strokeThickness);

  public void drawTriangle(int screenID,
                           float end1X, float end1Y,
                           float end2X, float end2Y,
                           float end3X, float end3Y,
                           float fillAlpha, int fillR, int fillG, int fillB);
      
  public void drawTriangle(int screenID,
                           float end1X, float end1Y,
                           float end2X, float end2Y,
                           float end3X, float end3Y,
                           float strokeAlpha, int strokeR, int strokeG, int strokeB,
                           float strokeThickness);
      
  public void drawTriangle(int screenID,
                           float end1X, float end1Y,
                           float end2X, float end2Y,
                           float end3X, float end3Y,
                           float fillAlpha, int fillR, int fillG, int fillB,
                           float strokeAlpha, int strokeR, int strokeG, int strokeB,
                           float strokeThickness);
                           
  public void drawLine(int screenID,
                       float end1X, float end1Y,
                       float end2X, float end2Y,
                       float strokeAlpha, int strokeR, int strokeG, int strokeB,
                       float strokeThickness);
  
  public void drawFieldObject(int screenID,
                              float x, float y, float radius,
                              float fillAlpha, int fillR, int fillG, int fillB,
                              float strokeAlpha, int strokeR, int strokeG, int strokeB,
                              float strokeThickness,
                              String label);
  
  public void drawFieldObject(int screenID,
                              float x, float y, float radius,
                              float fillAlpha, int fillR, int fillG, int fillB,
                              float strokeAlpha, int strokeR, int strokeG, int strokeB,
                              float strokeThickness,
                              float positionAlpha, int positionR, int positionG, int positionB,
                              float positionDeviation,
                              String label);

  public void drawFieldObject(int screenID,
                              float x, float y, float radius,
                              float fillAlpha, int fillR, int fillG, int fillB,
                              float strokeAlpha, int strokeR, int strokeG, int strokeB,
                              float strokeThickness,
                              float angleAlpha, int angleR, int angleG, int angleB,
                              float angle, float angleDeviation,
                              String label);

  public void drawFieldObject(int screenID,
                              float x, float y, float radius,
                              float fillAlpha, int fillR, int fillG, int fillB,
                              float strokeAlpha, int strokeR, int strokeG, int strokeB,
                              float strokeThickness,
                              float positionAlpha, int positionR, int positionG, int positionB,
                              float positionDeviation,
                              float angleAlpha, int angleR, int angleG, int angleB,
                              float angle, float angleDeviation,
                              String label);

  public void drawImage(int screenID,
                        javafx.scene.image.Image image);
}
