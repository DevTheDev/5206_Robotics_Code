#pragma config(Hubs,  S1, HTMotor,  HTServo,  HTMotor,  none)
#pragma config(Sensor, S2,     PaddleEOPD,     sensorAnalogActive)
#pragma config(Sensor, S3,     AutoIR,         sensorHiTechnicIRSeeker1200)
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

// Includes consts.c and robot.c, as well
// Drivers can be found in Google Drive
#include "actions.h"
int driveTurns;
#define options 3
typedef string strArray[options];
int numberOfGoals = 0;
int bridgeSpot = 0;
bool forwardBackward = false; //back = false
/**
* returns true if the robot is aligned with the beacon
*/
bool aligned(){
	return SensorValue[AutoIR] == irZone || nMotorEncoder[RightDr] > distanceBetweenPend*(numberOfGoals + 1);
}
Menu autoChooser;
/**
* The menus constructor
*/
void initautoChooser(strArray itemNames, strArray infos){
	autoChooser.itemCount = options;
	autoChooser.selected = 0;
	for(int i = 0; i < options; i++){
		autoChooser.itemNames[i] = itemNames[i];
		autoChooser.infos[i] = infos[i];
	}
}

/**
workaround to robot c not having function pointers
*/
void activateautoChooser(int f){
	switch(autoChooser.selected){
		case 0:
			if (f){
				numberOfGoals++;
			}
			else {
				numberOfGoals--;
			}
			numberOfGoals %= 4;
			if(numberOfGoals < 0){
				numberOfGoals += 4;
			}
			//*autoChooser.infos = numberOfGoals+1;
			StringFormat(autoChooser.infos[0], "%d", numberOfGoals);
			break;
		case 1:
			forwardBackward = !forwardBackward;
			autoChooser.infos[1] = forwardBackward ? "Forward" : "Backward";
			break;
		case 2:
			if (f){
				bridgeSpot++;
			}
			else {
				bridgeSpot--;
			}
			StringFormat(autoChooser.infos[2], "%d", bridgeSpot);
			break;
	}
}

task main()
{
	// Wait for start
	initializeRobot();
	string itemNames[options] = {"# of goals", "Forward/Back", "Bridge Park"};
	string infos[options] = {"", "", ""};
	initautoChooser(itemNames, infos);

	clearScreen();
	while(!held(orangebutton, 3000)){
		displayMenu(autoChooser);
		if(pressed(orangebutton)){
			selectNext(autoChooser);
		}

		if(pressed(leftarrow)){
			activateautoChooser(0);
		}

		else if(pressed(rightarrow)){
			activateautoChooser(1);
		}

		updateButtons();
	}
	clearScreen();
	nxtDisplayCenteredBigTextLine(4, "Ready!");
	waitForStart();
	//int autoCount;
	//int distToMove;
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
	// Drive to pendulum
	turnTime(700, -100);
	wait1Msec(100);
	move(7, 40);
	// Score the block
	scoreBlocks();
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
