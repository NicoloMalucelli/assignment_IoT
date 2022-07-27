#include "EnableInterrupt.h"
#include <avr/sleep.h>
#include <time.h>
#include <stdlib.h>

/* pins */
#define RED_PIN 9
#define LED_PIN_1 13
#define LED_PIN_2 12
#define LED_PIN_3 11
#define LED_PIN_4 10

#define BUTTON_1 2
#define BUTTON_2 3
#define BUTTON_3 4
#define BUTTON_4 5

#define POTENTIOMETER A0

/* states */
#define SLEEPING 0
#define WAIT_FOR_START 1
#define BALL_BOUNCING 2
#define BALL_STOP 3
#define LOOSE 4

#define BTN_DEBOUNCING_TIME 200
#define TIME_BEFORE_RESTART 10000
#define TIME_BEFORE_SLEEP 10000

#define RED_LED_FADE_TIME 30

#define START_S 5
#define START_T2 5000
#define S_MAX 20
#define T2_MIN 350
#define T1_MIN 1000
#define T1_MAX 10000

int currentState;

int redLedCurrent;
int redLedCurrentIncrement;

long lastButtonPressedTime = 0;

long ballTimer; //used to calculate elapsed time when game is in state BALL_BOUNCING or BALL_STOP
float s; //light changing per seconds
int lightChangeTime;
int t1; //t1
int t2; //t2
int f[] = {103, 105, 107, 110, 115, 118, 122, 130};
int l;

int lighIndex;
bool increase;
long lastLightChange = 0;
int lights[] = {LED_PIN_1, LED_PIN_2, LED_PIN_3, LED_PIN_4};
int buttons[] = {BUTTON_1, BUTTON_2, BUTTON_3, BUTTON_4};
int num_lights = sizeof(lights) / sizeof(lights[0]);

long looseTime;
int points;

void initGame(){
    s = START_S;
    t1 = rand()%(T1_MAX-T1_MIN)+T1_MIN;
    t2 = START_T2;
    lightChangeTime = int(1000 / s);
    currentState = WAIT_FOR_START;
    lastButtonPressedTime = millis();
    lighIndex = -1;
    increase = true;
    points = 0;
    redLedCurrent = 0;
    redLedCurrentIncrement = 10;

    enableInterrupt(BUTTON_1, startGame, RISING);

    Serial.println("Welcome to the Catch the Bouncing Led Ball Game. Press Key T1 to Start");
}

void setup(){
    srand(time(NULL));
    for(int i = 0; i < num_lights; i++){
        pinMode(lights[i], OUTPUT);
    }
    for(int i = 0; i < num_lights; i++){
        pinMode(buttons[i], INPUT);
    }
    pinMode(RED_PIN, OUTPUT);
    pinMode(POTENTIOMETER, INPUT);
    Serial.begin(9600);
    initGame();    
}

void loop(){
    switch(currentState){
        case SLEEPING:
            set_sleep_mode(SLEEP_MODE_PWR_DOWN);
            sleep_enable();
            sleep_mode();
            currentState = WAIT_FOR_START;
            for(int i = 0; i < num_lights; i++){
                disableInterrupt(buttons[i]);   
            };
            enableInterrupt(BUTTON_1, startGame, RISING);
            sleep_disable();
            break;
        case WAIT_FOR_START:
            /* check sleeping condition */
            if(millis() - lastButtonPressedTime >= TIME_BEFORE_SLEEP){
                disableInterrupt(BUTTON_1);
                currentState = SLEEPING;
                analogWrite(RED_PIN, 0);
                void* interrupts[] = {wakeUp, wakeUp, wakeUp, wakeUp};
                for(int i = 0; i < num_lights; i++){
                    enableInterrupt(buttons[i], interrupts[i], RISING);   
                };
                break;
            }
            
            /* update red led brightness */
            if(millis() - lastLightChange >= RED_LED_FADE_TIME){
                lastLightChange = millis();
                redLedCurrent += redLedCurrentIncrement;
                if(redLedCurrent >= 255){
                    redLedCurrent = 255;
                    redLedCurrentIncrement *= -1;
                } else if(redLedCurrent <= 0){
                    redLedCurrent = 0;
                    redLedCurrentIncrement *= -1;
                }
                analogWrite(RED_PIN, redLedCurrent);
            }
            break;
        case BALL_BOUNCING:
            if(millis() - ballTimer >= t1){
                currentState = BALL_STOP;
                ballTimer = millis();
                break;
            }
            /* update ball position */
            if(millis() - lastLightChange >= lightChangeTime){
                lastLightChange = millis();
                changeLight();
            }
            break;
        case BALL_STOP:
            if(millis() - ballTimer > t2){
                loose();
            }
            break;
        case LOOSE:
            if(millis() - looseTime > TIME_BEFORE_RESTART){
                initGame();
            }
            break;
    }
   
}

void changeLight(){
    if(lighIndex != -1){
        digitalWrite(lights[lighIndex], LOW);
    }
    if(increase){
        lighIndex++;
        if(lighIndex == num_lights-1){
            increase = false;
        }
    } else {
        lighIndex--;
        if(lighIndex == 0){
            increase = true;
        }
    }
    digitalWrite(lights[lighIndex], HIGH);
}

void wakeUp(){
    lastButtonPressedTime = millis();
}

void startGame(){
    if(isBouncing()){
        return;
    }
    disableInterrupt(BUTTON_1);
    void* interrupts[] = {clickBtn1, clickBtn2, clickBtn3, clickBtn4};
    for(int i = 0; i < num_lights; i++){
        disableInterrupt(buttons[i]);
        enableInterrupt(buttons[i], interrupts[i], RISING);   
    }
    analogWrite(RED_PIN, 0);

    l = map(analogRead(POTENTIOMETER), 0, 1024, 1, 9);

    currentState = BALL_BOUNCING;
    Serial.println((String)"Difficulty: " + l);
    Serial.println("Go!");
    ballTimer = millis();
}

void buttonClick(int buttonID){
    if(isBouncing()){
        return;
    }
    if(currentState == BALL_STOP && digitalRead(lights[buttonID-1]) == HIGH){
        points++;
        Serial.println((String)"New point! Score: " + points);
        currentState = BALL_BOUNCING;
        ballTimer = millis();
        setTimeAndSpeed();
    } else {
        loose();
    }
}

bool isBouncing(){
    bool bounce = (millis() - lastButtonPressedTime) <= BTN_DEBOUNCING_TIME;
    if(!bounce){
        lastButtonPressedTime = millis();
    }
    return bounce;
}

void loose(){
    digitalWrite(lights[lighIndex], LOW);
    for(int i = 0; i < num_lights; i++){
        disableInterrupt(buttons[i]);   
    };
    currentState = LOOSE;
    looseTime = millis();
    Serial.println((String)"Game Over. Final Score: " + points);
}

void setTimeAndSpeed(){
    t1 = rand()%(T1_MAX-T1_MIN)+T1_MIN;

    s *= f[l-1]/100.0;
    if(s > S_MAX){
        s = S_MAX;
    }
    lightChangeTime = int(1000 / s);

    t2 /= f[l-1]/100.0;
    if(t2 < T2_MIN){
        t2 = T2_MIN;
    }
}

void clickBtn1(){
    noInterrupts();
    buttonClick(1);
    interrupts();
}

void clickBtn2(){
    noInterrupts();
    buttonClick(2);
    interrupts();
}

void clickBtn3(){
    noInterrupts();
    buttonClick(3);
    interrupts();
}

void clickBtn4(){
    noInterrupts();
    buttonClick(4);
    interrupts();
}