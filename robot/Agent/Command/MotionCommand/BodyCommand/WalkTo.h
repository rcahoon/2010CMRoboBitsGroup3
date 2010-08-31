#ifndef WALKTO_H_
#define WALKTO_H_

#include <boost/serialization/export.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/utility.hpp>


/**
 * Walk to a relative position
 */
class WalkTo { 
public:

  /**
   * Constructor.
   */
  WalkTo(float vX = 0, float vY = 0, float vTheta = 0);

  /**
   * Destructor.
   */
  virtual ~WalkTo();

  /**
   * Returns the x position
   *
   * @return x position (in meters)
   */
  float getPositionX() const;

  /**
   * Returns the y position
   *
   * @return y position (in meters)
   */
  float getPositionY() const;

  /**
   * Returns the theta position
   *
   * @return theta position (in radians)
   */
  float getPositionTheta() const;

  /**
   * Sets the position to walk to
   *
   * @param _pX x position (in meters)
   * @param _pY y position (in meters)
   * @param _pTheta theta position (in radians)
   */
  void setPosition(float pX, float pY, float pTheta);

private:

  float pX, pY, pTheta;

  /**
   * Allow access for Boost serialization
   */
  friend class boost::serialization::access;

  template<class Archive>
  void serialize(Archive & ar, unsigned int version);
};

// Boost doesn't allow this to be placed in the cpp file for some reason.
template<class Archive>
void WalkTo::serialize(Archive & ar, unsigned int version) {

  ar & pX;
  ar & pY;
  ar & pTheta;
}

#endif /* WALKTO_H_ */
