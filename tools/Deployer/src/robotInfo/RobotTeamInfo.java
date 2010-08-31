/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package robotInfo;

/**
 *
 * @author junyun
 */
public class RobotTeamInfo implements Comparable{
    private String robotName;
    private String teamNumber;
    private String playerNumber;

    

    public RobotTeamInfo(String robotName, String teamNumber, String playerNumber) {
        this.robotName = robotName;
        this.teamNumber = teamNumber;
        this.playerNumber = playerNumber;
    }

    @Override
    public boolean equals(Object obj) {
        if (obj == null) {
            return false;
        }
        if (getClass() != obj.getClass()) {
            return false;
        }
        final RobotTeamInfo other = (RobotTeamInfo) obj;
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
        return "Name= " + robotName + " Team Number= " + teamNumber
             + " Player Number= " + playerNumber;
    }
    
    public int compareTo(Object anotherRobot) throws ClassCastException {
        if (!(anotherRobot instanceof RobotTeamInfo)){
          throw new ClassCastException("A RobotInfo object expected.");
        }  
        String anotherRobotName = ((RobotTeamInfo) anotherRobot).getRobotName();
        return this.robotName.compareTo(anotherRobotName);    
      }

    public String getPlayerNumber() {
        return playerNumber;
    }

    public void setPlayerNumber(String playerNumber) {
        this.playerNumber = playerNumber;
    }

    public String getTeamNumber() {
        return teamNumber;
    }

    public void setTeamNumber(String teamNumber) {
        this.teamNumber = teamNumber;
    }

    public String getRobotName() {
        return robotName;
    }

    public void setRobotName(String robotName) {
        this.robotName = robotName;
    }
}
