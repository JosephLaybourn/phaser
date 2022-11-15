// AllPass.cpp
// cs246 11/19

#include "AllPass.h"
#include <cmath>
using namespace std;


const double PI = 4.0*atan(1.0);


AllPass::AllPass(float f, float R)
    : irate(1/R),
      quadrature(f),
      x1(0),
      y1(0) {
  reset();
}


float AllPass::operator()(float x) {
  double y = a*(x + y1) - x1;
  x1 = x;
  y1 = y;
  return float(y);
}


void AllPass::setFrequency(float f) {
  quadrature = f;
  reset();
}


void AllPass::reset(void) {
  double t = tan(PI*quadrature*irate);
  a = (1-t)/(1+t);
}


