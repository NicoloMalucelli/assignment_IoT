#include "ProductSelection_Task.h"
#include "SharedVariables.h"
#include "DisplayImpl.h"
#include "Arduino.h"

#define MESSAGE_DURATION 5000

int selectedIndex;
bool selected;
bool idle;
long idleTime;

ProductSelectionTask::ProductSelectionTask(int pinUp, int pinDown, int pinMake, int pinPot){
    this->pin[Pin::B_UP] = pinUp;
    this->pin[Pin::B_DOWN] = pinDown;
    this->pin[Pin::B_MAKE] = pinMake;
    this->pin[Pin::POT] = pinPot;
    this->state = this->State::SELECTION;
    this->sugarLevel = 0;

    selectedIndex = 0;
    selected = false;
    idle = true;
    idleTime = 0;
}

void ProductSelectionTask::init(int period){
    Task::init(period);
    this->bUp = new ButtonImpl(pin[Pin::B_UP]);
    this->bDown = new ButtonImpl(pin[Pin::B_DOWN]);
    this->bMake = new ButtonImpl(pin[Pin::B_MAKE]);
    this->pot = new PotImpl(pin[Pin::POT]);
    disp->writeMsg(0, 0, (char*)"Ready");
    this->newSugarLevel = map(this->pot->getValue(), 0, 100, 0, 11);
}

void ProductSelectionTask::tick(){
    if(cantMake()){
        return;
    }
    updateState();
    doActivities();
}

void ProductSelectionTask::updateState(){
    switch(this->state)
    {
        case State::SELECTION:
            if(this->bMake->isPressed()){
                idle = false;
                if(pAvailable[selectedIndex] <= 0){
                    updateMsg();
                } else {
                    this->state = State::WAIT_FOR_DELIVERY;
                    selected = true;            
                }
            }
            break;
        case State::WAIT_FOR_DELIVERY:
            if(!selected){
                this->state = State::SELECTION;
                disp->writeMsg(0, 0, (char*)"Ready");
                idle = true;
                pAvailable[selectedIndex]--;
            }
            break;
    }
}

void ProductSelectionTask::doActivities(){
    switch(this->state)
    {
        case State::SELECTION:
            if(this->bUp->isPressed()){
                selectedIndex++;
                updateMsg();                
            }
            if(this->bDown->isPressed()){
                selectedIndex--;
                updateMsg();
            }
            if(!idle && millis() - idleTime >= MESSAGE_DURATION){
                idle = true;
                disp->writeMsg(0, 0, (char*)"Ready");
            }
            newSugarLevel = map(this->pot->getValue(), 0, 100, 0, 11);
            if(!idle && sugarLevel != newSugarLevel){
                sugarLevel = newSugarLevel;
                updateMsg();
            }
            break;
        case State::WAIT_FOR_DELIVERY:
            break;
    }
}

void ProductSelectionTask::updateMsg(){
    if(selectedIndex >= NUM_OF_BEVERAGES){
        selectedIndex = 0;
    } else if(selectedIndex < 0){
        selectedIndex = NUM_OF_BEVERAGES-1; 
    }
    idleTime = millis();
    idle = false;
    disp->writeMsg(0, 0, productsName[selectedIndex]);
    if(pAvailable[selectedIndex] <= 0){
        disp->writeMsg(1, 0, (char*)"no available", false);
    } else {
        displaySugar();
    }
}

void ProductSelectionTask::displaySugar(){
    char sugar[10];
    for(int i = 0; i < 10; i++){
        if(i < sugarLevel){
            sugar[i] = *"O";
        } else{
            sugar[i] = *"";
        }
    }
    disp->writeMsg(1, 0, (char*)"sugar:", false);
    disp->writeMsg(1, 6, sugar, false);
}

bool ProductSelectionTask::cantMake(){
    return (initializing || checking || quantAssistance || tempAssistance);
}