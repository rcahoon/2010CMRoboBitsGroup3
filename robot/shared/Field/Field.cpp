#include "Field.h"
#include "GameController/GameState.h"
#include "shared/ConfigFile/ConfigFile.h"

#include <assert.h>

Field::Field(ConfigFile & configFile)
  : carpetLength       (configFile.getFloat("field/carpetLength")),
    carpetWidth        (configFile.getFloat("field/carpetWidth")),
    fieldLength        (configFile.getFloat("field/fieldLength")),
    fieldWidth         (configFile.getFloat("field/fieldWidth")),
    halfFieldLength    (fieldLength / 2),
    halfFieldWidth     (fieldWidth / 2),
    penaltyBoxLength   (configFile.getFloat("field/penaltyBoxLength")),
    penaltyBoxWidth    (configFile.getFloat("field/penaltyBoxWidth")),
    halfPenaltyBoxWidth(penaltyBoxWidth / 2),
    goalWidth          (configFile.getFloat("field/goalWidth")),
    centerCircleRadius (configFile.getFloat("field/centerCircleRadius")),
    isOurColorBlue     (true),
    ourPenaltyPoint     (-configFile.getFloat("field/distCenterToPenaltyPoint"), 0),
    opponentPenaltyPoint( configFile.getFloat("field/distCenterToPenaltyPoint"), 0),
    ourGoal              (-halfFieldLength, 0),
    opponentGoal         ( halfFieldLength, 0),
    ourGoalPostLeft      (-halfFieldLength, -goalWidth / 2),
    ourGoalPostRight     (-halfFieldLength,  goalWidth / 2),
    opponentGoalPostLeft ( halfFieldLength,  goalWidth / 2),
    opponentGoalPostRight( halfFieldLength, -goalWidth / 2),
    goaliePenaltyBoxTopLeft (-halfFieldLength + penaltyBoxLength , penaltyBoxWidth / 2),
    goaliePenaltyBoxTopRight(-halfFieldLength + penaltyBoxLength , -penaltyBoxWidth / 2),
    goaliePenaltyBoxBottomLeft(-halfFieldLength, penaltyBoxWidth / 2),
    goaliePenaltyBoxBottomRight(-halfFieldLength, -penaltyBoxWidth / 2)
{
	L_corners[0].x = getHalfFieldLength();
	L_corners[0].y = getHalfFieldWidth();
	L_corner_angles[0] = -3 * M_PI / 4;
	L_corners[1].x = getHalfFieldLength();
	L_corners[1].y = -getHalfFieldWidth();
	L_corner_angles[1] = 3 * M_PI / 4;
	L_corners[2].x = -getHalfFieldLength();
	L_corners[2].y = getHalfFieldWidth();
	L_corner_angles[2] = -M_PI / 4;
	L_corners[3].x = -getHalfFieldLength();
	L_corners[3].y = -getHalfFieldWidth();
	L_corner_angles[3] = M_PI / 4;
	L_corners[4].x = getHalfFieldLength() - getPenaltyBoxLength();
	L_corners[4].y = getHalfPenaltyBoxWidth();
	L_corner_angles[4] = -M_PI / 4;
	L_corners[5].x = getHalfFieldLength() - getPenaltyBoxLength();
	L_corners[5].y = -getHalfPenaltyBoxWidth();
	L_corner_angles[5] = M_PI / 4;
	L_corners[6].x = -getHalfFieldLength() + getPenaltyBoxLength();
	L_corners[6].y = getHalfPenaltyBoxWidth();
	L_corner_angles[6] = -3 * M_PI / 4;
	L_corners[7].x = -getHalfFieldLength() + getPenaltyBoxLength();
	L_corners[7].y = -getHalfPenaltyBoxWidth();
	L_corner_angles[7] = 3 * M_PI / 4;

	T_corners[0].x = getHalfFieldLength();
	T_corners[0].y = getHalfPenaltyBoxWidth();
	T_corner_angles[0] = -M_PI;
	T_corners[1].x = getHalfFieldLength();
	T_corners[1].y = -getHalfPenaltyBoxWidth();
	T_corner_angles[1] = -M_PI;
	T_corners[2].x = -getHalfFieldLength();
	T_corners[2].y = getHalfPenaltyBoxWidth();
	T_corner_angles[2] = M_PI;
	T_corners[3].x = -getHalfFieldLength();
	T_corners[3].y = -getHalfPenaltyBoxWidth();
	T_corner_angles[3] = M_PI;
	T_corners[4].x = 0;
	T_corners[4].y = getHalfFieldWidth();
	T_corner_angles[4] = -M_PI / 2;
	T_corners[5].x = 0;
	T_corners[5].y = -getHalfFieldWidth();
	T_corner_angles[5] = M_PI / 2;

	cross_corners[0].x = 0;
	cross_corners[0].y = getCenterCircleRadius();
	cross_corners[1].x = 0;
	cross_corners[1].y = -getCenterCircleRadius();

	//two endpoints per line segment
	//LINE ENDPOINTS MUST BE ORDERED SO THAT POINT 2 HAS X & Y THAT ARE >= THE X & Y OF POINT 1!!!!!!!!!
	lines[0][0].x = getHalfFieldLength(); lines[0][0].y = -getHalfFieldWidth();
	lines[0][1].x = getHalfFieldLength(); lines[0][1].y = getHalfFieldWidth();
	lines[1][0].x = -getHalfFieldLength(); lines[1][0].y = -getHalfFieldWidth();
	lines[1][1].x = -getHalfFieldLength(); lines[1][1].y = getHalfFieldWidth();
	lines[2][0].x = -getHalfFieldLength(); lines[2][0].y = getHalfFieldWidth();
	lines[2][1].x = getHalfFieldLength(); lines[2][1].y = getHalfFieldWidth();
	lines[3][0].x = -getHalfFieldLength(); lines[3][0].y = -getHalfFieldWidth();
	lines[3][1].x = getHalfFieldLength(); lines[3][1].y = -getHalfFieldWidth();
	lines[4][0].x = 0; lines[4][0].y = -getHalfFieldWidth();
	lines[4][1].x = 0; lines[4][1].y = getHalfFieldWidth();
	lines[5][0].x = getHalfFieldLength() - getPenaltyBoxLength(); lines[5][0].y = getHalfPenaltyBoxWidth();
	lines[5][1].x = getHalfFieldLength(); lines[5][1].y = getHalfPenaltyBoxWidth();
	lines[6][0].x = getHalfFieldLength() - getPenaltyBoxLength(); lines[6][0].y = -getHalfPenaltyBoxWidth();
	lines[6][1].x = getHalfFieldLength(); lines[6][1].y = -getHalfPenaltyBoxWidth();
	lines[7][0].x = getHalfFieldLength() - getPenaltyBoxLength(); lines[7][0].y = -getHalfPenaltyBoxWidth();
	lines[7][1].x = getHalfFieldLength() - getPenaltyBoxLength(); lines[7][1].y = getHalfPenaltyBoxWidth();
	lines[8][0].x = -getHalfFieldLength(); lines[8][0].y = getHalfPenaltyBoxWidth();
	lines[8][1].x = -getHalfFieldLength() + getPenaltyBoxLength(); lines[8][1].y = getHalfPenaltyBoxWidth();
	lines[9][0].x = -getHalfFieldLength(); lines[9][0].y = -getHalfPenaltyBoxWidth();
	lines[9][1].x = -getHalfFieldLength() + getPenaltyBoxLength(); lines[9][1].y = -getHalfPenaltyBoxWidth();
	lines[10][0].x = -getHalfFieldLength() + getPenaltyBoxLength(); lines[10][0].y = -getHalfPenaltyBoxWidth();
	lines[10][1].x = -getHalfFieldLength() + getPenaltyBoxLength(); lines[10][1].y = getHalfPenaltyBoxWidth();
}

