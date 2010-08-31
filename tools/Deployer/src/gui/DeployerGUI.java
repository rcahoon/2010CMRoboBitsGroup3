package gui;

import java.awt.Color;
import java.awt.Component;
import java.awt.Dimension;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.event.MouseEvent;
import java.awt.event.WindowEvent;
import java.awt.event.WindowListener;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.PriorityQueue;
import java.util.Properties;
import java.util.StringTokenizer;
import java.util.Vector;


import javax.swing.*;
import javax.swing.text.BadLocationException;
import javax.swing.text.Style;
import javax.swing.text.StyleConstants;
import javax.swing.text.StyledDocument;

import layout.SpringUtilities;

import robotInfo.RobotInfo;
import robotInfo.RobotTeamInfo;

import config.ReadCheckBoxConfig;
import config.ReadDeployConfig;
import config.ReadRobotTeamSetupConfig;

public class DeployerGUI {
	public static void createAndShoWGUI() {
		// Create and set up window
		JFrame frame = new JFrame("CMurfs Deployer");
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		
		frame.setSize(windowWidth, windowHeight);
		frame.setResizable(false);
		
		//Add window listener
		frame.addWindowListener(new WindowListener(){
            public void windowClosed(WindowEvent arg0) {
                
            }
            public void windowActivated(WindowEvent arg0) {
                
            }
            public void windowClosing(WindowEvent arg0) {
            	try{
	                updateCheckBoxProperties();
	                ReadRobotTeamSetupConfig.setRobotTeamProperties(allRobotsTeamInfo);
	                ReadRobotTeamSetupConfig.writePropFile();
            	} catch (IOException ex) {
        			try{
        				resultDoc.insertString(resultDoc.getLength(), ex.toString()+"\n", errorStyle);
        			} catch (BadLocationException ble) {
        				System.err.println(ble.toString());
        			}
        		} catch (Exception ex) {
        			try{
        				resultDoc.insertString(resultDoc.getLength(), ex.toString()+"\n", errorStyle);
        			} catch (BadLocationException ble) {
        				System.err.println(ble.toString());
        			}        			
        		}
            }
            public void windowDeactivated(WindowEvent arg0) {
                
            }
            public void windowDeiconified(WindowEvent arg0) {
                
            }
            public void windowIconified(WindowEvent arg0) {
                
            }
            public void windowOpened(WindowEvent arg0) {
                
            }
		});
		
		try{
			checkBoxProperties = ReadCheckBoxConfig.readPropFile();
			allRobots = ReadDeployConfig.readConfigAndCreateRobots();
			ReadRobotTeamSetupConfig.readPropFile(); //Have to set the read properties first before setting it  in allRobotsTeamInfo
			allRobotsTeamInfo = ReadRobotTeamSetupConfig.readPropAndCreateRobotTeam(allRobots);
			ReadRobotTeamSetupConfig.writePropFile(); //Have to write new robot's team number and player number
			//Display the window and items
			initComponents(frame);
		} 
		catch (IOException ex) {
			try{
				resultDoc.insertString(resultDoc.getLength(), ex.toString()+"\n", errorStyle);
			} catch (BadLocationException ble) {
				System.err.println(ble.toString());
			}
		}
		
		frame.setVisible(true);
	}
	
	static void updateCheckBoxProperties() {
		Properties newCheckBoxProperties = new Properties();
		
		newCheckBoxProperties.setProperty("setUpSSHCheckBox", setUpSSHCheckBox.isSelected() ? "1" : "0");
		newCheckBoxProperties.setProperty("flashUSBStickCheckBox", flashUSBStickCheckBox.isSelected() ? "1" : "0");
		newCheckBoxProperties.setProperty("setUpUSBStickCheckBox", setUpUSBStickCheckBox.isSelected() ? "1" : "0");
		newCheckBoxProperties.setProperty("setUpCMurfsdCheckBox", setUpCMurfsdCheckBox.isSelected() ? "1" : "0");
		
		newCheckBoxProperties.setProperty("autoloadIniCheckBox", autoloadIniCheckBox.isSelected() ? "1" : "0");
		newCheckBoxProperties.setProperty("identityTextCheckBox", identityTextCheckBox.isSelected() ? "1" : "0");
		newCheckBoxProperties.setProperty("allConfigFilesCheckBox", allConfigFilesCheckBox.isSelected() ? "1" : "0");
		newCheckBoxProperties.setProperty("compileMainCheckBox", compileMainCheckBox.isSelected() ? "1" : "0");
		newCheckBoxProperties.setProperty("compileNaoqiCheckBox", compileNaoqiCheckBox.isSelected() ? "1" : "0");
		newCheckBoxProperties.setProperty("deployMainCheckBox", deployMainCheckBox.isSelected() ? "1" : "0");
		newCheckBoxProperties.setProperty("deployNaoqiCheckBox", deployNaoqiCheckBox.isSelected() ? "1" : "0");
		newCheckBoxProperties.setProperty("startCMurfsCheckBox", startCMurfsCheckBox.isSelected() ? "1" : "0");
		newCheckBoxProperties.setProperty("stopCMurfsCheckBox", stopCMurfsCheckBox.isSelected() ? "1" : "0");
		newCheckBoxProperties.setProperty("restartNaoqiCheckBox", restartNaoqiCheckBox.isSelected() ? "1" : "0");

		for (JCheckBox checkBox : allRobotsCheckBoxes) {
			newCheckBoxProperties.setProperty(checkBox.getText(), checkBox.isSelected() ? "1" : "0");
		}
		try{
			ReadCheckBoxConfig.writePropFile(newCheckBoxProperties);
		} 
		catch (IOException ex) {
			try{
				resultDoc.insertString(resultDoc.getLength(), ex.toString()+"\n", errorStyle);
			} catch (BadLocationException ble) {
				System.err.println(ble.toString());
			}
		}
	}
	
