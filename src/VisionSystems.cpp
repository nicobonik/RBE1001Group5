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

Ball redBalls[BallListSize], blueBalls[BallListSize], yellowBalls[BallListSize], holding[MaxHold];
int redBallCount, blueBallCount, yellowBallCount, holdCount;

const float camAngle = -30, camAngleOffsetX = 0, camX = 0, camY = -4.75, camZ = 12, //GET
            degreesPerPixelX = 0.19327, degreesPerPixelY = 0.17113,
            locationCheckTolerance = 4, pickupDrive = -5.5, driveKp = 10, turnKp = -5;
const int centerX = 157, centerY = 110,
          holdCapacity = 5;

signature signatures[] { camera__YELLOWBALL, camera__REDBALL, camera__BLUEBALL };

Bounds pickupCheckBounds = Bounds(Vector(157, 160), 35, 35);
int holdOffset = 0;

static Ball EMPTY_BALL = Ball(Vector(), -1, -1);

int ballsIndexOf(Ball balls[], int size, Ball ball) {
  if(ball.index >= 0 && (balls[ball.index].pos - ball.pos).len() < locationCheckTolerance) return ball.index;
  for(int i = 0;i < size;i++){
    if((balls[i].pos - ball.pos).len() < locationCheckTolerance)
      return i;
  }
  return -1;
}

void addBall(Ball& ball) {
  switch(ball.type){
    case 0:
      addYellow(ball);
      break;
    case 1:
      addRed(ball);
      break;
    case 2:
      addBlue(ball);
      break;
  }
}
void addRed(Ball& ball) {
  if(redBallCount == BallListSize) {
    redBalls[0].index = -1;
    for(int i = 0;i < BallListSize - 1;i++){
      redBalls[i] = redBalls[i + 1];
      redBalls[i].index = i;
    }
    redBalls[BallListSize - 1] = ball;
    ball.index = BallListSize - 1;
  } else {
    ball.index = redBallCount;
    redBalls[redBallCount] = ball;
    redBallCount++;
    printf("Added Red Ball: x: %f, y: %f, i: %d\n", ball.pos.x, ball.pos.y, ball.index);
  }
}
void addBlue(Ball& ball) {
  if(blueBallCount == BallListSize) {
    blueBalls[0].index = -1;
    for(int i = 0;i < BallListSize - 1;i++){
      blueBalls[i] = blueBalls[i + 1];
      blueBalls[i].index = i;
    }
    blueBalls[BallListSize - 1] = ball;
    ball.index = BallListSize - 1;
  } else {
    ball.index = blueBallCount;
    blueBalls[blueBallCount] = ball;
    blueBallCount++;
  }
}
void addYellow(Ball& ball) {
  if(yellowBallCount == BallListSize) {
    yellowBalls[0].index = -1;
    for(int i = 0;i < BallListSize - 1;i++){
      yellowBalls[i] = yellowBalls[i + 1];
      yellowBalls[i].index = i;
    }
    yellowBalls[BallListSize - 1] = ball;
    ball.index = BallListSize - 1;
  } else {
    ball.index = yellowBallCount;
    yellowBalls[yellowBallCount] = ball;
    yellowBallCount++;
  }
}

Ball& tryAddBall(Vector p, int type) {
  switch(type){
    case 0:
      return tryAddYellow(p);
    case 1:
      return tryAddRed(p);
    case 2:
      return tryAddBlue(p);
  }
  return EMPTY_BALL;
}
Ball& tryAddRed(Vector p) {
  Ball ball = Ball(p, -1, 1);
  int ind = ballsIndexOf(redBalls, redBallCount, ball);
  if(ind > -1) {
    redBalls[ind].pos = p;
    return redBalls[ind];
  }
  ind = redBallCount;
  addRed(ball);
  return redBalls[ind];
}
Ball& tryAddBlue(Vector p) {
  Ball ball = Ball(p, -1, 2);
  int ind = ballsIndexOf(blueBalls, blueBallCount, ball);
  if(ind > -1) {
    blueBalls[ind].pos = p;
    return blueBalls[ind];
  }
  ind = blueBallCount;
  addBlue(ball);
  return blueBalls[ind];
}
Ball& tryAddYellow(Vector p) {
  Ball ball = Ball(p, -1, 0);
  int ind = ballsIndexOf(yellowBalls, yellowBallCount, ball);
  if(ind > -1) {
    yellowBalls[ind].pos = p;
    return yellowBalls[ind];
  }
  ind = yellowBallCount;
  addYellow(ball);
  return yellowBalls[ind];
}

