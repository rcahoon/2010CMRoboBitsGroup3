package logwriter;

import java.io.*;

import config.*;
import network.message.*;

public class LogWriter implements MessageFromRobotListener {

  public LogWriter(String filename) {
    this.filename = filename;
    
    file = null;
  }
  
  @Override
  public void receiveRemoteMessageFromRobot(RemoteMessageFromRobot message) {
    if (message.getType() == RemoteMessageFromRobot.MessageType.ROBOT_STATE) {
      byte [] size = new byte[4];
      byte [] data = message.getData();
      
//      System.out.println(data.length);
      size[0] = (byte)((data.length >> 24) & 0xFF);
      size[1] = (byte)((data.length >> 16) & 0xFF);
      size[2] = (byte)((data.length >> 8) & 0xFF);
      size[3] = (byte)( data.length       & 0xFF);
//      System.out.printf("%x %x %x %x\n", size[0], size[1], size[2], size[3]);
      
      try {
        if (file == null) {
          file = new FileOutputStream(filename);
        }
        file.write(size);
        file.write(data);
        file.flush();
      }
      catch (IOException e) {
        System.err.println("Error occurred while writing to output file: " + filename);
      }
    }
  }
  
  public static LogWriter create(ConfigFile configFile) {
    try {
      boolean enabled = configFile.getBool("logToFile/enabled", false);
      if (!enabled) {
        return null;
      }
      
      String filename = configFile.getPath("logToFile/filename", "log.log");
      return new LogWriter(filename);
    }
    catch (Exception e) {
      return null;
    }
  }

  private String filename;
  private FileOutputStream file;
}
