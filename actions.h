/*
Function library for autonomous operations
Tele-op should be done in-line unless it needs to do something that might be needed in tele-op
*/
#include "consts.h"
#undef encodersPerInch
//#undef kyler //bye Kyler!//hi Kyler?

void resetLiftEncoders(){
		nMotorEncoder[liftL] = 0;
		nMotorEncoder[liftR] = 0;
	}
void resetDriveEncoders(){
		nMotorEncoder[driveL] = 0;
		nMotorEncoder[driveR] = 0;
}

const float lift_bottom = 5.0;
const float lift_30 = 15.0;
const float lift_60 = 35.0;
const float lift_90 = 65.0;
const float lift_120 = 105.0;

float lift_position = 0;//the desired lift position in cm,0 is the position at the start of teleop, max = 32.5 cm

const float power_difference_per_tick = 5;

float lift_threshhold = 3.0;

float lift_rotations_per_full_height = 3.5;
const float lift_cm_per_tick = (120.0-18.0*2.54)/lift_rotations_per_full_height/encoderticks;//the number of cm the lift raises when the lift motors rotate 1 encoder tick
const float lift_speed_constant = 50.0/(PI/2.0);
const float lift_slow_constant = 1.0/10.0;

void updateLift(){ //TODO: Add constraints on max and min
		if(lift_position <= lift_bottom){
			bool do_return = 0;
			if(nMotorEncoder[liftL]*lift_cm_per_tick <= lift_bottom+lift_threshhold){
				do_return = 1;
				motor[liftL] = 0;
			}
			if(nMotorEncoder[liftR]*lift_cm_per_tick <= lift_bottom+lift_threshhold){
				do_return = 1;
				motor[liftR] = 0;
			}
			if(do_return){
				return;
			}
		}
    int correction = 0;//nMotorEncoder[liftL] - nMotorEncoder[liftR]*power_difference_per_tick;
    motor[liftL] = lift_speed_constant*atan(lift_slow_constant*(lift_position-nMotorEncoder[liftL]*lift_cm_per_tick)) + correction;//this atan is totally arbitrary and was only chosen because gives a good curve(the motor will be at an approx. const. speed far from the wanted point and will slow down near the wanted point)
    motor[liftR] = lift_speed_constant*atan(lift_slow_constant*(lift_position-nMotorEncoder[liftR]*lift_cm_per_tick)) - correction;
}

/*
void updateLift(){ //TODO: Add constraints on max and min
	if(lift_position > lift_cm_per_tick*nMotorEncoder[liftL]){
		motor[liftL] = 50;
	}
	else if(lift_position < lift_cm_per_tick*nMotorEncoder[liftL]){
		motor[liftL] = -50;
	}
	else{
		motor[liftL] = 0;
	}
	if(lift_position > lift_cm_per_tick*nMotorEncoder[liftR]){
		motor[liftR] = 50;
	}
	else if(lift_position < lift_cm_per_tick*nMotorEncoder[liftR]){
		motor[liftR] = -50;
	}
	else{
		motor[liftR] = 0;
	}
}
*/
const float drive_cm_per_tick = (PI*2*2.54)/encoderticks;
void driveDist(float distance, int motor_vIs) //TODO: both motors separate
{
    resetDriveEncoders();
    
    int correction = 0;
    while(abs(abs(nMotorEncoder[driveL]*drive_cm_per_tick)-distance) >= 0.0)
    {
        correction = 0;//power_difference_per_tick*(nMotorEncoder[driveL] - nMotorEncoder[driveR]);
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
