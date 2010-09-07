package config;

import java.io.*;
import java.util.*;

/**
 * This class reads one or more text files to generate name-value pairs that can
 * be used to load constants or initial values of other classes. The file(s) is
 * opened only during the construction of the object, and when reload() is
 * called. Otherwise, other functions return values that were read when the
 * file(s) was opened.
 *
 * The text file should have the following format:
 *   name1 = value1
 *   name2 = value2
 * Names and values are stored as strings until another function is called to
 * process the value, e.g., getInt()
 *
 * Comments in the text file should use the # character, e.g.
 *   # This is a valid comment
 *   name1 = value1 # this is a valid comment too
 *
 * To read other files as input, use the ~INCLUDE command, e.g.
 *   ~INCLUDE other_file.txt
 * This will cause the ConfigFile class to load "other_file.txt" and process it
 * as well.
 *
 */
public class ConfigFile {

  /**
   * Construct a ConfigFile by reading the specified file.
   * 
   * @param path root directory to search for files
   * @param filename name of the file to read
   * @throws IOException if an IO error occurs while reading the file
   */
  public ConfigFile(String path, String filename) throws IOException {
    this.path     = path;
    // Ensure that the path ends with a / if it's not empty
    if ((this.path.length() > 0) && !this.path.endsWith("/")) {
      this.path += "/";
    }
    
    this.filename = filename;
    this.mapString = new HashMap<String, String>();
    this.mapInt    = new HashMap<String, Integer>();
    this.mapDouble = new HashMap<String, Double>();
    
    this.filesOpened = new ArrayList<String>();
    reload();
  }
  
  /**
   * Returns the value associated with the name. If the name does not exist in
   * the files, then an empty string is returned.
   *
   * @param name name of the value in the file(s)
   * @param defaultValue the default value to use if name does not exist
   *
   * @return value as a string
   */
  public String getString(String name, String defaultValue) {
    useDefault = false;
    
    // Check if the map to string has the key we need
    String result = mapString.get(name);
    
    if (result != null) {
      return result;
    }
    else {
      System.err.println(name + " not found in ConfigFile.");
      useDefault = true;
      return defaultValue;
    }
  }
  
  /**
   * Returns the path associated with the name. The value is read from the file,
   * and pre-pended by the path defined in the config file.
   * 
   * For example, if the path is /home/nao and the value in the file is a.txt,
   * then getPath will return /home/nao/a.txt
   * 
   * @param name name of the value in the file(s)
   * @param defaultValue default value to use as the filename
   * 
   * @return path as a string
   */
  public String getPath(String name, String defaultValue) {
    return path + getString(name, defaultValue);
  }
  
  /**
   * Returns the value associated with the name. If the name does not exist in
   * the files, then the default value is returned.
   *
   * @param name name of the value in the file(s)
   * @param defaultValue default value to be returned if name does not exist
   *
   * @return value as an integer
   */
  public int getInt(String name, int defaultValue) {
    useDefault = false;
    
    // Check if the map to int has the key we need
    Integer result = mapInt.get(name);
    if (result != null) {
      return result.intValue();
    }
    
    // Check if the key exists in the map to string
    try {
      String strValue = mapString.get(name);
      
      int intValue = 0;
      // Check if string is a hex value
      if (strValue.startsWith("0x")) {
        intValue = Integer.parseInt(strValue.substring(2), 16);
      }
      // Parse it as a regular number
      else {
        intValue = Integer.parseInt(strValue); 
      }

      result = Integer.valueOf(intValue);
      
      // Insert it into the map to int
      mapInt.put(name, result);
      
      return result;
    }
    // If an exception occurs, either the key does not exist in the map
    // to string, or a number format exception occurred
    catch (Exception e) {
      System.err.println(name + " not found in ConfigFile.");
      useDefault = true;
      return defaultValue;
    }
  }
  
  /**
   * Returns the value associated with the name. If the name does not exist in
   * the files, then the default value is returned.
   *
   * @param name name of the value in the file(s)
   * @param defaultValue default value to be returned if name does not exist
   *
   * @return value as a double
   */
  public double getDouble(String name, double defaultValue) {
    useDefault = false;
    
    // Check if the map to double has the key we need
    Double result = mapDouble.get(name);
    if (result != null) {
      return result.doubleValue();
    }
    
    // Check if the key exists in the map to string
    try {
      String strValue = mapString.get(name);
      
      double doubleValue = Double.parseDouble(strValue);

      result = Double.valueOf(doubleValue);
      
      // Insert it into the map to int
      mapDouble.put(name, result);
      
      return result;
    }
    // If an exception occurs, either the key does not exist in the map
    // to string, or a number format exception occurred
    catch (Exception e) {
      System.err.println(name + " not found in ConfigFile.");
      useDefault = true;
      return defaultValue;
    }
  }
  
  /**
   * Returns the value associated with the name. If the name does not exist in
   * the files, then the default value is returned. This function assumes that
   * the value in the file(s) is in seconds, and will convert it to
   * milliseconds.
   *
   * @param name name of the value (in seconds) in the file(s)
   * @param defaultValue default value to be returned if name does not exist
   *
   * @return value (milliseconds) as a long
   */
  public int getMillisecondsFromSeconds(String name, int defaultValue) {
    // Retrieve the value in the file (in seconds)
    double seconds = getDouble(name, 0);
    // Convert it to milliseconds
    if (!useDefault) {
      return (int)Math.round(seconds * 1000.0);
    }
    else {
      return defaultValue;
    }
  }
  
