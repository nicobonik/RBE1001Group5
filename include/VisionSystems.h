#pragma once

#include "vex.h"
#include <math.h>
#include "DataTypes.h"
#include "DriveFunctions.h"
#include "PID.h"

using namespace vex;

const int holdCapacity = 4;
extern int holdCount;

bool pickupBall(int type);

bool doBallCheck();
int getLargestBall();

void DepositBalls();