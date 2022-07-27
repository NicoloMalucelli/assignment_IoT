#ifndef __SONAR_IMPL__
#define __SONAR_IMPL__

#include "Sonar.h"

class SonarImpl: public Sonar{
    public:
        SonarImpl(int echoPin, int triggerPin);
        float getDistance();
    private:
        int echoPin;
        int triggerPin;
};

#endif