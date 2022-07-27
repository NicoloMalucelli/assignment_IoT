#ifndef __LIGHT_CONTROLLER_TASK__
#define __LIGHT_CONTROLLER_TASK__

#include "Task.h"
#include "SharedVariables.h"
#include "Light.h"
#include "DimmerLight.h"

class LightController: public Task {

    public:
        LightController(int pinL1, int pinL2, int pinL3, int pinL4);
        void init(int period);
        void tick();

    private:
        Light* l1;
        Light* l2;
        DimmerLight* l3;
        DimmerLight* l4;
};
#endif