#include "ProductMaking_Task.h"
#include "SharedVariables.h"
#include "Arduino.h"

bool done;

ProductMakingTask::ProductMakingTask(int pinServo){
    this->servo = new ServoMotorImpl(pinServo);
    this->state = State::WAIT_FOR_SELECTION;

    done = false;
}

void ProductMakingTask::init(int period){
    Task::init(period);
}

void ProductMakingTask::tick(){
    this->updateState();
    this->doActivities();
}

void ProductMakingTask::updateState(){
    switch (this->state)
    {
        case State::WAIT_FOR_SELECTION:
            if(selected){
                this->servoAngle = 0;
                this->servo->on();

                this->state = State::MAKING;
                disp->writeMsg(0, 0, (char*)"Making a");
                disp->writeMsg(1, 0, productsName[selectedIndex], false);
            }
            break;
        case State::MAKING:
            if(this->servoAngle > 180){
                this->state = State::WAIT_FOR_DELIVERY;
                this->servo->off();
                done = true;
            }
            break;
        case State::WAIT_FOR_DELIVERY:
            if(!done){
                this->state = State::WAIT_FOR_SELECTION;
            }
            break;
    }
}

void ProductMakingTask::doActivities(){
    switch (this->state)
    {
        case State::WAIT_FOR_SELECTION:
            //do nothing
            break;
        case State::MAKING:
            this->servo->setPosition(servoAngle++);
            //delay(3);//delay(15); 1.67ms per degree
            break;
        case State::WAIT_FOR_DELIVERY:
            //do nothing
            break;
    }
}