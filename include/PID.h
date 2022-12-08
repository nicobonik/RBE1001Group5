#pragma once
#include "vex.h"

typedef struct PIDCoefficients {
    float Kp;
    float Ki;
    float Kd;
    float max;
    float min;

    PIDCoefficients(float P, float I, float D, float MIN, float MAX): 
        Kp(P), Ki(I), Kd(D), max(MAX), min(MIN) {}

} PIDCoefficients;

class PID {
    
    public:
    PID(PIDCoefficients coefficients): coeffs(coefficients) {}
    float update(float error);

    //this would be used for when we want to use derivative of the measured val instead of derivative of error
    // float update(float target, float measurement);

    private:

    float prevError = 0;
    float integralAcc = 0;

    PIDCoefficients coeffs;

};