	static void initComponents(JFrame frame){
		mainPanel = new javax.swing.JPanel();
		
        tabbedPanel = new javax.swing.JTabbedPane();
        InitialSetupPane = new javax.swing.JPanel();
        setUpSSHCheckBox = new javax.swing.JCheckBox();
        flashUSBStickCheckBox = new javax.swing.JCheckBox();
        setUpUSBStickCheckBox = new javax.swing.JCheckBox();
        setUpCMurfsdCheckBox = new javax.swing.JCheckBox();
        executeInitialSetUpButton = new javax.swing.JButton();
        
        DeployPane = new javax.swing.JPanel();
        configurationsLabel = new javax.swing.JLabel();
        autoloadIniCheckBox = new javax.swing.JCheckBox();
        identityTextCheckBox = new javax.swing.JCheckBox();
        allConfigFilesCheckBox = new javax.swing.JCheckBox(); 
        compileLabel = new javax.swing.JLabel();
        compileMainCheckBox = new javax.swing.JCheckBox();
        compileNaoqiCheckBox = new javax.swing.JCheckBox();
        deployLabel = new javax.swing.JLabel();
        deployMainCheckBox = new javax.swing.JCheckBox();
        deployNaoqiCheckBox = new javax.swing.JCheckBox();
        runLabel = new javax.swing.JLabel();
        startCMurfsCheckBox = new javax.swing.JCheckBox();
        stopCMurfsCheckBox = new javax.swing.JCheckBox();
        restartNaoqiCheckBox = new javax.swing.JCheckBox();
        executeDeployButton = new javax.swing.JButton();
        
        TeamSetupScrollPane = new javax.swing.JScrollPane();
    	TeamSetupPane = new javax.swing.JPanel(new SpringLayout());
    	teamNumberLabel = new javax.swing.JLabel();
    	teamNumberTextField = new javax.swing.JTextField();
    	robotNameLabels = new Vector<JLabel>();
    	robotPlayerNumberTextField = new Vector<JTextField>();
    	executeSaveTeamSetUpButton = new javax.swing.JButton();
    	
        checkAllButton = new javax.swing.JButton();
        uncheckAllButton = new javax.swing.JButton();
        
        RobotsScrollPane = new javax.swing.JScrollPane();
        RobotsPane = new javax.swing.JPanel();
        robotsLabel = new javax.swing.JLabel();
        allRobotsCheckBoxes = new Vector<javax.swing.JCheckBox>();
        
        tabbedPanel.setMaximumSize(new java.awt.Dimension(choiceWidth, windowHeight));
        tabbedPanel.setMinimumSize(new java.awt.Dimension(choiceWidth, windowHeight));
        tabbedPanel.setPreferredSize(new java.awt.Dimension(choiceWidth, windowHeight));
        tabbedPanel.setName("tabbedPanel");
        
        InitialSetupPane.setMaximumSize(new java.awt.Dimension(choiceWidth, windowHeight));
        InitialSetupPane.setMinimumSize(new java.awt.Dimension(choiceWidth, windowHeight));
        InitialSetupPane.setPreferredSize(new java.awt.Dimension(choiceWidth, windowHeight));
        InitialSetupPane.setName("InitialSetUpPane");
        
        DeployPane.setMaximumSize(new java.awt.Dimension(choiceWidth, windowHeight));
        DeployPane.setMinimumSize(new java.awt.Dimension(choiceWidth, windowHeight));
        DeployPane.setPreferredSize(new java.awt.Dimension(choiceWidth, windowHeight));
        DeployPane.setName("DeployPane");            

        TeamSetupPane.setMaximumSize(new java.awt.Dimension(choiceWidth-spacing*8, windowHeight-spacing*12));
        TeamSetupPane.setMinimumSize(new java.awt.Dimension(choiceWidth-spacing*8, windowHeight-spacing*12));
        TeamSetupPane.setPreferredSize(new java.awt.Dimension(choiceWidth-spacing*8, windowHeight-spacing*12));
        TeamSetupPane.setName("TeamSetupPane"); 
        
        TeamSetupScrollPane.setMaximumSize(new java.awt.Dimension(choiceWidth, windowHeight));
        TeamSetupScrollPane.setMinimumSize(new java.awt.Dimension(choiceWidth, windowHeight));
        TeamSetupScrollPane.setPreferredSize(new java.awt.Dimension(choiceWidth, windowHeight));
        TeamSetupScrollPane.setName("TeamSetupPane"); 
        TeamSetupScrollPane.setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_AS_NEEDED);
        TeamSetupScrollPane.setHorizontalScrollBarPolicy(JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED);
        
        // Add the tabs
        tabbedPanel.addTab("Initial Setup", InitialSetupPane);
        tabbedPanel.addTab("Deploy", DeployPane);
        tabbedPanel.addTab("Team Setup", TeamSetupScrollPane);
        // Set as default tab
        tabbedPanel.setSelectedIndex(1);
        // Initial Setup Items
        String selectedValue;
        selectedValue = checkBoxProperties.getProperty("setUpSSHCheckBox");
        if (selectedValue != null) {
	        if (selectedValue.trim().equals("1")){
	        	setUpSSHCheckBox.setSelected(true);
	        }
        }
        setUpSSHCheckBox.setText("Setup SSH Connection (1 at a time)");
        setUpSSHCheckBox.setName("setUpSSHCheckBox");
        
        selectedValue = checkBoxProperties.getProperty("flashUSBStickCheckBox");
        if (selectedValue != null) {
	        if (selectedValue.trim().equals("1")){
	        	flashUSBStickCheckBox.setSelected(true);
	        }
        }
        flashUSBStickCheckBox.setText("Flash USB Stick");
        flashUSBStickCheckBox.setName("flashUSBStickCheckBox");
        
        selectedValue = checkBoxProperties.getProperty("setUpUSBStickCheckBox");
        if (selectedValue != null) {
	        if (selectedValue.trim().equals("1")){
	        	setUpUSBStickCheckBox.setSelected(true);
	        }
        }
        setUpUSBStickCheckBox.setText("Setup USB Stick");
        setUpUSBStickCheckBox.setName("setUpUSBStickCheckBox");
        
        selectedValue = checkBoxProperties.getProperty("setUpCMurfsdCheckBox");
        if (selectedValue != null) {
	        if (selectedValue.trim().equals("1")){
	        	setUpCMurfsdCheckBox.setSelected(true);
	        }
        }
        setUpCMurfsdCheckBox.setText("Setup CMurfs Daemon");
        setUpCMurfsdCheckBox.setName("setUpCMurfsdCheckBox");
        
        flashFileChooser = new javax.swing.JFileChooser();
        flashFilePath = null;
        executeInitialSetUpButton.setText("Execute");
        executeInitialSetUpButton.setName("executeInitialSetUpButton");
        executeInitialSetUpButton.addMouseListener(new java.awt.event.MouseAdapter() {
        	public void mouseClicked(java.awt.event.MouseEvent evt) {
        		executeInitialSetUpMouseClicked(evt);
        	}
        });
        