void removeBall(Ball& ball) {
  switch(ball.type){
    case 0:
      removeYellow(ball);
      break;
    case 1:
      removeRed(ball);
      break;
    case 2:
      removeBlue(ball);
      break;
  }
}
void removeRed(Ball& ball) {
  int ind = ballsIndexOf(redBalls, redBallCount, ball);
  ball.index = -1;
  if(ind < 0) return;
  for(int i = ind; i < redBallCount - 1; i++) {
    redBalls[i] = redBalls[i + 1];
    redBalls[i].index = i;
  }
  redBalls[redBallCount - 1] = EMPTY_BALL;
  redBallCount--;
}
void removeBlue(Ball& ball) {
  int ind = ballsIndexOf(blueBalls, blueBallCount, ball);
  ball.index = -1;
  if(ind < 0) return;
  for(int i = ind; i < blueBallCount - 1; i++) {
    blueBalls[i] = blueBalls[i + 1];
    blueBalls[i].index = i;
  }
  blueBalls[blueBallCount - 1] = EMPTY_BALL;
  blueBallCount--;
}
void removeYellow(Ball& ball) {
  int ind = ballsIndexOf(yellowBalls, yellowBallCount, ball);
  ball.index = -1;
  if(ind < 0) return;
  for(int i = ind; i < yellowBallCount - 1; i++) {
    yellowBalls[i] = yellowBalls[i + 1];
    yellowBalls[i].index = i;
  }
  yellowBalls[yellowBallCount - 1] = EMPTY_BALL;
  yellowBallCount--;
}

bool pickupBall(Ball& ball) {
  intakeMotor.setVelocity(120, rpm);
  APrepareDeposit(true);
  int c = 0;
  Vector checkCenter = pickupCheckBounds.center();
  while(true){
    camera.takeSnapshot(signatures[ball.type]);
    if (camera.objectCount > 0 && camera.largestObject.centerY > 80)
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
      driveRPM(error.y * driveKp, error.x * turnKp);
      c = 0;
    } else {
      c++;
      if(c > 50) {
        driveRPM(0, 0);
        printf("BREAK");
        return false;
      }
    }
  }
  driveRPM(0, 0);
  setSpeed(1.5);
  Controller1.Screen.clearScreen();
  Controller1.Screen.print("Speed Set To: %f", RPM);
  printf("Speed Set To: %f", RPM);
  driveStraight(pickupDrive);
  intakeMotor.spinFor(vex::directionType::fwd, intakeDegPerBall, degrees, true); //Add something to make sure it picked up the ball?
  //removeBall(ball);
  //holding[holdCount] = ball;
  //holdCount++;
  return true;
}

Vector getBallPosition(vision::object detected) {
  float theta_x = (detected.centerX - centerX) * degreesPerPixelX,
        theta_y = (detected.centerY - centerY) * degreesPerPixelY;
  Vector pos = Vector();
  float m = camZ / fabsf(sinf(camAngle + theta_y));
  pos.y = camY - m * cosf(camAngle + theta_y) ;
  pos.x = camX - m * tanf(camAngleOffsetX + theta_x);
  pos = pos.rotate(heading);
  pos += position;
  return pos;
}

// Returns true if new ball is detected
bool doBallCheck() {
  bool foundNew = false;
  for(int type = 0; type < 3; type++){
    camera.takeSnapshot(signatures[type]);
    for(int i = 0;i < camera.objectCount;i++){
      tryAddBall(getBallPosition(camera.objects[i]), type);
    }
  }
  return foundNew;
}

void DepositBalls(int num, bool rev) {
  if(num > holdCount) num = holdCount;
  PrepareDeposit(rev);
  intakeMotor.spinFor(rev? reverse : forward, intakeDegPerBall * num, degrees, true);
  if(rev) {
    for(int i = holdCount - num - 1; i < holdCount; i++) {
      holding[i] = EMPTY_BALL;
    }
    holdCount -= num;
  } else {
    holdOffset += num;
    for(int i = 0; i < holdCount - num; i++){
      if(i + num >= MaxHold)
      holding[i] = holding[i + num];
      holding[i + num] = EMPTY_BALL;
    }
    holdCount -= num;
  }
}

void PrepareDeposit(bool rev) {
  int error = 0;
  if(rev) {
    error = -holdOffset;
    holdOffset = 0;
  } else {
    error = holdCapacity - holdCount - holdOffset;
    holdOffset = holdCapacity - holdCount;
  }
  if(error == 0) return;
  intakeMotor.spinFor(vex::directionType::fwd, intakeDegPerBall * error, degrees, true);
}
void APrepareDeposit(bool rev) {
  int error = 0;
  if(rev) {
    error = -holdOffset;
    holdOffset = 0;
  } else {
    error = holdCapacity - holdCount - holdOffset;
    holdOffset = holdCapacity - holdCount;
  }
  if(error == 0) return;
  intakeMotor.spinFor(vex::directionType::fwd, intakeDegPerBall * error, degrees, false);
}

void PrintBallArray(Ball balls[], int size) {
  for(int i = 0;i < size;i++){
    printf("\ti: %d, x: %f, x: %f, ind: %d, t: %d\n", i, balls[i].pos.x, balls[i].pos.y, balls[i].index, balls[i].type);
  }
}
void PrintBalls() {
  printf("Yellow:\n");
  PrintBallArray(yellowBalls, yellowBallCount);
  printf("Red:\n");
  PrintBallArray(redBalls, redBallCount);
  printf("Blue:\n");
  PrintBallArray(blueBalls, blueBallCount);
  printf("Holding:\n");
  PrintBallArray(holding, holdCount);
}