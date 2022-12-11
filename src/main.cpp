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
  if(l < 0) return false;
  return pickupBall(l);
}

int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  printf("Starting...\n");
  
  forwardSonar.distance(inches);
  forwardSonar.distance(inches);
  wait(3, seconds);

  printf("\tSonar Wait Done\n");

  getStartPosition();

  if(testPickupBall()) {
    getPositionUsingSonar();
    //moveTo(NavNode(Vector(lineX * (onRight? 1 : -1), lineY), -1, true));
    printf("P2-t\n");
  } else {
    getPositionUsingSonar();
    //moveTo(NavNode(Vector(lineX * (onRight? 1 : -1), lineY), -1, true));
    printf("P2-f\n");
  }
  

  turnToHeading(PI * 3 / 2);
  
  driveStraight(lineY - 8);

  while(holdCount < holdCapacity){
    if(forwardSonar.distance(inches) > wallY + intakeLineSensorOffset - 10) break;
    while(!testPickupBall()){
      if(forwardSonar.distance(inches) > wallY + intakeLineSensorOffset - 10) break;
      driveStraight(-2);
    }
    getPositionUsingSonar();
    turnToHeading(PI * 3 / 2);
  }

  getPositionUsingSonar();
  
  turnToHeading(PI / 2);

  ADriveStraight(-wallY - intakeLineSensorOffset + 10);

  while(intakeRightLine.value(percent) > 60){ if(isADriveDone()) return 0; }

  driveStraight(0.01);
  
  getPositionUsingSonar();

  driveStraight(-4 + intakeLineSensorOffset);

  if(onRight) turnToHeading(0);
  else        turnToHeading(PI);

  ADriveStraight(-lineX);

  while(intakeRightLine.value(percent) > 60){ if(isADriveDone()) return 0; }

  driveStraight(0.01);

  turnLeft(75 * Deg2Rad * (onRight? -1 : 1));
  
  driveStraight(-20);
  do {
    followLine();
    wait(0.05, seconds);
  } while(leftBackSonar.distance(inches) < rampSonarLength);
  driveStraight(-.005);
  DepositBalls();

  printf("Done\n");
}


