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
// rightSonar           sonar         C, D            
// forwardLeftLine      line          G               
// forwardRightLine     line          H               
// intakeLeftLine       line          E               
// intakeRightLine      line          F               
// camera               vision        9               
// intakeMotor          motor         8               
// leftMotor            motor         1               
// rightMotor           motor         10              
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"
#include "NavigationController.h"
#include "VisionSystems.h"
#include <math.h>

using namespace vex;

void testPickupBall() {
  pickupBall(redBalls[0]);
}

int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  printf("Starting...\n");

  Controller1.ButtonLeft.pressed(getStartPosition);

  Controller1.ButtonRight.pressed(testPickupBall);
  
  
  heading = PI / 2;

  tryAddBlue(Vector(3, 7));
  tryAddRed(Vector(3, 7));
  tryAddRed(Vector(15, 6));
  tryAddRed(Vector(6, 7)); // Should overlap
  PrintBalls();

  while(true) {
    doBallCheck();
  }

  NavNode nodes[] = {NavNode(Vector(5,3)), NavNode(Vector(5, 10)), NavNode(Vector(-20, 5), -1, 361, true), NavNode(Vector(0,0), -1, 0)};
  InitNavigation(nodes, sizeof(nodes) / sizeof(NavNode));
  //PrintNodes();
  //Navigate();
  /*do{
    printf("Teeeeeeeeeeeeeeeeeeeeeeee");
    ANavigate();
  } while(IsNavActive());*/

  printf("Done\n");
}


