#include "joystickdriver.c"
#include "consts.h"
#include "robot.h"

/* -----------------------------------------------
* -----------------------------------------------
* TELE-OPERATED PERIOD
* -----------------------------------------------
* ----------------------------------------------- */

/*void singleJoyDrive();
void doubleJoyDrive();
void PaddleAndIntake();
void liftandflag();
void onePaddleTurn(int speed);
*/
/**
* If the joystick is inside a circle of radius threshhold set x and y to 0
* x: TODO
* y: TODO
*/
void filteredJoy(float& x, float& y) {
	x = joystick.joy1_x1;
	y = joystick.joy1_y1;

	float threshOffset = sqrt(x*x + y*y);

	// If the joystick is inside a circle of radius threshhold set x and y to 0
	if(threshOffset <= threshhold) {
		x = 0;
		y = 0;
	}
	else {
		x -= x * threshhold / threshOffset;
		y -= y * threshhold / threshOffset;
		// Keep the values in [-1, 1]
		x /= (joystickRange-threshhold);
		y /= (joystickRange-threshhold);
	}

}

/**
* Drive from the right joystick
*/
void singleJoyDrive() {
	float joyX, joyY;
	filteredJoy(joyX, joyY);
	motor[LeftDr] = (joyY+joyX*turnSensitivity)*constdrivereg;
	motor[RightDr] = (joyY-joyX*turnSensitivity)*constdrivereg;
}

/**
* Arcade (double joystick) drive
*/
void doubleJoyDrive() {
motor[LeftDr] = (abs(joystick.joy1_y1) > threshhold) ? joystick.joy1_y1*constdrivereg : DCstop;
motor[RightDr] = (abs(joystick.joy1_y2) > threshhold) ? joystick.joy1_y2*constdrivereg : DCstop;
}

/**
* Joystick Controls for Paddle and Intake
*/
//void PaddleAndIntake() {
// Intake algorithm
//	if (time1[T2] >= paddleWaitTime && time1[T2] < paddleFastTime+paddleWaitTime) {
//		motor[PaddleMtr] = paddlespeedreg;
//	}
//	else if (time1[T2] >= paddleFastTime+paddleWaitTime && time1[T2] < paddleIntakeTime) {
//		motor[PaddleMtr] = paddlespeedslow;
//		if(SensorValue[PaddleLIGHT] < paddleLightThresh) {
//			motor[PaddleMtr] = DCstop;
//		}
//	}

//	if (autoIntakeButton) {
//		//turnIntake;

//		if(EOPDDetect(BlockEOPD,nearblockconst) == 1) {
//			//turnPaddleOnce(paddlespeedreg, paddlespeedslow, FORWARD);
//		}
//	}
//	else {
//		Intake();
//		Paddle();
//	}
//}

/**
* Tele-op routine for lift motor
*/
void liftAndFlag()
{
	if (abs(joystick.joy2_y2) > threshhold) {
		motor[LiftFlagMtr] = (joystick.joy2_y2-(joystick.joy2_y2 > 0 ? 1 : -1)*threshhold)/(joystickRange-threshhold)*constdrivereg;
	}
	else {
		motor[LiftFlagMtr] = DCstop;
	}
}

/**
* Turn paddle one carriage
*/
void turnPaddle(int initspeed, int finalspeed, int dir)
{
	motor[PaddleMtr] = dir * initspeed;

	ClearTimer(T4);

	wait1Msec(100);

	while(HTEOPDreadProcessed(PaddleEOPD)>paddleEOPDThresh && time1[T4]<3000) {}

	motor[PaddleMtr] = dir * finalspeed;

	while(HTEOPDreadProcessed(PaddleEOPD)<paddleEOPDThresh && time1[T4]<3000) {}

	motor[PaddleMtr] = DCstop;
}

/**
* Tele-op routine for paddle
*/
task Paddle()
{
	while(true){
		if (autoIntakeButton && HTEOPDreadProcessed(BlockEOPD) > blockEOPDThresh){
			turnPaddle(paddlespeedreg, paddlespeedslow, FORWARD);
		}
		else if (abs(joystick.joy1_y2) > threshhold) {
			motor[PaddleMtr] = joystick.joy1_y2 * paddleratio;
		}
		else if (paddleAutoForwardButton) {
			turnPaddle(paddlespeedreg, paddlespeedslow, FORWARD);
		}
		else if (paddleAutoBackButton) {
			turnPaddle(paddlespeedreg, paddlespeedslow, BACKWARD);
		}
		else {
			motor[PaddleMtr] = DCstop;
		}
	}
	EndTimeSlice();
}


/**
* Turn intake forward or backward
*/
void turnIntake(int dir)
{
	if(dir==1) 
	{
		motor[Intake] = intakeFast;
	}
	else if(dir==-1) 
	{
		motor[Intake] = -intakeFast;
	}
	else 
	{
		motor[Intake] = intakestop;
	}
}

/**
* Tele-op routine for intake
* DOES THIS FUNCTION SERVE ANY USE ANYMORE? -PARKER
*/
void Intake()
{
	if (manualIntakeButton || autoIntakeButton) {
		turnIntake(FORWARD);
	}
	else if (manualOuttakeButton) {
		turnIntake(BACKWARD);
	}
	else {
		turnIntake(STOP);
	}
}

/*
* manual intake control
*/
void manualIntake()
{
	if (manualIntakeButton) 
	{
		turnIntake(FORWARD);
	}
	else if (manualOuttakeButton) 
	{
		turnIntake(BACKWARD);
	}
	else if(abs(joystick.joy1_y2>threshold))
	{
		motor[Intake]=joyIntake*intakeSpeedRatio;	
	}
	else 
	{
		turnIntake(STOP);
	}
	
}

/*
* manual paddle control
*/
void manualPaddle()
{
  if(paddleAutoForwardButton)
  {
  	servo[Turbofan] += manualPaddleChange;	         //can I use += for servos?
  }
  else if(paddleAutoBackwardButton)
  {
  	servo[Turbofan] -= manualPaddleChange;		//can I use -= for servos?
  }
  else if(paddle0Loaded)
  {
  	servo[Turbofan] = 255;		
  }
  else if(paddle2Loaded)
  {
  	servo[Turbofan] = 153;		
  }
  else if (paddle4Loaded)
  {
  	servo[Turbofan] = 51;	
  }
}

/*
* Turn the turbofan
*/
void paddleIntake()
{
  if(autoIntakeButton)
  {
  	//auto intake routine
  	turnIntake(FORWARD);
  	
  	//auto paddle routine
  	if (turboTouch)
  	{
		ClearTimer(T1);
		
		while(time1[T1] < 500){}  		//is this necessary?
		
		if(turboTouch)
		{
			servo[Turbofan] += 51;			
		}
	}
  }
  else
  {
  	manualIntake();
  	manualPaddle();
  }
}

/*
* Deploy the intake
*/
void deployIntake()
{
 if(manualDeployIntake)
 {
 	servo[RightIntake]=255;
 	servo[LeftIntake]=0;
 }
 if(manualRetractIntake)
 {
 	servo[RightIntake]=127-intakeOffset;
 	servo[LeftIntake]=127+intakeOffset;
 }
}
