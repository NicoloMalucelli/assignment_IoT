#include "Welcome_Task.h"
#include "SharedVariables.h"

#include "DisplayImpl.h"
#include "Arduino.h"

bool initializing;
char* productsName[NUM_OF_BEVERAGES];
int pAvailable[NUM_OF_BEVERAGES];

Display* disp;

WelcomeTask::WelcomeTask(){
    this->logging = false;

    /*shared vars*/
    initializing = true;

    productsName[0] = (char*) "COFFE";
    productsName[1] = (char*) "TEA";
    productsName[2] = (char*) "CHOCOLATE";
    pAvailable[0] = BEVERAGES_QUANTITES;
    pAvailable[1] = BEVERAGES_QUANTITES;
    pAvailable[2] = BEVERAGES_QUANTITES;
    disp = new DisplayImpl();
}

void WelcomeTask::init(int period){
    Task::init(period);
}

void WelcomeTask::tick(){
    if(!initializing){
        return;
    }
    if(!logging){
        logging = true;
        firstLogTime = millis();
        disp->writeMsg(0, 0, (char*)"Welcome");
    }
    if(millis() - this->firstLogTime >= WELCOME_MESSAGE_DURATION){
        disp->writeMsg(0, 0, (char*)"Ready");
        initializing = false;
    }
}