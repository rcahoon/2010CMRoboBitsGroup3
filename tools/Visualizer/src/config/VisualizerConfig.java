package config;

public class VisualizerConfig extends ConfigFile {

  private VisualizerConfig() throws Exception {
    super(getCMurfsDir() + "/robot/config/", "visualizer.txt");
  }
  
  public static VisualizerConfig getInstance() {
    return instance;
  }
  
  private static String getCMurfsDir() {
    // Check that CMURFS_DIR is valid
    if (System.getenv("CMURFS_DIR") == null) {
      System.err.println("Cannot find CMURFS_DIR environment variable, assuming ../../");
      return "../../";
    }
    
    return System.getenv("CMURFS_DIR");
  }
  
  static {
    try {
      instance = new VisualizerConfig();
    }
    catch (Exception e) {
      System.out.println(e.toString());
    }
  }
  
  private static VisualizerConfig instance;
}
