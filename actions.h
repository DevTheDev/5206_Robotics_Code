/*
Function library for autonomous operations
Tele-op should be done in-line unless it needs to do something that might be needed in tele-op
*/
#include "consts.h"
#undef encodersPerInch
#undef kyler //bye Kyler!

const float lift_bottom = 0.0;
const float lift_30 = 0.0;
const float lift_60 = 15.0;
const float lift_90 = 45.0;
const float lift_120 = 85.0;

float lift_position = 0;//the desired lift position in cm,0 is the position at the start of teleop, max = 32.5 cm

const float power_difference_per_tick = 5;

const float lift_cm_per_tick = 30.0/encoderticks;//the number of cm the lift raises when the lift motors rotate 1 encoder tick
const float lift_speed_constant = 80.0/(PI/2.0);
const float lift_slow_constant = 1.0/20.0;
void updateLift(){ //TODO: Add constraints on max and min
    int correction = nMotorEncoder[liftL] - nMotorEncoder[liftR]*power_difference_per_tick;
    motor[liftL] = lift_speed_constant*atan(lift_slow_constant*(lift_position-nMotorEncoder[liftL]*lift_cm_per_tick)) + correction;//this atan is totally arbitrary and was only chosen because gives a good curve(the motor will be at an approx. const. speed far from the wanted point and will slow down near the wanted point)
    motor[liftR] = lift_speed_constant*atan(lift_slow_constant*(lift_position-nMotorEncoder[liftR]*lift_cm_per_tick)) - correction;
}

const float drive_cm_per_tick = (pi*2*2.54)/encoderticks;

void driveDist(int distance, int motor_vIs){ //TODO: both motors separate
	int correction = 0;
	while(nMotorEncoder[driveL]*drive_cm_per_tick >= distance){
		correction = power_difference_per_tick*(nMotorEncoder[driveL] - nMotorEncoder[driveR]);
		motor[driveR] = motor_vIs+correction;
		motor[driveL] = motor_vIs-correction;
	}
}

#define robot_half_width 43.5/2.0//middle of the wheel to middle of the wheel

//RHR
void turnAngle(float radians, int motor_vIs){
	int correction = 0;
	while(nMotorEncoder[driveR]*drive_cm_per_tick >= radians*robot_half_width){
		correction = -power_difference_per_tick*(nMotorEncoder[driveL] + nMotorEncoder[driveR]);
		motor[driveR] = motor_vIs+correction;
		motor[driveL] = -(motor_vIs-correction);
	}
}
