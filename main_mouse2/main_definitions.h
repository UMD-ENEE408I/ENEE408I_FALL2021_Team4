#ifndef MAIN_DEFINITION   //This header file is just an extension of the main_mouse file.
#define MAIN_DEFINITION

//RTOS stuff
#include <mbed.h>
using namespace mbed;
using namespace rtos;
//Threads. Create a new one for each new function you want to run in parallel
Thread t1;
Thread t2;

//CURRENT STATE DEFINITIONS
#define IDLE_STATE  0
#define F_STATE     1   //drive forward
#define L_STATE     2   //turn left
#define R_STATE     3   //turn righ
#define U_STATE     4   //U-turn
#define END_STATE   5   //You've reach the goal
#define CAL_STATE   6   //Calibrate sensor


#endif
