#include "DimmerLightImpl.h"
#include "Arduino.h"

DimmerLightImpl::DimmerLightImpl(int pin){
  this->pin = pin;
  this->intensity = 0;
  pinMode(pin, OUTPUT);  
}

void DimmerLightImpl::setIntensity(int intensity){
    if(intensity > 100){
      intensity = 100;
    }
    if(this->intensity == intensity){
      return;
    }
    int val = (253/100.0)*intensity;
    analogWrite(this->pin, val);
    this->intensity = intensity;
}

int DimmerLightImpl::getIntensity(){
    return this->intensity;
}