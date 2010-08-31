#include "FSM.h"

// Define the logging constants
#define COMPONENT BEHAVIORS
//#define CLASS_LOG_LEVEL LOG_LEVEL_DEBUG
#include "Log/LogSettings.h"

using namespace std;

const int FSM::FSM_EXCEPTION(1);

//Constructs an FSM (states can now be added until finishInit is called)
FSM::FSM(Log & _log)
  : log(_log) {
  reset();
}

FSM::~FSM() {
}

void FSM::reset() {
  states.clear();
  transitions.clear();
  running = false;
  done_init = false;
}

//Adds a new state to the FSM (only allowed before finishInit has been called)
void FSM::addState(int stateNum, string stateName){
  if(done_init){
    LOG_ERROR("FSM ERROR: Adding a state after initialization is finished!");
    throw FSM_EXCEPTION;
  }
  if(states.find(stateNum) != states.end()){
    LOG_ERROR("FSM ERROR: Adding a state that already exists. Overwriting %s with %s", states[stateNum].c_str(), stateName.c_str());
    throw FSM_EXCEPTION;
  }
  states[stateNum] = stateName;
}

//Complete initialization (addState can no longer be called)
void FSM::finishInit(int start, unsigned long int time){
  curr_state = start;
  running = false;
  newToState = true;
  firstLoop = true;
  transitions.clear();
  timeOfLastTrans = time;
  currTime = time;
  if(states.find(curr_state) == states.end()){
    LOG_ERROR("FSM ERROR: Initial state provided is unknown (%d)", start);
    throw FSM_EXCEPTION;
  }
  done_init = true;
}

//Starts the FSM (now isRunning() is true)
void FSM::startLoop(unsigned long int time){
  if(!done_init){
    LOG_ERROR("FSM ERROR: startLoop called before initialization was finished (call finishInit)");
    throw FSM_EXCEPTION;
  }

  currTime = time;

  if(running){
    LOG_ERROR("FSM ERROR: startLoop called when loop was already running. Current state is: %s", states[curr_state].c_str());
    throw FSM_EXCEPTION;
  }
  running = true;
  if(firstLoop)
    firstLoop = false;
  else
    newToState = false;
  transitions.clear();
}

//Pauses the FSM (now isRunning() will be false)
void FSM::endLoop(){
  if(!running){
    LOG_ERROR("FSM ERROR: endLoop called when loop was not running. Current state is: %s", states[curr_state].c_str());
    throw FSM_EXCEPTION;
  }
  running = false;
}

//Returns in the the FSM is running (if startLoop has been called)
bool FSM::isRunning(){
  return running;
}

//Transition to a new state.  Be sure to provide a reason for later debugging
void FSM::trans(int new_state, string reason){
  if(!running){
    LOG_ERROR("FSM ERROR: Tried to trans while the FSM was not running. Current state is: %s", states[curr_state].c_str());
    throw FSM_EXCEPTION;
  }
  if(states.find(new_state) == states.end()){
    LOG_ERROR("FSM ERROR: Tried to trans to an unknown state (%s). Current state is: %s", states[new_state].c_str(), states[curr_state].c_str());
    throw FSM_EXCEPTION;
  }

  Transition temp;
  temp.fromState = curr_state;
  temp.toState = new_state;
  temp.reason = reason;

  list<Transition>::iterator iter;
  bool infiniteLoop = false;
  for(iter = transitions.begin(); iter != transitions.end(); iter++){
    if(iter->fromState == temp.fromState &&
       iter->toState == temp.toState &&
       iter->reason == temp.reason){
      infiniteLoop = true;
      break;
    }
  }

  transitions.insert(transitions.end(),temp);

  if(infiniteLoop){
    LOG_ERROR("FSM ERROR: Infinite loop detected: ");
    printTransitions(true);
    throw FSM_EXCEPTION;
  }

  curr_state = new_state;
  newToState = true;
  timeOfLastTrans = currTime;
}

//Returns the current state
int FSM::getState() const {
  return curr_state;
}

bool FSM::inState(int s) const{
  return (curr_state == s);
}

void FSM::setState(int new_state, const string reason){
  setState(new_state, reason, log.getTimestamp());
}

//Sets the FSM state, but should only be used when the FSM is not running.
//For instance, in a behavior reset function invoked from outside the behavior.
void FSM::setState(int new_state, const string reason, unsigned long timestamp){
  if(running){
    LOG_ERROR("FSM ERROR: Tried to setState while FSM was running (you should be using trans). Current state is: %s", states[curr_state].c_str());
    throw FSM_EXCEPTION;
  }
  if(states.find(new_state) == states.end()){
    LOG_ERROR("FSM ERROR: Tried to set an unknown state (%s). Current state is: %s", states[new_state].c_str(), states[curr_state].c_str());
    throw FSM_EXCEPTION;
  }

  Transition temp;
  temp.fromState = curr_state;
  temp.toState = new_state;
  temp.reason = reason;
  transitions.insert(transitions.end(),temp);

  curr_state = new_state;
  newToState = true;
  firstLoop = true;

  timeOfLastTrans = timestamp;
}

//Returns how long we have been in the current state in seconds
unsigned long int FSM::timeInState(){
  return (currTime - timeOfLastTrans);
}

//Prints all transitions that have occured so far during the current FSM loop
void FSM::printTransitions(bool printError){
  if(transitions.begin() == transitions.end())
    return;
  list<Transition>::iterator iter;
  LOG_INFO("BEGIN FSM TRANSITIONS");
  for(iter = transitions.begin(); iter != transitions.end(); iter++){
    if(printError){
      LOG_ERROR("{%s -> %s: %s}", states[iter->fromState].c_str(), states[iter->toState].c_str(), iter->reason.c_str());
    }
    else{
      LOG_INFO("{%s -> %s: %s}", states[iter->fromState].c_str(), states[iter->toState].c_str(), iter->reason.c_str());
    }
  }
  LOG_INFO("END FSM TRANSITIONS");
}

bool FSM::isNewState(){
  return newToState;
}
