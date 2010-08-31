package template;

import java.io.*;
import java.util.*;

public class Template {

  public Template() {
  }
  
  public void setValues(HashMap<String, String> values) {
    // Ensure that all the keys exist
    for (String key : keys) {
      if (!values.containsKey(key.toUpperCase())) {
        throw new IllegalArgumentException("Key '" + key.toUpperCase() + "' not found.");
      }
    }
    
    this.values = values;
  }
  
  public void setFSMStates(List<String> fsmStates) {
    this.fsmStates = fsmStates;
  }
  
  public void parseFile(String templateFilename, String outputFilename) throws IOException {
    BufferedReader input = new BufferedReader(new FileReader(templateFilename));
    
    StringBuilder outputString = new StringBuilder();
    
    String line;
    while ((line = input.readLine()) != null) {
      String encodedString = line + "\n";
      String trimmedLine = line.trim();
      
      boolean toRepeat = false;
      // Special case
      if (trimmedLine.equalsIgnoreCase("#BEGIN_REPEAT#")) {
        toRepeat = true;
        encodedString = "";
        // Keep reading until we get #END_REPEAT#
        while ((line = input.readLine()) != null) {
          trimmedLine = line.trim();
          if (trimmedLine.equalsIgnoreCase("#END_REPEAT#")) {
            break;
          }
          else {
            encodedString += line + "\n";
          }
        }
      }
      
      // Replace all keywords with their values
      for (String key : keys) {
        encodedString = encodedString.replace("$" + key.toUpperCase() + "$", values.get(key));
      }
      
      // If the encoded string contains the key for fsm states, we need to repeat
      if (encodedString.contains("$" + fsmStateKey.toUpperCase() + "$")) {
        toRepeat = true;
      }
      
      // Do we have to repeat?
      if (!toRepeat) {
        outputString.append(encodedString);
      }
      else {
        boolean firstState = true;
        for (String state : fsmStates) {
          String updatedString = encodedString.replace("$" + fsmStateKey.toUpperCase() + "$", state);
          if (!firstState) {
            updatedString = updatedString.replace("if (", "else if (");
          }
          else {
            firstState = false;
          }
          
          outputString.append(updatedString);
        }
      }
    }
    
    input.close();
    
    // Write to the output file
    PrintWriter output = new PrintWriter(new FileWriter(outputFilename));
    output.println(outputString.toString());
    
    output.close();
  }
  
  private HashMap<String, String> values;
  private List<String> fsmStates;
  
  private static final String [] keys = { "CLASS", "CLASS_UPPER_CASE", "FSM_INITIAL_STATE" };
  private static final String fsmStateKey = "FSM_STATE";
}
