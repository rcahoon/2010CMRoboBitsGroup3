#ifndef FSM_H_
#define FSM_H_

#include <iostream>
#include <map>
#include <list>
#include <string.h>
#include <stdlib.h>

class Log;

using namespace std;

class FSM {
public:

  FSM(Log & log);
  ~FSM();

  void reset();
  void addState(int stateNum, string stateName);
  void finishInit(int start, unsigned long int time);
  void startLoop(unsigned long int time);
  void endLoop();
  bool isRunning();
  void trans(int new_state, string reason);
  int getState() const;
  bool inState(int s) const;
  void setState(int new_state, const string reason="setState");
  void setState(int new_state, const string reason, unsigned long timestamp);
  unsigned long int timeInState();
  void printTransitions(bool printError=false);
  bool isNewState();

  static const int FSM_EXCEPTION;

private:

  class Transition {
  public:
    int fromState;
    int toState;
    string reason;
  };

  Log & log;

  map<int, string> states;
  int curr_state;
  list<Transition> transitions;
  unsigned long int timeOfLastTrans;
  unsigned long int currTime;
  bool done_init;
  bool running;
  bool newToState;
  bool firstLoop;

};

#endif // FSM_H_

