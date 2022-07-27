#ifndef __USER_DETECTION_TASK__
#define __USER_DETECTION_TASK__

#include "Task.h"
#include "PirImpl.h"

#define T_IDLE 60000

class UserDetection: public Task {

    public:
        UserDetection(int pinPir);
        void init(int period);
        void tick();
        static void wakeUp();

    private:
        Pir* pir;
        int pirPin;

};
#endif