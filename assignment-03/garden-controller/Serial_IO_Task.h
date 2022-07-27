#ifndef __READER_TASK__
#define __READER_TASK__

#include "Task.h"
#include "SharedVariables.h"

class Serial_IO: public Task {

    public:
        Serial_IO();
        void init(int period);
        void tick();

};
#endif