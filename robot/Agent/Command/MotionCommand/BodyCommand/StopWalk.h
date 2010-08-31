#ifndef STOPWALK_H_
#define STOPWALK_H_

#include <boost/serialization/export.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/utility.hpp>

//#include "BodyCommand.h"

/**
 * A body command to stop the current walk, and put the foot down if it is in
 * the air.
 */
class StopWalk { //: public BodyCommand {
public:

  /**
   * Constructor.
   */
  StopWalk();

  /**
   * Destructor.
   */
  virtual ~StopWalk();

  /**
   * Returns the body command type.
   *
   * @return the type of body command
   */
//  virtual BodyCommand::Type getType() const;

private:

  /**
   * Allow access for Boost serialization
   */
  friend class boost::serialization::access;

  template<class Archive>
  void serialize(Archive & ar, unsigned int version);
};

// Boost doesn't allow this to be placed in the cpp file for some reason.
template<class Archive>
void StopWalk::serialize(Archive & ar, unsigned int version) {
  // Serialize the base class
 // ar & boost::serialization::base_object<BodyCommand>(*this);
}

#endif /* STOPWALK_H_ */
