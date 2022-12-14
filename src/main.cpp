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

// Finds and attempts to pick up the largest ball it can find
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

  //Picking up the first ball
  if(!pickupBall(onRight? 0 : 1))
    driveStraight(12 - (position - Vector(lineX * (onRight? 1: -1), lineY)).len());

  if(onRight)
    getPositionUsingSonar();
  
  //Turning towards the back wall
  turnToHeading(PI * 3 / 2);
  
  driveStraight(lineY - 8);

  // Loops through moving forward and looking for balls until...
  //   ...we have as many balls as we can hold...
  while(holdCount < holdCapacity){
    // ...or it goes to far from the front wall 
    if(forwardSonar.distance(inches) > wallY + intakeLineSensorOffset - 20) break;
    while(!testPickupBall()){
      if(forwardSonar.distance(inches) > wallY + intakeLineSensorOffset - 20) break;
      driveStraight(-2);
    }
    if(onRight) {
      getPositionUsingSonar();
      turnToHeading(PI * 3 / 2);
    }
  }
  if(onRight)
    getPositionUsingSonar();
  
  // Turning towards the front wall
  turnToHeading(PI / 2, !onRight);
  if(!onRight) {
    getPositionUsingSonar();
    turnToHeading(PI / 2);
  }

  // Drive towards the front wall until you hit the line or go to far (stop the program)
  ADriveStraight(-wallY - intakeLineSensorOffset + 10);
  while(intakeRightLine.value(percent) > 60){ if(isADriveDone()) return 0; }
  driveStraight(0.01);
  if(!onRight) {
    getPositionUsingSonar();
    turnToHeading(PI / 2);
  }
  driveStraight(-4 + intakeLineSensorOffset);

  // Turn towards center
  if(onRight) turnToHeading(0);
  else        turnToHeading(PI);

  // Drive towards the center until the line is hit or it goes to far
  ADriveStraight(-lineX);
  while(intakeRightLine.value(percent) > 60){ if(isADriveDone()) return 0; }
  driveStraight(0.01);

  // Start the climb up the ramp
  turnLeft(75 * Deg2Rad * (onRight? -1 : 1));
  driveStraight(-20);
  // If the checking sonar is already too high, go until it isn't
  while(leftBackSonar.distance(inches) > rampSonarLength){
    followLine();
    wait(0.05, seconds);
  }
  // Check if we've hit the top of the ramp using the sonar
  do {
    followLine();
    wait(0.05, seconds);
  } while(leftBackSonar.distance(inches) < rampSonarLength);
  driveStraight(-.005);
  // Deposit
  DepositBalls();

  printf("Done\n");
}


