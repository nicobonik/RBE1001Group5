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

bool testPickupBall() {
  int l = getLargestBall();
  Ball ball = Ball(Vector(), -1, l);
  return pickupBall(ball);
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
  forwardSonar.distance(inches);
  wait(3, seconds);

  printf("\tSonar Wait Done\n");

  //Controller1.ButtonRight.pressed(testPickupBall);

  getStartPosition();

  printf("P1\n");

  if(testPickupBall()) {
    getPositionUsingSonar();
    //moveTo(NavNode(Vector(lineX * (onRight? 1 : -1), lineY), -1, true));
    printf("P2-t\n");
  } else {
    getPositionUsingSonar();
    //moveTo(NavNode(Vector(lineX * (onRight? 1 : -1), lineY), -1, true));
    printf("P2-f\n");
  }
  //getPositionUsingSonar();

  //return 0;
  

  turnToHeading(PI * 3 / 2);
  
  printf("P3\n");

  for(int n = 0;n < 2;n++){
    while(!testPickupBall()){
      driveStraight(-2);
    }
  }

  getPositionUsingSonar();

  printf("P4\n");

  turnToHeading(PI / 2);

  getPositionUsingSonar();
  
  printf("P5\n");

  ADriveStraight(position.x)
  

  
  /*driveStraight(-20);
  do {
    printf("dist: %f\n", leftBackSonar.distance(inches));
    followLine();
    wait(0.05, seconds);
  } while(leftBackSonar.distance(inches) < rampSonarLength);
  printf("dist: %f\n", leftBackSonar.distance(inches));
  driveStraight(0.01);
  //DepositBalls(holdCount);*/

  printf("Done\n");
}


