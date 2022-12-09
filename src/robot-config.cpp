#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
controller Controller1 = controller(primary);
sonar forwardSonar = sonar(Brain.ThreeWirePort.A);
sonar rightSonar = sonar(Brain.ThreeWirePort.C);
line intakeLeftLine = line(Brain.ThreeWirePort.G);
line intakeRightLine = line(Brain.ThreeWirePort.H);
/*vex-vision-config:begin*/
signature camera__YELLOWBALL = signature (1, 0, 0, 0, 0, 0, 0, 3, 0);
signature camera__REDBALL = signature (2, 8153, 10867, 9510, -1361, -715, -1038, 3, 0);
signature camera__BLUEBALL = signature (3, -2009, -1471, -1740, 7831, 9831, 8831, 2.5, 0);
vision camera = vision (PORT9, 50, camera__YELLOWBALL, camera__REDBALL, camera__BLUEBALL);
/*vex-vision-config:end*/
motor intakeMotor = motor(PORT8, ratio18_1, false);
motor leftMotor = motor(PORT1, ratio18_1, true);
motor rightMotor = motor(PORT10, ratio18_1, false);

// VEXcode generated functions
// define variable for remote controller enable/disable
bool RemoteControlCodeEnabled = true;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void vexcodeInit( void ) {
  // nothing to initialize
}