#ifndef COMM_H_
#define COMM_H_

#include "shared/UsefulMacros.h"

class Feedback;
class Messages;

/**
 * Interface for the Comm component.
 */
class Comm {
public:
  /**
   * Default constructor (empty since this is an interface).
   */
  Comm() {};

  /**
   * Destructor (empty since this is an interface).
   */
  virtual ~Comm() {};

  /**
   * Runs the Comm component, which sends and receives network messages.
   *
   * @param messages messages to be updates
   * @param feedback any feedback to be given to other components
   *
   * @return whether an error occurred (false = everything is fine)
   */
  virtual bool run(Messages & messages, Feedback & feedback) = 0;

private:
  PREVENT_COPY_AND_ASSIGNMENT(Comm);
};

#endif /* COMM_H_ */
