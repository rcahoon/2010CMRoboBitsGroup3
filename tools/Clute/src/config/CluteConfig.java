package config;

public class CluteConfig extends ConfigFile {

  private CluteConfig() throws Exception {
    super(getCMurfsDir() + "tools/Clute/config/", "clute.txt");
  }
  
  public static CluteConfig getInstance() {
    return instance;
  }
  
  private static String getCMurfsDir() {
    String CMurfsDir = System.getenv("CMURFS_DIR");
    // Check that CMURFS_DIR is valid
    if (CMurfsDir == null) {
      System.err.println("Cannot find CMURFS_DIR environment variable, assuming ../../");
      return "../../";
    }
    
    if (!CMurfsDir.endsWith("/")) {
      CMurfsDir += "/";
    }
    return CMurfsDir;
  }
  
  static {
    try {
      instance = new CluteConfig();
    }
    catch (Exception e) {
      System.out.println(e.toString());
    }
  }
  
  private static CluteConfig instance;
}
