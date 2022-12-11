#include "NavigationController.h"



bool onRight = true;

bool getStartPosition() {
  ADriveStraight(-lineY);
  while(intakeRightLine.value(percent) > 60) if(isADriveDone()) return false;

  position.y = lineY - lineHalfWidth + intakeLineSensorOffset;
  position.x = wallX - leftSonarOffsetX - leftForwardSonar.distance(inches);
  onRight = position.x > 0;

  heading = PI * 3/ 2;
  if(onRight) {
    Vector relPos = Vector(lineX - position.x, lineY - position.y);
    turnToHeading(relPos.heading() + PI);
  } else {
    Vector relPos = Vector(-lineX + position.x, lineY - position.y);
    turnToHeading(relPos.heading() + PI);
  }
  return true;
}

void getPositionUsingSonar() {
  float d1 = leftForwardSonar.distance(inches) + leftSonarOffsetX;
  float d2 = leftBackSonar.distance(inches) + leftSonarOffsetX;

  float dLeft = (d1 + d2) / 2.0;
  float dNormal = forwardSonar.distance(inches);

  float headingEstimate = atan2f(d1 - d2, leftSonarOffsetDistance);

  float xEstimate = wallX - (dLeft * cosf(headingEstimate));
  float yEstimate = (dNormal * cosf(headingEstimate)) + forwardSonarOffsetY;

  position.x = xEstimate;
  position.y = yEstimate;

  float closest = getClosestCardinal();

  printf("hs: %f, ", heading * Rad2Deg);
  heading = closest - headingEstimate;

  printf("hc: %f, he: %f, hf: %f\n", closest * Rad2Deg, headingEstimate * Rad2Deg, heading * Rad2Deg);
}

float getClosestCardinal() {
  return (int)roundf(heading * 2 / PI) * PI / 2;
}
