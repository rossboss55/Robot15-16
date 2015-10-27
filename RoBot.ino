

//Indirectly Used Library Includes
#include "XBOXRECV.h"
#include "DualVNH5019MotorShield.h"
#include "SimpleTimer.h"
#include "PID_v1.h"
#include "Encoder.h"

//Directly Used Library Includes
#include "a_Robot.h"
#include "a_Controller.h"

#include "a_Lift.h"
#include "a_Drive.h"
#include "a_Intake.h"
#include "a_Claw.h"
#include <Servo.h>




Robot Robot;
Controller Controller(&Robot.Usb);

Lift Lift(&Robot);
Drive Drive(&Robot);
Intake Intake(&Robot);
Claw Claw(&Robot);


SimpleTimer timer;

bool _autoRunning = false;
int _autoProgNum = 0;
int _autoInterval = 0;
bool _pauseForPID = false;

const int _maxAutonomousDuration = 60000;

unsigned long serialTime;
/*
double Setpoint, Input, Output;
PID myPID;
*/

void setup() {
	Serial.begin(115200);
	while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection


  Robot.init();
  Claw.init();
  Lift.init();
  Drive.init();

  Robot.SetLED(255,255,255); //Set LEDs to white

  timer.setInterval(20, runStuff);
  timer.setInterval(1000, manageLEDs);
  timer.setInterval(250, blink);
  
}

int secCount = 2; //default to 2, takes ~ 1.5 secs to boot
bool blinkLEDs = false;
int red, blu, grn;
bool blnkOn = true;
void manageLEDs()
{
  secCount++;



  //25 second auton
  //80 Second Drivers
  switch(secCount)
  {
    case 1:
      //LEDs green
        grn = 255;
        red = 0;
        blu = 0;
        Robot.SetLED(red,grn,blu);
        break;
    case 20:
      //Blink Green
        blinkLEDs = true;
        break;
    case 25:
      //LEDs White
        blinkLEDs = false;
        red = 255;
        blu = 255;
        grn = 255;
        Robot.SetLED(red,grn,blu);
        break;
    case 75:
      //Blink LEDs
        blinkLEDs = true;
        break;
    case 105:
      //LEDs White
        blinkLEDs = false;
        red = 255;
        blu = 255;
        grn = 255;
        Robot.SetLED(red,grn,blu);
        break;
    }
}
void blink()
{

  if(blinkLEDs)
  {
    if(blnkOn)
    {
      blnkOn = false;
      Robot.SetLED(red*.1,grn*.1,blu*.1);
    }
    else
    {
        blnkOn = true;
        Robot.SetLED(red,grn,blu);
    }
  }
}

void runStuff()
{
  Robot.Read();
  Controller.Task();

  MapRobot();

  Lift.Task();
  Drive.Task();
  Intake.Task();
  Claw.Task();

  Robot.Write();

  SerialReceiveMove();

  Serial.print(Robot.GetEncDriveLeft());
  Serial.print("\t");
  Serial.print(Robot.GetEncDriveRight());
  Serial.print("\t");
  Serial.print(Robot.GetEncLiftLeft());
  Serial.print("\t");
  Serial.print(Robot.GetEncLiftRight());
  Serial.print("\t"); 
  Serial.print(Robot.GetEncDriveRight());
  Serial.print("\t");
  Serial.print(Robot.GetEncDriveLeft());
  Serial.print("\t");
  Serial.println(Robot.GetEncClaw()); 


   /* Serial.print("LLiftI: ");
    Serial.print(digitalRead(19));
    Serial.print("\t LLiftD: ");
    Serial.print(digitalRead(29));
    Serial.print("\t RDriveI: ");
    Serial.print(digitalRead(3));
    Serial.print("\t RDriveD: ");
    Serial.println(digitalRead(23)); */

   // Serial.print(Robot.);
  // Serial.print(digitalRead(21));
    //Serial.println(digitalRead(49));






/*
  if(millis()>serialTime)
  {
    myPID = Drive.drivePID;
    Setpoint = Drive.driveSetPoint;
    Input = Drive.driveCurPos;
    Output = Drive.drivePIDOut;

    SerialReceive();
    SerialSend();
    serialTime+=500;
  }
*/
  if (_autoRunning)
  {
    switch(_autoProgNum)
    {
      case 1: 
      autoRedGoal();
      break;
      case 2:
      autoBlueGoal();
      break;
      case 3: 
        autoSkills();
      break;
      case 4:
        armOut();
      break;
  }
  if(!_pauseForPID)
      _autoInterval = _autoInterval + 20;

    //Safety switch in case forgot to call autoStop
  if(_autoInterval > _maxAutonomousDuration)
  {
      //autoStop();
  }
}


  //Grab Serial Input


}

