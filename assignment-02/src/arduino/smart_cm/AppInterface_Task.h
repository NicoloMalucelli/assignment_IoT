#ifndef __APP_INTERFACE_TASK__
#define __APP_INTERFACE_TASK__

#include "Task.h"
#include "SharedVariables.h"

class AppInterfaceTask: public Task {

    public:
        AppInterfaceTask();
        void init(int period);
        void tick();
        bool receive();
        void send();

    private:
        int lastPAvailable[NUM_OF_BEVERAGES];
        int lastNSelfTest;

};
#endif