#pragma once

#include "vex.h"
using namespace vex;
#include "DriveFunctions.h"
#include "VisionSystems.h"
#include <math.h>


extern const float wallX, wallY,
            rampX, rampY,
            rampSonarLength,
            rightSonarOffsetX,
            rightSonarOffsetY,
            forwardSonarOffsetY,
            dropoffDistance,
            intakeDegPerBall;

extern bool onRight;

void InitNavigation(NavNode n[], int size);
void StopNavigation();

bool IsNavActive();

bool getStartPosition();
void getPositionUsingLine();

void getPositionUsingSonar();

void moveTo(NavNode node);

void Navigate();
void ANavigate();

void PrintNodes();
