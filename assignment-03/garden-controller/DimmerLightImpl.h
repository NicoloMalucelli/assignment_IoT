#ifndef __DIMMER_LIGHT_IMPL__
#define __DIMMER_LIGHT_IMPL__

#include "DimmerLight.h"

class DimmerLightImpl: public DimmerLight {
 
public: 
  DimmerLightImpl(int pin);
  virtual void setIntensity(int intensity);
  virtual int getIntensity();

private:
  int pin;
  int intensity;

};

#endif