Field::~Field() {
}

void Field::update(const GameState & gameState) {
  isOurColorBlue = gameState.isOurColorBlue();
}

float Field::getCarpetLength() const {
  return carpetLength;
}

float Field::getCarpetWidth() const {
  return carpetWidth;
}

float Field::getFieldLength() const {
  return fieldLength;
}

float Field::getFieldWidth() const {
  return fieldWidth;
}

float Field::getHalfFieldLength() const {
  return halfFieldLength;
}

float Field::getHalfFieldWidth() const {
  return halfFieldWidth;
}

float Field::getPenaltyBoxLength() const {
  return penaltyBoxLength;
}

float Field::getPenaltyBoxWidth() const {
  return penaltyBoxWidth;
}

float Field::getHalfPenaltyBoxWidth() const {
  return halfPenaltyBoxWidth;
}

float Field::getGoalWidth() const {
  return goalWidth;
}

float Field::getCenterCircleRadius() const {
  return centerCircleRadius;
}

const Vector2D & Field::getOurPenaltyPoint() const {
  return ourPenaltyPoint;
}

const Vector2D & Field::getOpponentPenaltyPoint() const {
  return opponentPenaltyPoint;
}

const Vector2D & Field::getOurGoal() const {
  return ourGoal;
}

const Vector2D & Field::getOurGoalPostLeft() const {
  return ourGoalPostLeft;
}

