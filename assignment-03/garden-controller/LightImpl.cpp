#include "LightImpl.h"
#include "Arduino.h"

LightImpl::LightImpl(int pin){
  this->pin = pin;
  this->on = false;
  pinMode(pin, OUTPUT);  
}

void LightImpl::turnOn(){
  if(this->on){
    return;
  }
  digitalWrite(this->pin, HIGH);
  this->on = true;
}

void LightImpl::turnOff(){
  if(!this->on){
    return;
  }
  digitalWrite(this->pin, LOW);
  this->on = false;
}

bool LightImpl::isOn(){
  return this->on;
}