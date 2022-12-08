#include "NavigationController.h"

const float wallX = 20, //Get
            wallY = 20, //Get
            rampX = 7, //Get
            rampY = 10, //Get
            rampSonarLength = 20, //Get
            rightSonarOffsetX = 6, //Get
            rightSonarOffsetY = -4.25, //Get
            forwardSonarOffsetY = 1.5, //Get
            dropoffDistance = 0; //Get

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

void getStartPosition() {
  while(true) {
    float err = forwardRightLine.value(percent) - forwardLeftLine.value(percent);
    if(fabsf(err) < 5 && forwardRightLine.value(percent) < 30) {
      driveRPM(60, 0);
    } else if(fabsf(err) > 5) {
      driveRPM(30, err);
    } else break;
  }
  heading = 0;
  position.y = wallY - forwardSonarOffsetY - forwardSonar.distance(inches);
  position.x = wallX - rightSonarOffsetX - rightSonar.distance(inches);
  Controller1.Screen.print(position.x);
  Controller1.Screen.print(", ");
  Controller1.Screen.print(position.y);
}

void moveTo(NavNode node) {
  if(node.speed > 0) setSpeed(node.speed);
  Vector relPos = (node.pos - position) * (node.reverse? -1 : 1);
  printf("x:%f, y:%f, h:%f, rh: %f, d: %f\n", relPos.x, relPos.y, relPos.heading() * Rad2Deg, heading * Rad2Deg, relPos.len());
  turnLeft(relPos.heading() - heading);
  driveStraight(relPos.len() * (node.reverse? -1 : 1));
  if(node.heading < 360) turnLeft(node.heading - heading);
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

