package runner;

import java.util.*;

import template.Template;

public class BehaviorCreator {

  public static void main(String [] args) {
    Scanner in = new Scanner(System.in);
    
    // Read the class name
    System.out.print("Class name: ");
    String className = in.nextLine();
    if ((className == null) || (className.length() == 0)) {
      System.out.println("Class name cannot be empty!");
      return;
    }
    
    List<String> fsmStates = new Vector<String>();
    // Read the FSM states
    System.out.println("Please enter the FSM states, one in each line. Enter an empty line to finish.");
    boolean done = false;
    while (!done) {
      String line = in.nextLine();
      if ((line == null) || (line.length() == 0)) {
        done = true;
        break;
      }
      fsmStates.add(line);
    }
    if (fsmStates.size() == 0) {
      System.out.println("At least one FSM state is required.");
      return;
    }
    
    // Read the initial FSM state
    System.out.print("Initial FSM state: ");
    String fsmInitialState = in.nextLine();
    if ((fsmInitialState == null) || (fsmInitialState.length() == 0)) {
      System.out.println("Initial FSM state cannot be empty!");
      return;
    }
    if (!fsmStates.contains(fsmInitialState)) {
      System.out.println("List of FSM states does not include initial state, adding it automatically.");
      fsmStates.add(fsmInitialState);
    }
    
    // Create the HashMap of values
    HashMap<String, String> values = new HashMap<String, String>();
    values.put("CLASS", className);
    values.put("CLASS_UPPER_CASE", className.toUpperCase());
    values.put("FSM_INITIAL_STATE", fsmInitialState);
    
    // Create the template
    Template template = new Template();
    template.setValues(values);
    template.setFSMStates(fsmStates);
    
    // We want to run the template on the .h and .cpp
    System.out.print("Creating behavior files ... ");
    try {
      template.parseFile(templateFilename + ".h",   className + ".h");
      template.parseFile(templateFilename + ".cpp", className + ".cpp");
    }
    catch (Exception e) {
      System.out.println();
      System.out.println("Error creating behavior files.");
    }
    System.out.println("OK");
  }
  
  private static final String templateFilename = "template/BehaviorTemplate";
}
