#include "SonarImpl.h"
#include "Arduino.h"

const float vs = 331.45 + 0.62*20;

SonarImpl::SonarImpl(int echoPin, int triggerPin){
    this->echoPin = echoPin;
    this->triggerPin = triggerPin;

    pinMode(triggerPin, OUTPUT);
    pinMode(echoPin, INPUT);  
}

float SonarImpl::getDistance(){
    digitalWrite(this->triggerPin,LOW);
    delayMicroseconds(3);
    digitalWrite(this->triggerPin,HIGH);
    delayMicroseconds(5);
    digitalWrite(this->triggerPin,LOW);
    
    /* ricevi l’eco */
    float tUS = pulseIn(this->echoPin, HIGH);
    float t = tUS / 1000.0 / 1000.0 / 2;
    float d = t*vs;
    return d;
}