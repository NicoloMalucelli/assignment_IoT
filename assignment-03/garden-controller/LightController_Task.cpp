#include "LightController_Task.h"
#include "SharedVariables.h"

#include "LightImpl.h"
#include "DimmerLightImpl.h"

#include "Arduino.h"

LightController::LightController(int pinL1, int pinL2, int pinL3, int pinL4){
    this->l1 = new LightImpl(pinL1);
    this->l2 = new LightImpl(pinL2);
    this->l3 = new DimmerLightImpl(pinL3);
    this->l4 = new DimmerLightImpl(pinL4);
}

void LightController::init(int period){
    Task::init(period);
}

void LightController::tick(){
    if(String(mode) == "AUTO"){
        if(light <= 4){
            this->l1->turnOn();
        } else {
            this->l1->turnOff();
        }

        if(light <= 3){
            this->l2->turnOn();
        } else {
            this->l2->turnOff();
        }

        this->l3->setIntensity(100*(7-(light-1))/7);
        this->l4->setIntensity(100*(7-(light-1))/7);

        l1On = this->l1->isOn();
        l2On = this->l2->isOn();
        l3Value = this->l3->getIntensity();
        l4Value = this->l4->getIntensity();

    } else if(mode == "MANUAL"){
        if(l1On){
            this->l1->turnOn();
        } else {
            this->l1->turnOff();
        }

        if(l2On){
            this->l2->turnOn();
        } else {
            this->l2->turnOff();
        }

        l3->setIntensity(l3Value);
        l4->setIntensity(l4Value);
    }
}