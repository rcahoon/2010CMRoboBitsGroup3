package config;

import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.PriorityQueue;
import java.util.Properties;

import robotInfo.RobotInfo;
import robotInfo.RobotTeamInfo;

public class ReadRobotTeamSetupConfig {
	
	public static void readPropFile() throws IOException{	
//			System.out.println(fileName);
		allRobotsTeamInfoProperties.load(new FileInputStream(fileName));	
	}
	
	public static void writePropFile() throws IOException{
		allRobotsTeamInfoProperties.store(new FileOutputStream(fileName), null);
	}
	
	public static void setRobotTeamProperties(PriorityQueue<RobotTeamInfo> allRobotsTeamInfo) {
		String robotName;
		
		if ( (allRobotsTeamInfo != null) && (!allRobotsTeamInfo.isEmpty()) ){
			RobotTeamInfo firstRobot = allRobotsTeamInfo.peek();
			if (firstRobot!=null) {
				allRobotsTeamInfoProperties.setProperty("teamNumber", firstRobot.getTeamNumber());
			}
			
			for (RobotTeamInfo robotTeamInfo : allRobotsTeamInfo) {
				robotName = robotTeamInfo.getRobotName();
				allRobotsTeamInfoProperties.setProperty(robotName.trim().concat("PlayerNumber"), robotTeamInfo.getPlayerNumber());
			}
		}
	}
	
	public static PriorityQueue<RobotTeamInfo> readPropAndCreateRobotTeam(PriorityQueue<RobotInfo> allRobots) {
		PriorityQueue<RobotTeamInfo> robotTeamInfo = new PriorityQueue<RobotTeamInfo>();
		
		String robotName;
		String selectedValue;
		String teamNumber;
		String playerNumber;
		
		selectedValue = allRobotsTeamInfoProperties.getProperty("teamNumber");
        if (selectedValue != null) {
	        teamNumber = selectedValue.trim();
        } else {
        	//set team number
        	allRobotsTeamInfoProperties.setProperty("teamNumber", "0");
        	teamNumber = "0";
        }
        
		for (RobotInfo robotInfo: allRobots){
			robotName = robotInfo.getRobotName();
			if ( (robotName!=null) && (!robotName.isEmpty()) ) {
				selectedValue = allRobotsTeamInfoProperties.getProperty(robotName.trim().concat("PlayerNumber"));
		        if (selectedValue != null) {
			        playerNumber = selectedValue.trim();
		        } else {
		        	//set team number
		        	allRobotsTeamInfoProperties.setProperty(robotName.trim().concat("PlayerNumber"), "0");
		        	playerNumber = "0";
		        }
		        robotTeamInfo.add(new RobotTeamInfo(robotName, teamNumber, playerNumber));
			}
		}
        
		return robotTeamInfo;
	}
	
	public static Properties getPropertyFile() {
		return allRobotsTeamInfoProperties;
	}
	
	public static void setPropertyFile(Properties newProp) {
		allRobotsTeamInfoProperties = newProp;
	}
	
	private static Properties allRobotsTeamInfoProperties = new Properties();
	private static String fileName = ReadCheckBoxConfig.getCMurfsDir() + System.getProperty("file.separator") 
	                                 + "tools" + System.getProperty("file.separator")
	                                 + "Deployer" + System.getProperty("file.separator")
                                     + "config" + System.getProperty("file.separator")
                                     + "robotTeamSetup.properties";
}
