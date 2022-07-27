#ifndef __BT_TASK__
#define __BT_TASK__

#include "Task.h"
#include "SharedVariables.h"

class Bluetooth_IO: public Task {

    public:
        Bluetooth_IO();
        void init(int period);
        void tick();

};
#endif