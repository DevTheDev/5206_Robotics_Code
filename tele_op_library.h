#include "joystickdriver.c"
#include "consts.c"
#include "robot.c"

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
		motor[LiftFlagMtr] = joystick.joy2_y2*constdrivereg;
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
			turnPaddleOnce(paddlespeedreg, paddlespeedslow, FORWARD);
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
	if(dir==1) {
		servoChangeRate[LeftIntake] = intakeFast;
		servo[LeftIntake] = leftintakefwd;

		servoChangeRate[RightIntake] = intakeFast;
		servo[RightIntake] = rightintakefwd;
	}
	else if(dir==-1) {
		servoChangeRate[LeftIntake] = intakeFast;
		servo[LeftIntake] = leftintakebck;

		servoChangeRate[RightIntake] = intakeFast;
		servo[RightIntake] = rightintakebck;
	}
	else {
		servo[LeftIntake] = intakestop;
		servo[RightIntake] = intakestop;
	}
}

/**
* Tele-op routine for intake
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
