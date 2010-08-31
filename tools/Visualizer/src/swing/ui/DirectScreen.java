package swing.ui;

import java.awt.*;
import javax.swing.*;

public class DirectScreen extends JFrame {
  
  public DirectScreen(String title, int width, int height) {
    this(title, width, height, 0, 0);
    setLocationRelativeTo(null);
  }
  
  public DirectScreen(String title, int width, int height, int posX, int posY) {
    super(title);
    
    setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);

    setSize(new Dimension(width + WIDTH_ADD, height + HEIGHT_ADD));
    
    canvas = new DirectCanvas(width, height);
    getContentPane().add(canvas);
    
    setLocation(posX, posY);
    
    this.setVisible(true);
  }
  
  public void flipScreen() {
    canvas.flipScreen();
  }
  
  public Graphics2D getGraphicsToDraw() {
    return canvas.getGraphicsToDraw();
  }
  
  private DirectCanvas canvas;

  private static final long serialVersionUID = -822508356043237506L;
  
  // Additional width and height to handle the window frame size
  private static final int WIDTH_ADD = 10;
  private static final int HEIGHT_ADD = 33;
}
