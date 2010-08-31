#ifndef NULLCOMM_H_
#define NULLCOMM_H_

#include "Comm/Comm.h"
#include "shared/UsefulMacros.h"

class NullComm : public Comm {
public:
  NullComm();
  virtual ~NullComm();

  virtual bool run(Messages & messages, Feedback & feedback);

private:
  PREVENT_COPY_AND_ASSIGNMENT(NullComm);

};

#endif /* NULLCOMM_H_ */
