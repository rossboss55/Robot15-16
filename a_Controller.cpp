#include "a_Controller.h"

// Constructors ////////////////////////////////////////////////////////////////

Controller::Controller()
:Xbox(&dummyXboxPointer)
{

}

Controller::Controller(USB *p)
: Xbox(p)
{
	LeftJoystick = 0.0;
	RightJoystick = 0.0;
	TriggerAggregate = 0.0;
	LR2Aggregate = 0;
	DPadLeftRight = 0;
	APress = 0;
	YPress = 0;
	BPress = 0;
	XPress = 0;
	StartPress = 0;
	reverseMultiplier = 1;

}

void Controller::Task()
{
	//Set Controller Defaults
	LeftJoystick = 0.0;
	RightJoystick = 0.0;
	TriggerAggregate = 0.0;
	LR2Aggregate = 0;
	APress = 0;
	YPress = 0;
	BPress = 0;
	XPress = 0;
	DPadLeftRight = 0;
	StartPress = 0;



	//This is where you update controller items.
	if (Xbox.XboxReceiverConnected) 
	{
		//Set so player 1 takes priority
		for (uint8_t i = 0; i < 4; i++) 
		{
			

			if (Xbox.Xbox360Connected[i]) 
			{
				


				if (Xbox.getAnalogHat(LeftHatY, i) > joystickMinThresh || Xbox.getAnalogHat(LeftHatY, i) < -joystickMinThresh) 
				{
					LeftJoystick = (400.0 / 32767 * Xbox.getAnalogHat(LeftHatY, i) * reverseMultiplier); 
				}
				

				if (Xbox.getAnalogHat(RightHatY, i) > joystickMinThresh || Xbox.getAnalogHat(RightHatY, i) < -joystickMinThresh) 
				{
					RightJoystick = (400.0 / 32767 * Xbox.getAnalogHat(RightHatY, i) * reverseMultiplier); 
				}

				if (reverseMultiplier == -1)
				{
					float tempVal = LeftJoystick;
					LeftJoystick = RightJoystick;
					RightJoystick = tempVal;
				}

				if(Xbox.getButtonClick(BACK))
				{	
					if (reverseMultiplier == 1)
					{
						reverseMultiplier = -1;
					}
					else 
					{
						reverseMultiplier = 1;
					}
				}



				

				//L2 Trigger
		        if (Xbox.getButtonPress(R2, i)) 
		        {
		          TriggerAggregate = 400.0 / 255 * Xbox.getButtonPress(R2, i) * 1; 
		        }
		        //R2 Trigger
		        else if (Xbox.getButtonPress(L2, i)) 
		        {
		          TriggerAggregate = 400.0 / 255 * Xbox.getButtonPress(L2, i) * -1; 
		        }



		        if (Xbox.getButtonPress(L1, i))
		        {
		          LR2Aggregate = 1;
		        
		        }
		        //R1 Button
		        else if (Xbox.getButtonPress(R1, i))
		        {
		          LR2Aggregate = -1;
		          
		        }



		        if (Xbox.getButtonPress(Y, i))
		        {
		          YPress = 1;
		        }
			


		        if (Xbox.getButtonPress(A, i))
		        {
		          APress = 1;
		        }

		        if (Xbox.getButtonPress(B, i))
		        {
		        	BPress = 1;
		        }
		       
		       	if (Xbox.getButtonPress(X, i))
		        {
		        	XPress = 1;
		        }

		        if (Xbox.getButtonPress(START, i)) 
		        {
		        	StartPress = 1;
		        }


		        if (Xbox.getButtonPress(LEFT, i)) 
		        {
		          DPadLeftRight = -1; 
		        }
		        else if (Xbox.getButtonPress(RIGHT, i)) 
		        {
		          DPadLeftRight = 1; 
		        }
		        else if (Xbox.getButtonPress(UP, i)) 
		        {
		          DPadLeftRight = 2; 
		        }
		        else if (Xbox.getButtonPress(DOWN, i)) 
		        {
		          DPadLeftRight = -2; 
		        }

		      /*  Serial.print(Xbox.getButtonPress(UP));
		        Serial.print(Xbox.getButtonPress(LEFT));
		        Serial.print(Xbox.getButtonPress(DOWN));
		        Serial.println(Xbox.getButtonPress(RIGHT)); */


			}
		}
	}
}