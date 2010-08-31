#ifndef ROBOTSTATE_H_
#define ROBOTSTATE_H_

#include "shared/HMatrix/HMatrix.h"
#include "shared/Vector/Vector2D.h"
#include "shared/UsefulMacros.h"
#include "Agent/Command/MotionCommand/BodyCommand/StaticAction.h"
#include "Agent/Command/MotionCommand/MotionCommand.h"
#include <boost/serialization/utility.hpp>

class RobotState {
public:

  RobotState(unsigned long _timestamp = 0,
             bool _chestButton = false,
             bool _leftFootButton = false,
             bool _rightFootButton = false,
             char const * _rawImage = NULL,
             unsigned int _rawImageSize = 0,
             const HMatrix & _transformationFromCamera = HMatrix(),
             const HMatrix & _transformationFromNeck = HMatrix(),
             bool _usingBottomCamera = false,
             float _headPan = 0,
             float _headTilt = 0,
             Vector2D _odometryTranslation = Vector2D(0, 0),
             float _odometryRotation = 0,
             BodyCommand bodyCommandType = bcNoBodyCommand,
             StaticAction::StaticActionType staticActionType = StaticAction::actNone,
             float _leftUltraSoundDistance = 0,
             float _rightUltraSoundDistance = 0,
             float _bodyAngleX = 0,
             float _bodyAngleY = 0,
             bool _robotOnGround = true);

  virtual ~RobotState();

  
  unsigned long getTimestamp() const;
  void setTimestamp( unsigned long _timeStamp );

  bool getChestButton() const;
  void setChestButton( bool _chestButton );

  bool getLeftFootButton() const;
  void setLeftFootButton( bool _leftFootButton );

  bool getRightFootButton() const;
  void setRightFootButton( bool _rightFootButton );

  char const * getRawImage() const;
  void setRawImage( const char* _rawImage );

  unsigned int getRawImageSize() const;
  void setRawImageSize( unsigned int _rawImageSize );

  const HMatrix & getTransformationFromCamera() const;
  void  setTransformationFromCamera( HMatrix& _transformationFromCamera );

  const HMatrix & getTransformationFromNeck() const;
  void  setTransformationFromNeck( HMatrix& _transformationFromNeck );

  bool isUsingBottomCamera() const;
  void setCameraInUse( bool _usingBottomCamera );

  float getHeadPan() const;
  void  setHeadPan( float _headPan);

  float getHeadTilt() const;
  void  setHeadTilt( float _headTilt);

  void getOdometryUpdate(Vector2D & translation, float & rotation) const;

  void setOdometryTranslation( Vector2D& _translation );
  void setOdometryRotation( float _rotation );

  float getLeftUltraSoundDistance() const;
  float getRightUltraSoundDistance() const;
  void setUltraSound(float _leftUltraSoundDistance, float _rightUltraSoundDistance);

  float getBodyAngleX() const;
  float getBodyAngleY() const;
  void setBodyAngle(float _bodyAngleX, float _bodyAngleY);

  bool isRobotOnGround() const;
  void setRobotOnGround(bool _robotOnGround);

  BodyCommand getBodyCommandType() const;
  void  setBodyCommandType( BodyCommand _bodyCommandType );

  StaticAction::StaticActionType getStaticActionType() const;
  void  setStaticActionType( StaticAction::StaticActionType _staticActionType );
private:
  PREVENT_COPY_AND_ASSIGNMENT(RobotState);

  unsigned long timestamp;

  bool chestButton, leftFootButton, rightFootButton;

  char const * rawImage;
  unsigned int rawImageSize;
  bool         shouldCleanImage;

  HMatrix transformationFromCamera, transformationFromNeck;
  bool    usingBottomCamera;

  float headPan, headTilt;

  Vector2D odometryTranslation;
  float    odometryRotation;

  BodyCommand bodyCommandType;
  StaticAction::StaticActionType staticActionType;

  float leftUltraSoundDistance, rightUltraSoundDistance;
  float bodyAngleX, bodyAngleY;
  bool  robotOnGround;

  /**
   * Allow access for Boost serialization
   */
  friend class boost::serialization::access;
  BOOST_SERIALIZATION_SPLIT_MEMBER();

  template<class Archive>
  void save(Archive & ar, const unsigned int version) const;

  template<class Archive>
  void load(Archive & ar, const unsigned int version);
};

// Boost doesn't allow these to be placed in the cpp file for some reason.

template<class Archive>
void RobotState::save(Archive & ar, const unsigned int version) const {
  ar & timestamp;

  ar & chestButton;
  ar & leftFootButton;
  ar & rightFootButton;

  ar & rawImageSize;

  ar & transformationFromCamera;
  ar & transformationFromNeck;
  ar & usingBottomCamera;

  ar & headPan;
  ar & headTilt;

  ar & odometryTranslation;
  ar & odometryRotation;

  ar & staticActionType;
  ar & bodyCommandType;

  ar & leftUltraSoundDistance;
  ar & rightUltraSoundDistance;

  ar & bodyAngleX;
  ar & bodyAngleY;

  ar & robotOnGround;
}

template<class Archive>
void RobotState::load(Archive & ar, const unsigned int version) {
  ar & timestamp;

  ar & chestButton;
  ar & leftFootButton;
  ar & rightFootButton;

  ar & rawImageSize;

  shouldCleanImage = false;
  rawImage = NULL;

  ar & transformationFromCamera;
  ar & transformationFromNeck;
  ar & usingBottomCamera;

  ar & headPan;
  ar & headTilt;

  ar & odometryTranslation;
  ar & odometryRotation;

  ar & staticActionType;
  ar & bodyCommandType;

  ar & leftUltraSoundDistance;
  ar & rightUltraSoundDistance;

  ar & bodyAngleX;
  ar & bodyAngleY;

  ar & robotOnGround;
}

#endif /* ROBOTSTATE_H_ */