        javax.swing.GroupLayout InitialSetupPaneLayout = new javax.swing.GroupLayout(InitialSetupPane);
        InitialSetupPane.setLayout(InitialSetupPaneLayout);
        InitialSetupPaneLayout.setHorizontalGroup(
        		InitialSetupPaneLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
        		.addGroup(InitialSetupPaneLayout.createParallelGroup()
    				.addComponent(setUpSSHCheckBox)
        			.addComponent(flashUSBStickCheckBox)
        			.addComponent(setUpUSBStickCheckBox)
        			.addComponent(setUpCMurfsdCheckBox)
        			.addComponent(executeInitialSetUpButton))
        );
        InitialSetupPaneLayout.setVerticalGroup(
        		InitialSetupPaneLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
        		.addGroup(InitialSetupPaneLayout.createSequentialGroup()
        			.addComponent(setUpSSHCheckBox)
        			.addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED, 10, 10)
        			.addComponent(flashUSBStickCheckBox)
        			.addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED, 10, 10)
        			.addComponent(setUpUSBStickCheckBox)
        			.addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED, 10, 10)
        			.addComponent(setUpCMurfsdCheckBox)
        			.addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED, 10, 10)
        			.addComponent(executeInitialSetUpButton)
        		)
        );

        // Deploy Items
        configurationsLabel.setText("Configurations");
        configurationsLabel.setName("configurationsLabel");
        selectedValue = checkBoxProperties.getProperty("autoloadIniCheckBox");
        if (selectedValue != null) {
	        if (selectedValue.trim().equals("1")){
	        	autoloadIniCheckBox.setSelected(true);
	        }
        }
        autoloadIniCheckBox.setText("Autoload.ini & Autoload.sh");
        autoloadIniCheckBox.setName("autoloadIniCheckBox");
        selectedValue = checkBoxProperties.getProperty("identityTextCheckBox");
        if (selectedValue != null) {
	        if (selectedValue.trim().equals("1")){
	        	identityTextCheckBox.setSelected(true);
	        }
        }
        identityTextCheckBox.setText("Identity.txt");
        identityTextCheckBox.setName("identityTextCheckBox");
        selectedValue = checkBoxProperties.getProperty("allConfigFilesCheckBox");
        if (selectedValue != null) {
	        if (selectedValue.trim().equals("1")){
	        	allConfigFilesCheckBox.setSelected(true);
	        }
        }        
        allConfigFilesCheckBox.setText("All files in config directory");
        allConfigFilesCheckBox.setName("allConfigFilesCheckBox");
        
        compileLabel.setText("Compile");
        compileLabel.setName("compileLabel");
        selectedValue = checkBoxProperties.getProperty("compileMainCheckBox");
        if (selectedValue != null) {
	        if (selectedValue.trim().equals("1")){
	        	compileMainCheckBox.setSelected(true);
	        }
        } 
        compileMainCheckBox.setText("Make Main");
        compileMainCheckBox.setName("compileMainCheckBox");
        selectedValue = checkBoxProperties.getProperty("compileNaoqiCheckBox");
        if (selectedValue != null) {
	        if (selectedValue.trim().equals("1")){
	        	compileNaoqiCheckBox.setSelected(true);
	        }
        } 
        compileNaoqiCheckBox.setText("Make Naoqi");
        compileNaoqiCheckBox.setName("compileNaoqiCheckBox");
        
        deployLabel.setText("Deploy");
        deployLabel.setName("deployLabel");
        selectedValue = checkBoxProperties.getProperty("deployMainCheckBox");
        if (selectedValue != null) {
	        if (selectedValue.trim().equals("1")){
	        	deployMainCheckBox.setSelected(true);
	        }
        }
        deployMainCheckBox.setText("Copy Main to Robot");
        deployMainCheckBox.setName("deployMainCheckBox");
        selectedValue = checkBoxProperties.getProperty("deployNaoqiCheckBox");
        if (selectedValue != null) {
	        if (selectedValue.trim().equals("1")){
	        	deployNaoqiCheckBox.setSelected(true);
	        }
        }
        deployNaoqiCheckBox.setText("Copy Naoqi to Robot");
        deployNaoqiCheckBox.setName("deployNaoqiCheckBox");
        
        runLabel.setText("Run");
        runLabel.setName("runLabel");
        selectedValue = checkBoxProperties.getProperty("restartNaoqiCheckBox");
        if (selectedValue != null) {
	        if (selectedValue.trim().equals("1")){
	        	restartNaoqiCheckBox.setSelected(true);
	        }
        }
        restartNaoqiCheckBox.setText("Restart Naoqi");
        restartNaoqiCheckBox.setName("restartNaoqiCheckBox");
        executeDeployButton.setText("Execute");
        executeDeployButton.setName("executeDeployButton");
        executeDeployButton.addMouseListener(new java.awt.event.MouseAdapter() {
        	public void mouseClicked(java.awt.event.MouseEvent evt) {
        		executeDeployMouseClicked(evt);
        	}
        });
        selectedValue = checkBoxProperties.getProperty("startCMurfsCheckBox");
        if (selectedValue != null) {
	        if (selectedValue.trim().equals("1")){
	        	startCMurfsCheckBox.setSelected(true);
	        }
        }
        startCMurfsCheckBox.setText("Start CMurfs");
        startCMurfsCheckBox.setName("startCMurfsCheckBox");
        selectedValue = checkBoxProperties.getProperty("stopCMurfsCheckBox");
        if (selectedValue != null) {
	        if (selectedValue.trim().equals("1")){
	        	stopCMurfsCheckBox.setSelected(true);
	        }
        }
        stopCMurfsCheckBox.setText("Stop CMurfs");
        stopCMurfsCheckBox.setName("stopCMurfsCheckBox");

        checkAllButton.setText("Check All Checkboxes");
        checkAllButton.setName("checkAllButton");
        checkAllButton.setMinimumSize(new Dimension(210,25));
        checkAllButton.setMaximumSize(new Dimension(210,25));
        checkAllButton.setPreferredSize(new Dimension(210,25));
        checkAllButton.addMouseListener(new java.awt.event.MouseAdapter() {
        	public void mouseClicked(java.awt.event.MouseEvent evt) {
        		checkAllMouseClicked(evt);
        	}
        });
        uncheckAllButton.setText("Uncheck All Checkboxes");
        uncheckAllButton.setName("uncheckAllButton");
        uncheckAllButton.setMinimumSize(new Dimension(210,25));
        uncheckAllButton.setMaximumSize(new Dimension(210,25));
        uncheckAllButton.setPreferredSize(new Dimension(210,25));
        uncheckAllButton.addMouseListener(new java.awt.event.MouseAdapter() {
        	public void mouseClicked(java.awt.event.MouseEvent evt) {
        		uncheckAllMouseClicked(evt);
        	}
        });
        
        javax.swing.GroupLayout DeployPaneLayout = new javax.swing.GroupLayout(DeployPane);
        DeployPane.setLayout(DeployPaneLayout);
        DeployPaneLayout.setHorizontalGroup(
        		DeployPaneLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
        		.addGroup(DeployPaneLayout.createParallelGroup()
        			.addComponent(compileLabel)
        			.addComponent(compileMainCheckBox)
        			.addComponent(compileNaoqiCheckBox)
        			
        			.addComponent(configurationsLabel)
        			.addComponent(autoloadIniCheckBox)
        			.addComponent(allConfigFilesCheckBox)
        			.addComponent(identityTextCheckBox)
        			
        			.addComponent(deployLabel)
        			.addComponent(deployMainCheckBox)
        			.addComponent(deployNaoqiCheckBox)
        			        			
        			.addComponent(runLabel)
        			.addComponent(restartNaoqiCheckBox)
        			.addComponent(startCMurfsCheckBox)
        			.addComponent(stopCMurfsCheckBox)

        			.addComponent(executeDeployButton)
        			.addComponent(checkAllButton)
        			.addComponent(uncheckAllButton)        			
        		)
        );
        DeployPaneLayout.setVerticalGroup(
        		DeployPaneLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
        		.addGroup(DeployPaneLayout.createSequentialGroup()        			
        			.addComponent(compileLabel)
        			.addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
        			.addComponent(compileMainCheckBox)
        			.addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
        			.addComponent(compileNaoqiCheckBox)
        			.addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED, 10, 10)
        			
        			.addComponent(configurationsLabel)	
        			.addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
        			.addComponent(autoloadIniCheckBox)
        			.addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
        			.addComponent(allConfigFilesCheckBox)
        			.addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
        			.addComponent(identityTextCheckBox)	
        			.addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED, 10, 10)
        			
        			.addComponent(deployLabel)
        			.addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
        			.addComponent(deployMainCheckBox)
        			.addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
        			.addComponent(deployNaoqiCheckBox)        			
        			.addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED, 10, 10)
        			
        			.addComponent(runLabel)
        			.addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
        			.addComponent(restartNaoqiCheckBox)
        			.addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
        			.addComponent(startCMurfsCheckBox)
        			.addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
        			.addComponent(stopCMurfsCheckBox)
        			.addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED, 10, 10)
        			
        			.addComponent(executeDeployButton)
        			.addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED, 10, 10)
        			.addComponent(checkAllButton)
        			.addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED, 10, 10)
        			.addComponent(uncheckAllButton)
        		)
        );
        
        // Team Setup Items       
    	teamNumberLabel.setText("Team Number:");
    	teamNumberLabel.setName("teamNumberLabel");
    	teamNumberTextField.setEditable(true);
    	teamNumberTextField.setText(getRobotTeamNumber());
    	teamNumberTextField.setName("teamNumberTextField");
        KeyListener textFieldChanged = new KeyListener() {
        	public void keyPressed(KeyEvent keyEvent) {
        		
        	}
        	
        	public void keyReleased(KeyEvent keyEvent) {
        		
        	}
        	@Override 
        	public void keyTyped(KeyEvent keyEvent) {
				tabbedPanel.setTitleAt(tabbedPanel.indexOfComponent(TeamSetupScrollPane), "*Team Setup");
        	}
		};
    	teamNumberTextField.addKeyListener(textFieldChanged);
    	
        // Add all components to TeamSetupPane
        teamNumberLabel.setMinimumSize(new Dimension(150,25));
        teamNumberLabel.setMaximumSize(new Dimension(150,25));
        teamNumberLabel.setPreferredSize(new Dimension(150,25));
        teamNumberTextField.setMinimumSize(new Dimension(50,25));
        teamNumberTextField.setMaximumSize(new Dimension(50,25));
        teamNumberTextField.setPreferredSize(new Dimension(50, 25));
        TeamSetupPane.add(teamNumberLabel);
        TeamSetupPane.add(teamNumberTextField);
        
    	JTextField tempTextField;
    	JLabel tempLabel;
    	String robotName;
    	for (RobotTeamInfo robotTeamInfo : allRobotsTeamInfo) {
    		robotName = robotTeamInfo.getRobotName();
    		tempLabel = new JLabel(robotName);
    		tempLabel.setMinimumSize(new Dimension(100,25));
    		tempLabel.setMaximumSize(new Dimension(100,25));
    		tempLabel.setPreferredSize(new Dimension(100,25));
    		robotNameLabels.add(tempLabel);
    		tempTextField = new JTextField(robotTeamInfo.getPlayerNumber());
    		tempTextField.setName(robotTeamInfo.getRobotName());
    		tempTextField.addKeyListener(textFieldChanged);
    		tempTextField.setMinimumSize(new Dimension(50,25));
    		tempTextField.setMaximumSize(new Dimension(50,25));
    		tempTextField.setPreferredSize(new Dimension(50,25));
    		tempTextField.setEditable(true);
    		robotPlayerNumberTextField.add(tempTextField);
    	}

        for (int i=0; i<allRobotsTeamInfo.size(); i++) {
        	tempLabel = robotNameLabels.get(i);
        	tempTextField = robotPlayerNumberTextField.get(i);
        	TeamSetupPane.add(tempLabel);
        	TeamSetupPane.add(tempTextField);;
        }

        executeSaveTeamSetUpButton.setText("Save Team Setup");
        executeSaveTeamSetUpButton.setName("executeSaveTeamSetUpButton");
        executeSaveTeamSetUpButton.setMinimumSize(new Dimension(160,25));
        executeSaveTeamSetUpButton.setMaximumSize(new Dimension(160,25));
        executeSaveTeamSetUpButton.setPreferredSize(new Dimension(160,25));
        executeSaveTeamSetUpButton.addMouseListener(new java.awt.event.MouseAdapter() {
        	public void mouseClicked(java.awt.event.MouseEvent evt) {
        		executeSaveTeamSetUpButtonMouseClicked(evt);
        	}
        });
        TeamSetupPane.add(executeSaveTeamSetUpButton);
        TeamSetupPane.add(new JLabel());
        SpringUtilities.makeCompactGrid(TeamSetupPane, allRobotsTeamInfo.size()+2, 2, spacing, spacing, spacing, spacing);
        TeamSetupScrollPane.setViewportView(TeamSetupPane);
       
        
        // Robots Items;
        RobotsPane.setName("RobotsPane");
        robotsLabel.setText("Robots");
        robotsLabel.setName("robotsLabel");
        RobotsScrollPane.setName("RobotsScrollPane");
        RobotsScrollPane.setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_AS_NEEDED);
        RobotsScrollPane.setHorizontalScrollBarPolicy(JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED);
        RobotsPane.setMaximumSize(new Dimension(windowWidth-choiceWidth-spacing*4, windowHeight-spacing*8));
        RobotsPane.setMinimumSize(new Dimension(windowWidth-choiceWidth-spacing*4, windowHeight-spacing*8));
        RobotsPane.setPreferredSize(new Dimension(windowWidth-choiceWidth-spacing*4, windowHeight-spacing*8));
        RobotsScrollPane.setMaximumSize(new Dimension(windowWidth-choiceWidth-spacing*3, windowHeight-spacing*6));
        RobotsScrollPane.setMinimumSize(new Dimension(windowWidth-choiceWidth-spacing*3, windowHeight-spacing*6));
        RobotsScrollPane.setPreferredSize(new Dimension(windowWidth-choiceWidth-spacing*3, windowHeight-spacing*6));
        RobotsPane.add(Box.createVerticalStrut(spacing));
        robotsLabel.setAlignmentX(Component.LEFT_ALIGNMENT);
        RobotsPane.add(robotsLabel);
        boolean selected;
        for (RobotInfo robot: allRobots) {
        	robotName = robot.getRobotName().trim();
        	if ( (robotName != null) && (!robotName.isEmpty()) ){
        		selectedValue = checkBoxProperties.getProperty(robotName);
        		if (selectedValue != null) {
        			selected = selectedValue.trim().equals("1");
        		} else {
        			selected = false;
        		}
        		allRobotsCheckBoxes.add(new JCheckBox(robotName,selected));
        	}
        }   
        for (JCheckBox checkBox : allRobotsCheckBoxes) {
        	checkBox.setAlignmentX(Component.LEFT_ALIGNMENT);
        	RobotsPane.add(checkBox);
        }
        checkAllRobotsButton = new javax.swing.JButton();
        checkAllRobotsButton.setText("Check All Robots");
        checkAllRobotsButton.setName("checkAllRobotsButton");
        checkAllRobotsButton.setMinimumSize(new Dimension(170,25));
        checkAllRobotsButton.setMaximumSize(new Dimension(170,25));
        checkAllRobotsButton.setPreferredSize(new Dimension(170,25));
        checkAllRobotsButton.addMouseListener(new java.awt.event.MouseAdapter() {
        	public void mouseClicked(java.awt.event.MouseEvent evt) {
        		checkAllRobotsMouseClicked(evt);
        	}
        });
        RobotsPane.add(checkAllRobotsButton);
        uncheckAllRobotsButton = new javax.swing.JButton();
        uncheckAllRobotsButton.setText("Uncheck All Robots");
        uncheckAllRobotsButton.setName("uncheckAllRobotsButton");
        uncheckAllRobotsButton.setMinimumSize(new Dimension(170,25));
        uncheckAllRobotsButton.setMaximumSize(new Dimension(170,25));
        uncheckAllRobotsButton.setPreferredSize(new Dimension(170,25));
        uncheckAllRobotsButton.addMouseListener(new java.awt.event.MouseAdapter() {
        	public void mouseClicked(java.awt.event.MouseEvent evt) {
        		unCheckAllRobotsMouseClicked(evt);
        	}
        });     
        RobotsPane.add(Box.createVerticalStrut(spacing));
        RobotsPane.add(uncheckAllRobotsButton);
        
        outputResults = new javax.swing.JTextPane();
        outputResults.setEditable(false);
        
        resultDoc = outputResults.getStyledDocument();
        outputResults.setStyledDocument(resultDoc);
        outputResults.setMaximumSize(new Dimension(outputResultsWidth-spacing*4, outputResultsHeight-spacing*8));
        outputResults.setMinimumSize(new Dimension(outputResultsWidth-spacing*4, outputResultsHeight-spacing*8));
        outputResults.setPreferredSize(new Dimension(outputResultsWidth-spacing*4, outputResultsHeight-spacing*8));
        outputResults.setCaretPosition(resultDoc.getLength());
        errorStyle = resultDoc.addStyle("errorStyle", null);
        outputStyle = resultDoc.addStyle("outputStyle", null);
        StyleConstants.setBold(errorStyle, true);
        StyleConstants.setForeground(errorStyle, Color.RED);
        StyleConstants.setBold(outputStyle, false);
        StyleConstants.setForeground(outputStyle, Color.BLACK);
        
        RobotsPane.add(Box.createVerticalStrut(spacing));
        outputResultsScrollPane = new javax.swing.JScrollPane();
        outputResultsScrollPane.setName("outputResultsScrollPane");
        outputResultsScrollPane.setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_AS_NEEDED);
        outputResultsScrollPane.setHorizontalScrollBarPolicy(JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED);
        outputResultsScrollPane.setMaximumSize(new Dimension(outputResultsWidth-spacing*3, outputResultsHeight-spacing*8));
        outputResultsScrollPane.setMinimumSize(new Dimension(outputResultsWidth-spacing*3, outputResultsHeight-spacing*8));
        outputResultsScrollPane.setPreferredSize(new Dimension(outputResultsWidth-spacing*3, outputResultsHeight-spacing*8));
        outputResultsScrollPane.setViewportView(outputResults);
        outputResultsScrollPane.setAlignmentX(Component.LEFT_ALIGNMENT);
        RobotsPane.add(outputResultsScrollPane);
        
        BoxLayout robotsLayout = new BoxLayout(RobotsPane, BoxLayout.Y_AXIS);
        RobotsPane.setLayout(robotsLayout);
        RobotsScrollPane.setViewportView(RobotsPane);
        
        // Grouping everything together
        mainPanel.setMaximumSize(new java.awt.Dimension(windowWidth, windowHeight));
        mainPanel.setMinimumSize(new java.awt.Dimension(windowWidth, windowHeight));
        mainPanel.setPreferredSize(new java.awt.Dimension(windowWidth, windowHeight));
        mainPanel.add(tabbedPanel);
        mainPanel.add(Box.createRigidArea(new Dimension(spacing,0)));
        mainPanel.add(RobotsScrollPane);
        BoxLayout mainLayout = new BoxLayout(mainPanel, BoxLayout.X_AXIS);
        mainPanel.setLayout(mainLayout);
        
        frame.getContentPane().add(mainPanel);
        // Start a thread to print output
        new Thread(new Runnable() {
        	public void run() {
        		compileOutput();
        	}
        }).start();
	}
	
	static void executeInitialSetUpMouseClicked(java.awt.event.MouseEvent evt) {
		try{
			String robotName;
			if (setUpSSHCheckBox.isSelected()) {
				boolean many = false;
				int numOfChecks = 0;
				for (JCheckBox checkBox : allRobotsCheckBoxes) {
					if (checkBox.isSelected()) {
						numOfChecks++;
					}
				}
				if (numOfChecks > 1) {
					many = true;
				}
				for (JCheckBox checkBox : allRobotsCheckBoxes) {
					if (checkBox.isSelected()) {
						robotName = checkBox.getText();
						if  ( (robotName != null) && (!robotName.isEmpty()) ) {
							resultDoc.insertString(resultDoc.getLength(), "[IN PROGRESS] SSH Setup->" + robotName + "\n", outputStyle);
							new Thread(new ScriptRunner(false, setupSSH, getRobotIPAddress(robotName), 
					                   "[FAILED] SSH setup->" + robotName + " \n", "[SUCCESS] SSH setup->" + robotName + "\n")).start();
							if (many) {
								JOptionPane.showMessageDialog(setUpSSHCheckBox, "Only setting up SSH for " + robotName);
							}
							return;
						}
					}
				}
			}
			if (flashUSBStickCheckBox.isSelected()) {
				if (flashFilePath == null) {
					flashFileChooser.showOpenDialog(flashUSBStickCheckBox);
					flashFilePath = flashFileChooser.getSelectedFile().getAbsolutePath();
//					System.out.println(flashFilePath);
				}
				int answer = JOptionPane.showOptionDialog(flashUSBStickCheckBox, "Are you sure you want to flash the usb stick? Please ensure that no other thumb drives are plugged in.", "Warning for flashing USB Stick", JOptionPane.YES_NO_OPTION, JOptionPane.WARNING_MESSAGE, null, null, null);
				if (answer==JOptionPane.NO_OPTION) {
					return;
				} else {
					resultDoc.insertString(resultDoc.getLength(), "[IN PROGRESS] Flash USB\n", outputStyle);
					new Thread(new ScriptRunner(false, usbFlash, flashFilePath, 
			                   "[FAILED] Flash USB\n", "[SUCCESS] Flash USB\n")).start();
				}
			}
			if (setUpUSBStickCheckBox.isSelected()) {
				for (JCheckBox checkBox : allRobotsCheckBoxes) {
					if (checkBox.isSelected()) {
						robotName = checkBox.getText();
						if  ( (robotName != null) && (!robotName.isEmpty()) ) {
							resultDoc.insertString(resultDoc.getLength(), "[IN PROGRESS] USB Stick Setup->" + robotName + "\n", outputStyle);
							new Thread(new ScriptRunner(true, setupUSB, getRobotIPAddress(robotName), 
					                   "[FAILED] USB Stick Setup->" + robotName + " \n", "[SUCCESS] USB Stick Setup->" + robotName + "\n")).start();
						}
					}
				}
			}
			if (setUpCMurfsdCheckBox.isSelected()) {
				for (JCheckBox checkBox : allRobotsCheckBoxes) {
					if (checkBox.isSelected()) {
						robotName = checkBox.getText();
						if  ( (robotName != null) && (!robotName.isEmpty()) ) {
							resultDoc.insertString(resultDoc.getLength(), "[IN PROGRESS] CMurfs Daemon Setup->" + robotName + "\n", outputStyle);
							new Thread(new ScriptRunner(true, setupCMurfsDaemon, getRobotIPAddress(robotName), 
					                   "[FAILED] CMurfs Daemon setup->" + robotName + " \n", "[SUCCESS] CMurfs Daemon setup->" + robotName + "\n")).start();
							
						}
						JOptionPane.showMessageDialog(setUpCMurfsdCheckBox, "Please execute installCMurfsd.sh on the robot when CMurfs Daemon setup is completed. e.g.:  ./installCMurfsd.sh install");
					}
				}
			}
		} catch (BadLocationException ble) {
			System.err.println(ble.toString());
		}
		outputResults.setCaretPosition(resultDoc.getLength());
	}
	
	static void makeNaoqi() {
		
	}
	
	static void executeDeployMouseClicked(java.awt.event.MouseEvent evt) {
		try{
			if (compileMainCheckBox.isSelected()) {			
				resultDoc.insertString(resultDoc.getLength(), "[IN PROGRESS] Make Main\n", outputStyle);
				outputResults.revalidate();
				String parameters = compileMainParameters;
				String error = "[FAILED] Make Main\n";
				String success = "[SUCCESS] Make Main\n";
				compileMainFailure = false;
				compileMainThread = new Thread(new ScriptRunner(true, setupTeam, parameters, error, success));		
			}
			
			if (compileNaoqiCheckBox.isSelected()) {
				resultDoc.insertString(resultDoc.getLength(), "[IN PROGRESS] Make Naoqi\n", outputStyle);
				outputResults.revalidate();
				String parameters = compileNaoqiParameters;
				String error = "[FAILED] Make Naoqi\n";
				String success = "[SUCCESS] Make Naoqi\n";
				compileNaoqiFailure = false;
				compileNaoqiThread = new Thread(new ScriptRunner(true, setupTeam, parameters, error, success));
			}
			
			String robotName;
			for (JCheckBox checkBox : allRobotsCheckBoxes) {
				if (checkBox.isSelected()) {
					int numberOfFunctions = 9;
					boolean [] selectFunctions = new boolean[numberOfFunctions];
					for (int i=0; i<numberOfFunctions; i++) {
						selectFunctions[i] = false;
					}
					/*
			         * selectFunctions[0] : autoload.ini & autoload.sh
					 * selectFunctions[1] : all config files
					 * selectFunctions[2] : identity.txt 
					 * selectFunctions[3] : stopCMurfs
					 * selectFunctions[4] : stopNaoqi
					 * selectFunctions[5] : copyMain
					 * selectFunctions[6] : copyNaoqi
					 * selectFunctions[7] : restartNaoqi
					 * selectFunctions[8] : startCMurfs
					 * 
					 */
					robotName = checkBox.getText();					
					
					if  ( (robotName != null) && (!robotName.isEmpty()) ) {
						if (autoloadIniCheckBox.isSelected()) {
							selectFunctions[0] = true;
						}
						
						if (allConfigFilesCheckBox.isSelected()) {
							selectFunctions[1] = true;
						}
						
						if (identityTextCheckBox.isSelected()) {
							selectFunctions[2] = true;
						}
						
						if (deployMainCheckBox.isSelected()) {
							selectFunctions[3] = true;
							selectFunctions[5] = true;
						}
						
						if (deployNaoqiCheckBox.isSelected()) {
							selectFunctions[3] = true;
							selectFunctions[4] = true;
							selectFunctions[6] = true;							
						}
						
						if (restartNaoqiCheckBox.isSelected()) {
							selectFunctions[3] = true;
							selectFunctions[7] = true;
						}
						
						if (startCMurfsCheckBox.isSelected()) {
							selectFunctions[8] = true;
						}
						
						if (stopCMurfsCheckBox.isSelected()) {
							selectFunctions[3] = true;
						}
						
						if ( selectFunctions[4] || selectFunctions[7] ) {
							if (JOptionPane.showConfirmDialog(deployMainCheckBox, "Is " + robotName + " in a stable position?") != JOptionPane.YES_OPTION) {
								resultDoc.insertString(resultDoc.getLength(), "[CANCELLED] All operations->" + robotName + "\n", errorStyle);
								continue;
							}
						}
						String parameters = "";
						String error = null;
						String success = null;
						for (int i=0; i<numberOfFunctions; i++) {
							if (selectFunctions[i]){
								switch(i) {
									case 0: //autoload.ini & autoload.sh
										parameters = parameters.concat(" --copyAutoload");
										resultDoc.insertString(resultDoc.getLength(), "[IN PROGRESS] Autoload.ini & Autoload.sh->" + robotName + "\n", outputStyle);
										break;
									case 1: //all config files
										parameters = parameters.concat(" --copyConfig");
										resultDoc.insertString(resultDoc.getLength(), "[IN PROGRESS] Config files->" + robotName + "\n", outputStyle);
										break;
									case 2: //identity.txt 
										parameters = parameters.concat(" --copyIdentity --teamNum "
		                                         + getRobotTeamNumber() + " --playerNum " + getRobotPlayerNumber(robotName)
		                                         + " --robotIdentityFile " + getRobotIdentityFile(robotName));
										resultDoc.insertString(resultDoc.getLength(), "[IN PROGRESS] Identity.txt->" + robotName + "\n", outputStyle);
										break;									
									case 3: // stop cmurfs
										parameters = parameters.concat(" --stopCMurfs");
										resultDoc.insertString(resultDoc.getLength(), "[IN PROGRESS] Stop CMurfs->" + robotName + "\n", outputStyle);
										break;
									case 4: // stop naoqi
										parameters = parameters.concat(" --stopNaoqi");
										resultDoc.insertString(resultDoc.getLength(), "[IN PROGRESS] Stop Naoqi->" + robotName + "\n", outputStyle);
										break;
									case 5: // copy Main
										while (compileMainThread != null) {}
										if (compileMainFailure) {
											resultDoc.insertString(resultDoc.getLength(), "[CANCELLED ALL OPERATIONS] Compile Main failed\n", errorStyle);																	
										} else {
											parameters = parameters.concat(" --copyMain");									
											resultDoc.insertString(resultDoc.getLength(), "[IN PROGRESS] Copy Main->" + robotName + "\n", outputStyle);
										}										
										break;
									case 6: // copy Naoqi
										while (compileNaoqiThread != null) {}
										if (compileNaoqiFailure) {
											resultDoc.insertString(resultDoc.getLength(), "[CANCELLED ALL OPERATIONS] Compile Naoqi failed\n", errorStyle);																			
										} else {
											parameters = parameters.concat(" --copyNaoqi");	
											resultDoc.insertString(resultDoc.getLength(), "[IN PROGRESS] Copy Naoqi->" + robotName + "\n", outputStyle);
										}
										break;
									case 7: // restart Naoqi
										parameters = parameters.concat(" --restartNaoqi");
										resultDoc.insertString(resultDoc.getLength(), "[IN PROGRESS] Restart Naoqi->" + robotName + "\n", outputStyle);
										break;
									case 8: // start CMurfs
										parameters = parameters.concat(" --startCMurfs");
										resultDoc.insertString(resultDoc.getLength(), "[IN PROGRESS] Start CMurfs->" + robotName + "\n", outputStyle);
										break;
									default: // Invalid function
										resultDoc.insertString(resultDoc.getLength(), "Invalid function for " + robotName + "\n", errorStyle);
										break;
										
								}
							}
						}
						if (compileMainFailure || compileNaoqiFailure) {
							compileMainFailure = false;		
							compileNaoqiFailure = false;
							return;
						}
						parameters = parameters.concat(" --ip " + getRobotIPAddress(robotName));
//						System.out.println("parameters2="+parameters);
						error = "[FAILED] Operation(s) for " + robotName + "\n";
						success = "[SUCCESS] Operation(s) for " + robotName + "\n";
						new Thread(new ScriptRunner(true, setupTeam, parameters, error, success)).start();						
					}
				}
			}
			
		} catch (BadLocationException ble) {
			System.err.println(ble.toString());
		}
		outputResults.setCaretPosition(resultDoc.getLength());
	}

	static void checkAllMouseClicked(java.awt.event.MouseEvent evt) {
		autoloadIniCheckBox.setSelected(true);
		identityTextCheckBox.setSelected(true);
		allConfigFilesCheckBox.setSelected(true);
		compileMainCheckBox.setSelected(true);
		compileNaoqiCheckBox.setSelected(true);
		deployMainCheckBox.setSelected(true);
		deployNaoqiCheckBox.setSelected(true);
		restartNaoqiCheckBox.setSelected(true);
		startCMurfsCheckBox.setSelected(true);
		stopCMurfsCheckBox.setSelected(true);
	}
	
	static void uncheckAllMouseClicked(java.awt.event.MouseEvent evt) {
		autoloadIniCheckBox.setSelected(false);
		identityTextCheckBox.setSelected(false);
		allConfigFilesCheckBox.setSelected(false);
		compileMainCheckBox.setSelected(false);
		compileNaoqiCheckBox.setSelected(false);
		deployMainCheckBox.setSelected(false);
		deployNaoqiCheckBox.setSelected(false);
		restartNaoqiCheckBox.setSelected(false);	
		startCMurfsCheckBox.setSelected(false);
		stopCMurfsCheckBox.setSelected(false);
	}
	
	static void checkAllRobotsMouseClicked(java.awt.event.MouseEvent evt) {
		for (JCheckBox checkBox: allRobotsCheckBoxes){
			checkBox.setSelected(true);
		}
	}
	
	static void unCheckAllRobotsMouseClicked(java.awt.event.MouseEvent evt) {
		for (JCheckBox checkBox: allRobotsCheckBoxes){
			checkBox.setSelected(false);
		}		
	}
	
	static void executeSaveTeamSetUpButtonMouseClicked(MouseEvent evt) {
		//Save all information back to allRobotsTeamInfo 
		String teamNumber = teamNumberTextField.getText();
		String robotName;
//		System.out.println("Team Number="+teamNumber);
    	for (RobotTeamInfo robotTeamInfo : allRobotsTeamInfo) {
    		robotName = robotTeamInfo.getRobotName().trim();
    		robotTeamInfo.setTeamNumber(teamNumber.trim());
    		for (JTextField tempTextField:robotPlayerNumberTextField) {
    			if (robotName.equals(tempTextField.getName().trim())) {
    				robotTeamInfo.setPlayerNumber(tempTextField.getText().trim());
    				break;
    			}
    		}
//    		System.out.println(robotTeamInfo.toString());
    	}
		//Set title back to Team Setup
		tabbedPanel.setTitleAt(tabbedPanel.indexOfComponent(TeamSetupScrollPane), "Team Setup");
	}
	
	static int runPythonscript(String program, String longArgument){
		try{
			String line;
			List<String> arguments = new ArrayList<String>();
			arguments.add("python");
			arguments.add(scriptPath.concat(program));
			StringTokenizer st = new StringTokenizer(longArgument);
			while (st.hasMoreTokens()) {
				arguments.add(st.nextToken());
			}
			ProcessBuilder pb = new ProcessBuilder(arguments);
			System.out.println(scriptPath.concat(program) + " " + longArgument);
			Map<String, String> env = pb.environment();

			for (Map.Entry entry: System.getenv().entrySet()) {
//		        System.out.println(entry.getKey() + " / " +
//		          entry.getValue());
		        env.put(entry.getKey().toString(), entry.getValue().toString());
		    }
			Process p = pb.start();

			BufferedReader br = new BufferedReader(new InputStreamReader(p.getInputStream()));
			while ((line=br.readLine()) !=null){
				System.out.println(line);
//				resultDoc.insertString(resultDoc.getLength(), line+"\n", outputStyle);
				outputResults.setCaretPosition(resultDoc.getLength());
			}
	        BufferedReader brError = new BufferedReader(new InputStreamReader(p.getErrorStream())); 
	        while ((line=brError.readLine()) !=null){
	          resultDoc.insertString(resultDoc.getLength(), line+"\n", errorStyle);
	          outputResults.setCaretPosition(resultDoc.getLength());
	        }
			outputResults.setCaretPosition(resultDoc.getLength());
			return p.waitFor();
		} catch (Exception e){
			try{
				resultDoc.insertString(resultDoc.getLength(), e.toString()+"\n", errorStyle);
				outputResults.setCaretPosition(resultDoc.getLength());
			} catch (BadLocationException ble) {
				System.err.println(ble.toString());
			}
		}
		return -1;
	}
	
	static int runScript(String program, String argument){
		try{
			String line;
			if (program.equals(usbFlash)) {
				JLabel label = new JLabel("Enter your sudo password:");
				JPasswordField jpf = new JPasswordField();
				jpf.requestFocus(true);
				int ans = JOptionPane.showConfirmDialog(null,
				  new Object[]{label, jpf}, "Password:",
				  JOptionPane.OK_OPTION);
				if (ans != JOptionPane.YES_OPTION) {
					resultDoc.insertString(resultDoc.getLength(), "No password supplied!\n", errorStyle);
					return -1;
				}
				String password = String.valueOf(jpf.getPassword());
				if ( (password == null) || password.isEmpty() ) {
					resultDoc.insertString(resultDoc.getLength(), "Null password supplied!\n", errorStyle);
					return -1;
				}
			
				ProcessBuilder pb = new ProcessBuilder(scriptPath.concat(program), argument);
				Map<String, String> env = pb.environment();
				
				for (Map.Entry entry: System.getenv().entrySet()) {
//			        System.out.println(entry.getKey() + " / " +
//			          entry.getValue());
			        env.put(entry.getKey().toString(), entry.getValue().toString());
			    }
				env.put("TERM", "xterm");
				Process p = pb.start();

				PrintWriter writer = new PrintWriter(new OutputStreamWriter(p.getOutputStream()));
				writer.write(password + "\n");
				writer.flush();
				writer.close();

				BufferedReader br = new BufferedReader(new InputStreamReader(p.getInputStream()));
				while ((line=br.readLine()) !=null){
					System.out.println(line);
//					resultDoc.insertString(resultDoc.getLength(), line+"\n", outputStyle);
					outputResults.setCaretPosition(resultDoc.getLength());
				}
		        BufferedReader brError = new BufferedReader(new InputStreamReader(p.getErrorStream())); 
		        while ((line=brError.readLine()) !=null){
		          resultDoc.insertString(resultDoc.getLength(), line+"\n", errorStyle);
		          outputResults.setCaretPosition(resultDoc.getLength());
		        }
				outputResults.setCaretPosition(resultDoc.getLength());
				return p.waitFor();
			} else if (program.equals(setupSSH)) {
				List<String> arguments = new ArrayList<String>();
				arguments.add("python");
				arguments.add(scriptPath.concat(program));
				StringTokenizer st = new StringTokenizer(argument);
				while (st.hasMoreTokens()) {
					arguments.add(st.nextToken());
				}
				ProcessBuilder pb = new ProcessBuilder(arguments);
				Map<String, String> env = pb.environment();
				
				for (Map.Entry entry: System.getenv().entrySet()) {
//			        System.out.println(entry.getKey() + " / " +
//			          entry.getValue());
			        env.put(entry.getKey().toString(), entry.getValue().toString());
			    }
				Process p = pb.start();

				PrintWriter writer = new PrintWriter(new OutputStreamWriter(p.getOutputStream()));
				writer.write("\n");
				writer.flush();
				
				BufferedReader br = new BufferedReader(new InputStreamReader(p.getInputStream()));
				while ((line=br.readLine()) !=null){
					System.out.println(line);
//					resultDoc.insertString(resultDoc.getLength(), line+"\n", outputStyle);
					outputResults.setCaretPosition(resultDoc.getLength());
				}
		        BufferedReader brError = new BufferedReader(new InputStreamReader(p.getErrorStream())); 
		        while ((line=brError.readLine()) !=null){
		          resultDoc.insertString(resultDoc.getLength(), line+"\n", errorStyle);
		          outputResults.setCaretPosition(resultDoc.getLength());
		        }
				outputResults.setCaretPosition(resultDoc.getLength());
				return p.waitFor();
			}
			else {
				ProcessBuilder pb = new ProcessBuilder(scriptPath.concat(program), argument);
				Map<String, String> env = pb.environment();
				
				for (Map.Entry entry: System.getenv().entrySet()) {
//			        System.out.println(entry.getKey() + " / " +
//			          entry.getValue());
			        env.put(entry.getKey().toString(), entry.getValue().toString());
			    }
				Process p = pb.start();

				BufferedReader br = new BufferedReader(new InputStreamReader(p.getInputStream()));
				while ((line=br.readLine()) !=null){
					System.out.println(line);
//					resultDoc.insertString(resultDoc.getLength(), line+"\n", outputStyle);
					outputResults.setCaretPosition(resultDoc.getLength());
				}
		        BufferedReader brError = new BufferedReader(new InputStreamReader(p.getErrorStream())); 
		        while ((line=brError.readLine()) !=null){
		          resultDoc.insertString(resultDoc.getLength(), line+"\n", errorStyle);
		          outputResults.setCaretPosition(resultDoc.getLength());
		        }
				outputResults.setCaretPosition(resultDoc.getLength());
				return p.waitFor();
			}
		} catch (Exception e){
			try{
				resultDoc.insertString(resultDoc.getLength(), e.toString()+"\n", errorStyle);
				outputResults.setCaretPosition(resultDoc.getLength());
			} catch (BadLocationException ble) {
				System.err.println(ble.toString());
			}
		}
		return -1;
	}
	
	static void compileOutput() {
		BufferedReader br;
		String line = "";
		try{
			while (true) {
        		if (compileMainThread != null) {
        			if (compileMainThread.getState() == Thread.State.NEW) {
        				compileMainThread.run();
        				compileMainThread.join();
        				compileMainThread = null;
        			}
        		}
        		if (compileNaoqiThread != null) {
        			if (compileNaoqiThread.getState() == Thread.State.NEW) {
        				compileNaoqiThread.run();
        				compileNaoqiThread.join();
        				compileNaoqiThread = null;
        			}
        		}
				Thread.sleep(100);
			}
		} catch (Exception e){
			try{
				resultDoc.insertString(resultDoc.getLength(), e.toString()+"\n", errorStyle);
				outputResults.setCaretPosition(resultDoc.getLength());
			} catch (BadLocationException ble) {
				System.err.println(ble.toString());
			}
		}
	}
	
	static String getRobotIPAddress(String robotName) {
		for (RobotInfo robot : allRobots) {
			if (robot.getRobotName().equals(robotName)) {
				return robot.getIpAddress();
			}
		}
		return "";
	}
	
	static String getRobotPlayerNumber(String robotName) {
		for (RobotTeamInfo robot : allRobotsTeamInfo) {
			if (robot.getRobotName().equals(robotName)) {
				return robot.getPlayerNumber();
			}
		}
		return "";
	}

	static String getRobotTeamNumber() {
		RobotTeamInfo firstRobot = allRobotsTeamInfo.peek();
		if (firstRobot!=null) {
			return firstRobot.getTeamNumber();
		}
		return "";
	}
	
	static String getRobotIdentityFile(String robotName) {
		for (RobotInfo robot : allRobots) {
			if (robot.getRobotName().equals(robotName)) {
				return robot.getIdentityFileName();
			}
		}
		return "";
	}
	
	static javax.swing.JPanel mainPanel;
	static Properties checkBoxProperties;
	
	static javax.swing.JTabbedPane tabbedPanel;
	static javax.swing.JPanel InitialSetupPane;
	static javax.swing.JCheckBox setUpSSHCheckBox;
	static javax.swing.JCheckBox flashUSBStickCheckBox;
	static javax.swing.JCheckBox setUpUSBStickCheckBox;
	static javax.swing.JCheckBox setUpCMurfsdCheckBox;
	static javax.swing.JButton executeInitialSetUpButton;
	static javax.swing.JFileChooser flashFileChooser;
	static String flashFilePath;
	
	static javax.swing.JPanel DeployPane;
	static javax.swing.JLabel configurationsLabel;
	static javax.swing.JCheckBox autoloadIniCheckBox;
	static javax.swing.JCheckBox identityTextCheckBox;
	static javax.swing.JCheckBox allConfigFilesCheckBox;
	static javax.swing.JLabel compileLabel;
	static javax.swing.JCheckBox compileMainCheckBox;
    static javax.swing.JCheckBox compileNaoqiCheckBox;
    static Thread compileMainThread = null;
	static Thread compileNaoqiThread = null;
	static boolean compileMainFailure = false;
	static boolean compileNaoqiFailure = false;
    static javax.swing.JLabel deployLabel;
    static javax.swing.JCheckBox deployMainCheckBox;
    static javax.swing.JCheckBox deployNaoqiCheckBox;
    static javax.swing.JLabel runLabel;
    static javax.swing.JCheckBox startCMurfsCheckBox;
    static javax.swing.JCheckBox stopCMurfsCheckBox;
    static javax.swing.JCheckBox restartNaoqiCheckBox;
    static javax.swing.JButton executeDeployButton;
    static javax.swing.JButton checkAllButton;
    static javax.swing.JButton uncheckAllButton;
    
    static javax.swing.JScrollPane TeamSetupScrollPane;
	static javax.swing.JPanel TeamSetupPane;
	static javax.swing.JLabel teamNumberLabel;
	static javax.swing.JTextField teamNumberTextField;
	static Vector<JLabel> robotNameLabels;
	static Vector<JTextField> robotPlayerNumberTextField;
	static javax.swing.JButton executeSaveTeamSetUpButton;
	static PriorityQueue<RobotTeamInfo> allRobotsTeamInfo;
    
    static javax.swing.JScrollPane RobotsScrollPane;
    static javax.swing.JPanel RobotsPane;
    static javax.swing.JLabel robotsLabel;
    static PriorityQueue<RobotInfo> allRobots;
    static Vector<JCheckBox> allRobotsCheckBoxes;
    static javax.swing.JButton checkAllRobotsButton;
    static javax.swing.JButton uncheckAllRobotsButton;
    static javax.swing.JScrollPane outputResultsScrollPane;
    static javax.swing.JTextPane outputResults;
    static StyledDocument resultDoc;
    static Style errorStyle;
    static Style outputStyle;
    
    static final int windowWidth = 520;
    static final int windowHeight= 520;
    static final int choiceWidth = 300;
    static final int outputResultsWidth = 210;
    static final int outputResultsHeight = 330;
    static final int spacing = 5;
    
    //Script settings
	static final String scriptPath = config.ReadCheckBoxConfig.getCMurfsDir() + System.getProperty("file.separator") 
                                       + "bin" + System.getProperty("file.separator");
	static final String setupSSH = "sshsetup.py";
	static final String usbFlash = "flashUSB.sh";
	static final String setupTeam = "setupteam.py";
	static final String setupUSB = "setupUSB.py";
	static final String setupCMurfsDaemon = "setupCMurfsDaemon.py";
	static final String compileMainParameters = " --main";
	static final String compileNaoqiParameters = " --naoqi";
}

