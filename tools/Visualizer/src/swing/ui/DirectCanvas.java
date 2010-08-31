package swing.ui;

import javax.swing.*;
import java.awt.*;
import java.awt.image.*;

public class DirectCanvas extends JPanel {

  public DirectCanvas(int width, int height) {
    super( true );
    
    image     = new BufferedImage(width, height, BufferedImage.TYPE_INT_ARGB);
    backImage = new BufferedImage(width, height, BufferedImage.TYPE_INT_ARGB);
    
    backGraphics = backImage.createGraphics();
  }
  
  @Override
  public synchronized void paint(Graphics g) {
    g.drawImage(image, 0, 0, null);
  }
  
  public Graphics2D getGraphicsToDraw() {
    return backGraphics;
  }
  
  public synchronized void flipScreen() {
    BufferedImage tempImage = image;
    image = backImage;
    backImage = tempImage;
    backGraphics = backImage.createGraphics();
    repaint();
  }
  
  private BufferedImage image, backImage;
  private Graphics2D backGraphics;

  private static final long serialVersionUID = -6847717957700865417L;
}
