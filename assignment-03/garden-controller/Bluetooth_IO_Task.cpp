#include "Arduino.h"
#include "Bluetooth_IO_Task.h"
#include "MsgServiceBT.h"

MsgServiceBT msgService(12, 13);

Bluetooth_IO::Bluetooth_IO(){
}

void Bluetooth_IO::init(int period){
    Task::init(period);
    msgService.init();
}

void Bluetooth_IO::tick(){
    if (msgService.isMsgAvailable()) {
        Msg* msg = msgService.receiveMsg();
        String s = msg->getContent();
        //Serial.println("received: " + s);
        delete msg;

        String tmp = "";
        String k = "";
        bool kFound = false;
        for(int i = 0; i < s.length(); i++){
            if(s[i] == *":"){
                kFound = true;
            } else if(s[i] == *";"){
                if(k == "mode"){
                    mode = tmp;
                }else if(k == "l1"){
                    if(tmp == "on"){
                        l1On = true;
                    }else{
                        l1On = false;
                    }
                }else if(k == "l2"){
                    if(tmp == "on"){
                        l2On = true;
                    }else{
                        l2On = false;
                    }
                }else if(k == "l3"){
                    l3Value = tmp.toInt();
                }else if(k == "l4"){
                    l4Value = tmp.toInt();
                }else if(k == "irr"){
                    irrigationSpeed = tmp.toInt();
                }
                tmp = "";
                k = "";
                kFound = false;
            } else {
                if(!kFound){
                    k += s[i];
                }else{
                    tmp += s[i];
                }
            }
        }
    }

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
    
    //Serial.println("sent: " + toSend);
    msgService.sendMsg(toSend);
}