class ScriptRunner implements Runnable {
	ScriptRunner(boolean runPython, String script, String parameters, String error, String success) {
		this.runPython = runPython;
		this.script = script;
		this.parameters = parameters;
		this.error = error;
		this.success = success;
	}
	
	public void run() {
		int runResult = 0;
		if (runPython) {
			runResult = DeployerGUI.runPythonscript(script, parameters);
		}
		else {
			runResult = DeployerGUI.runScript(script, parameters);
		}
		if (runResult != 0) {
			try {
				if (parameters.equals(DeployerGUI.compileMainParameters)) {
					DeployerGUI.compileMainFailure = true;
				} else if (parameters.equals(DeployerGUI.compileNaoqiParameters)) {
					DeployerGUI.compileNaoqiFailure = true;
				}
				DeployerGUI.resultDoc.insertString(DeployerGUI.resultDoc.getLength(), error, DeployerGUI.errorStyle);
				DeployerGUI.outputResults.setCaretPosition(DeployerGUI.resultDoc.getLength());
			} catch (BadLocationException ble) {
				System.err.println(ble.toString());
			}
		} else {
			try {
				DeployerGUI.resultDoc.insertString(DeployerGUI.resultDoc.getLength(), success, DeployerGUI.outputStyle);
				DeployerGUI.outputResults.setCaretPosition(DeployerGUI.resultDoc.getLength());
			} catch (BadLocationException ble) {
				System.err.println(ble.toString());
			}
		}
		
	}

