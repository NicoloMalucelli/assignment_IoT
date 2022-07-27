#include "SelfTest_Task.h"
#include "SharedVariables.h"
#include "Arduino.h"

bool tempAssistance;
int nSelfTest;

SelftTestTask::SelftTestTask(int pinTemp, int pinServo){
    this->temp = new TemperatureSensorImpl(pinTemp);
    this->servo = new ServoMotorImpl(pinServo);

    tempAssistance = false;
    nSelfTest = 0;
}

void SelftTestTask::init(int period){
    Task::init(period);
    this->state = State::WAIT_FOR_CHECK;
    this->lastCheckTime = millis();
}

void SelftTestTask::tick(){
    this->updateState();
    this->doActivities();
}

void SelftTestTask::updateState(){
    switch(this->state){
        case State::WAIT_FOR_CHECK:
            if(!selected && millis() - this->lastCheckTime >= T_CHECK){
                checking = true;
                this->state = State::CHECK_A;
                this->servo->on();
                this->angle = 0;
            }
            break;
        case State::CHECK_A:
            if(this->angle > 180){
                this->state = State::CHECK_B;
            }
            break;
        case State::CHECK_B:
            {
                if(this->angle < 0){
                    float t = this->temp->getTemperature();
                    this->servo->off();
                    checking = false;
                    nSelfTest++;
                    if(t < T_MIN || t > T_MAX){
                        disp->writeMsg(0, 0, (char*)"ASSISTANCE");
                        this->state = State::WAIT_FOR_ASSISTANCE;
                        tempAssistance = true;
                    } else {
                        this->state = State::WAIT_FOR_CHECK;
                        this->lastCheckTime = millis();
                    }
                }
                break;
            }
        case State::WAIT_FOR_ASSISTANCE:
            if(!tempAssistance){
                this->state = State::WAIT_FOR_CHECK;
                this->lastCheckTime = millis();
            }
            break;
    }
}

void SelftTestTask::doActivities(){
    switch(this->state){
        case State::WAIT_FOR_CHECK:
            //do nothing
            break;
        case State::CHECK_A:
            this->servo->setPosition(angle);
            this->angle += 3;
            break;
        case State::CHECK_B:
            this->servo->setPosition(angle);
            this->angle -= 3;
            break;
        case State::WAIT_FOR_ASSISTANCE:
            //do nothing
            break;
    }
}