void loop() {

  timer.run();

  

}

void MapRobot()
{
  Drive.LeftControllerSpeed = Controller.LeftJoystick;
  Drive.RightControllerSpeed = Controller.RightJoystick;

  Lift.ControllerSpeed = Controller.TriggerAggregate;

  Intake.ControllerSpeed = Controller.LR2Aggregate;

  Claw.ControllerClawPosition = Controller.APress;
  Claw.ControllerArmSpeed = Controller.DPadLeftRight;

  Claw.ControllerYPress = Controller.YPress;

  

  if(Controller.BPress == 1 && !_autoRunning)
  {
    _autoProgNum = 1;
    if(_autoProgNum != 0)
    {
      if(!_autoRunning)
      {
                  // Start Program
        autoStart();
    } 
    else 
    {
                  // Stop Program
        autoStop();
    }
}
}

  if(Controller.XPress == 1 && !_autoRunning)
  {
    _autoProgNum = 2;
    if(_autoProgNum != 0)
    {
      if(!_autoRunning)
      {
                  // Start Program
        autoStart();
    } 
    else 
    {
                  // Stop Program
        autoStop();
    }
}
}

  if(Controller.YPress == 1 && !_autoRunning)
  {
    _autoProgNum = 3;
    if(_autoProgNum != 0)
    {
      if(!_autoRunning)
      {
                  // Start Program
        autoStart();
    } 
    else 
    {
                  // Stop Program
        autoStop();
    }
}
}

if (Controller.StartPress == 1 && !_autoRunning)
{
    _autoProgNum = 4;
    if(_autoProgNum != 0)
    {
      if(!_autoRunning)
      {
        autoStart();
      }
      else 
      {
        autoStop();
      }
    }
}

}

// ===========================================
// AUTONOMOUS METHODS
// ===========================================



// Initialize Autonomous Mode
void autoStop()
{
  _autoRunning = false;
  _autoProgNum = 0;
  _autoInterval = 0;
}

void autoStart()
{
  if(_autoProgNum != 0)
  {
    _autoRunning = true;
    _autoInterval = 0;
}
}



void autoRedGoal()
{
 // Serial.println(_autoInterval);
int LS = 2200/2;
int RS = 4210/2;
int MS = 3200/2;
int B = 1;

 Robot._autonRuning = true;
  switch(_autoInterval)
  {
      case 8000:
        Lift.LiftTo(10);
      break;

      case 9500:
        Drive.Move(42);
      break;

      case 12000:
        Lift.LiftTo(7);
        Intake.Out();
      break; 

      case 13500:
        Intake.Stop();
      break;

      case 14000:
        Lift.LiftTo(15);
      break; 

      case 14500:
        Drive.Move(-40);
      break;

      case 16000:
        autoStop();
        Robot._autonRuning = false;
      break;
      

  }
}

