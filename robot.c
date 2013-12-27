/*!
 * robot.c
 * initialize the robot and build a model of it via structs
 */

#include "consts.c"

// Setup robot sensors and motors
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
