/* Actions.h
Library of all functions
*/

#include "robot.c"
#include "joystickdriver.c"
#include "3rd Party Sensor Drivers/drivers/hitechnic-eopd.h"
#include "3rd Party Sensor Drivers/drivers/hitechnic-irseeker-v2.h"
#include "menu.h"
//#include "consts.c"

/**
 * Reset the motor encoders
 */
void reset()
{
	nMotorEncoder[RightDr] = 0;
	nMotorEncoder[LeftDr] = 0;
}

/**
 * Pause the robot in one spot
 */
void pause()
{
	motor[LeftDr] = 0;
	motor[RightDr] = 0;
}

/**
 * Stop the robot and wait for time to end
 * THIS IS NOT AN ERROR
 */
void hold()
{
	pause();

	while (true) {}//Wait for time to end
}

/**
 * Bring the robot to a complete stop
 */
void stopRobot() {
	motor[LeftDr] = DCstop;
	motor[RightDr] = DCstop;
	motor[PaddleMtr] = DCstop;
	motor[LiftFlagMtr] = DCstop;
	servo[LeftIntake] = intakestop;
	servo[RightIntake] = intakestop;
}

/**
 * Drive until told elsewhere to stop
 * speed: the speed to drive at (negative speed will cause the robot to go backwards)
 */
void drive(int speed) {
	motor[LeftDr] = speed;
	motor[RightDr] = speed;
}

/**
 * Set turn speed
 * speed: the speed to drive at (Positive speed will turn left, negative speed will turn right)
 */

void turn(int speed) {
	motor[LeftDr] = -speed;
	motor[RightDr] = speed;
}

/**
 * turns for the time ms
 * time: the time in ms for which to turn
 * speed: the speed to drive at (Positive speed will turn left, negative speed will turn right)
 */

void turnTime(int time, int speed) {
	motor[LeftDr] = -speed;
	motor[RightDr] = speed;
	wait1Msec(time);
	pause();
}

/**
 * Drive a set number of encoder ticks
 * rotations: number of encoder ticks to rotate
 * speed: speed to traval at (negative values will cause the robot to drive backwards)
 */
void moveRotations(float rotations, int speed)
{
	reset();

	drive(speed);
	while ((rotations > 0) == (nMotorEncoder[RightDr] <= rotations) || (rotations>0) == (nMotorEncoder[LeftDr] <= rotations)) {
		// Wait for right motor to reach goal
		if ((rotations > 0) == (nMotorEncoder[RightDr] >= rotations)) {
			motor[RightDr] = 0;
		}

		// Wait for left motor to reach goal
		if ((rotations > 0) == (nMotorEncoder[LeftDr] >= rotations)) {
			motor[LeftDr] = 0;
		}
	}
}

/**
 * Drive a set distance
 * inches: number of inches to move
 * speed: speed to traval at (negative values will cause the robot to drive backwards)
 */
void move(float inches, int speed)
{
	int rotations = inches * 1440.0 / robot.wheel.circumference * robot.wheel.dRatio;

	reset();

	drive(speed);
	while (((inches > 0) == (nMotorEncoder[RightDr] <= rotations)) || ((inches>0) == (nMotorEncoder[LeftDr] <= rotations))) {
		// Wait for right motor to reach goal
		if ((inches > 0) == (nMotorEncoder[RightDr] >= rotations)) {
			motor[RightDr] = 0;
		}

		// Wait for left motor to reach goal
		if ((inches > 0) == (nMotorEncoder[LeftDr] >= rotations)) {
			motor[LeftDr] = 0;
		}
	}
}

/**
 * Perform a pivot turn
 * degrees: number of degrees to turn - positive values turn left, negative values turn right
 * speed: speed to travel at
 */
void pointByTime(float degrees, int speed)
{
	int time = timeSpeedPerDegrees*degrees/speed;
	turnTime(time, speed);
}

/**
 * Perform a pivot turn
 * degrees: number of degrees to turn - positive values turn left, negative values turn right
 * speed: speed to travel at
 */
