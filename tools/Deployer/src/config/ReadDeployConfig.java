package config;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.PriorityQueue;
import java.util.StringTokenizer;

import robotInfo.RobotInfo;

public class ReadDeployConfig {
    public static PriorityQueue<RobotInfo> readConfigAndCreateRobots() throws IOException{
    	PriorityQueue<RobotInfo> robots = new PriorityQueue<RobotInfo>();
    	String filePath = ReadCheckBoxConfig.getCMurfsDir() + System.getProperty("file.separator") 
								        + "tools" + System.getProperty("file.separator")
								        + "Deployer" + System.getProperty("file.separator")
								        + "config" + System.getProperty("file.separator")
								        + "deployConfig.txt";

//        System.out.println(filePath);

        BufferedReader input = new BufferedReader(new FileReader(filePath));
        String line = null;
        int commentIndex;
        int lineNumber = 1;

        // Read deployConfig.txt
        while ((line = input.readLine()) != null){
            line = line.trim();
            commentIndex = line.indexOf("#");
//            System.out.println("commentIndex=" + commentIndex);
            if (commentIndex != -1) {
                line = line.substring(0, commentIndex);
            }
            if ((line != null) && (!line.isEmpty())) {
//	                System.out.println(line);
                StringTokenizer st = new StringTokenizer(line, " ");
                if (st.countTokens() != 3) {
                    System.err.println("Cannot read line " + lineNumber + ", not enough fields!");
                    continue;
                }
                robots.add(new RobotInfo(st.nextToken().trim(), st.nextToken().trim(),
                                        st.nextToken().trim()));
            }
            lineNumber++;
        }
        input.close();
      
//        System.out.println(robots.toString());
        return robots;
    }
}
