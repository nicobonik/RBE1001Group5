#pragma once

#include "vex.h"
#include <math.h>
#include "DataTypes.h"

using namespace vex;

extern const float wheelDiameter;
extern const float wheelCircumference;
extern const float gearRatio;
extern const float wheelTrack;
extern const float degreesPerInch;
extern const float intakeDegPerBall;

extern const float DefaultRPM;
extern float RPM;

extern bool wheelForwardLeft, wheelForwardRight;
extern const float wheelCorrectiveOffset, wheelCorrectiveRPM;

extern Vector position;
extern float heading;

void correctWheels(bool leftForward, bool rightForward);

void setSpeed(float speed);

bool isADriveDone();

void driveStraight(float inches);
void ADriveStraight(float inches);

void turnLeft(float targetRadians);
void ATurnLeft(float targetRadians);

void turnAround(float targetDegrees, float offset);

void driveRPM(float speed, float turn);