#include "CheckQuantities_Task.h"
#include "SharedVariables.h"

bool checking;
bool quantAssistance;

CheckQuantitiesTask::CheckQuantitiesTask(){
    checking = false;
    quantAssistance = false;
}

void CheckQuantitiesTask::init(int period){
    Task::init(period);
}

void CheckQuantitiesTask::tick(){
    if(quantAssistance){
        return;
    }
    for(int i = 0; i < NUM_OF_BEVERAGES; i++){
        if(pAvailable[i] > 0){
            return;
        }
    }
    quantAssistance = true;
    disp->writeMsg(0, 0, (char*)"ASSISTANCE");
}