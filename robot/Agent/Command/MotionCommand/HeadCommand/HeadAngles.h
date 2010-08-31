#ifndef HEADANGLES_H_
#define HEADANGLES_H_

#include <boost/serialization/export.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/utility.hpp>

//#include "HeadCommand.h"

/**
 * Directly set the head angles (pan, tilt) in radians.
 */
class HeadAngles { // : public HeadCommand {
public:

  /**
   * Constructor.
   */
  HeadAngles(float _pan = 0, float _tilt = 0, float _speed = 0.8f);

  /**
   * Destructor.
   */
  virtual ~HeadAngles();

  /**
   * Returns the head command type.
   *
   * @return the type of head command
   */
  //virtual HeadCommand::Type getType() const;

  /**
   * Retrieves the angle to pan (in radians).
   *
   * @return the angle to pan
   */
  float getPan() const;

  /**
   * Retrieves the angle to tilt (in radians).
   *
   * @return the angle to tilt
   */
  float getTilt() const;

  /**
   * Retrieves the desired speed of the head.
   *
   * @return the speed of the head
   */
  float getSpeed() const;

  /**
   * Sets the angle to pan (in radians).
   * Positive means pan to the left.
   *
   * @param _pan the angle to pan
   */
  void setPan(float pan);

  /**
   * Sets the angle to tilt (in radians).
   * Positive means tilt down.
   *
   * @param _tilt the angle to tilt
   */
  void setTilt(float tilt);

  /**
   * Sets the angle to pan and tilt (in radians)
   *
   * @param _pan the angle to pan
   * @param _tilt the angle to tilt
   */
  void setAngles(float _pan, float _tilt);

  /**
   * Sets the speed of the head (from 0 to 1). 0 is minimum speed (no movement)
   * and 1 is maximum.
   *
   * @param _speed the speed of the head (from 0 to 1)
   */
  void setSpeed(float _speed);

private:

  float pan, tilt;
  float speed;

  /**
   * Allow access for Boost serialization
   */
  friend class boost::serialization::access;

  template<class Archive>
  void serialize(Archive & ar, unsigned int version);
};

// Boost doesn't allow this to be placed in the cpp file for some reason.
template<class Archive>
void HeadAngles::serialize(Archive & ar, unsigned int version) {
  // Serialize the base class
  //ar & boost::serialization::base_object<HeadCommand>(*this);

  ar & pan;
  ar & tilt;
}

#endif /* HEADANGLES_H_ */
