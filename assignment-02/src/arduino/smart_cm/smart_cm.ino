#include "Scheduler.h"
#include "Task.h"
#include "ProductSelection_Task.h"
#include "ProductMaking_Task.h"
#include "Delivery_Task.h"
#include "CheckQuantities_Task.h"
#include "UserDetection_Task.h"
#include "SelfTest_Task.h"
#include "AppInterface_Task.h"
#include "Welcome_Task.h"

Scheduler sched;

void setup() {
    
    Serial.begin(1000000);
    sched.init(5);

    Task* welcome = new WelcomeTask();
    welcome->init(100);
    sched.addTask(welcome);

    Task* productSelection = new ProductSelectionTask(7,8,9, A1);
    productSelection->init(130);
    sched.addTask(productSelection);

    Task* productMaking = new ProductMakingTask(3);
    productMaking->init(55);
    sched.addTask(productMaking);

    Task* delivery = new DeliveryTask(3, 5, 6);
    delivery->init(5);
    sched.addTask(delivery);

    Task* checkQuantities = new CheckQuantitiesTask();
    checkQuantities->init(130);
    sched.addTask(checkQuantities);

    Task* userDetection = new UserDetection(2);
    userDetection->init(100);
    sched.addTask(userDetection);

    Task* selftTest = new SelftTestTask(A0, 3);
    selftTest->init(5);
    sched.addTask(selftTest);

    Task* appInterface = new AppInterfaceTask();
    appInterface->init(300);
    sched.addTask(appInterface);
}

void loop() {
    sched.schedule();
    //Serial.println(millis());
}