	boolean runPython;
	String script, parameters, error, success;
}

class MultipleScriptRunner implements Runnable {
	MultipleScriptRunner(Vector<Boolean> runPython, Vector<String> script, Vector<String> parameters, Vector<String> error, Vector<String> success) {
		this.runPython = runPython;
		this.script = script;
		this.parameters = parameters;
		this.error = error;
		this.success = success;
	}
	
	public void run() {
		for (int i=0; i<script.size(); i++) {
			String scriptName = script.get(i);
			String parametersString = parameters.get(i);
			String errorString = error.get(i);
			String successString = success.get(i);
			
			int runResult = 0;
			if (runPython.get(i)) {
				runResult = DeployerGUI.runPythonscript(scriptName, parametersString);
			}
			else {
				runResult = DeployerGUI.runScript(scriptName, parametersString);
			}
			if (runResult != 0) {
				try {
					DeployerGUI.resultDoc.insertString(DeployerGUI.resultDoc.getLength(), errorString, DeployerGUI.errorStyle);
					DeployerGUI.outputResults.setCaretPosition(DeployerGUI.resultDoc.getLength());
				} catch (BadLocationException ble) {
					System.err.println(ble.toString());
				}
			} else {
				try {
					DeployerGUI.resultDoc.insertString(DeployerGUI.resultDoc.getLength(), successString, DeployerGUI.outputStyle);
					DeployerGUI.outputResults.setCaretPosition(DeployerGUI.resultDoc.getLength());
				} catch (BadLocationException ble) {
					System.err.println(ble.toString());
				}
			}
		}
	}
	
	Vector<Boolean> runPython;
	Vector<String> script, parameters, error, success;
}