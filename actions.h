/*
Function library for autonomous operations
Tele-op should be done in-line unless it needs to do something that might be needed in tele-op
*/
#include "consts.h"
#include "misc.h"
#undef encodersPerInch
#undef kyler //bye Kyler!//hi Kyler?

#define launcher_update_wait 16 //the rate at which we check for a jam
#define launcher_stop_wait 96 //the time the motor needs to be stopped for a jam to be detected

#define launcher_tolerance 5 //the minimum number of andymark motor encoder ticks(280 per revolution) the motor needs to rotate to not trigger a jam

#define MAX_LAUNCHER_POSITIONS (1*kilobyte/sizeof(int))
int launcher_positions[MAX_LAUNCHER_POSITIONS]; //looped buffer
unsigned int current_launcher_position = 0;
short launcher_accounted_time = 0;
bool not_jammed = 1;
short motor_start_time = 0;
int launcher_last_check = 0;

#if 1
void initializeLauncherTimer()
{
    clearTimer(T3);
}

void updateLauncher(int launcher_vIs)
{// it seems to stop after stop_wait after the motor starts
    if(launcher_last_check == 0 && motor[launcher])
    {
        motor_start_time = time1[T3];
    }
    launcher_last_check = motor[launcher];
    if(time1[T3] - launcher_accounted_time > launcher_update_wait)
    {
        launcher_accounted_time += launcher_update_wait;
        launcher_positions[current_launcher_position++] = nMotorEncoder[launcher];
        unsigned int old_launcher_position = (current_launcher_position + MAX_LAUNCHER_POSITIONS - launcher_stop_wait/launcher_update_wait)%MAX_LAUNCHER_POSITIONS;
        if(time1[T3]-motor_start_time > launcher_stop_wait &&
           motor[launcher] > 0 &&
           launcher_positions[current_launcher_position] - launcher_positions[old_launcher_position] < launcher_tolerance)
           //don't run the launcher backwards yet
        {
            not_jammed = 0;
        }
        current_launcher_position++;
        current_launcher_position %= MAX_LAUNCHER_POSITIONS;
    }
    if(not_jammed)
    {
        motor[launcher] = (float)(clamp(lerp((float)time1[T4]/launcher_slow_time, launcher_vIs, 0.0), 0.0, launcher_vIs));
    }
    else
    {
        motor[launcher] = 0;
    }
}
#endif

void resetLiftEncoders()
{
	nMotorEncoder[liftL] = 0;
}

void resetDriveEncoders()
{
	nMotorEncoder[driveR] = 0;
}

const float lift_bottom = 0.0;
const float lift_30 = 8.0;
const float lift_60 = 36.0;
const float lift_90 = 67.0;//62.0;
const float lift_120 = 90.0;

float lift_position = 0;//the desired lift position in cm,0 is the position at the start of teleop, max = 32.5 cm

const float power_difference_per_tick = 5;

float lift_threshhold = 3.0;

const float lift_cm_per_tick = PI*4.6/3.0/encoderticks; //the number of cm the lift raises when the lift motors rotate 1 encoder tick
const float lift_speed_constant = 80.0/(PI/2.0);
const float lift_slow_constant = 1.0/10.0;

float lift_start = lift_bottom;

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

    //writeDebugStreamLine("%i", nMotorEncoder[liftL]);
    motor[liftL] = lift_speed_constant*atan(lift_slow_constant*(lift_position-lift_start-nMotorEncoder[liftL]*lift_cm_per_tick));//this atan is totally arbitrary and was only chosen because gives a good curve(the motor will be at an approx. const. speed far from the wanted point and will slow down near the wanted point)
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
const float drive_cm_per_tick = (2*PI*WHEEL_RADIUS)/encoderticks;
void driveDist(float distance, int motor_vIs) //TODO: both motors separate
{
    distance -= 5.0;
    resetDriveEncoders();

    //int correction = 0;
    while(abs(nMotorEncoder[driveR]*drive_cm_per_tick) < distance)
    {
        //correction = 0;//power_difference_per_tick*(nMotorEncoder[driveL] - nMotorEncoder[driveR]);
        motor[driveR] = motor_vIs;//+correction;
        motor[driveL] = motor_vIs;//-correction;
    }
    motor[driveR] = 0;
    motor[driveL] = 0;
}

#define robot_half_width 40.0/2.0//middle of the wheel to middle of the wheel

//RHR
void turnAngle(float radians, int motor_vIs){
    resetDriveEncoders();

	int correction = 0;
	while(absf(nMotorEncoder[driveR]*drive_cm_per_tick*0.65) <= radians*robot_half_width)
	    {
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