void point(float degrees, int speed)
{
	int counts = (degrees / 360.0) * (robot.wheel.dRatio * robot.wheel.around / robot.wheel.circumference) * (robot.encoder.ticks);
	// counts = (proportion of circle turned) * (number of rotations per full turn) * (encoderticks per wheel rotation)

	if(abs(counts) * maxAccel <= .5 * speed * speed){
		speed = ((degrees > 0) ? 1 : -1) * sqrt(2 * abs(counts) * maxAccel);
	}

	reset();

	turn(speed);
	while((degrees > 0) == (nMotorEncoder[RightDr] <= counts) || (degrees > 0) == (nMotorEncoder[LeftDr] >= -counts)) {
		// Wait for right motor to reach goal
		if ((degrees > 0) == (nMotorEncoder[RightDr] >= counts)) {
			motor[RightDr] = 0;
		}
		else if((abs(counts) - abs(nMotorEncoder[RightDr])) * maxAccel <= .5 * speed * speed){
			motor[RightDr] = ((speed > 0) ? 1: -1)*sqrt(2 * maxAccel * (abs(counts) - abs(nMotorEncoder[RightDr])));
		}

		// Wait for left motor to reach goal
		if ((degrees > 0) == (nMotorEncoder[LeftDr] <= -counts)) {
			motor[LeftDr] = 0;
		}
		else if((abs(counts) - abs(nMotorEncoder[LeftDr])) * maxAccel <= .5 * speed * speed){
			motor[LeftDr] = ((speed > 0) ? -1: 1)*sqrt(2 * maxAccel * (abs(counts) - abs(nMotorEncoder[LeftDr])));
		}
	}
}

/**
 * Moves the robot relative to it's current position
 * y: the distance in inches if front of the robot to move
 * x: the distance in inches left of the robot to move
 * speed: the driving speed
 * turnSpeed: the turning speed
 */
void moveTo(float y, float x, int speed, int turnSpeed){
	if (y == 0) {
		point((x > 0) ? 90:-90, turnSpeed);
		move(x*x, speed);
	}
	else {
		point(radiansToDegrees(atan(x/y)), 100);
		move((y > 0)? sqrt(y*y+x*x):-sqrt(y*y+x*x), (y > 0)? speed:-speed);
	}
}

/**
 * Perform a swing turn
 * degrees: number of degrees to turn - positive values turn right, negative values turn left
 * speed: speed to travel at
 */
void swing(int degrees, int speed)
{
	int counts = 2 * -encoderticks * robot.wheel.around * robot.wheel.dRatio * degrees / (robot.wheel.circumference*360.0);

	reset();

	//nMotorEncoderTarget[RightDr] = counts;
	//nMotorEncoderTarget[LefttDr] = counts;

	if (degrees >= 0) {
		motor[RightDr] = -speed;
		while(nMotorEncoder[RightDr] >= counts) {}
	}
	else {
		motor[LeftDr] = -speed;
		while(nMotorEncoder[LeftDr] <= counts) {}
	}

	pause();
}

/**
 * Score the blocks that the robot is carring
 */
void scoreBlocks()
{
	// TODO: Optimize time
	motor[PaddleMtr] = 10;
	wait1Msec(2000);
	motor[PaddleMtr] = -10;
	wait1Msec(2000);
	motor[PaddleMtr] = DCstop;
}

/**
 * Checks if a block is in front of the EOPD
 */
bool EOPDDetect(tSensors EOPD, int eopdetect) {
	return(HTEOPDreadProcessed(EOPD) >= eopdetect);
}


/* TELE-OP
 * -----------------------------------------------*/

void singleJoyDrive();
void doublejoyDrive();
void PaddleAndIntake();
void liftandflag();
void onePaddleTurn(int speed);

/**
 * If the joystick is inside a circle of radius threshhold set x and y to 0
 * x: TODO
 * y: TODO
 */
void filteredJoy(float& x, float& y) {
	x = joystick.joy1_x1;
	y = joystick.joy1_y1;

	// If the joystick is inside a circle of radius threshhold set x and y to 0
	if(x*x + y*y <= threshhold*threshhold) {
		x = 0;
		y = 0;
	}

	// Keep the values in [-1, 1]
	x /= joystickRange;
	y /= joystickRange;
}

/**
 * TODO: Explain what this function does
 */
