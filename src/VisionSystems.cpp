#include "VisionSystems.h"

//10.25 for 1ft width
//12 from front of robot, 11 from camera
//3.75 from ground

//theta_y = arctan(3.75/11) = 18.8247 degrees
//degreesPerPixel = 0.17113
//pixelHeight = 220

//theta_x = arctan(6/10.25) = 30.3432 degrees
//degreesPerPixel = 0.19327
//pixelWidth = 314

int holdCount;

const float pickupDrive = -5.5;

signature signatures[] { camera__REDBALL, camera__BLUEBALL };

Bounds pickupCheckBounds = Bounds(Vector(135, 160), 35, 35);
int holdOffset = 0;

PID forwardPID(PIDCoefficients(20, 0, 10, -60, 60)), turnPID(PIDCoefficients(3, 0, 2, -30, 30));

bool pickupBall(int type) {
  intakeMotor.setVelocity(120, rpm);
  int c = 0;
  Vector checkCenter = pickupCheckBounds.center();
  while(true){
    camera.takeSnapshot(signatures[type]);
    if (camera.objectCount > 0 && camera.largestObject.centerY > 60)
    {
      float x = camera.largestObject.centerX,
            y = camera.largestObject.centerY,
            halfwidth = camera.largestObject.width / 2,
            halfheight = camera.largestObject.height / 2;
      Bounds ballBounds = Bounds(x - halfwidth, y - halfheight, x + halfwidth, y + halfheight);
      //printf("x: %f, y: %f\n", x, y);
      if(pickupCheckBounds.contains(ballBounds)) break;
      Vector error = ballBounds.center() - checkCenter;
      //printf("\tx: %f, y: %f\n", checkCenter.x, checkCenter.y);
      //printf("\tx: %f, y: %f\n", error.x, error.y);
      //Call odometry
      driveRPM(forwardPID.update(error.y), turnPID.update(-error.x));
      c = 0;
    } else {
      c++;
      if(c > 50) {
        printf("BREAK\n");
        driveRPM(0, 0);
        return false;
      }
    }
  }
  driveRPM(0, 0);
  setSpeed(1.5);
  Controller1.Screen.clearScreen();
  Controller1.Screen.print("Speed Set To: %f", RPM);
  printf("Speed Set To: %f\n", RPM);
  driveStraight(pickupDrive);
  intakeMotor.spinFor(vex::directionType::fwd, intakeDegPerBall, degrees, true); //Add something to make sure it picked up the ball?
  holdCount++;
  return true;
}

int getLargestBall() {
  int largest = 0;
  int maxArea = 0;
  for(int i = 1; i < 2; i++) {
    camera.takeSnapshot(signatures[i]);
    int area = camera.largestObject.width * camera.largestObject.height;
    if(area > maxArea) {
      maxArea = area;
      largest = i;
    }
  }
  return largest;
}

void DepositBalls() {
  intakeMotor.spinFor(reverse, intakeDegPerBall * holdCount, degrees, true);
}
