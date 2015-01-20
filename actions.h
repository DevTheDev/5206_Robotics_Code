/*
Function library for autonomous operations
Tele-op should be done in-line unless it needs to do something that might be needed in tele-op
*/
#include "consts.h"
#undef encodersPerInch
#undef kyler //bye Kyler!//hi Kyler?

#define clamp(a, min, max) (((a) < (min)) ? (min) : (((a) > (max)) ? (max) : (a)))

float absv(float a)
{
    return abs(a);
}

#define sq(a) (a)*(a)

float lerp(float t, float p1, float p2)
{
   return (1-t)*p1 + t*p2;
}

float quadBezier(float t, float p1, float p2, float p3)
{
    return sq(1-t)*p1 + 2.0*t*(1-t)*p2 + sq(t)*p3;
}

void resetLiftEncoders(){
		nMotorEncoder[liftL] = 0;
		nMotorEncoder[liftR] = 0;
	}
void resetDriveEncoders(){
		nMotorEncoder[driveL] = 0;
		nMotorEncoder[driveR] = 0;
}

const float lift_bottom = 0.0;
const float lift_30 = 8.0;
const float lift_60 = 36.0;
const float lift_90 = 62.0;
const float lift_120 = 90.0;

float lift_position = 0;//the desired lift position in cm,0 is the position at the start of teleop, max = 32.5 cm

const float power_difference_per_tick = 5;

float lift_threshhold = 3.0;

const float lift_cm_per_tick = PI*4.6/3.0/encoderticks; //the number of cm the lift raises when the lift motors rotate 1 encoder tick
const float lift_speed_constant = 80.0/(PI/2.0);
const float lift_slow_constant = 1.0/10.0;

void updateLift(){ //TODO: Add constraints on max and min
#if 0
    if(lift_position <= lift_bottom){
		bool do_return = 0;
		if(nMotorEncoder[liftL]*lift_cm_per_tick <= lift_bottom+lift_threshhold){
			do_return = 1;
			motor[liftL] = 0;
			motor[liftR] = 0;
		}
		if(do_return){
			return;
		}
	}
#endif
    motor[liftL] = lift_speed_constant*atan(lift_slow_constant*(lift_position-nMotorEncoder[liftL]*lift_cm_per_tick));//this atan is totally arbitrary and was only chosen because gives a good curve(the motor will be at an approx. const. speed far from the wanted point and will slow down near the wanted point)
    motor[liftR] = motor[liftL];
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
    while(absv(absv(nMotorEncoder[driveL]*drive_cm_per_tick)-distance) >= 0.0)
    {
        correction = 0;//power_difference_per_tick*(nMotorEncoder[driveL] - nMotorEncoder[driveR]);
        motor[driveR] = motor_vIs+correction;
        motor[driveL] = motor_vIs-correction;
    }
    motor[driveR] = 0;
    motor[driveL] = 0;
}

#define robot_half_width 43.5/2.0//middle of the wheel to middle of the wheel

//RHR
void turnAngle(float radians, int motor_vIs){
	int correction = 0;
	while(nMotorEncoder[driveR]*drive_cm_per_tick >= radians*robot_half_width){
		//correction = -power_difference_per_tick*(nMotorEncoder[driveL] + nMotorEncoder[driveR]);
		motor[driveR] = motor_vIs+correction;
		motor[driveL] = -(motor_vIs-correction);
	}
	motor[driveR] = 0;
    motor[driveL] = 0;
}

void launcherOn(int motor_vIs){
    //motor[launcher] = motor_vIs;
}

#define max_auto_launcher 80
void launcherOff(){
    clearTimer(T4);
    while(time1[T4] <= 1000){
        //motor[launcher] = (float)(clamp(lerp((float)time1[T4]/1000, max_auto_launcher, 0.0), 0.0, max_auto_launcher));
    }
}
