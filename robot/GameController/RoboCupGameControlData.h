typedef unsigned char  uint8;
typedef unsigned short uint16;
typedef unsigned int   uint32;

#define GAMECONTROLLER_PORT             3838

#define GAMECONTROLLER_STRUCT_VERSION   7

#define GAMECONTROLLER_STRUCT_HEADER    "RGme"

#define MAX_NUM_PLAYERS             11

#define TEAM_BLUE                   0
#define TEAM_CYAN                   0
#define TEAM_RED                    1
#define TEAM_MAGENTA                1

#define GOAL_BLUE                   0
#define GOAL_YELLOW                 1

#define STATE_INITIAL               0
#define STATE_READY                 1
#define STATE_SET                   2
#define STATE_PLAYING               3
#define STATE_FINISHED              4

#define STATE2_NORMAL               0
#define STATE2_PENALTYSHOOT         1

#define PENALTY_NONE                0

struct RobotInfo {
    uint16 penalty;             // penalty state of the player
    uint16 secsTillUnpenalised; // estimate of time till unpenalised
};

struct TeamInfo {
    uint8 teamNumber;          // unique team number
    uint8 teamColour;          // colour of the team
    uint8 goalColour;          // colour of the goal
    uint8 score;               // team's score
    RobotInfo players[MAX_NUM_PLAYERS];       // the team's players
};

struct RoboCupGameControlData {
    char   header[4];           // header to identify the structure
    uint32 version;             // version of the data structure
    uint8 playersPerTeam;       // The number of players on a team
    uint8 state;                // state of the game (STATE_READY, STATE_PLAYING, etc)
    uint8 firstHalf;            // 1 = game in first half, 0 otherwise
    uint8 kickOffTeam;          // the next team to kick off
    uint8 secondaryState;       // Extra state information - (STATE2_NORMAL, STATE2_PENALTYSHOOT, etc)
    uint8 dropInTeam;           // team that caused last drop in
    uint16 dropInTime;          // number of seconds passed since the last drop in.  -1 before first dropin
    uint32 secsRemaining;       // estimate of number of seconds remaining in the half
    TeamInfo teams[2];
};
