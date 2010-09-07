package image;

import java.io.*;
import java.util.zip.*;

import image.format.*;

public class Threshold {

  private Threshold(int numY, int numU, int numV, int [][][] thresholds) {
    this.thresholds = thresholds;
    
    this.numY = numY;
    this.numU = numU;
    this.numV = numV;
    
    // How many bits do we need to shift the raw yuv values?
    bitsY = 8 - getNumBits(numY);
    bitsU = 8 - getNumBits(numU);
    bitsV = 8 - getNumBits(numV);
  }
  
  public byte [] segmentImage(YUVImage image) {
    byte [] result = new byte[image.getWidth() * image.getHeight()];
    
    int index = 0;
    for (int y = 0; y < image.getHeight(); y++) {
      for (int x = 0; x < image.getWidth(); x++) {
        result[index] = (byte)getIndex(image.getYUV(x, y));
        index++;
      }
    }
    
    return result;
  }
  
  public int getIndex(YUV yuv) {
    int indexY = (yuv.getY() >> bitsY) & 0xFF;
    int indexU = (yuv.getU() >> bitsU) & 0xFF;
    int indexV = (yuv.getV() >> bitsV) & 0xFF;
    
    return thresholds[indexY][indexU][indexV];
  }
  
  public void setIndex(YUV yuv, int index) {
    int indexY = (yuv.getY() >> bitsY) & 0xFF;
    int indexU = (yuv.getU() >> bitsU) & 0xFF;
    int indexV = (yuv.getV() >> bitsV) & 0xFF;
    
    thresholds[indexY][indexU][indexV] = index;
  }
  
  public void saveToFile(String filename) {
    try {
      OutputStream output = new FileOutputStream(filename);
      // Check if the input is gzipped
      if (filename.endsWith(".gz")) {
        output = new GZIPOutputStream(output);
      }
      output = new BufferedOutputStream(output);

      // The first string should be TMAP, followed by the type
      output.write("TMAP\nYUV1\n".getBytes("UTF-8"));
      
      // Next is the size
      String sizeStr = numY + " " + numU + " " + numV + "\n";
      output.write(sizeStr.getBytes("UTF-8"));

      for (int y = 0; y < numY; y++) {
        for (int u = 0; u < numU; u++) {
          for (int v = 0; v < numV; v++) {
            output.write((byte)thresholds[y][u][v]);
          }
        }
      }
      
      output.close();
    }
    catch (Exception e) {
      System.err.println("Error writing threshold file: " + filename);
    }
  }
  
  private int getNumBits(int value) {
    int count = 1;
    int power = 0;
    
    while (count < value) {
      count *= 2;
      power++;
    }
    
    return power;
  }
  
  private int numY, numU, numV;
  private int bitsY, bitsU, bitsV;
  private int [][][] thresholds;
  
  public static Threshold createFromFile(String filename) {
    try {
      InputStream input = new FileInputStream(filename);
      // Check if the input is gzipped
      if (filename.endsWith(".gz")) {
        input = new GZIPInputStream(input);
      }

      // The first string should be TMAP
      String string = readString(input);
      if (!string.equals("TMAP")) {
        System.err.println("Error finding TMAP header in threshold file.");
        throw new IOException();
      }
      
      // Read the type (which is ignored)
      string = readString(input);
      
      // Read the size
      int numY, numU, numV;
      string = readString(input);
      String [] tokens = string.split("[\\s]+");
      numY = Integer.parseInt(tokens[0]);
      numU = Integer.parseInt(tokens[1]);
      numV = Integer.parseInt(tokens[2]);
      
      int size = numY * numU * numV;
      byte [] thresholdData = new byte[size];
      int read = 0;
      while (read < size) {
        int count = input.read(thresholdData, read, size - read);
        if (count == -1) {
          System.err.println("Error reading threshold data.");
          throw new IOException();
        }
        read += count;
      }
      
      int [][][] thresholds = new int[numY][numU][numV];
      int index = 0;
      for (int y = 0; y < numY; y++) {
        for (int u = 0; u < numU; u++) {
          for (int v = 0; v < numV; v++) {
            thresholds[y][u][v] = (int)thresholdData[index];
            index++;
          }
        }
      }
      
      input.close();
      
      return new Threshold(numY, numU, numV, thresholds);
    }
    catch (Exception e) {
      System.err.println("Error reading threshold file: " + filename);
      return null;
    }
  }
  
  private static String readString(InputStream input) throws IOException {
    StringBuilder string = new StringBuilder();
    
    boolean done = false;
    while (!done) {
      char value = (char)input.read();
      if ((value == 0) || (value == '\n')) {
        done = true;
      }
      else {
        string.append(value);
      }
    }
    
    return string.toString();
  }
  
}
