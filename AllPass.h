// AllPass.h
// -- AP filter
// cs246 11/19

#ifndef CS246_ALLPASS_H
#define CS246_ALLPASS_H


#include <complex>
#include "FrequencyFilter.h"


class AllPass : FrequencyFilter {
  public:
    AllPass(float f4=0, float R=44100);
    float operator()(float x) override;
    void setFrequency(float f4) override;
  private:
    float irate,
          quadrature;
    double a, x1, y1;
    void reset(void);
};


#endif

