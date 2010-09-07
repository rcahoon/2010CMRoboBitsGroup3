package runner;

import ui.swing.*;

public class Runner {

  public static void printUsage() {
    System.out.println("./run.sh robot_ip port");
  }
  
  public static void main(String[] args) {
    if (args.length < 2) {
      printUsage();
      return;
    }
    
    int port = 0;
    try {
      port = Integer.parseInt(args[1]);
    }
    catch (Exception e) {
      printUsage();
      return;
    }
    
    new CluteWindow(args[0], port);
  }

}
