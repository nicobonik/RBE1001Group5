#include "PID.h"
#include <math.h>

float clip(float val, float min, float max) {
  return fminf(fmaxf(val, min), max);
}

float PID::update(float error) {
  float p = coeffs.Kp * error;
  integralAcc += (error - prevError) / 2.0;

  float limMinInt, limMaxInt;
  limMinInt = fmin(p, coeffs.max);
  limMaxInt = fmax(p, coeffs.min);

  p = clip(p, coeffs.min, coeffs.max);
  integralAcc = clip(integralAcc, limMinInt, limMaxInt);

  float d = coeffs.Kd * (error - prevError);

  float rawOutput = p + integralAcc + d;

  prevError = error;
  return rawOutput;
}