void autoBlueGoal()
{
 // Serial.println(_autoInterval);

 Robot._autonRuning = true;
  switch(_autoInterval)
  {
      // //peg 1
      // case 0:
      //   Drive.Move(30);
      //   Intake.Out();
      // break;

      // case 500:
      //   Intake.Stop();
      // break;

      // case 800://-400:
      //   Lift.LiftTo(33);
      // break;

      // case 2200://-654:
      //   Claw.DeClamp();
      //   Claw.ArmTo(3200);
      // break;

      // case 3000:
      //   Claw.StopClamp();
      //   Lift.LiftTo(7);
      // break;

      // case 3500:
      //   Claw.DeClamp();
      //   Claw.ArmTo(4000);
      // break; 
      
      // case 4200:
      //   Claw.Clamp();
      // break;

      // case 4600:
      //   Lift.LiftTo(18);
      // break;

      // case 4900:
      //   Claw.ArmTo(2200);
      // break;

      // case 5500:
      //   Lift.LiftTo(0);
      // break;

      // //peg 2
      // case 6520:
      //   Claw.DeClamp();
      // break;

      // case 6600:
      //   Lift.LiftTo(12);
      // break;

      // case 6800:
      //   Claw.ArmTo(4200);
      // break;

      // case 7100:
      //   Lift.LiftTo(7);
      // break;

      // case 8000:
      //   Claw.Clamp();
      // break;

      // case 8300:
      //   Lift.LiftTo(20);
      // break;

      // case 8900:
      //   Claw.ArmTo(2250);
      // break;

      // case 9900:
      //   Lift.LiftTo(9);
      // break;

      // //peg 3
      // case 12900:
      //   Claw.DeClamp();
      //   Lift.LiftTo(20);
      // break;

      // case 13900:
      //   Claw.ArmTo(4000);
      // break;

      // case 14900:
      //   Lift.LiftTo(7);
      // break;

      // case 16900:
      //   Claw.Clamp();
      // break;

      // case 17800:
      //   Lift.LiftTo(22);
      // break;

      // case 18200:
      //   Drive.Move(-18);
      // break;

      // case 18800:
      //   Claw.ArmTo(2180);
      // break;

      // case 19800:
      //   Lift.LiftTo(12);
      // break;

      // case 20200:
      //   Claw.StopClamp();
      // break;

      // //peg 4
      // case 29200:
      //   Claw.DeClamp();
      //   Lift.LiftTo(24);
      // break;

      // case 29500:
      //   Claw.StopClamp();
      //   Claw.ArmTo(3200);
      //   Drive.Move(19);
      // break;

      // case 31200:
      //   Lift.LiftTo(7);
      // break;

      // case 32000: 
      //   Claw.ArmTo(4200);
      // break;

      // case 35000:
      //   Claw.Clamp();
      // break;

      // case 37000:
      //   Lift.LiftTo(30);
      //   Drive.Move(-42);
      // break;

      // case 38000:
      //   Claw.ArmTo(2300);
      // break;

      // case 39000:
      //   Lift.LiftTo(21);
      // break;

      // case 40000:
      //   Claw.DeClamp();
      // break;

      // case 42000:
      //   Claw.StopClamp();
      //   autoStop();
      //   Robot._autonRuning = false;
      // break;


      

  }
}

void autoSkills()
{
 // Serial.println(_autoInterval);

 Robot._autonRuning = true;
  switch(_autoInterval)
  {
      case 0:
        Lift.LiftTo(15);
      break;

      case 2000:
        Drive.DriveRight(200);
      break;

      case 4000:  
        Drive.DriveLeft(-50);
        Drive.DriveRight(-70);
      break;

      case 5000:  
        Intake.Out();
      break;

      case 6500:
        Lift.LiftTo(30);
      break;

      case 8500:  
        Intake.Stop();
        Robot._autonRuning = false;
        autoStop();
      break;

      

  }
}

void armOut()
{
 // Serial.println(_autoInterval);
int LS = 2200/2;
int RS = 4210/2;
int MS = 3200/2;
int B = 1;

  switch(_autoInterval)
  {
      case 0:
        Lift.LiftTo(33);
      break;

      case 2000:
        Claw.DeClamp();
        Claw.ArmTo(MS);
      break;

      case 3500:
        Claw.StopClamp();
        Lift.LiftTo(0);
      break;

      case 5000:
        Claw.StopClamp();
        autoStop();
      break;
    }
  }

/********************************************
 * Serial Communication functions / helpers
 ********************************************/


union {                // This Data structure lets
  byte asBytes[24];    // us take the byte array
  float asFloat[6];    // sent from processing and
}                      // easily convert it to a
foo;                   // float array

