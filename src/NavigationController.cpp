#include "NavigationController.h"

const float wallX = 47,
            wallY = 72.5,
            rampX = 12.75,
            rampY = 26.75,
            rampSonarLength = 20, //Get
            leftSonarOffsetX = 7.875,
            leftForwardSonarOffsetY = 1.75,
            leftBackwardSonarOffsetY = 7.875,
            leftSonarOffsetDistance = leftBackwardSonarOffsetY - leftForwardSonarOffsetY,
            leftSonarMidOffset = leftForwardSonarOffsetY + leftSonarOffsetDistance / 2,
            forwardSonarOffsetY = 1.5,
            intakeLineSensorOffset = -8.75, //Get
            lineHalfWidth = 1,
            lineX = wallX - 9.75,
            lineY = 18.25;

bool onRight = true;

RotatedBounds collisionBounds = RotatedBounds(Vector(), Vector(), Vector(), 0);

bool navDone = true;
int nodeIndex = 0, nodesSize = 0, navFlag = 0;
NavNode nodes[10];

void InitNavigation(NavNode* n, int size) {
  nodesSize = size;
  for(int i = 0; i < nodesSize; i++){
    nodes[i] = n[i];
  }
  nodeIndex = 0;
  navDone = false;
  navFlag = 0;
}

bool IsNavActive() {
  return !navDone;
}

void StopNavigation() {
  navDone = true;
  nodesSize = 0;
  navFlag = 0;
  nodeIndex = 0;
}

bool getStartPosition() { // we start with the intake towards the wall, end with the intake away from the center
  position.y = lineY;
  position.x = wallX - rightSonarOffsetX - rightSonar.distance(inches);
  onRight = position.x > 0;
  ADriveStraight(-lineY);
  while(intakeRightLine.value(percent) > 60) if(isADriveDone()) return false;
  driveStraight(lineHalfWidth + intakeLineSensorOffset);
  turnLeft(90 * (onRight? 1 : -1) * Deg2Rad);
  heading = (onRight? PI : 0); // use sonar to get rotation
  return true;
}

void getPositionUsingSonar() {
  float d1 = leftForwardSonar.distance(inches) + leftSonarOffsetX;
  float d2 = leftBackSonar.distance(inches) + leftSonarOffsetX;

  float dLeft = (d1 + d2) / 2.0;
  float dNormal = forwardSonar.distance(inches);

  float headingEstimate = atan2f(d1 - d2, leftSonarOffsetDistance);

}

void moveTo(NavNode node) {
  if(node.speed > 0) setSpeed(node.speed);
  Vector relPos = (node.pos - position) * (node.reverse? -1 : 1);
  printf("x:%f, y:%f, h:%f, rh: %f, d: %f\n", relPos.x, relPos.y, relPos.heading() * Rad2Deg, heading * Rad2Deg, relPos.len());
  turnLeft(relPos.heading() - heading);
  //Do odometry
  driveStraight(relPos.len() * (node.reverse? -1 : 1));
  //Do Odometry
  if(node.heading < 360) {
    turnLeft(node.heading - heading);
    //Do Odometry
  }
  if(node.pickup) {
    pickupBall(node.ball);
    //Do Odometry
  }
}

//Only interupts from sensors or timer can be run while this is active, if those can even be run
void Navigate() {
  if(navDone) nodeIndex = 0;
  for(;nodeIndex < nodesSize;nodeIndex++){
    moveTo(nodes[nodeIndex]);
  }
  StopNavigation();
}

//Async / won't wait to complete to return
//You need to repeatedly call it in order to progress, it shouldn't overrun if not called
void ANavigate() {
  if(!navDone) {
    if(isADriveDone() && nodeIndex < nodesSize) {
      switch(navFlag){
        case 0:
          if(nodes[nodeIndex].speed > 0) setSpeed(nodes[nodeIndex].speed);
          ATurnLeft(((nodes[nodeIndex].pos - position) * (nodes[nodeIndex].reverse ? -1 : 1)).heading() - heading);
          navFlag = 1;
          break;
        case 1:
          ADriveStraight((nodes[nodeIndex].pos - position).len() * (nodes[nodeIndex].reverse ? -1 : 1));
          if(nodes[nodeIndex].heading < 360) navFlag = 2;
          else if (nodes[nodeIndex].pickup) navFlag = 3;
          else goto indexANav;
          break;
        case 2:
          ATurnLeft(((position - nodes[nodeIndex].ball.pos)).heading() - heading);
          //pickupBall(nodes[nodeIndex].ball);
          goto indexANav;
        case 3:
          ATurnLeft(nodes[nodeIndex].heading - heading);
        default:
        indexANav:
          nodeIndex++;
          navFlag = 0;
          if(nodeIndex >= nodesSize) StopNavigation();
      }
    } else {
      StopNavigation();
    }
  }
}

void PrintNodes() {
  printf("node count: %d\n", nodesSize);
  for(int i = 0; i < nodesSize; i++) {
    printf("x: %f, y: %f, h: %f, speed: %f\n", nodes[i].pos.x, nodes[i].pos.y, nodes[i].heading, nodes[i].speed);
  }
}

