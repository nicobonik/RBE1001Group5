/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       C:\Users\Mason Sakai                                      */
/*    Created:      Thu Dec 01 2022                                           */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Controller1          controller                    
// forwardSonar         sonar         A, B            
// leftForwardSonar     sonar         C, D            
// intakeLeftLine       line          G               
// intakeRightLine      line          H               
// camera               vision        9               
// intakeMotor          motor         8               
// leftMotor            motor         1               
// rightMotor           motor         10              
// leftBackSonar        sonar         E, F            
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"
#include "NavigationController.h"
#include "VisionSystems.h"
#include <math.h>

using namespace vex;

void testPickupBall() {
  int l = getLargestBall();
  Ball ball = Ball(Vector(), -1, l);
  pickupBall(ball);
}

void NavigationLoop() {
  StopNavigation();
  while(true) {
    //printf("Nav Loop...");
    Navigate();
  }
}

void BallSensorLoop() {
  while(true) {
    //printf("Sensor Loop...");
    
    //doBallCheck();
  }
}

int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  printf("Starting...\n");
  
  forwardSonar.distance(inches);
  rightSonar.distance(inches);
  wait(1, seconds);

  printf("\tSonar Wait Done\n");

  Controller1.ButtonRight.pressed(testPickupBall);
  
  heading = PI / 2;

  getStartPosition();

  testPickupBall();

  turnLeft(PI / 2 * (onRight? -1 : 1));

  testPickupBall();

  /*tryAddBlue(Vector(3, 7));
  tryAddRed(Vector(3, 7));
  tryAddRed(Vector(15, 6));
  tryAddRed(Vector(6, 7)); // Should overlap
  PrintBalls();*/

  printf("Starting Thread");
  //thread(NavigationLoop).detach();
  //thread(BallSensorLoop).detach();
  printf("Threads Active");

  NavNode nodes[] = {NavNode(Vector(5,3)), NavNode(Vector(5, 10)), NavNode(Vector(-20, 5), -1, 361, true), NavNode(Vector(0,0), -1, 0)};
  printf("Starting test navigation");
  InitNavigation(nodes, sizeof(nodes) / sizeof(NavNode));

  printf("Done\n");
}


