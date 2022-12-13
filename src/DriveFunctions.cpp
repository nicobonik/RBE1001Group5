#include "DriveFunctions.h"

const float wheelDiameter = 4.0;
const float wheelCircumference = PI * wheelDiameter;
const float gearRatio = 5.0;
const float wheelTrack = 11;
const float degreesPerInch = 360.0 / wheelCircumference;
const float intakeDegPerBall = 1950; // 4.5 in / ball * 5 (gear ratio) * 360 (deg / rot) / (16 * 2/8) (in/rot) = 900 deg / ball -> 1200?

const float DefaultRPM = 120;
float RPM = DefaultRPM;

bool wheelForwardLeft = false, wheelForwardRight = false;
const float wheelCorrectiveOffset = .25 * degreesPerInch/ gearRatio, wheelCorrectiveRPM = 60;

Vector position = Vector();
float heading = 0;

// The arguments are what they're going to be, not what they are
void correctWheels(bool leftForward, bool rightForward) {
  if(leftForward != wheelForwardLeft) {
    leftMotor.setVelocity(wheelCorrectiveRPM, rpm);
    leftMotor.spinFor(vex::directionType::fwd, wheelCorrectiveOffset * (leftForward? 1 : -1), degrees, rightForward == wheelForwardRight);
    leftMotor.setVelocity(RPM, rpm);
    wheelForwardLeft = leftForward;
  }
  
  if(rightForward != wheelForwardRight) {
    rightMotor.setVelocity(wheelCorrectiveRPM, rpm);
    rightMotor.spinFor(vex::directionType::fwd, wheelCorrectiveOffset * (rightForward? 1 : -1), degrees, true);
    rightMotor.setVelocity(RPM, rpm);
    wheelForwardRight = rightForward;
  }

  leftMotor.setVelocity(RPM, rpm);
  rightMotor.setVelocity(RPM, rpm);
}

// In inches per second
void setSpeed(float speed) {
  RPM = fabsf(speed * gearRatio * wheelCircumference);
  leftMotor.setVelocity(RPM, rpm);
  rightMotor.setVelocity(RPM, rpm);
}

bool isADriveDone() {
  return rightMotor.isDone() && leftMotor.isDone();
}

void driveStraight(float inches) {
  bool isForward = inches >= 0;
  correctWheels(isForward, isForward);
  leftMotor.spinFor(vex::directionType::fwd, gearRatio * inches * degreesPerInch, degrees, false);
  rightMotor.spinFor(vex::directionType::fwd, gearRatio * inches * degreesPerInch, degrees, true);
  position += Vector(inches, 0).rotate(heading);
}

void ADriveStraight(float inches) {
  bool isForward = inches >= 0;
  correctWheels(isForward, isForward);
  leftMotor.spinFor(vex::directionType::fwd, gearRatio * inches * degreesPerInch, degrees, false);
  rightMotor.spinFor(vex::directionType::fwd, gearRatio * inches * degreesPerInch, degrees, false);
  position += Vector(inches, 0).rotate(heading);
}

void turnLeft(float targetRadians) {
  bool isRight = targetRadians >= 0;
  correctWheels(isRight, !isRight);
  float rotationDegrees = targetRadians * Rad2Deg * gearRatio * wheelTrack / wheelDiameter;
  leftMotor.spinFor(vex::directionType::fwd, -rotationDegrees, degrees, false);
  rightMotor.spinFor(vex::directionType::fwd, rotationDegrees, degrees, true);
  heading += targetRadians;
}

void turnToHeading(float targetRad) {
  turnLeft(targetRad - heading);
  heading = targetRad;
}
void turnToHeading(float targetRad, bool reverse) {
  if(reverse) {
    float dif = targetRad - heading;
    if(dif > 0) turnLeft(dif - 2 * PI);
    else turnLeft(2 * PI - dif);
    heading = targetRad;
  } else turnToHeading(targetRad);
}

void ATurnLeft(float targetRadians) {
  bool isRight = targetRadians >= 0;
  correctWheels(isRight, !isRight);
  float rotationDegrees = targetRadians * Rad2Deg * gearRatio * wheelTrack / wheelDiameter;
  leftMotor.spinFor(vex::directionType::fwd, rotationDegrees, degrees, false);
  rightMotor.spinFor(vex::directionType::fwd, -rotationDegrees, degrees, false);
  heading += targetRadians * Deg2Rad;
}

void turnAround(float targetDegrees, float offset) {
  float rotationMult = (targetDegrees / 180.0) * PI * gearRatio * degreesPerInch;
  float leftDistance = offset + wheelTrack / 2;
  float rightDistance = offset - wheelTrack / 2;
  float m = wheelTrack / 2;
  if(offset > 0) {
    m = leftDistance;
  }
  else if (offset < 0) {
    m = rightDistance;
  }

  if(offset < 0) {
    rotationMult *= -1;
  }
  
  correctWheels(leftDistance * rotationMult > 0,
                rightDistance * rotationMult > 0);
  
  leftMotor.setVelocity(RPM * leftDistance / m, rpm);
  rightMotor.setVelocity(RPM * rightDistance / m, rpm);

  leftMotor.spinFor(vex::directionType::fwd, leftDistance * rotationMult, degrees, false);
  rightMotor.spinFor(vex::directionType::fwd, rightDistance * rotationMult, degrees, true);

  leftMotor.setVelocity(RPM, rpm);
  rightMotor.setVelocity(RPM, rpm);
  
  position += Vector(0,offset).rotate(heading) +
              Vector(0, -offset).rotate(heading + targetDegrees);
  heading += targetDegrees * Deg2Rad;
}

void driveRPM(float speed, float turn) {

  correctWheels(speed - turn > 0,
                speed + turn > 0);

  leftMotor.setVelocity( speed - turn, rpm);
  rightMotor.setVelocity(speed + turn, rpm);

  leftMotor.spin(forward);
  rightMotor.spin(forward);

}

void followLine(float inches, bool reverse) {
  float m = reverse? -1 : 1;
  printf("%f, %f\n", (inches * 60 / (RPM * wheelCircumference)), RPM);
  double finalTime = (inches * 60 / (RPM * wheelCircumference)) + Brain.timer(seconds);
  while(Brain.timer(seconds) < finalTime) {
    driveRPM(RPM * m, intakeRightLine.reflectivity() - intakeLeftLine.reflectivity());
  }
  position += Vector(inches,0).rotate(heading) * m;
}

void followLine() {
  driveRPM(-RPM, 3*(intakeLeftLine.value(percent) - intakeRightLine.value(percent)));
}
