#ifndef __DELIVERY_MAKING_TASK__
#define __DELIVERY_MAKING_TASK__

#include "Task.h"
#include "ServoMotorImpl.h"
#include "SonarImpl.h"

#define T_TIMEOUT 10000

class DeliveryTask: public Task {

    public:
        DeliveryTask(int pinServo, int pinEcho, int pinTrigger);
        void init(int period);
        void tick();
        void updateState();

    private:
        enum State {WAIT_FOR_MAKING, WAIT_FOR_DELIVERY, DELIVERY};
        State state;
        ServoMotor* servo;
        Sonar* sonar;
        long timeAtProductReady;
        int angle;

};
#endif