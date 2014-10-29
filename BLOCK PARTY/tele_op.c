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
#define normal_teleop 1
#include "tele_op_library.h"

/*const tMUXSensor Light1 = msensor_S4_1;
const tMUXSensor Light2 = msensor_S4_2;
const tMUXSensor AutoIR = msensor_S4_3;

*/
/**
* REMOTE 1
*  Left Joystick - Drive
*  Right Joystick - Paddle (slow variable speed)
*  Button LB / RB - Paddle forward / reverse (fixed fast speed)
*  Button LT / RT - Intake forward / reverse
*  Top Hat, right / left - paddle, one-carriage turn in forward /reverse direction
*    + button A - slow
*
* REMOTE 2
*  Right Joystick - Lift
*  Button A - move lift up slightly
*/

//#define wavey 10.0

task main() {
	//writeDebugStreamLine("OM");
	initializeRobot();
	//	initializeRobotnext();

	clearTimer(T1); // clear the timer for the beeps

	waitForStart(); // wait for start of tele-op phase
	while(true){
		/*string wave = "";
		for(int a = 0; a <= wavey; a++){
		strcat(wave, (a == round(wavey/2*(1+sin(time1[T1]/10.0))) ? "#" : " "));
		}
		writeDebugStreamLine(wave);*/
		updateButtons();
		paddle();
		getJoystickSettings(joystick);
		lift();
		flag();
		singleJoyDrive();
		intakeBlocks();
		deployIntake();

	}
}
