#pragma config(Hubs,  S1, HTMotor,  HTServo,  HTMotor,  HTMotor)
#pragma config(Sensor, S2,     turboTouch,  sensorTouch)
#pragma config(Sensor, S3,     blank,         sensorTouch)                                         //currently no sensor here
#pragma config(Sensor, S4,     HTSMUX,        sensorI2CCustom)
#pragma config(Motor,  mtr_S1_C1_1,     LiftMtr1,   tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_2,     LeftDr,        tmotorTetrix, openLoop, encoder, reversed)
#pragma config(Motor,  mtr_S1_C3_1,     RightDr,       tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     FlagMtr,       tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C4_2,     Intake,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C4_1,     LiftMtr2,        tmotorTetrix, openLoop)                      //no current motor here
#pragma config(Servo,  srvo_S1_C2_1,    Turbofan,            tServoStandard)
#pragma config(Servo,  srvo_S1_C2_2,    LeftIntake,           tServoStandard)
#pragma config(Servo,  srvo_S1_C2_3,    RightIntake,          tServoStandard)
#pragma config(Servo,  srvo_S1_C2_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S1_C2_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C2_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//
#include "actions.h"

const tMUXSensor Light1 = msensor_S4_1;
const tMUXSensor Light2 = msensor_S4_2;
const tMUXSensor AutoIR = msensor_S4_3;


//#include "robot.c"

task LeftDrive (){
	motor(LeftDr) = 100;
	wait1Msec(100);
	EndTimeSlice();
}
task RightDrive (){
	motor(RightDr) = 100;
	wait1Msec(100);
	EndTimeSlice();
}
task main()
{
	waitForStart();
	initializeRobot();
	//StartTask(LeftDrive);
	//StartTask(RightDrive);
	//point(90, 80);
	reset();
	turnTime(700, 80);
	writeDebugStreamLine("Done");
	int degreesmoved = (90/360.0) * (robot.wheel.dRatio * robot.wheel.around / robot.wheel.circumference) * (robot.encoder.ticks);
	writeDebugStreamLine("%d",degreesmoved);
	writeDebugStreamLine("%d,%d", nMotorEncoder[LeftDr], nMotorEncoder[RightDr]);
	wait1Msec(100000);
}