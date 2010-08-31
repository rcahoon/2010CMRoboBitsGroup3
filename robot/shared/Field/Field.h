#ifndef FIELD_H_
#define FIELD_H_

#include "shared/Vector/Vector2D.h"
#include "shared/UsefulMacros.h"

class ConfigFile;
class GameState;

#define FIELD_NUM_L_CORNERS 8
#define FIELD_NUM_T_CORNERS 6
#define FIELD_NUM_CROSS_CORNERS 2
#define FIELD_NUM_LINES 11

class Field {
public:
  Field(ConfigFile & configFile);

  virtual ~Field();

  /**
   * Update the coordinates of objects in the field, based on the team color we
   * are on.
   *
   * @param gameState state of the game
   */
  void update(const GameState & gameState);

  float getCarpetLength() const;
  float getCarpetWidth() const;

  float getFieldLength() const;
  float getFieldWidth() const;

  float getHalfFieldLength() const;
  float getHalfFieldWidth() const;

  float getPenaltyBoxLength() const;
  float getPenaltyBoxWidth() const;

  float getHalfPenaltyBoxWidth() const;

  float getGoalWidth() const;

  float getCenterCircleRadius() const;

  const Vector2D & getOurPenaltyPoint() const;
  const Vector2D & getOpponentPenaltyPoint() const;

  const Vector2D & getOurGoal() const;
  const Vector2D & getOurGoalPostLeft() const;
  const Vector2D & getOurGoalPostRight() const;

  const Vector2D & getOpponentGoal() const;
  const Vector2D & getOpponentGoalPostLeft() const;
  const Vector2D & getOpponentGoalPostRight() const;

  const Vector2D & getBluePenaltyPoint() const;
  const Vector2D & getYellowPenaltyPoint() const;

  const Vector2D & getYellowGoalPostLeft() const;
  const Vector2D & getYellowGoalPostRight() const;

  const Vector2D & getBlueGoalPostLeft() const;
  const Vector2D & getBlueGoalPostRight() const;

  const Vector2D & getLCorner(int corner) const;
  const float getLCornerAngle(int corner) const;
  const Vector2D & getTCorner(int corner) const;
  const float getTCornerAngle(int corner) const;
  const Vector2D & getCrossCorner(int corner) const;
  void getLineSegment(int segment, Vector2D* p1, Vector2D* p2) const;

  const Vector2D & getGoaliePenaltyBoxTopLeft() const;
  const Vector2D & getGoaliePenaltyBoxTopRight() const;
  const Vector2D & getGoaliePenaltyBoxBottomLeft() const;
  const Vector2D & getGoaliePenaltyBoxBottomRight() const;

private:
  PREVENT_COPY_AND_ASSIGNMENT(Field);

  const float    carpetLength,    carpetWidth;

  const float     fieldLength,     fieldWidth;
  const float halfFieldLength, halfFieldWidth;

  const float penaltyBoxLength, penaltyBoxWidth;
  const float               halfPenaltyBoxWidth;

  const float goalWidth;

  const float centerCircleRadius;


  bool isOurColorBlue;

  Vector2D      ourPenaltyPoint, opponentPenaltyPoint;
  Vector2D      ourGoal,         opponentGoal;
  Vector2D      ourGoalPostLeft,      ourGoalPostRight;
  Vector2D opponentGoalPostLeft, opponentGoalPostRight;

  Vector2D goaliePenaltyBoxTopLeft, goaliePenaltyBoxTopRight, goaliePenaltyBoxBottomLeft, goaliePenaltyBoxBottomRight;

  Vector2D L_corners[FIELD_NUM_L_CORNERS];
  float L_corner_angles[FIELD_NUM_L_CORNERS];
  Vector2D T_corners[FIELD_NUM_T_CORNERS];
  float T_corner_angles[FIELD_NUM_L_CORNERS];
  Vector2D cross_corners[FIELD_NUM_CROSS_CORNERS];
  Vector2D lines[FIELD_NUM_LINES][2];
};

#endif /* FIELD_H_ */
