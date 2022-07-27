#ifndef __DISPLAY__
#define __DISPLAY__

class Display{

    public: 
        virtual void writeMsg(int row, int col, char* text, bool clear = true) = 0;
        virtual void clearDisplay() = 0;

};

#endif
