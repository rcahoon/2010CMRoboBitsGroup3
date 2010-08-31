#ifndef NOBODYCOMMAND_H_
#define NOBODYCOMMAND_H_

#include <boost/serialization/export.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/utility.hpp>

//#include "BodyCommand.h"

/**
 * A blank body command, i.e. do nothing.
 */
class NoBodyCommand { //: public BodyCommand{
public:

  /**
   * Constructor.
   */
  NoBodyCommand();

  /**
   * Destructor.
   */
  virtual ~NoBodyCommand();

  /**
   * Returns the body command type.
   *
   * @return the type of body command
   */
  //virtual BodyCommand::Type getType() const;

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
void NoBodyCommand::serialize(Archive & ar, unsigned int version) {
  // Serialize the base class
  //ar & boost::serialization::base_object<BodyCommand>(*this);
}

#endif /* NOBODYCOMMAND_H_ */
