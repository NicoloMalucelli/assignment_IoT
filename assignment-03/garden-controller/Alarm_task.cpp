#include "Alarm_Task.h"
#include "SharedVariables.h"

AlarmController::AlarmController(){

}

void AlarmController::init(int period){
    Task::init(period);
}

void AlarmController::tick(){
    if(temp == 5 && irrigationSpeed == 0){
        mode = "ALARM";
    }
}