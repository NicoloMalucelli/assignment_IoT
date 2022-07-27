#ifndef __SELF_TEST_TASK__
#define __SELF_TEST_TASK__

#include "Task.h"
#include "ServoMotorImpl.h"
#include "TemperatureSensorImpl.h"

#define T_CHECK 180000
#define T_MIN 17
#define T_MAX 24

class SelftTestTask: public Task {

    public:
        SelftTestTask(int pinTemp, int pinServo);
        void init(int period);
        void tick();
        void updateState();
        void doActivities();

    private:
        enum State {WAIT_FOR_CHECK, CHECK_A, CHECK_B, WAIT_FOR_ASSISTANCE};
        State state;
        ServoMotor* servo;
        TemperatureSensor* temp;
        long lastCheckTime;
        int angle;

};
#endif