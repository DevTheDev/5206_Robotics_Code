#pragma config(Hubs,  S1, HTMotor,  HTServo,  HTMotor,  none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
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

// Includes consts.c and robot.c, as well
#include "actions.h"

task main()
{
	// Wait for start
	initializeRobot();
	waitForStart();
	// Drive to goal
	motor[LiftFlagMtr]=100;
	ClearTimer(T1);
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

	// Drive to pendulum
	turn(100);
	wait1Msec(100);
	pause();
	move(4, 80);
	// Score Block
	scoreBlock();

	// Drive to ramp

	// Find IR Beacon


}
