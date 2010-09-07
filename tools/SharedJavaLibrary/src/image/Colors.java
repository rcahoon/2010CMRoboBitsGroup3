package image;

import java.io.*;
import java.util.*;

import image.format.*;

public class Colors {

  private Colors(ColorIndex [] indices) {
    this.indices = indices;
    
    allRGB = new RGB[indices.length];
    allNames = new String[indices.length];
    for (int i = 0; i < indices.length; i++) {
      allRGB[i] = indices[i].rgb;
      allNames[i] = indices[i].name;
    }
  }
  
  public RGB getRGB(int index) {
    if ((index < 0) || (index >= indices.length)) {
      return null;
    }
    
    return indices[index].rgb;
  }
  
  public RGB [] getAllRGB() {
    return allRGB;
  }
  
  public String getName(int index) {
    if ((index < 0) || (index >= indices.length)) {
      return null;
    }
    
    return indices[index].name;
  }
  
  public String [] getAllNames() {
    return allNames;
  }
  
  private ColorIndex [] indices;
  private RGB [] allRGB;
  private String [] allNames;
  
  public static Colors createFromFile(String filename) {
    try {
      BufferedReader input = new BufferedReader(new FileReader(filename));
      
      String str;
      
      // The first line is a comment
      str = input.readLine().trim();
      if (!str.startsWith("#")) {
        System.err.println("Error reading comment on line 1 of colors file.");
        throw new IOException();
      }
      
      List<ColorIndex> indices = new Vector<ColorIndex>();
      int index = 0;
      while ((str = input.readLine()) != null) {
        String [] tokens = str.split("[()\"\\s]+");
        
        // The first token should be a number
        int readIndex = Integer.parseInt(tokens[0]);
        if (readIndex != index) {
          System.err.println("Error reading index " + index + " of colors file.");
          throw new IOException();
        }
        
        // The next 3 tokens are the RGB color
        int red   = Integer.parseInt(tokens[1]);
        int green = Integer.parseInt(tokens[2]);
        int blue  = Integer.parseInt(tokens[3]);
        RGB rgb = new RGB(red, green, blue);
        
        // The next token is the name
        
        ColorIndex color = new ColorIndex(rgb, tokens[4]);
        indices.add(color);
        index++;
      }
      
      input.close();
      
      return new Colors(indices.toArray(new ColorIndex[0]));
    }
    catch (Exception e) {
      System.err.println("Error reading colors file: " + filename);
      return null;
    }
  }
}

class ColorIndex {
  RGB rgb;
  String name;
  
  ColorIndex(RGB rgb, String name) {
    this.rgb  = rgb;
    this.name = name;
  }
}