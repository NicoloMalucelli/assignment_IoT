#ifndef __SHARED_VARS__
#define __SHARED_VARS__

#define NUM_OF_BEVERAGES 3
#define BEVERAGES_QUANTITES 30

#include "string.h"
#include "Display.h"

extern bool initializing;
extern bool done;
extern bool selected;
extern bool checking;

extern bool idle;
extern int nSelfTest;
extern int selectedIndex;
extern char* productsName[NUM_OF_BEVERAGES];
extern int pAvailable[NUM_OF_BEVERAGES];

extern bool quantAssistance;
extern bool tempAssistance;
extern long idleTime;

extern Display* disp;

#endif
