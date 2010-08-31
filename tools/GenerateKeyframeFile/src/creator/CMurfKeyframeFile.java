package creator;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.Collections;
import java.util.Iterator;
import java.util.Map;
import java.util.Vector;

import keyframe.JointInfo;
import keyframe.JointInfoComparator;
import keyframe.KeyframeInfo;
import keyframe.KeyframeInfoComparator;

public class CMurfKeyframeFile {
    private Vector<KeyframeInfo> keyframes;
    private Vector<JointInfo> currentJoints;
    private Vector<String> legalJoints;

	
	public CMurfKeyframeFile(String inputFileName, String outputFileName) throws Exception {
	    keyframes = new Vector<KeyframeInfo>();
	    currentJoints = new Vector<JointInfo>();
	    legalJoints = new Vector<String>();
	    readLegalJoints();
	    parseFile(inputFileName);
	    createOutput(outputFileName);
//	    System.out.println(currentJoints.toString());
	}
	
	private void readLegalJoints() throws IOException{
	    BufferedReader input = new BufferedReader(new FileReader(new File("JointAngles.txt")));
	    String line = null;
	    while ((line = input.readLine()) != null){
	        if (line.indexOf("#") != 0){
	            legalJoints.add(line.trim());
//	            System.out.println(line);
	        }
	    }
	    input.close();
	}
	
	private void parseFile(String inputFileName) throws IOException {
		BufferedReader input = new BufferedReader(new FileReader(new File(inputFileName)));
		String line = null;
		String jointName = null;
		String time = null;
		String jointAngle = null;
		int startIndex = -1;
		int endIndex = -1;
		
		while ((line = input.readLine()) != null){
			// Line containing joint name
		    if (line.startsWith("names[") && line.contains("] = ") && line.endsWith("\";")) {
                startIndex = line.indexOf("] = ") + 5;
                endIndex   = line.indexOf("\";", startIndex);
                jointName = line.substring(startIndex, endIndex);
//                System.out.println("Joint Name = " + jointName);
			}

            // Line containing time
            if (line.startsWith("times[") && line.contains("] = ") &&line.endsWith(";")) {
                startIndex = line.indexOf("] = ") + 4;
                endIndex   = line.indexOf(";", startIndex);
                time = line.substring(startIndex, endIndex);
//                System.out.println("Time = " + time);
            }
		    // Line containing an angle
            if (line.startsWith("keys[") && line.contains("] = AL::ALValue::array(")) {
                startIndex = line.indexOf("] = AL::ALValue::array(") + 23;
                endIndex = line.indexOf(");", startIndex);
                jointAngle = line.substring(startIndex, endIndex);
                jointAngle = jointAngle.replaceAll("0,", "0.0,");
                jointAngle = jointAngle.replaceAll("0\\)", "0.0)");
//                System.out.println("Joint Angle = " + jointAngle);
            }		    
            
            if ((jointName!=null) && (time!=null) && (jointAngle!=null)){
                insertKeyframe(Float.parseFloat(time), jointName, jointAngle);
                time = null;
                jointAngle = null;
            }
		}		
		input.close();
	}

    private void insertKeyframe(float time, String jointName, String jointAngle){        
        if (! legalJoints.contains(jointName)) {            
//            System.out.println("Throw away joint "+jointName);
            return;
        } else { //legal joint
            int indexOfJoint = legalJoints.indexOf(jointName);
            JointInfo tempJoint = new JointInfo(indexOfJoint, jointName);
            if (! currentJoints.contains(tempJoint)){ //joint hasn't been created
                currentJoints.add(tempJoint);
            } else { //increase number of keyframes if joint has been created
                int index = currentJoints.indexOf(tempJoint);
                int numKeyframes = currentJoints.get(index).getNumKeyframes() + 1;
                currentJoints.get(index).setNumKeyframes(numKeyframes);
            }
        }
        for (KeyframeInfo keyframe : keyframes) {
            if (keyframe.getTime() == time){
                if (!keyframe.insertJointNameAngle(jointName, jointAngle)) {
                    System.err.println(jointName + " already exists for keyframe time " + time);
                }
//                System.out.println("Time exists for "+ time + " Insert jointAngle=" + jointAngle + " for " + jointName);
                return;
            }            
        }    

        //if keyframe does not exist
        KeyframeInfo newKeyframe = new KeyframeInfo(time);
        if (!newKeyframe.insertJointNameAngle(jointName, jointAngle)){
            System.err.println(jointName + " already exists for keyframe time " + time);
        }
//        System.out.println("Insert jointAngle=" + jointAngle + " for " + jointName + " at time=" + time);
        keyframes.add(newKeyframe);
    }

