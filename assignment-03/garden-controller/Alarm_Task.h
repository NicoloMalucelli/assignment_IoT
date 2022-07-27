#ifndef __ALARM_TASK__
#define __ALARM_TASK__

#include "Task.h"

class AlarmController: public Task {

    public:
        AlarmController();
        void init(int period);
        void tick();
};
#endif