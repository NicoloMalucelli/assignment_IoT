#include "DisplayImpl.h"
#include "Arduino.h"

DisplayImpl::DisplayImpl(){
    lcd.init();
    lcd.backlight();
}

void DisplayImpl::writeMsg(int row, int col, char* text, bool clear){
    if(clear){
        lcd.clear();
    }
    lcd.setCursor(col, row);
    lcd.print(text);
}

void DisplayImpl::clearDisplay(){
    lcd.clear();
}