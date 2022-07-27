#ifndef __IRRIGATION_TASK__
#define __IRRIGATION_TASK__

#include "Task.h"
#include "ServoMotorImpl.h"

#define IRRIGATION_DURATION 5000
#define IRRIGATION_PAUSE 5000 

class IrrigationController: public Task {

    public:
        IrrigationController(int pinServo);
        void init(int period);
        void tick();
        void updateState();
        void doActivities();

    private:
        enum State {PAUSE, ACTIVE};
        State state;
        ServoMotor* servo;

        int angle;
        bool increase;
        long lastIrrigation;
        long irrigationStartTime;
};
#endif