#ifndef __TASK__
#define __TASK__

class Task{
    public:
        virtual void init(int period){
            this->period = period;
            elapsedTime = 0;
        }

        bool checkTime(int scheduleTime){
            elapsedTime += scheduleTime;
            if(elapsedTime >= period){
                elapsedTime = 0;
                return true; //this->tick();
            }
            return false;
        }

        virtual void tick() = 0;

    private:
        int period;
        int elapsedTime; //time elapsed from last tick
};

#endif