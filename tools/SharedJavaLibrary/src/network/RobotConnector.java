package network;

import java.io.*;
import java.net.*;
import java.util.*;

import config.*;
import network.message.*;

public class RobotConnector implements Runnable, NetworkConnector {

  public RobotConnector(ConfigFile configFile) {
    this.socketTimeout = configFile.getMillisecondsFromSeconds("tools/shared/connector/socketTimeout", 100);
    this.bufferSize    = configFile.getInt("tools/shared/connector/bufferSize", 1000000);
    
    running = false;
    
    buffer = new byte[bufferSize];
    
    listeners = new Vector<MessageFromRobotListener>();
  }
  
  
  @Override
  public boolean isConnected() {
    return running;
  }
  
  @Override
  public boolean connect(String host, int port) {
    try {
      disconnect();
      
      socket = new Socket(host, port);
      socket.setSoTimeout(socketTimeout);
      
      // Start listening
      thread = new Thread(this);
      thread.start();
      
      return true;
    }
    catch (Exception e) {
      socket = null;
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
      
      InputStream in = socket.getInputStream();
      
      int offset = 0;
      
      int type = -1;
      int size = 0;
      
      boolean done = false;
      while (!done && running) {
        int numRead = 0;
        
        try {
          numRead = in.read(buffer, offset, buffer.length - offset);
        }
        catch (SocketTimeoutException e) {
          continue;
        }
        
        if (numRead <= 0) {
          done = true;
          break;
        }
        
        offset += numRead;
        
        boolean tryAgain = false;
        
        do {
          tryAgain = false;
          
          // Do we have a size for the packet?
          if ((size == 0) && (offset >= RemoteMessageFromRobot.MESSAGE_HEADER_SIZE)) {
            // The first byte is the type
            type = (int)buffer[0];
            // The next 4 bytes are the size
            size = RemoteMessage.readInt(buffer, 1);
            
            //System.out.printf("Expecting a packet of type %d with size %d.\n", type, size);
          }
          
          // Has the whole packet been read?
          if ((size > 0) && (offset >= RemoteMessageFromRobot.MESSAGE_HEADER_SIZE + size)) {
            // Copy the data out
            byte [] data = new byte[size];
            System.arraycopy(buffer, RemoteMessageFromRobot.MESSAGE_HEADER_SIZE, data, 0, size);
            
            RemoteMessageFromRobot message = network.message.fromrobot.Factory.create(type, data);
            handleMessage(message);
            
            // Move the rest of buffer's data towards the front
            offset -= RemoteMessageFromRobot.MESSAGE_HEADER_SIZE + size;
            if (offset >= 0) {
              System.arraycopy(buffer, RemoteMessageFromRobot.MESSAGE_HEADER_SIZE + size, buffer, 0, offset);
            }

            size = 0;
            tryAgain = true;
          }
        } while (tryAgain);
        
      }
      
      socket.close();
    }
    catch (Exception e) {
    }
    
    running = false;
  }
  
  public void addListener(MessageFromRobotListener listener) {
    listeners.add(listener);
  }
  
  public void removeListener(MessageFromRobotListener listener) {
    listeners.remove(listener);
  }
  
  private void handleMessage(RemoteMessageFromRobot message) {
    for (MessageFromRobotListener listener : listeners) {
      listener.receiveRemoteMessageFromRobot(message);
    }
  }
 
  private int    socketTimeout;
  private int    bufferSize;
  
  private boolean running;
  
  private Socket socket;
  private Thread thread;
  
  private byte [] buffer;
  
  private Vector<MessageFromRobotListener> listeners;
}