void SerialReceiveMove()
{




///////////////////////////////////

  String readString, funcName;
  int funcVal;

  readString="";

  while (Serial.available()) {
    delay(10);  //delay to allow buffer to fill 
    if (Serial.available() >0) {
      char c = Serial.read();  //gets one byte from serial buffer
      readString += c; //makes the string readString
  } 
}

if (readString.length() >0) {
      //Serial.println(readString); //see what was received
      
      // expect a string like XX #### containing the two servo positions      
      funcName = readString.substring(0, 2); //get the first four characters
      String tmpFuncVal = readString.substring(3, 15); //get the next 12 characters 
      funcVal = tmpFuncVal.toFloat();


  } 
  Serial.flush();  

  if (funcName == "LT")
  {
      Lift.LiftTo(funcVal);
  }
  else if (funcName == "LA")
  {
      Lift.LiftAdd(funcVal);
  }
  else if (funcName == "DL")
  {
      Drive.DriveLeft(funcVal);
  }
  else if (funcName == "DR")
  {
      Drive.DriveRight(funcVal);
  }
  else if (funcName == "MO")
  {
      Drive.Move(funcVal);
  }
  else if (funcName == "CL")
  {
      Claw.Clamp();
  }
  else if (funcName == "DC")
  {
      Claw.DeClamp();
  }
  else if (funcName == "AT")
  {
      Claw.ArmTo(funcVal);
  }
  else if (funcName == "SC")
  {
      Claw.StopClamp();
  }
  else if (funcName == "TL") //Torque Limit Lift
  {
    Lift.SetTorqueLimit(funcVal);
  }
  else if (funcName == "TD") //Torque Limit Drive
  {
    Drive.SetTorqueLimit(funcVal);
  }
  else if (funcName == "AO")
  {
    Robot.AutonOn();
  }
  else if (funcName == "AF")
  {
    Robot.AutonOff();
  }



}

// getting float values from processing into the arduino
// was no small task.  the way this program does it is
// as follows:
//  * a float takes up 4 bytes.  in processing, convert
//    the array of floats we want to send, into an array
//    of bytes.
//  * send the bytes to the arduino
//  * use a data structure known as a union to convert
//    the array of bytes back into an array of floats

//  the bytes coming from the arduino follow the following
//  format:
//  0: 0=Manual, 1=Auto, else = ? error ?
//  1-4: float setpoint
//  5-8: float input
//  9-12: float output  
//  13-16: float P_Param
//  17-20: float I_Param
//  21-24: float D_Param
/*
void SerialReceive()
{

  // read the bytes sent from Processing
  int index=0;
  byte Auto_Man = -1;
  while(Serial.available()&&index<25)
  {
    if(index==0) Auto_Man = Serial.read();
    else foo.asBytes[index-1] = Serial.read();
    index++;
  } 
  
  // if the information we got was in the correct format, 
  // read it into the system
  if(index==25  && (Auto_Man==0 || Auto_Man==1))
  {
    Drive.driveLeftSetPoint=double(foo.asFloat[0]);
    //Input=double(foo.asFloat[1]);       // * the user has the ability to send the 
                                          //   value of "Input"  in most cases (as 
                                          //   in this one) this is not needed.
    if(Auto_Man==0)                       // * only change the output if we are in 
    {                                     //   manual mode.  otherwise we'll get an
      Output=double(foo.asFloat[2]);      //   output blip, then the controller will 
    }                                     //   overwrite.
    
    double p, i, d;                       // * read in and set the controller tunings
    p = double(foo.asFloat[3]);           //
    i = double(foo.asFloat[4]);           //
    d = double(foo.asFloat[5]);           //
    Drive.driveLeftPID.SetTunings(p, i, d);            //
    
    if(Auto_Man==0) myPID.SetMode(MANUAL);// * set the controller mode
    else myPID.SetMode(AUTOMATIC);             //
  }
  Serial.flush();                         // * clear any random data from the serial buffer
}*/



// unlike our tiny microprocessor, the processing ap
// has no problem converting strings into floats, so
// we can just send strings.  much easier than getting
// floats from processing to here no?
/*
void SerialSend()
{
  Serial.print("PID ");
  Serial.print(Setpoint);   
  Serial.print(" ");
  Serial.print(Input);   
  Serial.print(" ");
  Serial.print(Output);   
  Serial.print(" ");
  Serial.print(myPID.GetKp());   
  Serial.print(" ");
  Serial.print(myPID.GetKi());   
  Serial.print(" ");
  Serial.print(myPID.GetKd());   
  Serial.print(" ");
  if(myPID.GetMode()==AUTOMATIC) Serial.println("Automatic");
  else Serial.println("Manual");  
}*/

