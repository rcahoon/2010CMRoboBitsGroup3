#ifndef USEFULMACROS_H_
#define USEFULMACROS_H_

#define PREVENT_COPY_AND_ASSIGNMENT(ClassName); ClassName(const ClassName &); ClassName & operator=(const ClassName &);

#define PREVENT_ASSIGNMENT(ClassName); ClassName & operator=(const ClassName &);

#define FSM_ADDSTATE(fsm, state); fsm.addState(state, #state);

// Should mainly be used for prints (since there is cost in conversion)
#define TO_RAD(degrees) (degrees / 180.0f * M_PI)
#define TO_DEG(radians) (radians / M_PI * 180.0f)

#endif /* USEFULMACROS_H_ */
