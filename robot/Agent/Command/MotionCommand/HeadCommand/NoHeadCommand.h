#ifndef NOHEADCOMMAND_H_
#define NOHEADCOMMAND_H_

#include <boost/serialization/export.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/utility.hpp>

//#include "HeadCommand.h"

/**
 * A blank head command, i.e. do nothing.
 */
class NoHeadCommand { // : public HeadCommand {
public:

  /**
   * Constructor.
   */
  NoHeadCommand();

  /**
   * Destructor.
   */
  virtual ~NoHeadCommand();

  /**
   * Returns the head command type.
   *
   * @return the type of head command
   */
  //virtual HeadCommand::Type getType() const;

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
void NoHeadCommand::serialize(Archive & ar, unsigned int version) {
  // Serialize the base class
  //ar & boost::serialization::base_object<HeadCommand>(*this);
}

#endif /* NOHEADCOMMAND_H_ */
