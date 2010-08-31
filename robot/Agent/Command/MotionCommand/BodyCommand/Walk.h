#ifndef WALK_H_
#define WALK_H_

#include <boost/serialization/export.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/utility.hpp>

//#include "BodyCommand.h"

/**
 * Walk at a general velocity
 */
class Walk { //: public BodyCommand {
public:

  /**
   * Constructor.
   */
  Walk(float vX = 0, float vY = 0, float vTheta = 0);

  /**
   * Destructor.
   */
  virtual ~Walk();

  /**
   * Returns the body command type.
   *
   * @return the type of body command
   */
  //virtual BodyCommand::Type getType() const;

  /**
   * Returns the x velocity
   *
   * @return x velocity (in m/s)
   */
  float getVelocityX() const;

  /**
   * Returns the y velocity
   *
   * @return y velocity (in m/s)
   */
  float getVelocityY() const;

  /**
   * Returns the theta velocity
   *
   * @return theta velocity (in rad/s)
   */
  float getVelocityTheta() const;

  /**
   * Sets the velocity to walk
   *
   * @param _vX x velocity (in m/s)
   * @param _vY y velocity (in m/s)
   * @param _vTheta theta velocity (in rad/s)
   */
  void setVelocity(float vX, float vY, float vTheta);

private:

  float vX, vY, vTheta;

  /**
   * Allow access for Boost serialization
   */
  friend class boost::serialization::access;

  template<class Archive>
  void serialize(Archive & ar, unsigned int version);
};

// Boost doesn't allow this to be placed in the cpp file for some reason.
template<class Archive>
void Walk::serialize(Archive & ar, unsigned int version) {
  // Serialize the base class
  //ar & boost::serialization::base_object<BodyCommand>(*this);

  ar & vX;
  ar & vY;
  ar & vTheta;
}

#endif /* WALK_H_ */
