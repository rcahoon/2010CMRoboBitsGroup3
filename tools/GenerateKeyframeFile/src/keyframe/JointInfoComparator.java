package keyframe;

import java.util.Comparator;

public class JointInfoComparator implements Comparator<JointInfo>{
    @Override
    public int compare(JointInfo jointInfo1, JointInfo jointInfo2) {
        int index1 = jointInfo1.getIndex();
        int index2 = jointInfo2.getIndex();
        
        if (index1 > index2) {
            return +1;
        } else if (index1 < index2) {
            return -1;
        } else {
            return 0;
        }
    }
}