    private void sortKeyframes() {
        KeyframeInfoComparator comparator = new KeyframeInfoComparator();
        Collections.sort(keyframes, comparator);
    }
    
    private void sortCurrentJoints(){
        JointInfoComparator comparator = new JointInfoComparator();
        Collections.sort(currentJoints, comparator);
    }
    
	private void createOutput(String outputFileName) throws IOException {
        //Create output file
        PrintWriter out = new PrintWriter(new FileWriter(outputFileName));
        sortKeyframes();
        sortCurrentJoints();
        int arraySize = currentJoints.size();
        out.printf("//Total time=%.1f\n", keyframes.get(keyframes.size()-1).getTime());
        out.println("names.arraySetSize(" + arraySize + ");");
        out.println("times.arraySetSize(" + arraySize + ");");
        out.println("keys.arraySetSize(" + arraySize + ");");
        out.println();

        //Print all the joint names in order
        for (JointInfo joint: currentJoints){
            out.println("names[" + joint.getIndex() + "] = \"" + joint.getJointName() + "\";");
            out.println("times[" + joint.getIndex() + "].arraySetSize(" + joint.getNumKeyframes() + ");");
            out.println("keys[" + joint.getIndex() + "].arraySetSize(" + joint.getNumKeyframes() + ");");
        }
        out.println();
        
        out.println("float duration;");
        float currentTime = 0;
        float previousTime = 0;
        float duration = 0;

        Map<String, String> tempjointNameAngles;
        KeyframeInfo currentKeyframe;
        float tempJointAngle;
        String temp = null;
        
        //iterate through each keyframe
        for (int i=0; i<keyframes.size(); i++){
            currentKeyframe = keyframes.get(i);
            currentTime = currentKeyframe.getTime();                           
            duration = currentTime-previousTime;
            previousTime = currentTime;
            out.println();
            out.printf("duration = %.1f; //time=%.1f\n", duration, currentTime);
            tempjointNameAngles = currentKeyframe.getJointNameAngles();
            if (i == 0){
                for (JointInfo joint: currentJoints){ //loop through all possible legal joints to ensure it's correctly indexed
                    if (tempjointNameAngles.containsKey(joint.getJointName())){
                        out.printf("times[%d][%d] = duration; //%s\n", joint.getIndex(), i, joint.getJointName());
//                        out.printf("times[%d][%d] = %.1f; //%s\n", joint.getIndex(), i, currentTime, joint.getJointName());
                        out.printf("keys[%d][%d] = AL::ALValue::array(%s);\n", joint.getIndex(), i, tempjointNameAngles.get(joint.getJointName()));
                    }
                }
            } else{
                for (JointInfo joint: currentJoints){ //loop through all possible legal joints to ensure it's correctly indexed                                        
                    if (tempjointNameAngles.containsKey(joint.getJointName())){
                        int previousKeyframeIndexForJoint = i-1;                        
                        float currentKeyframeTime = currentTime;
                        float previousKeyframeTime = 0;
                        int currentJointIndex = 0;
                        int keyframeIndexForJoint = 0;
                        while ( (previousKeyframeIndexForJoint >= 0)){
                            if (keyframes.get(previousKeyframeIndexForJoint).getJointNameAngles().containsKey(joint.getJointName()) ){
                                currentJointIndex++;
                                if (previousKeyframeTime == 0){
                                    previousKeyframeTime = keyframes.get(previousKeyframeIndexForJoint).getTime();
                                    keyframeIndexForJoint = currentJointIndex;
                                }
                            }
                            previousKeyframeIndexForJoint--;
                        }
                        float tempDuration = currentKeyframeTime-previousKeyframeTime;
//                        out.printf("times[%d][%d] = %.1f; //%s\n", joint.getIndex(), currentJointIndex, currentTime, joint.getJointName());
                        if (tempDuration != duration){
                            out.printf("times[%d][%d] = (float)times[%d][%d] + %.1f; //%s\n", joint.getIndex(), currentJointIndex, joint.getIndex(), currentJointIndex-keyframeIndexForJoint, tempDuration, joint.getJointName());                            
                        } else{
                            out.printf("times[%d][%d] = (float)times[%d][%d] + duration; //%s\n", joint.getIndex(), currentJointIndex, joint.getIndex(), currentJointIndex-keyframeIndexForJoint, joint.getJointName());                            
                        }
                        out.printf("keys[%d][%d] = AL::ALValue::array(%s);\n", joint.getIndex(), currentJointIndex, tempjointNameAngles.get(joint.getJointName()));
                    }
                }
            }
        }
        out.flush();
        out.close();
	}
}
