package config;

import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.Properties;



public class ReadCheckBoxConfig {
	public static Properties readPropFile() throws IOException{
		Properties properties = new Properties();
//			System.out.println(fileName);
		properties.load(new FileInputStream(fileName));
			
		return properties;		
	}
	
	public static void writePropFile(Properties properties) throws IOException{
		properties.store(new FileOutputStream(fileName), null);
	}
	
	public static String getCMurfsDir() {
    // Check that CMURFS_DIR is valid
    if (System.getenv("CMURFS_DIR") == null) {
      System.err.println("Cannot find CMURFS_DIR environment variable, assuming ../../");
      return "../../";
    }
    
    return System.getenv("CMURFS_DIR");
	}

	private static String fileName = getCMurfsDir() + System.getProperty("file.separator") 
								    + "tools" + System.getProperty("file.separator")
								    + "Deployer" + System.getProperty("file.separator")
								    + "config" + System.getProperty("file.separator")
								    + "checkBoxProperties.properties";
}
