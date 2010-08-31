/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package robotInfo;

/**
 *
 * @author junyun
 */
public class RobotInfo implements Comparable{
    private String robotName;
    private String ipAddress;
    private String identityFileName;

    

    public RobotInfo(String robotName, String ipAddress, String identityFileName) {
        this.robotName = robotName;
        this.ipAddress = ipAddress;
        this.identityFileName = identityFileName;
    }

    @Override
    public boolean equals(Object obj) {
        if (obj == null) {
            return false;
        }
        if (getClass() != obj.getClass()) {
            return false;
        }
        final RobotInfo other = (RobotInfo) obj;
        if ((this.robotName == null) ? (other.robotName != null) : !this.robotName.equals(other.robotName)) {
            return false;
        }
        return true;
    }

    @Override
    public int hashCode() {
        int hash = 3;
        hash = 47 * hash + (this.robotName != null ? this.robotName.hashCode() : 0);
        return hash;
    }

    @Override
    public String toString() {
        return "name= " + robotName + " IP Address= " + ipAddress
             + " identity filename= " + identityFileName;
    }
    
    public int compareTo(Object anotherRobot) throws ClassCastException {
        if (!(anotherRobot instanceof RobotInfo)){
          throw new ClassCastException("A RobotInfo object expected.");
        }  
        String anotherRobotName = ((RobotInfo) anotherRobot).getRobotName();
        return this.robotName.compareTo(anotherRobotName);    
      }

    public String getIdentityFileName() {
        return identityFileName;
    }

    public void setIdentityFileName(String identityFileName) {
        this.identityFileName = identityFileName;
    }

    public String getIpAddress() {
        return ipAddress;
    }

    public void setIpAddress(String ipAddress) {
        this.ipAddress = ipAddress;
    }

    public String getRobotName() {
        return robotName;
    }

    public void setRobotName(String robotName) {
        this.robotName = robotName;
    }
}