  /**
   * Returns the value associated with the name. If the name does not exist in
   * the files, then the default value is returned. This function assumes that
   * the value in the file(s) is in degrees, and will convert it to radians.
   *
   * @param name name of the value (in degrees) in the file(s)
   * @param defaultValue default value to be returned if name does not exist
   *
   * @return value (in radians) as a double
   */
  public double getRadiansFromDegrees(String name, double defaultValue) {
    // Retrieve the value in the file (in degrees)
    double degrees = getDouble(name, 0);
    // Convert it to radians
    if (!useDefault) {
      return Math.toRadians(degrees);
    }
    else {
      return defaultValue;
    }
  }
  
  /**
   * Returns the value associated with the name. If the name does not exist in
   * the files, then the default value is returned. This function assumes that
   * the value in the file(s) is in meters, and will convert it to centimeters.
   *
   * @param name name of the value (in meters) in the file(s)
   * @param defaultValue default value to be returned if name does not exist
   *
   * @return value (in centimeters) as a double
   */
  public double getCentimetersFromMeters(String name, double defaultValue) {
    // Retrieve the value in the file (in m)
    double meters = getDouble(name, 0);
    // Convert it to cm
    if (!useDefault) {
      return meters * 100.0;
    }
    else {
      return defaultValue;
    }
  }
  
  /**
   * Returns the value associated with the name. If the name does not exist in
   * the files, then the default value is returned. This function assumes that
   * the value in the file(s) is in centimeters, and will convert it to meters.
   *
   * @param name name of the value (in centimeters) in the file(s)
   * @param defaultValue default value to be returned if name does not exist
   *
   * @return value (in meters) as a double
   */
  public double getMetersFromCentimeters(String name, double defaultValue) {
    // Retrieve the value in the file (in cm)
    double meters = getDouble(name, 0);
    // Convert it to m
    if (!useDefault) {
      return meters / 100.0;
    }
    else {
      return defaultValue;
    }
  }
  
  /**
   * Returns the value associated with the name. If the name does not exist in
   * the files, then the default value is returned. This function assumes that
   * 0 means false, and any other integer means true.
   *
   * @param name name of the value (0 or 1) in the file(s)
   * @param defaultValue default value to be returned if name does not exist
   *
   * @return value as a bool
   */
  public boolean getBool(String name, boolean defaultValue) {
    // Retrieve the value in the file (as an int)
    int value = getInt(name, 0);
    // Cast the int as a boo (0 = false, everything else = true)
    if (!useDefault) {
      return (value != 0);
    }
    else {
      return defaultValue;
    }
  }
  
  /**
   * Reloads the file, and all included files (if any).
   */
  public void reload() throws IOException {
    // Clear all the maps
    mapString.clear();
    mapInt.clear();
    mapDouble.clear();
    
    // Clear the list of opened files
    filesOpened.clear();
    
    // Reload the main file
    readFile(filename);
  }

  /**
   * Read a file and fill up the map to string.
   * 
   * @param filename name of file top read
   * @throws IOException if an IO error occurs while reading the file
   */
  private void readFile(String filename) throws IOException {
    BufferedReader input = new BufferedReader(new FileReader(new File(path + filename)));
    String line = null;
    
    // Add the filename into the list of files opened
    if (filesOpened.contains(filename)) {
      return;
    }
    filesOpened.add(filename);
    
    int lineNum = 0;
    
    while ((line = input.readLine()) != null) {
      lineNum++;
      
      // Trim off comments and spaces/tabs
      String trimmedLine = trimString(line);
      
      // Check if the line is empty
      if (trimmedLine.length() == 0) {
        continue;
      }
      
      // Check if we should include another file
      if (trimmedLine.startsWith(INCLUDE_COMMAND)) {
        String includedFilename = trimmedLine.substring(INCLUDE_COMMAND.length()); // remove the include command
        includedFilename = trimString(includedFilename);
        
        // Makesure we have a filename
        if (includedFilename.length() == 0) {
          System.err.printf("Error reading ~INCLUDE command in file: %s at line number %d%n",
                            filename, lineNum);
          continue;
        }
        else {
          // Make sure we haven't already imported the file
          if (filesOpened.contains(includedFilename)) {
            System.err.printf("Repeated ~INCLUDE in file: %s at line number %d%n",
                              filename, lineNum);
          }
          else {
            readFile(includedFilename);
          }
          continue;
        }
      }
      
      // Find the occurrence of =
      int index = trimmedLine.indexOf('=');
      if (index == -1) {
        System.err.printf("Error parsing '=' in file: %s at line number %d%n",
                          filename, lineNum);
        continue;
      }
      
      String name = trimString(trimmedLine.substring(0, index));
      String value = trimString(trimmedLine.substring(index + 1));
      if ((name.length() == 0) || (value.length() == 0)) {
        System.err.printf("Error parsing name/value in file: %s at line number %d%n",
                          filename, lineNum);
        continue;
      }
      
      mapString.put(name, value);
    }
  }

  /**
   * Trim a line of whitespace and comments.
   * 
   * @param line the line to trim
   * @return the trimmed line
   */
  private String trimString(String line) {
    // Trim the string of whitespace
    line = line.trim();
    
    // Check if a comment marker exists
    int commentPos = line.indexOf(COMMENT_MARKER);
    // If there are comments, we want the last non-whitespace before the comment
    if (commentPos >= 0) {
      line = line.substring(0, commentPos).trim();
    }
    
    return line;
  }
  
  private String path, filename;
  
  private Map<String, String>  mapString;
  private Map<String, Integer> mapInt;
  private Map<String, Double>  mapDouble;
  private List<String> filesOpened;
  
  private boolean useDefault;
  
  private static final String COMMENT_MARKER = "#";
  private static final String INCLUDE_COMMAND = "~INCLUDE";
}
