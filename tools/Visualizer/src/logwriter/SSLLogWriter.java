package logwriter;

import java.io.FileWriter;
import java.io.IOException;


import config.ConfigFile;

import network.message.MessageFromRobotListener;
import network.message.MessageFromSSLListener;
import network.message.RemoteMessageFromRobot;
import network.message.SSLMessage;
import network.message.fromrobot.LogBeginMessage;
import network.message.ssl.BallMessage;
import network.message.ssl.RobotMessage;
import network.message.ssl.RobotMessage.RobotColor;

public class SSLLogWriter implements MessageFromRobotListener, MessageFromSSLListener {

  public SSLLogWriter(String filename) {
    this.filename = filename;
    this.currentTimeStamp = 0;
    file = null;
  }
  
  @Override
  public void receiveRemoteMessageFromRobot(RemoteMessageFromRobot message) {
    if (message instanceof LogBeginMessage) {
      currentTimeStamp = ((LogBeginMessage) message).getTimestamp();
    }

  }

  @Override
  public void receiveSSLMessage(SSLMessage sslMessage) {
    if (sslMessage instanceof BallMessage) {      
      BallMessage ballMessage = (BallMessage)sslMessage;
      
      try {
        if (file == null) {
          file = new FileWriter(filename);
        }
        file.write(currentTimeStamp + separator
                   + "TYPE=BALL" + separator
                   + "X="+ ballMessage.getX() + separator
                   + "Y="+ ballMessage.getY()+ separator
                   + "Conf="+ ballMessage.getConfidence() 
                   + System.getProperty("line.separator") );
        file.flush();
      }
      catch (IOException e) {
        System.err.println("Error occurred while writing to output file: " + filename);
      }
    }
    else if (sslMessage instanceof RobotMessage) {
      RobotMessage robotMessage = (RobotMessage)sslMessage;
      
      try {
        if (file == null) {
          file = new FileWriter(filename);
        }
        file.write(currentTimeStamp + separator
                   + (robotMessage.getColor() == RobotColor.blue ? 
                       "TYPE=BLUE" + separator
                      :"TYPE=YELLOW" + separator)
                   + "X="+ robotMessage.getX() + separator
                   + "Y="+ robotMessage.getY()+ separator
                   + "Conf="+ robotMessage.getConfidence() 
                   + System.getProperty("line.separator") );
        file.flush();
      }
      catch (IOException e) {
        System.err.println("Error occurred while writing to output file: " + filename);
      }         
    }
   
  }

  public static SSLLogWriter create(ConfigFile configFile) {
    try {
      boolean enabled = configFile.getBool("logToFile/sslLogEnabled", false);
      if (!enabled) {
        return null;
      }
      
      String filename = configFile.getPath("logToFile/sslLog", "sslLog.log");
      return new SSLLogWriter(filename);
    }
    catch (Exception e) {
      return null;
    }
  }
  
  private String filename;
  private FileWriter file;
  private int currentTimeStamp;
  private final String separator = " ";
  
}
