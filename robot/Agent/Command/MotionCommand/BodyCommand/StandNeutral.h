#ifndef STANDNEUTRAL_H_
#define STANDNEUTRAL_H_

#include <boost/serialization/export.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/utility.hpp>

//#include "BodyCommand.h"

/**
 * A body command to just stand straight.
 */
class StandNeutral { //: public BodyCommand {
public:

  enum WhichLeg {
    Any = 0,
    Left,
    Right
  };

  /**
   * Constructor.
   */
  StandNeutral();

  /**
   * Destructor.
   */
  virtual ~StandNeutral();

  void setWhichLeg(WhichLeg whichLeg = Any);

  WhichLeg getWhichLeg() const;

  /**
   * Returns the body command type.
   *
   * @return the type of body command
   */
//  virtual BodyCommand::Type getType() const;

private:
  WhichLeg whichLeg;

  /**
   * Allow access for Boost serialization
   */
  friend class boost::serialization::access;

  template<class Archive>
  void serialize(Archive & ar, unsigned int version);
};

// Boost doesn't allow this to be placed in the cpp file for some reason.
template<class Archive>
void StandNeutral::serialize(Archive & ar, unsigned int version) {
  // Serialize the base class
 // ar & boost::serialization::base_object<BodyCommand>(*this);
}

#endif /* STANDNEUTRAL_H_ */
