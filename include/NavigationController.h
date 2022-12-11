#pragma once

#include "vex.h"
using namespace vex;
#include "DriveFunctions.h"
#include "VisionSystems.h"
#include <math.h>


const float wallX = 47,
            wallY = 72.5,
            rampX = 12.75,
            rampY = 26.75,
            leftSonarOffsetX = 7.875,
            leftForwardSonarOffsetY = 1.75,
            leftBackwardSonarOffsetY = 7.875,
            leftSonarOffsetDistance = leftBackwardSonarOffsetY - leftForwardSonarOffsetY,
            leftSonarMidOffset = leftForwardSonarOffsetY + leftSonarOffsetDistance / 2,
            forwardSonarOffsetY = 1.5,
            intakeLineSensorOffset = -8.75,
            rampSonarLength = rampX - leftSonarOffsetX + 4,
            lineHalfWidth = 1,
            lineX = wallX - 9.75,
            lineY = 18.25;

extern bool onRight;

bool getStartPosition();

void getPositionUsingSonar();

float getClosestCardinal();
