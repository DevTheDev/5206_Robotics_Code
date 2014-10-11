/*!
* robot.c
* initialize the robot and build a model of it via structs
*/
#include "consts.h"\
#include "3rd Party Sensor Drivers/drivers/hitechnic-sensormux.h"\
#include "3rd Party Sensor Drivers/drivers/hitechnic-irseeker-v2.h"\
#include "3rd Party Sensor Drivers/drivers/hitechnic-eopd.h"\

// Robot's wheel
typedef struct {
	float radius;
	float circumference;
	float width;
	float around;
	float dRatio;
} Wheel;

// Robot's encoder
typedef struct {
	float ticks;
} Encoder;


// Robot structure
typedef struct {
	Wheel wheel;
	Encoder encoder;
	float optimalSpeed;
} Robot;

// Robot
Robot robot;

/**
* Initilizes all the values and gets the robot ready to go
*/
void initializeRobot()
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
