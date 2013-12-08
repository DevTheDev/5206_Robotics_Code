#include "robot.c"
#include "consts.c"
#include "joystickdriver.c"
#include "3rd Party Sensor Drivers/drivers/hitechnic-eopd.h"
#include "3rd Party Sensor Drivers/drivers/hitechnic-irseeker-v2.h"


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
 */
void hold()
{
	pause();

	//Wait for time to end
	while (true) {}
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
 * Speed: the speed to drive at (Positive speed will turn left, negative speed will turn right)
 */

void turn(int speed) {
	motor[LeftDr] = speed;
	motor[RightDr] = -speed;
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
	if (inches > 0) {
		while (true) {
			if (nMotorEncoder[RightDr] < rotations) {
				motor[RightDr] = 0;
			}
			if (nMotorEncoder[LeftDr] < rotations) {
				motor[LeftDr] = 0;
			}
		}
	}
	else {
		while (true) {
			if (nMotorEncoder[RightDr] > rotations) {
				motor[RightDr] = 0;
			}
			if (nMotorEncoder[LeftDr] > rotations) {
				motor[LeftDr] = 0;
			}
		}
	}
}

/**
 * Perform a pivot turn
 * degrees: number of degrees to turn - positive values turn left, negative values turn right
 * speed: speed to travel at
 */
void point(float degrees, int speed)
{
	int counts = (degrees / 360.0) * (robot.wheel.dRatio * robot.wheel.around / robot.wheel.circumference) * (-robot.encoder.ticks);
	// counts = (proportion of circle turned) * (number of rotations per full turn) * (encoderticks per wheel rotation)

	reset();

	// turn left
	if (degrees > 0) {
		turn(abs(speed));
		while(true) {
			if (nMotorEncoder[RightDr] > counts) {
				motor[RightDr] = 0;
			}
			if (nMotorEncoder[LeftDr] > counts) {
				motor[LeftDr] = 0;
			}
		}
	}

	// turn right
	else {
		turn(-abs(speed));
		while(true) {
			if (nMotorEncoder[RightDr] < counts) {
				motor[RightDr] = 0;
			}
			if (nMotorEncoder[LeftDr] < counts) {
				motor[LeftDr] = 0;
			}
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

	if (degrees > 0) {
		motor[RightDr] = -speed;
		while(nMotorEncoder[RightDr] > counts || nMotorEncoder[LeftDr] > counts) {}
	}
	else {
		motor[RightDr] = speed;
		while(nMotorEncoder[RightDr] < counts || nMotorEncoder[LeftDr] < counts) {}
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
TODO: Give description of what this function does
This appears to be a function that will shorten, and neaten Tele-op, but is unimplimented, correct me if I'm wrong.
*/
bool joy1sector(int type) {
	switch(type) {
	case 1:
		if (joystick.joy1_y1 > abs(joystick.joy1_x1) || joystick.joy1_y1 <- abs(joystick.joy1_x1)) {
			return true;
		}
		else {
			return false;
		}
		break;
	case 2:
		if (joystick.joy1_x1 > abs(joystick.joy1_y1) || joystick.joy1_x1<-abs(joystick.joy1_y1)) {
			return true;
		}
		else {
			return false;
		}
		break;
	default:
		return false;
	}
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
 * if the joystick is inside a circle of radius threshhold set x and y to 0
 */
void clampToThreshhold(float& x, float& y) {
	if(x*x + y*y <= threshhold*threshhold) {
		x = 0;
		y = 0;
	}
}

/* DRIVE
 * -----------------------------------------------*/

/**
 * TODO: Explain what this function does
 */
void singleJoyDrive() {
	float joyX = joystick.joy1_x1;
	float joyY = joystick.joy1_y1;
	clampToThreshhold(joyX, joyY);
	motor[LeftDr] = (joyY+abs(joyX))*(2*joyX/joystickRange+1)*constdrivereg;
	motor[RightDr] = (joyY+abs(joyX))*(-2*joyX/joystickRange+1)*constdrivereg;
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

	// INTAKE ALGORITHM
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
		// INTAKE
		if (manualIntakeButton) {
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

		// PADDLE
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
