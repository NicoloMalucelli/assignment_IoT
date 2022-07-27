#include "AppInterface_Task.h"
#include "MsgService.h"
#include "Display.h"
#include <string.h>

AppInterfaceTask::AppInterfaceTask(){
    for(int i = 0; i < NUM_OF_BEVERAGES; i++){
        this->lastPAvailable[i] = 0;
    }
    this->lastNSelfTest = 0;
}

void AppInterfaceTask::init(int period){
    Task::init(period);
}

void AppInterfaceTask::tick(){
    if(this->receive()){
        this->send();
    }
}

bool AppInterfaceTask::receive(){
    //bool ret;
    if (MsgService.isMsgAvailable()) {
        Msg* msg = MsgService.receiveMsg();
        String s = msg->getContent();
        //ret = (s == "");

        String tmp = "";
        for(int i = 0; i < s.length(); i++){
            if(s[i] == *";"){
                if(tmp == "Ref"){
                    for(int i = 0; i < NUM_OF_BEVERAGES; i++){
                        pAvailable[i] = BEVERAGES_QUANTITES;
                    }
                    disp->writeMsg(0, 0, (char*)"Ready");
                    quantAssistance = false;
                } else if(tmp == "Res"){
                    tempAssistance = false;
                    disp->writeMsg(0, 0, (char*)"Ready");
                }
                tmp = "";
                continue;
            }
            tmp += s[i];
        }
        
        delete msg;
        return true;
    }
    //return ret;
    return false;
}

void AppInterfaceTask::send(){
    String msg = "";

    if((tempAssistance || quantAssistance)){
        msg += "STATE:assistance";
    } else if(idle) {
        msg += "STATE:idle";
    } else{
        msg += "STATE:working";
    }

    if(tempAssistance){
        msg += ";ASS:temp";
    }
    if(quantAssistance){
        msg += ";ASS:quant";
    }

    if(this->lastPAvailable[0] != pAvailable[0]){
        msg +=  ";COF:"; 
        msg += pAvailable[0];
        this->lastPAvailable[0] = pAvailable[0]; 
    }

    if(this->lastPAvailable[1] != pAvailable[1]){
        msg +=  ";TEA:"; 
        msg += pAvailable[1];
        this->lastPAvailable[1] = pAvailable[1]; 
    }

    if(this->lastPAvailable[2] != pAvailable[2]){
        msg += ";CHO:";
        msg += pAvailable[2];
        this->lastPAvailable[2] = pAvailable[2];
    }

    if(this->lastNSelfTest != nSelfTest){
        this->lastNSelfTest = nSelfTest;
        msg += ";TEST:";
        msg += this->lastNSelfTest;
    }

    MsgService.sendMsg(msg);
}