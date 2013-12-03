#pragma config(Hubs,  S1, HTMotor,  HTServo,  HTMotor,  none)
#pragma config(Sensor, S2,     PaddleEOPD,     sensorAnalogActive)
#pragma config(Sensor, S3,     AutoIR,         sensorI2CCustom)
#pragma config(Sensor, S4,     PaddleLIGHT,    sensorLightActive)
#pragma config(Motor,  mtr_S1_C1_1,     LiftFlagMtr,   tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C1_2,     PaddleMtr,     tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C3_1,     RightDr,       tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C3_2,     LeftDr,        tmotorTetrix, openLoop, reversed)
#pragma config(Servo,  srvo_S1_C2_1,    LeftIntake,           tServoContinuousRotation)
#pragma config(Servo,  srvo_S1_C2_2,    RightIntake,          tServoContinuousRotation)
#pragma config(Servo,  srvo_S1_C2_3,    servo3,               tServoNone)
#pragma config(Servo,  srvo_S1_C2_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S1_C2_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C2_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "consts.c"
#include "robot.c"
#include "actions.h"
#include "joystickdriver.c"
#include "3rd Party Sensor Drivers/drivers/hitechnic-eopd.h"
#include "3rd Party Sensor Drivers/drivers/hitechnic-irseeker-v2.h"

int driveTurns;

/**
 * returns true if the robot is aligned with the beacon
 */
bool aligned(){
	return nMotorEncoder[RightDr] > 1440*24/robot.wheel.circumference*robot.wheel.dRatio;
}

task main()
{
	eraseDisplay(); // Clear the NXT screen
	bDisplayDiagnostics = false; // Takes control away from FCS
	bNxtLCDStatusDisplay = false; // Takes control away from NXT firmware
	
	while(nNxtButtonPressed != orangebutton) {
		nxtDisplayCenteredTextLine(1, "IR: %d", HTIRS2readACDir(AutoIR));
	}
	
	initializeRobot();
	waitForStart();
	
	// Drive to basket
	motor[LiftFlagMtr]=100;
	ClearTimer(T1);
	//wait1Msec(2000);
	reset();
	drive(60);
	
	while(!aligned() || time1[T1] < 3500){
		if(aligned()){
			driveTurns = nMotorEncoder[RightDr];
			pause();
		}
		if(time1[T1] < 3500){
			motor[LiftFlagMtr]=0;
		}
	}
	//motor[LiftFlagMtr]=0;
	//pause();
	point(90, 100);
	move(distToPend, 40);
	
	// Score block
	scoreBlocks();
	
	// Move back
	move(-distToPend, -80);
	point(-90, 100);
	drive(-driveTurns*robot.wheel.circumference/(encoderticks*robot.wheel.dRatio)+4 ,-80);//make another move function that takes raw encoder values?
	pause();
	point(90, 100);
	move(12, 80);
	point(-90, 100);
	move(distToBridge, 80);
	stopRobot();
}
