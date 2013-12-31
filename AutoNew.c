// Includes consts.c and robot.c, as well
// Drivers can be found in Google Drive
#include "actions.h"
int driveTurns;
/**
* returns true if the robot is aligned with the beacon
*/
bool aligned(){
	return nMotorEncoder[RightDr] > 1440*24/robot.wheel.circumference*robot.wheel.dRatio;
}
task main()
{
	// Wait for start
	initializeRobot();
	waitForStart();
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
	//return;
	// Drive to pendulum
	turnTime(700, -100);
	wait1Msec(100);
	move(12, 40);
	// Score the block
	scoreBlocks();
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
