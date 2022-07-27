#include "UserDetection_Task.h"
#include "Arduino.h"
#include "EnableInterrupt.h"
#include "SharedVariables.h"
#include <avr/sleep.h>

UserDetection::UserDetection(int pinPir){
    this->pir = new PirImpl(pinPir);
    this->pirPin = pinPir;
}

void UserDetection::init(int period){
    Task::init(period);
}

void UserDetection::tick(){
    if(!pir->isReady()){
        return;
    }
    if(!idle){
        return;
    }
    if(millis() - idleTime >= T_IDLE && !this->pir->isDetected()){
        enableInterrupt(this->pirPin, &UserDetection::wakeUp, RISING);

        set_sleep_mode(SLEEP_MODE_PWR_DOWN);
        sleep_enable();
        sleep_mode();
        sleep_disable();

        disableInterrupt(this->pirPin);
        idleTime = millis();
    }
}

void UserDetection::wakeUp(){
}