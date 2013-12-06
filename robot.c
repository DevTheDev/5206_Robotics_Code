#include "consts.c"
#include "actions.c"

// Robot's wheel
typedef struct {
	float radius;
	float circumference;
	float width;
	float around;
	float dRatio;
} Wheel;

// Robot structure
typedef struct {
	Wheel wheel;
	float optimalSpeed;
} Robot;

// Robot
Robot robot;

/**
* Initilizes all the value and get the robot read to go
* (also has a 0.01% change of causing world destruction)
*/
void initializeRobot() // a.k.a. initialize()
{
	// Set robot values
	robot.optimalSpeed = OPTIMAL_SPEED;
	robot.wheel.radius = WHEEL_RADIUS;
	robot.wheel.circumference = WHEEL_RADIUS * 2 * PI;
	robot.wheel.width = WHEEL_WIDTH;
	robot.wheel.around = WHEEL_WIDTH * PI;
	robot.wheel.dRatio = D_RATIO;

	// Encoder Ticks
	robot.encoder.ticks = encoderticks;

	// TODO: Place code here to sinitialize servos to starting positions.

	// TODO: Sensors are automatically configured and setup by ROBOTC. They may need a brief time to stabilize.
	// Maybe wait1Msec(100)?

	return;
}
