/* Actions.h
Library of all functions
*/

#include "3rd Party Sensor Drivers/drivers/hitechnic-sensormux.h"
#include "3rd Party Sensor Drivers/drivers/hitechnic-irseeker-v2.h"
#include "3rd Party Sensor Drivers/drivers/hitechnic-eopd.h"
#include "consts.h"
#include "robot.h"
#include "menu.h"


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
	motor[Turbofan] = DCstop;
	motor[FlagMtr] = DCstop;
	motor[LiftMtr] = DCstop;
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

void turnTime(int waitTime, int speed) {
	motor[LeftDr] = -speed;
	motor[RightDr] = speed;
	wait1Msec(waitTime);
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
	int pointTime = timeSpeedPerDegrees*degrees/speed;
	turnTime(pointTime, speed);
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

/*
	turn the paddle by the specified amount
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

/**
* Score the blocks that the robot is carrying
*/
void scoreBlocks()
{
	// TODO: Optimize time
	turnPaddle(255);
	wait1Msec(1000);
	turnPaddle(-255);
}

/**
* Checks if a block is in front of the EOPD
*/
bool EOPDDetect(tSensors EOPD, int eopdetect) {
	return(HTEOPDreadProcessed(EOPD) >= eopdetect);
}

/**
* Raises the lift
* speed: the speed at which to lift; negative values lower the lift
* liftTime: the time for which to lift; if it is greater thab maxLiftTime, then it will lift for maxLiftTime
*/
void lift(int speed, int liftTime){
	motor(LiftMtr) = speed;
	wait1Msec((liftTime < maxLiftTime) ? liftTime : maxLiftTime);
}

/**
* Use to multitask the lift
* Up = True
*/
//void liftTask(bool upDown, int liftTime){
//	task lift(){
//		if (upDown){
//			motor(LiftFlagMtr) = 100;
//			wait1Msec(liftTime);
//		}
//		else{
//			motor(LiftFlagMtr) = -100;
//			wait1Msec(liftTime);
//		}
//	StartTask (lift);
//	}
//}
