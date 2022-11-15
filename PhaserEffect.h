// PhaserEffect.h
// -- interface for simple phaser effect
// cs246 11/19

#ifndef CS246_PHASEREFFECT_H
#define CS246_PHASEREFFECT_H


#include "AllPass.h"
#include "Filter.h"
#include "Control.h"


class PhaserEffect : public Filter,
                     private Control {
  public:
    PhaserEffect(float R=44100);
    ~PhaserEffect(void);
    float operator()(float x) override;
  private:
    AllPass ap1, ap2;
    float sweep_center,
          sweep_range,
          sweep_rate,
          feedback,
          mix,
          irate;
    double lfo_argument,
           lfo_dargument;
    void valueChanged(unsigned,int) override;
    float var1, var2, var3, var4;
};

#endif

