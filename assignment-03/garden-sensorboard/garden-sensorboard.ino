#include <WiFi.h>
#include <HTTPClient.h>
#include <math.h>

#include "LightImpl.h"
#include "LightSensorImpl.h"
#include "TemperatureSensorImpl.h"

#define UPDATE_TIME 100
#define PHOTO_RESISTOR_PIN 33
#define TEMP_RESISTOR_PIN 32
#define LED_PIN 16

int l;
int temp;
bool ledOn = true;
    
const char* ssid = "EOLO - FRITZ!Box 7430 YT";
const char* password = "41195728343377587300";
const char* serviceURI = "http://192.168.178.104:8124";

Light* light;
LightSensor* lightSensor;
TemperatureSensor* temperatureSensor;

void connectToWifi(const char* ssid, const char* password){
  int timeout_counter = 0;
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
    timeout_counter++;
    if(timeout_counter >= 30*2){
      ESP.restart();
    }
  }
  Serial.println(WiFi.localIP());
}

void sendData(String address, int light, int temp){
   HTTPClient http;
   http.begin(address + "/api/sendTempLight?temp=" + String(temp) + "&light=" + String(light));
   int ret = http.GET();
   http.end();
}

void updateModeValue(String address){
   HTTPClient http;
   http.begin(address + "/api/mode");
   
   int ret = http.GET();

   //Serial.println("ret: " + String(ret));
   if(ret != 200){
      Serial.println("error occurred while receiving datas");
      return;
   }
   String str = http.getString();
   if(str == "ALARM"){
      if(ledOn){
         ledOn = false;
         light->turnOff();
      }
   } else {
      if(!ledOn){
          ledOn = true;
          light->turnOn();
      }
   }
   http.end();
}


void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);
  connectToWifi(ssid, password);

  light = new LightImpl(LED_PIN);
  lightSensor = new LightSensorImpl(PHOTO_RESISTOR_PIN);
  temperatureSensor = new TemperatureSensorImpl(TEMP_RESISTOR_PIN);
}

void loop() {
  if(WiFi.status()== WL_CONNECTED){ 
    l = map(lightSensor->getValue(), 0, 4095, 1, 8);
    int Tc = int(temperatureSensor->getTemperature());
    Serial.println(Tc);
    if(Tc < 20){
      Tc = 20;
    } else if(Tc > 33){
      Tc = 33;
    }
    temp = map(int(Tc), 20, 32, 1, 5);
    sendData(serviceURI, l, temp); //update temp and light values on server
    
    updateModeValue(serviceURI);
    
    delay(UPDATE_TIME);
   }else{
    connectToWifi(ssid, password);
  }
}
