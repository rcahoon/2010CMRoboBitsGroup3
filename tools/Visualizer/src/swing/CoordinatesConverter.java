package swing;

import config.*;
import shape.*;

public class CoordinatesConverter {

  public CoordinatesConverter(ConfigFile configFile) {
    fieldScreenWidth  = configFile.getInt("visualizer/fieldScreenWidth", 686);
    fieldScreenHeight = configFile.getInt("visualizer/fieldScreenHeight", 486);
    
    isBlue = true;
  }
  
  public void setBlue(boolean isBlue) {
    this.isBlue = isBlue;
  }
  
  public int getX(Screen screen, float x) {
    if (screen == Screen.field){
      int adjustedX = (int)x;
      if (isBlue) {
        adjustedX = (int)((float)fieldScreenWidth / 2 + x);
      } else{
        adjustedX = (int)((float)fieldScreenWidth / 2 - x);
      }
      
      if (adjustedX < 0) {
        adjustedX = 0;
      }
      if (adjustedX > fieldScreenWidth) {
        adjustedX = fieldScreenWidth;
      }
      
      return adjustedX;
    }
    return (int)x;
  }
  
  public int getY(Screen screen, float y) {
    if (screen == Screen.field){
      int adjustedY = (int)y;
      if (isBlue) {
        adjustedY = (int)((float)fieldScreenHeight / 2 - y);
      } else {
        adjustedY = (int)((float)fieldScreenHeight / 2 + y);
      }

      if (adjustedY < 0) {
        adjustedY = 0;
      }
      if (adjustedY > fieldScreenHeight) {
        adjustedY = fieldScreenHeight;
      }
      
      return adjustedY;
    }    
    return (int)y;
  }
  
  public float getAngle(Screen screen, float angle) {
    if (screen == Screen.field){
      if (isBlue) {
        return angle;
      } else {
        return (float)(angle + Math.PI);
      }
    }
    return angle;
  }
  
  private final int fieldScreenWidth, fieldScreenHeight;
  
  private boolean isBlue;
}
