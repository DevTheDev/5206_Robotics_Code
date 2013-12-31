//#include "JoystickDriver.c"
#include "actions.h"

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

task main() {
	initializeRobot();
//	initializeRobotnext();

	waitForStart(); // wait for start of tele-op phase

	ClearTimer(T1);
	while(true) {
		getJoystickSettings(joystick);
		singleJoyDrive();
		PaddleAndIntake();
		liftandflag();
	}
}
