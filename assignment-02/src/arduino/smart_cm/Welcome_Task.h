#ifndef __INIT_TASK__
#define __INIT_TASK__

#include "Task.h"

#define WELCOME_MESSAGE_DURATION 3000

class WelcomeTask : public Task{

    public:
        WelcomeTask();
        void init(int period);
        void tick();

    private:
        long firstLogTime;
        bool logging;

};

#endif