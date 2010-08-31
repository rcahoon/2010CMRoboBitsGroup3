package keyframe;

public class JointInfo {
    private int index;
    private int numKeyframes;
    private String jointName;
    
    public JointInfo(int _index, String _jointName){
        index = _index;
        jointName = _jointName;
        numKeyframes = 1;
    }

    public int getIndex() {
        return index;
    }

    public void setIndex(int index) {
        this.index = index;
    }

    public int getNumKeyframes() {
        return numKeyframes;
    }

    public void setNumKeyframes(int numKeyframes) {
        this.numKeyframes = numKeyframes;
    }

    public String getJointName() {
        return jointName;
    }

    public void setJointName(String jointName) {
        this.jointName = jointName;
    }
    
    
    public boolean equals(JointInfo jointInfo){
        if (jointInfo.jointName.equals(this.jointName)){
            return true;
        }
        return false;
    }
    
    public boolean equals(Object object) {
        if (object instanceof JointInfo){
            if (((JointInfo) object).getJointName().equals(this.jointName)){
                return true;
            }
        }
        return false;
    }
    
    @Override
    public String toString(){
        return "index="+index+", jointName="+jointName+", numKeyframes="+numKeyframes;
    }
}
