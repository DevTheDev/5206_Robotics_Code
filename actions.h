#include "joystickdriver.c"
#include "3rd Party Sensor Drivers/drivers/hitechnic-eopd.h"
#include "3rd Party Sensor Drivers/drivers/hitechnic-irseeker-v2.h"

/**
 * Reset the motor encoders
 */
void reset()
{
	nMotorEncoder[RightDr] = 0;
	//nMotorEncoder[LeftDr] = 0;
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
 */
void hold()
{
	pause();

	//Wait for time to end
	while(true) {}
}

/**
 * Wait until the robot has completed its current movement
 * WARNING: Calling this function will limit the robot to only moving
 */
void waitUntilStop()
{
	while(nMotorRunState[RightDr] != runStateIdle) {}

	pause();
}

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
 * Drive a set distance
 * inches: number of inches to move
 * speed: speed to traval at (negative values will cause the robot to drive backwards)
 */
void move(int inches, int speed)
{
	int distance = robot.wheel.circumference * 1440 * robot.wheel.dRatio;

	reset();

	nMotorEncoderTarget[RightDr] = distance;

	motor[RightDr] = speed;
	motor[LeftDr] = speed;

	waitUntilStop();
}

/**
 * Preform a point turn
 * degrees: number of degrees to turn - positive values turn right, negative values turn left
 * speed: speed to travel at
 * May Still need Debugging, but this is what the ROBOTC tutorial suggested.  - Dev
 */
void point(int degrees, int speed)
{
	int counts = (((robot.wheel.around / robot.wheel.circumference) * (degrees / 360)) / 2) / (1440 * robot.wheel.dRatio);

	reset();

	nMotorEncoderTarget[RightDr] = counts;

	if(degrees > 0);
	{
		motor[LeftDr] = speed;
		motor[RightDr] = -speed;
		wait1Msec(500);
		motor[LeftDr] = DCstop;
		motor[RightDr] = DCstop;
	}
	else
	{
		motor[LeftDr] = -speed;
		motor[RightDr] = speed;
		wait1Msec(500);
		motor[LeftDr] = DCstop;
		motor[RightDr] = DCstop;
	}
}

/**
 * Preform a swing turn
 * degrees: number of degrees to turn - positive values turn right, negative values turn left
 * speed: speed to travel at
 */
void swing(int degrees, int speed)
{
	int counts = ((robot.wheel.around / robot.wheel.circumference) * (degrees / 360)) / (1440 * robot.wheel.dRatio);

	reset();

	nMotorEncoderTarget[RightDr] = counts;

	if (degrees > 0)
	{
		motor[LeftDr] = speed;
		motor[RightDr] = -speed;
	}
	else
	{
		motor[LeftDr] =- speed;
		motor[RightDr] = speed;
	}
}

/**
 * Score the blocks that the robot is carring
 */
void scoreBlocks()
{
	motor[PaddleMtr] = 15;
	wait1Msec(2000);
}

/**
 * TODO: Give description of what this fuction does
 */
bool joy1sector(int type) {
	switch(type) {
		case 1: {
			if (joystick.joy1_y1 > abs(joystick.joy1_x1) || joystick.joy1_y1 <- abs(joystick.joy1_x1)) {
				return true;
			}
			else
			{
				return false;
			}
			break;
	}
	case 2: {
		if (joystick.joy1_x1 > abs(joystick.joy1_y1) || joystick.joy1_x1<-abs(joystick.joy1_y1))
		{
			return true;
		}
		else
		{
			return false;
		}
		break;
	}
	default: {
		return false;
	}
}

/**
 * TODO: Give description of what this fuction does
 */
bool EOPDDetect(tSensors EOPD, int eopdetect) {
	return(HTEOPDreadProcessed(EOPD) >= eopdetect);
}
