#include "Scheduler.h"
#include "SharedVariables.h"

#include "Serial_IO_Task.h"
#include "LightController_Task.h"
#include "Irrigation_Task.h"
#include "Bluetooth_IO_Task.h"
#include "Alarm_Task.h"

Scheduler sched;

int temp;
int light;
String mode;

int irrigationSpeed;
bool l1On;
bool l2On;
int l3Value;
int l4Value;

void setup() {

  temp = 1;
  light = 1;
  mode = "AUTO";
  irrigationSpeed = 0;

  l1On = true;
  l2On = true;
  l3Value = 100;
  l4Value = 100;


  Serial.begin(250000);
  sched.init(5);

  Task* serial_io = new Serial_IO();
  serial_io->init(100);
  sched.addTask(serial_io);

  Task* alarmController = new AlarmController();
  alarmController->init(10);
  sched.addTask(alarmController);

  Task* lightController = new LightController(7, 8, 6, 5);
  lightController->init(50);
  sched.addTask(lightController);

  Task* irrigationController = new IrrigationController(11);
  irrigationController->init(50);
  sched.addTask(irrigationController);

  Task* bluetooth_io = new Bluetooth_IO();
  bluetooth_io->init(500);
  sched.addTask(bluetooth_io);

}

void loop() {
  sched.schedule();
}