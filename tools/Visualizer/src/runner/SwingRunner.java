package runner;

import swing.*;

public class SwingRunner {

  public static void printUsage() {
    System.out.println("Usage:");
    System.out.println("java -jar Visualizer.jar [address of robot]");
    System.out.println("java -jar Visualizer.jar [address of robot] [port of robot]");
  }
  
  public static void main(String[] args) {
    if (args.length < 1) {
      printUsage();
      return;
    }
    
    String robotAddress = args[0];
    int robotPort = -1;
    if (args.length >= 2) {
      try {
        robotPort = Integer.parseInt(args[1]);
      }
      catch (NumberFormatException e) {
        robotPort = -1;
      }
    }
    
    if (robotPort >= 0) {
      new Visualizer(robotAddress, robotPort);
    }
    else {
      new Visualizer(robotAddress);
    }
  }

}
