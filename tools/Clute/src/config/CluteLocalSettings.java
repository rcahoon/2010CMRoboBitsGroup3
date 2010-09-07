package config;

import java.io.*;

public class CluteLocalSettings {

  public CluteLocalSettings(ConfigFile configFile) {
    filename = configFile.getPath("tools/clute/localSettingsFilename", "localSettings.txt");
    
    load();
  }
  
  public int getWindowPosX() {
    return windowPosX;
  }
  
  public int getWindowPosY() {
    return windowPosY;
  }
  
  public void setWindowPos(int windowPosX, int windowPosY) {
    this.windowPosX = windowPosX;
    this.windowPosY = windowPosY;
  }
  
  public int getSegmentedAlpha() {
    return segmentedAlpha;
  }
  
  public void setSegmentedAlpha(int segmentedAlpha) {
    this.segmentedAlpha = segmentedAlpha;
  }
  
  public boolean isImagePaused() {
    return imagePaused;
  }
  
  public void setImagePaused(boolean imagePaused) {
    this.imagePaused = imagePaused;
  }
  
  public boolean shouldAutoSegment() {
    return autoSegment;
  }
  
  public void setAutoSegment(boolean autoSegment) {
    this.autoSegment = autoSegment;
  }
  
  public void save() {
    try {
      BufferedWriter output = new BufferedWriter(new FileWriter(filename));
      
      // Write the window posX, posY
      output.write(windowPosX + "\n");
      output.write(windowPosY + "\n");
      
      // Write the segmented alpha
      output.write(segmentedAlpha + "\n");
      
      // Write whether the image is paused, and if segmentation should be done
      // automatically
      output.write(imagePaused + "\n");
      output.write(autoSegment + "\n");
      
      output.close();
    }
    catch (IOException e) {
      System.err.println("Error writing local settings to file.");
    }
  }
  
  private void load() {
    try {
      BufferedReader input = new BufferedReader(new FileReader(filename));
      
      // Read the window posX, posY
      windowPosX = Integer.parseInt(input.readLine());
      windowPosY = Integer.parseInt(input.readLine());
      
      // Read the segmented alpha
      segmentedAlpha = Integer.parseInt(input.readLine());
      
      // Read whether the image is paused, and if segmentation should be done
      // automatically
      imagePaused = Boolean.parseBoolean(input.readLine());
      autoSegment = Boolean.parseBoolean(input.readLine());
      
      input.close();
    }
    catch (Exception e) {
      System.err.println("Error reading local settings. Using default values.");
      
      windowPosX = 0;
      windowPosY = 0;
      
      segmentedAlpha = 50;
      
      imagePaused = false;
      autoSegment = true;
    }
  }
  
  private String filename;
  
  private int windowPosX, windowPosY;
  private int segmentedAlpha;
  private boolean imagePaused, autoSegment;
  
}