const Vector2D & Field::getOurGoalPostRight() const {
  return ourGoalPostRight;
}

const Vector2D & Field::getOpponentGoal() const {
  return opponentGoal;
}

const Vector2D & Field::getOpponentGoalPostLeft() const {
  return opponentGoalPostLeft;
}

const Vector2D & Field::getOpponentGoalPostRight() const {
  return opponentGoalPostRight;
}

const Vector2D & Field::getBluePenaltyPoint() const {
  return isOurColorBlue ? ourPenaltyPoint : opponentPenaltyPoint;
}

const Vector2D & Field::getYellowPenaltyPoint() const {
  return isOurColorBlue ? opponentPenaltyPoint : ourPenaltyPoint;
}

const Vector2D & Field::getYellowGoalPostLeft() const {
  return isOurColorBlue ? opponentGoalPostLeft : ourGoalPostLeft;
}

const Vector2D & Field::getYellowGoalPostRight() const {
  return isOurColorBlue ? opponentGoalPostRight : ourGoalPostRight;
}

const Vector2D & Field::getBlueGoalPostLeft() const {
  return isOurColorBlue ? ourGoalPostLeft : opponentGoalPostLeft;
}

const Vector2D & Field::getBlueGoalPostRight() const {
  return isOurColorBlue ? ourGoalPostRight : opponentGoalPostRight;
}

const Vector2D & Field::getLCorner(int corner) const {
	assert(corner >= 0 && corner < FIELD_NUM_L_CORNERS);
	return L_corners[corner];
}

const float Field::getLCornerAngle(int corner) const {
	assert(corner >= 0 && corner < FIELD_NUM_L_CORNERS);
	return L_corner_angles[corner];
}

const Vector2D & Field::getTCorner(int corner) const {
	assert(corner >= 0 && corner < FIELD_NUM_T_CORNERS);
	return T_corners[corner];
}

const float Field::getTCornerAngle(int corner) const {
	assert(corner >= 0 && corner < FIELD_NUM_T_CORNERS);
	return T_corner_angles[corner];
}

const Vector2D & Field::getCrossCorner(int corner) const {
	assert(corner >= 0 && corner < FIELD_NUM_CROSS_CORNERS);
	return cross_corners[corner];
}

void Field::getLineSegment(int segment, Vector2D* p1, Vector2D* p2) const {
	assert(segment >= 0 && segment < FIELD_NUM_LINES);
	if (p1 != NULL)
		*p1 = lines[segment][0];
	if (p2 != NULL)
		*p2 = lines[segment][1];
}

const Vector2D & Field::getGoaliePenaltyBoxTopLeft() const {
  return goaliePenaltyBoxTopLeft;
}

const Vector2D & Field::getGoaliePenaltyBoxTopRight() const {
  return goaliePenaltyBoxTopRight;
}

const Vector2D & Field::getGoaliePenaltyBoxBottomLeft() const {
  return goaliePenaltyBoxBottomLeft;
}

const Vector2D & Field::getGoaliePenaltyBoxBottomRight() const {
  return goaliePenaltyBoxBottomRight;
}
