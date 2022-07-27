#include "PirImpl.h"
#include "Arduino.h"

#define PIR_CALIBRATING_TIME 10000

PirImpl::PirImpl(int pin){
    this->pin = pin;
    this->initTime = millis();
    pinMode(pin, INPUT);
}

bool PirImpl::isDetected(){
    return digitalRead(pin) == HIGH;
}

bool PirImpl::isReady(){
    return millis() - this->initTime >= PIR_CALIBRATING_TIME;
}
