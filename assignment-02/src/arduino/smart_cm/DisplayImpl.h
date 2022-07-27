#ifndef __DISPLAT_IMPL__
#define __DISPLAT_IMPL__

#include <LiquidCrystal_I2C.h>
#include "Display.h"

class DisplayImpl: public Display{
    public:
        DisplayImpl();
        virtual void writeMsg(int row, int col, char* text, bool clear = true);
        void clearDisplay();

    private:
        LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27,16,2); 
};

#endif
