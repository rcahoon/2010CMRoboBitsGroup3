#include "RobotState.h"

RobotState::RobotState(unsigned long _timestamp,
                       bool _chestButton,
                       bool _leftFootButton,
                       bool _rightFootButton,
                       char const * _rawImage,
                       unsigned int _rawImageSize,
                       const HMatrix & _transformationFromCamera,
                       const HMatrix & _transformationFromNeck,
                       bool _usingBottomCamera,
                       float _headPan,
                       float _headTilt,
                       Vector2D _odometryTranslation,
                       float _odometryRotation,
                       BodyCommand _bodyCommandType,
                       StaticAction::StaticActionType _staticActionType,
                       float _leftUltraSoundDistance, float _rightUltraSoundDistance,
                       float _bodyAngleX, float _bodyAngleY,
                       bool _robotOnGround)
  : timestamp(_timestamp),
    chestButton(_chestButton),
    leftFootButton(_leftFootButton),
    rightFootButton(_rightFootButton),
    rawImage(_rawImage),
    rawImageSize(_rawImageSize),
    shouldCleanImage(false),
    transformationFromCamera(_transformationFromCamera),
    transformationFromNeck(_transformationFromNeck),
    usingBottomCamera(_usingBottomCamera),
    headPan(_headPan),
    headTilt(_headTilt),
    odometryTranslation(_odometryTranslation),
    odometryRotation(_odometryRotation),
    bodyCommandType(_bodyCommandType),
    staticActionType(_staticActionType),
    leftUltraSoundDistance(_leftUltraSoundDistance),
    rightUltraSoundDistance(_rightUltraSoundDistance),
    bodyAngleX(_bodyAngleX),
    bodyAngleY(_bodyAngleY),
    robotOnGround(_robotOnGround) {
}

RobotState::~RobotState() {
  if (shouldCleanImage && (rawImage != NULL)) {
    delete [] rawImage;
    rawImage = NULL;
  }
}

unsigned long RobotState::getTimestamp() const {
  return timestamp;
}

void RobotState::setTimestamp( unsigned long _timeStamp ){
    timestamp = _timeStamp;
}

BodyCommand RobotState::getBodyCommandType() const {
  return bodyCommandType;
}

void RobotState::setBodyCommandType( BodyCommand _bodyCommandType ){
    bodyCommandType = _bodyCommandType;
}

StaticAction::StaticActionType RobotState::getStaticActionType() const {
  return staticActionType;
}

void RobotState::setStaticActionType( StaticAction::StaticActionType _staticActionType ){
    staticActionType = _staticActionType;
}

bool RobotState::getChestButton() const {
  return chestButton;
}

void RobotState::setChestButton( bool _chestButton ){
    chestButton = _chestButton;
}

bool RobotState::getLeftFootButton() const {
  return leftFootButton;
}

void RobotState::setLeftFootButton( bool _leftFootButton ){
    leftFootButton = _leftFootButton;
}

bool RobotState::getRightFootButton() const {
  return rightFootButton;
}

void RobotState::setRightFootButton( bool _rightFootButton ){
    rightFootButton = _rightFootButton;
}

char const * RobotState::getRawImage() const {
  return rawImage;
}

void RobotState::setRawImage( const char* _rawImage ){
    rawImage = _rawImage;
}

unsigned int RobotState::getRawImageSize() const {
  return rawImageSize;
}

void RobotState::setRawImageSize( unsigned int _rawImageSize ){
    rawImageSize = _rawImageSize;
}

const HMatrix & RobotState::getTransformationFromCamera() const {
  return transformationFromCamera;
}

void RobotState::setTransformationFromCamera( HMatrix& _transformationFromCamera ){
    transformationFromCamera.setFrom( &_transformationFromCamera );
}

const HMatrix & RobotState::getTransformationFromNeck() const {
  return transformationFromNeck;
}

void RobotState::setTransformationFromNeck( HMatrix& _transformationFromNeck ){
    transformationFromNeck.setFrom( &_transformationFromNeck );
}

bool RobotState::isUsingBottomCamera() const {
  return usingBottomCamera;
}

void RobotState::setCameraInUse( bool _usingBottomCamera ){
    usingBottomCamera = _usingBottomCamera;
}

float RobotState::getHeadPan() const {
  return headPan;
}

void RobotState::setHeadPan( float _headPan ){
    headPan = _headPan;
}

float RobotState::getHeadTilt() const {
  return headTilt;
}

void RobotState::setHeadTilt( float _headTilt ){
    headTilt = _headTilt;
}

void RobotState::getOdometryUpdate(Vector2D & translation, float & rotation) const {
  translation = odometryTranslation;
  rotation    = odometryRotation;
}

void RobotState::setOdometryTranslation( Vector2D& _odometryTranslation ){
    odometryTranslation = _odometryTranslation;
}

void RobotState::setOdometryRotation( float _odometryRotation ){
    odometryRotation = _odometryRotation;
}

float RobotState::getLeftUltraSoundDistance() const {
  return leftUltraSoundDistance;
}

float RobotState::getRightUltraSoundDistance() const {
  return rightUltraSoundDistance;
}

void RobotState::setUltraSound(float _leftUltraSoundDistance, float _rightUltraSoundDistance) {
  leftUltraSoundDistance = _leftUltraSoundDistance;
  rightUltraSoundDistance = _rightUltraSoundDistance;
}

float RobotState::getBodyAngleX() const {
  return bodyAngleX;
}

float RobotState::getBodyAngleY() const {
  return bodyAngleY;
}

void RobotState::setBodyAngle(float _bodyAngleX, float _bodyAngleY) {
  bodyAngleX = _bodyAngleX;
  bodyAngleY = _bodyAngleY;
}

bool RobotState::isRobotOnGround() const {
  return robotOnGround;
}

void RobotState::setRobotOnGround(bool _robotOnGround) {
  robotOnGround = _robotOnGround;
}
