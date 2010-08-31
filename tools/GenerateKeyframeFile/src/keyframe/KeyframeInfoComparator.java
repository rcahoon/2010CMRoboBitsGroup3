package keyframe;

import java.util.Comparator;

public class KeyframeInfoComparator implements Comparator<KeyframeInfo>{
    @Override
    public int compare(KeyframeInfo keyframe1, KeyframeInfo keyframe2) {
        float time1 = keyframe1.getTime();
        float time2 = keyframe2.getTime();
        
        if (time1 > time2) {
            return +1;
        } else if (time1 < time2) {
            return -1;
        } else {
            return 0;
        }
    }
}
