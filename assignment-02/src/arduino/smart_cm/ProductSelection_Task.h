#ifndef __PRODUCT_SELECTION_TASK__
#define __PRODUCT_SELECTION_TASK__

#include "Task.h"
#include "ButtonImpl.h"
#include "PotImpl.h"

class ProductSelectionTask: public Task {

    public:
        ProductSelectionTask(int pinUp, int pinDown, int pinMake, int pinPot);
        void init(int period);
        void tick();
        void updateState();
        void doActivities();
        void updateMsg();
        void displaySugar();
        bool cantMake();

    private:
        enum State {SELECTION, WAIT_FOR_DELIVERY};
        enum Pin {B_UP, B_DOWN, B_MAKE, POT};

        State state;

        int pin[4];
        int sugarLevel;
        int newSugarLevel;
        Button* bUp;
        Button* bDown;
        Button* bMake;
        Pot* pot;

};
#endif