// Includes consts.c and robot.c, as well
// Drivers can be found in Google Drive
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
#include "actions.h"
int driveTurns;
/**
* returns true if the robot is aligned with the beacon
*/
bool aligned(){
	return SensorValue[AutoIR] == irZone;
}
task main()
{
	// Wait for start
	initializeRobot();
	waitForStart();
	int autoCount;
	int distToMove;
	// Drive to pendulum goal
	motor[LiftFlagMtr] = 100; //Raise the BSM
	ClearTimer(T1);
	reset();
	drive(60);

	while(!aligned() || time1[T1] <= 3500){
		if(aligned()){
			driveTurns = nMotorEncoder[RightDr];
			pause();
		}
		if(time1[T1] >= 3500){
			motor[LiftFlagMtr] = 0;
		}
	}
	motor[LiftFlagMtr] = 0;
	// Find IR Beacon5
	while (autoCount <= 4){
		move(10, 50);
		if (aligned()){
			turnTime(700, -100);
			scoreBlocks();
			distToMove += autoCount;
			autoCount += 3;
		}
		else{
			autoCount ++;
		}
	}
	if (autoCount <= 2){
		turnTime(700, 100);
		move(-distToMove*10, 50);
	}
	else{
		turnTime(700, 100);
		move((distToMove*10), 50);
	}
//return;
// Drive to pendulum
//turnTime(700, -100);
//wait1Msec(100);
//move(12, 40);
//// Score the block
//scoreBlocks();
//return;
// Drive to ramp
move(-7, -40);// Back away from the goal
motor[LiftFlagMtr] = -100;// Begin to lower the BSM
wait1Msec(100);
turnTime(650, 100);
moveRotations(-driveTurns+4, -80);
turnTime(500, -100);
motor[LiftFlagMtr] = DCstop;// Stop lowering the BSM
move(40, 80);
turnTime(250, 100);
move(40, 80); // Drive onto the ramp



}
