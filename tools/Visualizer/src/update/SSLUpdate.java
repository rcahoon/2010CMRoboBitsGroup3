package update;

import java.util.*;

import network.message.*;

public class SSLUpdate implements Iterable<SSLMessage> {

  public SSLUpdate() {
    messages = new Vector<SSLMessage>();
  }
  
  public void addMessage(SSLMessage message) {
    messages.add(message);
  }
  
  public Iterator<SSLMessage> iterator() {
    return messages.iterator();
  }
  
  List<SSLMessage> messages;
}
