#pragma once

#include "vex.h"
#include <math.h>
#include "DataTypes.h"
#include "DriveFunctions.h"
#include "PID.h"

using namespace vex;

const int BallListSize = 8, MaxHold = 5;
extern Ball redBalls[], blueBalls[], holding[];
extern int redBallCount, blueBallCount, holdCount;

void addBall(Ball& ball);
void addRed(Ball& ball);
void addBlue(Ball& ball);

Ball& tryAddBall(Vector p, int type);
Ball& tryAddRed(Vector p);
Ball& tryAddBlue(Vector p);

void removeBall(Ball& ball);
void removeRed(Ball& ball);
void removeBlue(Ball& ball);

bool pickupBall(Ball& ball);

bool doBallCheck();

void DepositBalls(int num, bool rev = false);
void PrepareDeposit(bool rev = false);
void APrepareDeposit(bool rev = false);

void PrintBalls();