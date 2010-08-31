#ifndef WORLDOBJECT_H_
#define WORLDOBJECT_H_

#include "shared/Vector/Vector2D.h"

class Log;

/**
 * This is the superclass of all world objects.
 *
 * World objects have local and global coordinates, a confidence (higher is
 * better), and flags for visible, valid and suspicious.
 *
 * Visible means the object is currently visible.
 * Valid means the positions mean something (and vice versa).
 * Suspicious means the object is valid, but the position should be confirmed by
 * behaviors.
 */
class WorldObject {
public:

  enum Type {
    Unknown = 0,
    BlueGoalPost,
    YellowGoalPost,
    BlueGoalBar,
    YellowGoalBar,
    BlueGoal,
    YellowGoal,
    Ball,
    Obstacle,
    Self,
    Teammate,
    Opponent,
    FreeSpace,
    KickTarget,
    numTypes // this should always be last
  };

  enum Source {
    ActuationSource = 0,
    VisionSource,
    TeammateSource,
    UnknownSource,
  };

  /**
   * Basic constructor.
   *
   * @param _log reference to the Log component
   * @param _type type of object, e.g., ball, goal post, etc
   */
  WorldObject(Log & _log,
              const Type _type = Unknown);

  /**
   * Copy constructor.
   */
  WorldObject(const WorldObject & other);

  /**
   * Destructor.
   */
  virtual ~WorldObject();

  /**
   * Retrieves the type of object.
   *
   * @return type of object
   */
  Type getType() const;

  /**
   * Retrieves the source of the object.
   *
   * @return source of object
   */
  Source getSource() const;

  /**
   * Retrieves the position of the object in local coordinates.
   *
   * @return reference to the local coordinates of the object
   */
  Vector2D const & getLocalPosition() const;

  /**
   * Retrieves the position of the object in global coordinates.
   *
   * @return reference to the global coordinates of the object
   */
  Vector2D const & getGlobalPosition() const;

  /**
   * Retrieves the confidence of the object.
   *
   * @return the confidence of the object (higher is better)
   */
  float getConfidence() const;

  /**
   * Is the object currently visible?
   *
   * @return whether the object is currently visible
   */
  bool isVisible() const;

  /**
   * Is the object valid, i.e., does its position mean anything?
   *
   * @return whether the object is valid
   */
  bool isValid() const;

  /**
   * Is the object suspicious, i.e., should we confirm if it is really there?
   *
   * @return whether the object is suspicious
   */
  bool isSuspicious() const;

  /**
   * Is the object based on a new hypothesis?
   *
   * @return whether the object is based on a new hypothesis
   */
  bool isNewHypothesis() const;

  /**
   * Sets the local position of the object.
   *
   * @param _localPosition the position of the object in local coordinates
   */
  void setLocalPosition(const Vector2D & _localPosition);

  /**
   * Sets the global position of the object.
   *
   * @param _globalPosition the position of the object in global coordinates
   */
  void setGlobalPosition(const Vector2D & _globalPosition);

  /**
   * Sets the confidence of the object (higher is better).
   *
   * @param _confidence the confidence of the object
   */
  void setConfidence(const float _confidence);

  /**
   * Sets whether the object is currently visible.
   * Visible => Valid and not Suspicious
   *
   * @param _visible whether the object is currently visible
   */
  void setVisible(const bool _visible = true);

  /**
   * Sets whether the object is valid.
   * Invalid => not Visible and not Suspicious
   *
   * @param _valid whether the object is valid
   */
  void setValid(const bool _valid = true);

  /**
   * Sets whether the object is suspicious.
   * Suspicious => not Visible and Valid
   *
   * @param _suspicious whether the object is suspicious
   */
  void setSuspicious(const bool _suspicious = true);

  /**
   * Sets the source of the object.
   *
   * @param _source source of the object
   */
  void setSource(Source _source);

  /**
   * Sets that this is a new hypothesis.
   *
   * @param _newHypothesis whether this is a new hypothesis
   */
  void setNewHypothesis(bool _newHypothesis);

  /**
   * Clears the members variables of the object. However, the type remains the
   * same.
   */
  virtual void clear();

  /**
   * Copies the content of another world object. We use this instead of having
   * an assignment operator to prevent issues with inheritance.
   *
   * @param other other vision object to copy from
   */
  virtual void copyFrom(const WorldObject & other);

  /**
   * Assignment operator - calls copyFrom method.
   *
   * @param other other vision object to be assigned from
   */
  WorldObject & operator=(const WorldObject & other);

protected:
  Log & log;

private:
  Type type;

  Source source;

  Vector2D localPosition;
  Vector2D globalPosition;

  float confidence;

  bool visible, valid, suspicious;

  bool newHypothesis;
};

#endif /* WORLDOBJECT_H_ */