void singleJoyDrive() {
	float joyX, joyY;
	filteredJoy(joyX, joyY);
	motor[LeftDr] = (joyY+joyX)*constdrivereg;
	motor[RightDr] = (joyY-joyX)*constdrivereg;
}

/**
 * TODO: Explain what this function does
 */
void doublejoyDrive() {
	motor[LeftDr] = (abs(joystick.joy1_y1) > threshhold) ? joystick.joy1_y1*constdrivereg : DCstop;
	motor[RightDr] = (abs(joystick.joy1_y2) > threshhold) ? joystick.joy1_y2*constdrivereg : DCstop;
}

/**
 * TODO: Explain what this function does
 */
void paddleStart() {
	ClearTimer(T2);
}

/**
 * TODO: Explain what this function does
 */
void PaddleAndIntake() {
	if (paddleForwardButton) {
		onePaddleTurn((slowButton) ? paddlespeedslow : paddlespeedreg);
	}
	else if (paddleBackButton) {
		onePaddleTurn(-(slowButton) ? paddlespeedslow : paddlespeedreg);
	}

	// Intake algorithm
	if (time1[T2] >= paddleWaitTime && time1[T2] < paddleFastTime+paddleWaitTime) {
		motor[PaddleMtr] = paddlespeedreg;
	}
	else if (time1[T2] >= paddleFastTime+paddleWaitTime && time1[T2] < paddleIntakeTime) {
		motor[PaddleMtr] = paddlespeedslow;
		if(SensorValue[PaddleLIGHT] < paddleLightThresh) {
			motor[PaddleMtr] = DCstop;
		}
	}

	if (autoIntakeButton) {
		servo[LeftIntake] = leftintakefwd;
		servo[RightIntake] = rightintakefwd;

		if(EOPDDetect(PaddleEOPD,farblockconst) == 1 && time1[T2] > paddleIntakeTime) {
			paddleStart();
		}
	}
	else {
		// Intake
		if (manualIntakeButton) {
			servoChangeRate[LeftIntake] = 0; // Add constant
			servoChangeRate[RightIntake] = 0;
			servo[LeftIntake] = leftintakefwd;
			servo[RightIntake] = rightintakefwd;
		}

		else if (manualOuttakeButton) {
			servo[LeftIntake] = leftintakebck;
			servo[RightIntake] = rightintakebck;
		}

		else {
			servo[LeftIntake] = intakestop;
			servo[RightIntake] = intakestop;
		}

		// Paddle
		if (abs(joystick.joy1_y2) > threshhold) {
			motor[PaddleMtr] = joystick.joy1_y2 * paddleratio;
		}
		else if (joy1Btn(btnRB) == 1) {
			motor[PaddleMtr] = paddlespeedreg;
		}
		else if (joy1Btn(btnLB) == 1) {
			motor[PaddleMtr] = -paddlespeedreg;
		}
		else {
			motor[PaddleMtr] = DCstop;
		}
	}
}

/**
 * TODO: Explain what this function does
 */
void liftandflag()
{
	if (abs(joystick.joy2_y2) > threshhold) {
		motor[LiftFlagMtr] = joystick.joy2_y2*constdrivereg;
	}
	else if (joy2Btn(btnA) == 1) {
		motor[LiftFlagMtr] = 100;
		wait1Msec(500);
		motor[LiftFlagMtr] = DCstop;
	}
	else {
		motor[LiftFlagMtr] = DCstop;
	}
}

/**
 * TODO: Explain what this function does
 */
void onePaddleTurn(int speed) {
	motor[PaddleMtr] = speed;
	wait1Msec(250);
	ClearTimer(T2);
	while (SensorValue[PaddleLIGHT] < paddleLightThresh && time1[T2] < 2000) {
		singleJoyDrive();
		liftandflag();
	}
	ClearTimer(T2);
	while (SensorValue[PaddleLIGHT] > paddleLightThresh && time1[T2] < 2000) {
		singleJoyDrive();
		liftandflag();
	}
	motor[PaddleMtr] = DCstop;
	wait1Msec(100);
	motor[PaddleMtr] = -paddlespeedreg;
	wait1Msec(50);
	motor[PaddleMtr] = DCstop;
}

/**
 * TODO: Explain what this function does
 */
void initializeRobotnext() {
	HTEOPDsetShortRange(PaddleEOPD);
}
