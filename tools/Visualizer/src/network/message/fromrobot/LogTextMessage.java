package network.message.fromrobot;

import java.util.*;

import network.message.RemoteMessageFromRobot;

public class LogTextMessage extends RemoteMessageFromRobot {

  public enum LogLevel {
    TRACE  (1),
    DEBUG  (2),
    INFO   (3),
    WARN   (4),
    ERROR  (5),
    NONE   (6);
    
    LogLevel(int level) {
      this.level = level;
    }
    
    private int level;

    public static LogLevel create(int level) {
      for (LogLevel logLevel : LogLevel.values()) {
        if (logLevel.level == level) {
          return logLevel;
        }
      }
        
      return LogLevel.NONE;
    }
  }
  
  public enum Component {
    OTHERS         (0),
    AGENT          (1),
    BEHAVIORS      (2),
    COMM           (3),
    GAME_CONTROLLER(4),
    LOCALIZATION   (5),
    LOG            (6),
    VISION         (7),
    WORLD_MODEL    (8),
    MAIN_MODULE    (9);
    
    Component(int id) {
      this.id = id;
    }
    
    private int id;

    public static Component create(int id) {
      for (Component component : Component.values()) {
        if (component.id == id) {
          return component;
        }
      }
        
      return Component.OTHERS;
    }
  }
  
  public LogTextMessage(LogLevel logLevel, Component component,
                        String className,
                        String functionName,
                        String text) {
    super(RemoteMessageFromRobot.MessageType.LOG_TEXT, null);
    
    this.logLevel     = logLevel;
    this.component    = component;
    this.className    = className;
    this.functionName = functionName;
    this.text         = text;
  }
  
  public LogLevel getLogLevel() {
    return logLevel;
  }
  
  public Component getComponent() {
    return component;
  }
  
  public String getClassName() {
    return className;
  }
  
  public String getFunctionName() {
    return functionName;
  }
  
  public String getText() {
    return text;
  }
  
  private LogLevel  logLevel;
  private Component component;
  private String    className;
  private String    functionName;
  private String    text;
  
  public static LogTextMessage create(byte [] data) {
    try {
      // The first byte is the log level
      LogLevel logLevel = LogLevel.create((int)data[0]);
      // The second byte is the component id
      Component component = Component.create((int)data[1]);
      
      // Convert everything else into a string
      StringTokenizer stringTokenizer = new StringTokenizer(new String(data, 2, data.length - 2), "\0");
      
      // The first string is the class name
      String className = stringTokenizer.nextToken();
      // The second string is the function name
      String functionName = stringTokenizer.nextToken();
      // The last string is the text
      String text = stringTokenizer.nextToken();
      
      return new LogTextMessage(logLevel, component, className, functionName, text);
    }
    catch (Exception e) {
      return null;
    }
  }
}
