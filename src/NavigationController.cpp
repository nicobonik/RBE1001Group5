#include "NavigationController.h"



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
  float yEstimate = dNormal + forwardSonarOffsetY;

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

void moveTo(NavNode node) {
  if(node.speed > 0) setSpeed(node.speed);
  Vector relPos = (node.pos - position) * (node.reverse? -1 : 1);
  printf("x:%f, y:%f, h:%f, rh: %f, d: %f\n", relPos.x, relPos.y, relPos.heading() * Rad2Deg, heading * Rad2Deg, relPos.len());
  turnLeft(relPos.heading() - heading);
  //Do odometry
  driveStraight(relPos.len() * (node.reverse? -1 : 1));
  //Do Odometry
  if(node.heading < 360) {
    turnLeft(node.heading * Deg2Rad - heading);
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

