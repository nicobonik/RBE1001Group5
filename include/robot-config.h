using namespace vex;

extern brain Brain;

using signature = vision::signature;

// VEXcode devices
extern controller Controller1;
extern sonar forwardSonar;
extern sonar leftForwardSonar;
extern line intakeLeftLine;
extern line intakeRightLine;
extern signature camera__YELLOWBALL;
extern signature camera__REDBALL;
extern signature camera__BLUEBALL;
extern signature camera__SIG_4;
extern signature camera__SIG_5;
extern signature camera__SIG_6;
extern signature camera__SIG_7;
extern vision camera;
extern motor intakeMotor;
extern motor leftMotor;
extern motor rightMotor;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );