#include "joystickdriver.c"
#include "consts.h"
#include "robot.h"

/* -----------------------------------------------
* -----------------------------------------------
* TELE-OPERATED PERIOD
* -----------------------------------------------
* ----------------------------------------------- */

/*void singleJoyDrive();
//void doubleJoyDrive();
void PaddleAndIntake();
void liftandflag();
void onePaddleTurn(int speed);
*/
/**
* If the joystick is inside a circle of radius threshhold set x and y to 0
* x: TODO
* y: TODO
*/
void filterJoy(float& x, float& y) {
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

	//PlayImmediateTone((y+1.0)*440*(sinDegrees(time1[T1]/5)+2), 10);
}

/**
* Drive from the right joystick
*/
void singleJoyDrive() {
	float joyX = driveX;
	float joyY = driveY;
	filterJoy(joyX, joyY);
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
void lift()
{
	if (abs(joystick.joy2_y2) > threshhold) {
		motor[LiftMtr1] = (liftControl-threshhold*liftControl/abs(liftControl))/(joystickRange-threshhold)*constdrivereg;
		motor[LiftMtr2] = (liftControl-threshhold*liftControl/abs(liftControl))/(joystickRange-threshhold)*constdrivereg;
	}
	else {
		motor[LiftMtr1] = DCstop;
		motor[LiftMtr2] = DCstop;
	}
}

void flag(){
	if (flagButton) {
		motor[FlagMtr] = flagSpeed;
	}
	else if (flagBackButton){
		motor[FlagMtr] = -flagSpeed;
	}
	else {
		motor[FlagMtr] = DCstop;
	}
}

/**
* Turn paddle the given amount
*/
void turnPaddle(int amount)
{
	servo[Turbofan] += amount;
	if(servo[Turbofan] > 255){
		servo[Turbofan] = 255;
	}
	if(servo[Turbofan] < 0){
		servo[Turbofan] = 0;
	}
}

int requestedPosition(){
	if(turboButton1){
		return 0;
	}
	if(turboButton2){
		return 1;
	}
	if(turboButton3){
		return 2;
	}
	return -1;
}

/**
* Tele-op routine for paddle
*/
const int positions[3] = {0, 127, 255};
float turboPos;
bool autoIntakeOn = false;
bool autoIntakePressed = false;
int turboSlot = 0;

void updateButtons(){
	if(autoIntakeButton){
		if(!autoIntakePressed){
			autoIntakeOn = !autoIntakeOn;
			for(int k = 0; k < 3; k++){
				if(abs(positions[k] - servo[Turbofan]) < abs(positions[turboSlot] - servo[Turbofan])){
					turboSlot = k;
				}
			}
		}
		autoIntakePressed = true;
	}
	else{
		autoIntakePressed = false;
	}
}

bool touched = false;

void paddle()
{
	if(!SensorValue[turboTouch]){
		clearTimer(T3);
	}
	if(autoIntakeOn){
		writeDebugStreamLine("NOM: %d", turboSlot);
		servo[Turbofan] = positions[turboSlot];
	}
	if (autoIntakeOn && time1[T3] >= nomWait){
		if(!touched && time1[T4] >= omWait){
			playSound(soundFastUpwardTones);
			if (turboSlot < 3-1){
				turboSlot++;
			}
			turnPaddle(51);
			clearTimer(T3);
			clearTimer(T4);
		}
		touched = true;
	}
	else{
		touched = false;
		if (paddleAutoForwardButton) {
			turnPaddle(manualPaddleChange);
		}
		if (paddleAutoBackButton) {
			turnPaddle(-manualPaddleChange);
		}
		float joyY = joystick.joy1_y2;
		if(abs(joyY) > threshhold){
			turboPos += joyY*.01;
			servo[Turbofan] = turboPos;
		}
		else{
			turboPos = servo[Turbofan];
		}

		if(requestedPosition() != -1){
			servo[Turbofan] = positions[requestedPosition()];
		}
	}
}


/**
* Turn intake forward or backward
*/
void turnIntake(int speed)
{
	motor[Intake] = speed;
}

/**
* Tele-op routine for intake
* DOES THIS FUNCTION SERVE ANY USE ANYMORE? -PARKER
*
* I updated it for the new hardware - Kyler
*/
void intakeBlocks()
{
	if (manualIntakeButton || autoIntakeOn) {
		turnIntake(intakeFast);
	}
	else if (manualOuttakeButton) {
		turnIntake(-intakeFast);
	}
	else {
		turnIntake(0);
	}
}

/*
* Deploy the intake
*/
void deployIntake()
{
 if(manualDeployIntake)
 {
 	servo[RightIntake]=255 - intakeOffset;
 	servo[LeftIntake]=0 + intakeOffset;
 }
 if(manualRetractIntake)
 {
 	servo[RightIntake]= 0;
 	servo[LeftIntake]= 255;
 }
}
