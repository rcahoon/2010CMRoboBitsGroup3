package keyframe;

import java.util.HashMap;
import java.util.Map;

public class KeyframeInfo {
	private float time;
	private Map<String, String> jointNameAngles;
	
	public KeyframeInfo(float _time){
		time = _time;
		jointNameAngles = new HashMap<String,String>();
	}

	public float getTime() {
		return time;
	}

	public void setTime(float time) {
		this.time = time;
	}

	public Map<String, String> getJointNameAngles() {
		return jointNameAngles;
	}

	public void setJointNameAngles(Map<String, String> jointNameAngles) {
		this.jointNameAngles = jointNameAngles;
	}
	
	public boolean insertJointNameAngle(String jointName, String angle){
		if (jointNameAngles.containsKey(jointName)){
			return false;
		} else{
			jointNameAngles.put(jointName, angle);
		}		
	    return true;
	}
	
	/*
	 * Assumes that angle for joint definitely exists, hence no check
	 */
	public String getAngleWithJointName(String jointName){
		return jointNameAngles.get(jointName);
	}
	
	public boolean containsJoint(String jointName){
	    return jointNameAngles.containsKey(jointName);
	}
	/*
	 * Returns the size of jointNameAngles
	 */
	public int getSizeOfJointNameAngles(){
	    return jointNameAngles.size();
	}
}
