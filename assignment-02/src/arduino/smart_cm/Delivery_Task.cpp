#include "Delivery_Task.h"
#include "SharedVariables.h"
#include "Arduino.h"

DeliveryTask::DeliveryTask(int pinServo, int pinEcho, int pinTrigger){
    this->servo = new ServoMotorImpl(pinServo);
    this->sonar = new SonarImpl(pinEcho, pinTrigger);
    this->state = State::WAIT_FOR_MAKING;
}

void DeliveryTask::init(int period){
    Task::init(period);
}

void DeliveryTask::tick(){
    this->updateState();
}

void DeliveryTask::updateState(){
    switch (this->state)
    {
        case State::WAIT_FOR_MAKING:
            if(done){
                timeAtProductReady = millis();
                char tmp[16];
                strcpy(tmp, "The ");
                strcat(tmp, productsName[selectedIndex]);
                disp->writeMsg(0, 0, tmp);
                disp->writeMsg(1, 0, (char*)"is ready", false);
                this->state = State::WAIT_FOR_DELIVERY;
            }
            break;
        case State::WAIT_FOR_DELIVERY:
            if(this->sonar->getDistance() > 0.4 || (millis() - timeAtProductReady >= T_TIMEOUT)){
                this->servo->on();
                this->angle = 180; 
                this->state = State::DELIVERY;
            }
            break;
        case State::DELIVERY:
            if(this->angle >= 0){
                this->servo->setPosition(this->angle);
                this->angle -= 3;
            } else {
                this->servo->off();
                done = false;
                selected = false;
                this->state = State::WAIT_FOR_MAKING;
                delay(3);
            }
            break;
    }
}