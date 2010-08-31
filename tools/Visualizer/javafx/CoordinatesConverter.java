package javafx;

import shape.Screen;
import config.*;

public class CoordinatesConverter {

  public CoordinatesConverter(ConfigFile configFile) {
    fieldScreenWidth  = configFile.getInt("visualizer/fieldScreenWidth", 686);
    fieldScreenHeight = configFile.getInt("visualizer/fieldScreenHeight", 486);
    
    isBlue = true;
  }
  
  public void setBlue(boolean isBlue) {
    this.isBlue = isBlue;
  }
  
  public float getX(Screen screen, float x) {
    if (screen == Screen.field){
      if (isBlue) {
        return (fieldScreenWidth / 2 + x);
      } else{
        return (fieldScreenWidth / 2 - x);
      }
    }
    return x;
  }
  
  public float getY(Screen screen, float y) {
    if (screen == Screen.field){
      if (isBlue) {
        return (fieldScreenHeight / 2 - y);
      } else {
        return (fieldScreenHeight / 2 + y);
      }
    }    
    return y;
  }
  
  public float getAngle(Screen screen, float angle) {
    if (screen == Screen.field){
      if (isBlue) {
        return (float)Math.toDegrees(angle);
      } else {
        return (float)Math.toDegrees(angle + Math.PI);
      }
    }
    return (float)Math.toDegrees(angle);
  }
  
  private final int fieldScreenWidth, fieldScreenHeight;
  
  private boolean isBlue;
}
