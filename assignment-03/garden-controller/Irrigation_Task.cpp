#include "Irrigation_Task.h"
#include "SharedVariables.h"
#include "Arduino.h"

IrrigationController::IrrigationController(int pinServo){
    this->state = State::PAUSE;
    
    this->servo = new ServoMotorImpl(pinServo);
    this->increase = true;
    this->angle = 0;
    this->lastIrrigation = 0;
}

void IrrigationController::init(int period){
    Task::init(period);
}

void IrrigationController::tick(){
    this->updateState();
    this->doActivities();
}

void IrrigationController::updateState(){
    switch (this->state)
    {
        case State::PAUSE:
            if(mode == "AUTO"){
                if(light < 2 && (millis() - this->lastIrrigation) >= IRRIGATION_PAUSE){
                    this->servo->on();
                    this->state = State::ACTIVE;
                    this->irrigationStartTime = millis();
                }
            } else if(mode == "MANUAL"){
                if(irrigationSpeed > 0){
                    this->servo->on();
                    this->state = State::ACTIVE;
                    this->irrigationStartTime = millis();
                }
            }
            
            break;
        case State::ACTIVE:
            if((mode == "AUTO" && (millis() - this->irrigationStartTime) >= IRRIGATION_DURATION) ||
                (mode == "MANUAL" && irrigationSpeed == 0)){
                this->servo->off();
                this->state = State::PAUSE;
                this->lastIrrigation = millis();
                irrigationSpeed = 0;
            }
    }
}

void IrrigationController::doActivities(){
    switch (this->state)
    {
        case State::PAUSE:
            break;
        case State::ACTIVE:
            if(mode == "AUTO"){
                irrigationSpeed = temp;
            }
            
            if(increase){
                this->angle += irrigationSpeed;
            } else {
                this->angle -= irrigationSpeed;
            }

            if(this->angle < 0){
                this->increase = true;
                this->angle = 0;
            } else if (this->angle > 180){
                this->increase = false;
                this->angle = 180;
            }

            this->servo->setPosition(this->angle);
            //delay(3);//delay(15); 1.67ms per degree

            break;
    }
}