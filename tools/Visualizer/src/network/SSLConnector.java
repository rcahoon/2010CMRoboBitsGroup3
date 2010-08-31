package network;

import java.net.*;
import java.util.*;

import config.*;
import network.message.*;
import network.message.ssl.*;

public class SSLConnector implements Runnable, NetworkConnector {

  public SSLConnector(ConfigFile configFile) {
    this.socketTimeout = configFile.getMillisecondsFromSeconds("visualizer/connector/socketTimeout", 100);
    this.bufferSize    = configFile.getInt("visualizer/connector/bufferSize", 1000000);
    
    running = false;
    
    buffer = new byte[bufferSize];
    
    listeners = new Vector<MessageFromSSLListener>();
    
    group = null;
  }

  @Override
  public boolean isConnected() {
    return running;
  }
  
  @Override
  public boolean connect(String host, int port) {
    try {
      disconnect();
      
      group = InetAddress.getByName(host);
      mcs = new MulticastSocket(port);
//      mcs.setNetworkInterface(NetworkInterface.getByInetAddress(InetAddress.getByName("192.168.1.100")));

      mcs.joinGroup(group);  
      mcs.setSoTimeout(socketTimeout);
      
      // Start listening
      thread = new Thread(this);
      thread.start();
      
      return true;
    }
    catch (Exception e) {
      mcs = null;
      return false;
    }
  }

  @Override
  public void disconnect() {
    synchronized (this) {
      running = false;
    }
    while ((thread != null) && thread.isAlive());
    thread = null;
  }
  
  @Override
  public void run() {
    synchronized (this) {
      if (running) {
        return;
      }
      
      running = true;
    }
    
    try {
      
      boolean done = false;
      
      while (!done && running) {
        DatagramPacket packet = null;
        
        try {
          packet = new DatagramPacket(buffer, buffer.length);
          mcs.receive(packet);
        }
        catch (SocketTimeoutException e) {
          continue;
        }
        
        if (packet == null) {
          continue;
        }
      
        // Copy the bytes out of the packet
        byte [] result = new byte[packet.getLength()];
        System.arraycopy(packet.getData(), 0, result, 0, packet.getLength());

        SSLWrapper.SSL_WrapperPacket sslWrapper = SSLWrapper.SSL_WrapperPacket.parseFrom(result);
        if (sslWrapper.hasDetection()) {
          // Add a tick message to signify a new SSL tick
          handleMessage(new TickMessage());
          
          SSLDetection.SSL_DetectionFrame sslDetection = sslWrapper.getDetection();
          
          // Balls
          java.util.List<network.message.ssl.SSLDetection.SSL_DetectionBall> ballsList = sslDetection.getBallsList();
          for (network.message.ssl.SSLDetection.SSL_DetectionBall ball: ballsList) {
            // SSL Vision uses mm, we use cm
            handleMessage(new BallMessage(ball.getX() / 10, ball.getY() / 10, ball.getConfidence()));
          }
          
          // Yellow robots 
          java.util.List<network.message.ssl.SSLDetection.SSL_DetectionRobot> robots = sslDetection.getRobotsYellowList();
          for (network.message.ssl.SSLDetection.SSL_DetectionRobot robot : robots){
            // SSL Vision uses mm, we use cm
            handleMessage(new RobotMessage(RobotMessage.RobotColor.yellow,
                                           robot.getRobotId(),
                                           robot.getX() / 10,
                                           robot.getY() / 10,
                                           robot.getOrientation(),
                                           robot.getConfidence()));
          }
          
          // Blue robots
          robots = sslDetection.getRobotsBlueList();
          for (network.message.ssl.SSLDetection.SSL_DetectionRobot robot : robots){
            // SSL Vision uses mm, we use cm
            handleMessage(new RobotMessage(RobotMessage.RobotColor.blue,
                                           robot.getRobotId(),
                                           robot.getX() / 10,
                                           robot.getY() / 10,
                                           robot.getOrientation(),
                                           robot.getConfidence()));
          }
        }
      }
      
      mcs.leaveGroup(group);
    }
    catch (Exception e) {
    }
    
    running = false;
  }

  public void addListener(MessageFromSSLListener listener) {
    listeners.add(listener);
  }
  
  public void removeListener(MessageFromSSLListener listener) {
    listeners.remove(listener);
  }
  
  private void handleMessage(SSLMessage message) {
    for (MessageFromSSLListener listener : listeners) {
      listener.receiveSSLMessage(message);
    }
  }
 
  private int    socketTimeout;
  private int    bufferSize;
  
  private boolean running;
  
  private MulticastSocket mcs;
  private Thread thread;
  
  private byte [] buffer;
  
  private Vector<MessageFromSSLListener> listeners;
  
  private InetAddress group;
}
