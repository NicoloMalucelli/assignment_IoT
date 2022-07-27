#include "Serial_IO_Task.h"
#include <ArduinoJson.h>
#include "SharedVariables.h"
#include "MsgService.h"

#include "Arduino.h"

Serial_IO::Serial_IO(){
}

void Serial_IO::init(int period){
    Task::init(period);
}

void Serial_IO::tick(){
    ///*
    if(MsgService.isMsgAvailable()) {
        Msg* msg = MsgService.receiveMsg();
        String s = msg->getContent();//"AUTO;4;5;";
        delete msg;
        
        String toSend = "";

        toSend += "mode:" + String(mode);

        toSend += ";irrigator:" + String(irrigationSpeed) + ";";

        if(l1On){
            toSend += "l1:on;";
        } else {
            toSend += "l1:off;";
        }

        if(l2On){
            toSend += "l2:on;";
        } else {
            toSend += "l2:off;";
        }

        toSend += "l3:" + String(l3Value);
        toSend += ";l4:" + String(l4Value);
        //toSend += ";light:" + String(light);
        

        Serial.println(toSend);
        
        String tmp = "";
        int c = 0;
        for(int i = 0; i < s.length(); i++){
            if(s[i] == *";"){
                switch(c){
                    case 0:
                        light = tmp.toInt();
                        break;  
                    case 1:
                        temp = tmp.toInt();  
                        break;
                }
                tmp = "";
                c++;
            } else {
                tmp += s[i];
            }
        }
    }
}