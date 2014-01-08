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

#include "actions.h"

Menu resetMenu;

#define options 4
/**
 * The menus constructor
*/
void initResetMenu(string * itemNames, string * infos){
	resetMenu.itemCount = options;
	resetMenu.selected = 0;
	resetMenu.itemNames = itemNames;
	resetMenu.infos = infos;
}

/**
	workaround to robot c not having function pointers
*/
void activateResetMenu(int f){
	switch(resetMenu.selected){
		case 0: //lift
			switch(f){
				default:
					motor[LiftFlagMtr] = 0;
					break;
				case 0:
					motor[LiftFlagMtr]=-100;
					break;
				case 1:
					motor[LiftFlagMtr]=100;
					break;
			}
			break;

		case 1: //hopper
			switch(f){default:
					motor[PaddleMtr] = 0;
					break;
				case 0:
					motor[PaddleMtr]=-20;
					break;
				case 1:
					motor[PaddleMtr]=20;
					break;
			}
			break;

		case 2: //intake
			switch(f){
				default:
					servo[LeftIntake] = intakestop;
					servo[RightIntake] = intakestop;
					break;
				case 0:
					servo[LeftIntake] = -255;
					servo[RightIntake] = 255;
					break;
				case 1:
					servo[LeftIntake] = 255;
					servo[RightIntake] = -255;
					break;

			}
			break;

		case 3:
			if(f == -1){
				stopRobot();
				break;
			}
			motor[PaddleMtr] = -20;
			//motor[LiftFlagMtr] = -50;
			servo[LeftIntake] = -255;
			servo[RightIntake] = -255;
			wait1Msec(1000);
			motor[PaddleMtr] = 20;
			//motor[LiftFlagMtr] = 50;
			servo[LeftIntake] = 255;
			servo[RightIntake] = 255;
			wait1Msec(1000);
			stopRobot();
			break;
			case 4: //IR
			switch(f){default:
				case 0:
					clearScreen();
					nxtDisplayCenteredTextLine(2, %d%, SensorValue[AutoIR]);
					break;
				case 1:
					clearScreen();
					nxtDisplayCenteredTextLine(2, %d%, SensorValue[AutoIR]);
					break;
			}
	}
}

task main()
{
	string itemNames[options] = {"Lift", "Hopper", "Intake", "IR"};
	string infos[options] = {"", "", "", ""};
	initResetMenu(itemNames, infos);
	clearScreen();
	while(true){
		displayMenu(resetMenu);
		if(pressed(orangebutton)){
			selectNext(resetMenu);
		}

		if(held(leftarrow, 1)){
			activateResetMenu(0);
		}

		else if(held(rightarrow, 1)){
			activateResetMenu(1);
		}
		else {
			activateResetMenu(-1);
		}

		updateButtons();
	}
}
