#ifndef __CHECK_QUANTITIES_TASK__
#define __CHECK_QUANTITIES_TASK__

#include "Task.h"

class CheckQuantitiesTask: public Task {

    public:
        CheckQuantitiesTask();
        void init(int period);
        void tick();

};
#endif