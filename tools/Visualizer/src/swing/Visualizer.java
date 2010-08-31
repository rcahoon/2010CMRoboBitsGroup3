package swing;

import logwriter.LogWriter;
import logwriter.SSLLogWriter;
import config.*;
import network.*;
import network.message.*;
import network.message.fromrobot.*;
import network.message.ssl.*;
import swing.ui.*;
import update.*;

public class Visualizer implements Runnable, MessageFromRobotListener, MessageFromSSLListener {

  public Visualizer(String robotHost) {
    this(robotHost, VisualizerConfig.getInstance().getInt("comm/remoteListenPort", 10000));
  }
  
  public Visualizer(String robotHost, int robotPort) {
    configFile = VisualizerConfig.getInstance();
    
    RECONNECT_SLEEP_TIME = configFile.getMillisecondsFromSeconds("visualizer/reconnectSleepTime", 100);
    
    this.robotHost = robotHost;
    this.robotPort = robotPort;
    
    sslHost = configFile.getString("sslVision/address", "224.5.23.2");
    sslPort = configFile.getInt   ("sslVision/port",    10002);
    
    robotConnector = new RobotConnector(configFile);
    sslConnector   = new SSLConnector(configFile);
    
    robotConnector.addListener(this);
    sslConnector.addListener(this);
    
    logWriter = LogWriter.create(configFile);
    sslLogWriter = SSLLogWriter.create(configFile);
    
    robotConnector.addListener(logWriter);
    sslConnector.addListener(sslLogWriter);
    
    screen = new DirectScreen(configFile.getString("visualizer/windowTitle",  "CMurfs Visualizer"),
                              configFile.getInt   ("visualizer/screenWidth",  986),
                              configFile.getInt   ("visualizer/screenHeight", 486));
    displayHandler = new DisplayHandler(configFile);
    
    robotUpdate = new RobotUpdate(true);
    sslUpdate   = new SSLUpdate();

    // We should show the initial screen
    displayHandler.receiveUpdate(robotUpdate, screen.getGraphicsToDraw());
    screen.flipScreen();
    
    // Start a thread to keep connecting to the robot and SSL
    new Thread(this).start();
  }
  
  @Override
  public void run() {
    if (reconnectRunning) {
      return;
    }
    
    reconnectRunning = true;
    while (reconnectRunning) {
      if (!robotConnector.isConnected()) {
        if (robotConnector.connect(robotHost, robotPort)) {
          System.out.println("Connected to robot at " + robotHost + ".");
        }
      }
      if (!sslConnector.isConnected()) {
        if (sslConnector.connect(sslHost, sslPort)) {
          System.out.println("Connected to SSL Vision at " + sslHost + ".");
        }
      }
      if (robotConnector.isConnected() && sslConnector.isConnected()) {
        try {
          Thread.sleep(RECONNECT_SLEEP_TIME);
        }
        catch (Exception e) {
        }
      }
    }
  }
  
  public void stopRunning() {
    reconnectRunning = false;
  }
  
  @Override
  public void receiveRemoteMessageFromRobot(RemoteMessageFromRobot message) {
    if (message instanceof LogBeginMessage) {
      robotUpdate = new RobotUpdate(robotUpdate.isBlue());
    }
    else if (message instanceof LogEndMessage) {
      // Send the update to the DisplayHandler
      displayHandler.receiveUpdate(robotUpdate, screen.getGraphicsToDraw());
      // Flip the screen
      screen.flipScreen();
      
      robotUpdate = new RobotUpdate(robotUpdate.isBlue());
    }
    else {
      robotUpdate.addMessage(message);
    }
  }
  
  @Override
  public void receiveSSLMessage(SSLMessage message) {
    if (message instanceof TickMessage) {
      // Send the update to the DisplayHandler
      displayHandler.receiveUpdate(sslUpdate, screen.getGraphicsToDraw());
      // Flip the screen
      screen.flipScreen();
      
      sslUpdate = new SSLUpdate();
    }
    else {
      sslUpdate.addMessage(message);
    }
  }
  
  private final long RECONNECT_SLEEP_TIME;
  
  private ConfigFile configFile;
  
  private String robotHost, sslHost;
  private int    robotPort, sslPort;
  
  private DirectScreen   screen;
  private DisplayHandler displayHandler;
  
  private RobotConnector robotConnector;
  private SSLConnector   sslConnector;
  
  private LogWriter logWriter;
  private SSLLogWriter sslLogWriter;
  
  private boolean reconnectRunning;
  
  RobotUpdate robotUpdate;
  SSLUpdate   sslUpdate;
  
}
