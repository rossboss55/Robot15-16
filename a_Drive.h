#ifndef Drive_h
#define Drive_h

#include <Arduino.h>
#include "a_Robot.h"
#include "PID_v1.h"

class Drive 
{
  public:  
    // CONSTRUCTORS
    Drive(Robot *p); // Default pin selection.

    void Task();
    void init();




   
};

#endif