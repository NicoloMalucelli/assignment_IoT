#ifndef __PRODUCT_MAKING_TASK__
#define __PRODUCT_MAKING_TASK__

#include "Task.h"
#include "ServoMotorImpl.h"

class ProductMakingTask: public Task {

    public:
        ProductMakingTask(int pinServo);
        void init(int period);
        void tick();
        void updateState();
        void doActivities();

    private:
        enum State {WAIT_FOR_SELECTION, MAKING, WAIT_FOR_DELIVERY};
        State state;
        ServoMotor* servo;

        int servoAngle;

};
#endif