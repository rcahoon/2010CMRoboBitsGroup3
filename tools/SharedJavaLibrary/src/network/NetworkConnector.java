package network;

public interface NetworkConnector {

  /**
   * Tries to connect to a host.
   * 
   * @param host address of the host
   * @param port port of the host
   * 
   * @return whether the connection succeeded
   */
  boolean connect(String host, int port);
  
  /**
   * Disconnect from the host (if any).
   */
  void disconnect();
  
  /**
   * Checks if the network connector is connected to a host.
   * 
   * @return whether the network is connected
   */
  boolean isConnected();
  
}
