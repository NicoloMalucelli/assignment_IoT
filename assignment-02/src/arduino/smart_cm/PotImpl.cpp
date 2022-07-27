#include "PotImpl.h"
#include "Arduino.h"

PotImpl::PotImpl(int pin){
    this->pin = pin;
    pinMode(pin, INPUT);
}

int PotImpl::getValue(){
    return map(analogRead(pin), 0, 1023, 0, 100);
}