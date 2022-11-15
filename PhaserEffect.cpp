// PhaserEffect.h
// -- implementation for simple phaser effect
// cs246 11/19
// Joey Laybourn

#include "PhaserEffect.h"
#include <string>
#include <iostream>

#define PI 3.1415926

PhaserEffect::PhaserEffect(float R) :
  Control(5, "Phaser"),
  ap1(0, R),
  ap2(0, R),
  sweep_center(50000),
  sweep_range(0),
  sweep_rate(0),
  feedback(0),
  mix(50),
  irate(R),
  lfo_argument(0),
  lfo_dargument(0),
  var1(0),
  var2(0),
  var3(0),
  var4(0)
{
  show();
  setRange(0,  50, 5000); 
  setLabel(0, "Sweep Center: 25000 Hz");
  setValue(0, 5000 / 2);

  setRange(1, 0, 2400);
  setLabel(1, "Sweep Range: 0 cents");

  setRange(2, 0, 1000);
  setLabel(2, "Sweep Rate: 0.00 Hz");

  setRange(3, 0, 1000);
  setLabel(3, "Feedback: 0.0%");

  setRange(4, 0, 1000);
  setLabel(4, "Mix: 50.0%");
  setValue(4, 500);
}

PhaserEffect::~PhaserEffect(void)
{
  show(false);
}

float PhaserEffect::operator()(float x)
{
  ap1.setFrequency(sweep_center + (sweep_range * (1.f + sin((2.f * PI * var3) / irate))));
  ap2.setFrequency(sweep_center + (sweep_range * (1.f + sin((2.f * PI * var3) / irate))));
  var3 += sweep_rate;
  float y = ap1(var1 + x);
  y += ap2(y);
  var1 = y * (feedback / 2000);
  return ((1.f - (mix / 1000)) * x) + ((mix / 1000) * y);
}

void PhaserEffect::valueChanged(unsigned index, int value)
{
  std::string newLabel;
  switch(index)
  {
    case(0):
    {
      sweep_center = value;
      newLabel = "Sweep Center: " + std::to_string(value) + " Hz";
      ap1.setFrequency(sweep_center + (sweep_range * sin(2.f * PI * var3)));
      ap2.setFrequency(sweep_center + (sweep_range * sin(2.f * PI * var3)));
      var3 += sweep_rate;
      setLabel(0, newLabel.c_str());
      break;
    }
    case(1):
    {
      sweep_range = value;
      newLabel = "Sweep Range: " + std::to_string(value) + " cents";
      setLabel(1, newLabel.c_str());
      break;
    }
    case(2):
    {
      sweep_rate = float(value) / 100;
      newLabel = "Sweep Rate: " + std::to_string(value / 100) + '.' + std::to_string(value % 100) + " Hz";
      setLabel(2, newLabel.c_str());
      break;
    }
    case(3):
    {
      feedback = value;
      newLabel = "Feedback: " + std::to_string(value / 10) + '.' + std::to_string(value % 10) + '%';
      setLabel(3, newLabel.c_str());
      break;
    }
    case(4):
    {
      mix = value;
      newLabel = "Mix: " + std::to_string(value / 10) + '.' + std::to_string(value % 10) + '%';
      setLabel(4, newLabel.c_str());
      break;
    }
  }
}