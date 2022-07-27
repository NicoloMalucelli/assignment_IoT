#ifndef __LIGHT_IMPL__
#define __LIGHT_IMPL__

#include "Light.h"

class LightImpl: public Light {
 
public: 
  LightImpl(int pin);
  void turnOn();
  void turnOff();
  bool isOn();

private:
  int pin;
  bool on;

};

#endif