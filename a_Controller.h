#ifndef Controller_h
#define Controller_h

#include <Arduino.h>
#include "XBOXRECV.h"


class Controller
{
  public:  
    // CONSTRUCTORS
    Controller();
    Controller(USB *pUsb); // Default pin selection.

    void Task();


    float LeftJoystick;
    float RightJoystick;
    float TriggerAggregate;
    int LR2Aggregate;

    int DPadLeftRight;
    int APress;
    int YPress;
    int BPress;
    int XPress;
    int StartPress;


   private:

    XBOXRECV Xbox;
    const int joystickMinThresh = 5000;
    USB dummyXboxPointer;

    int reverseMultiplier;

};

#endif