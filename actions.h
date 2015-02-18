/*
Function library for autonomous operations
Tele-op should be done in-line unless it needs to do something that might be needed in tele-op
*/
#include "consts.h"
#include "misc.h"

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
const float lift_30 = 8.0;//needs to be lowered for new net
const float lift_60 = 32.0;
const float lift_90 = 60.0;//62.0; needs to be lowered for new net
const float lift_120 = 85.0;

float lift_position = 0;//the desired lift position in cm,0 is the position at the start of teleop, max = 32.5 cm

const float power_difference_per_tick = 5;

float lift_threshhold = 3.0;

const float lift_cm_per_tick = PI*4.6/3.0/encoderticks; //the number of cm the lift raises when the lift motors rotate 1 encoder tick
const float lift_speed_constant = 80.0/(PI/2.0);
const float lift_slow_constant = 1.0/10.0;

float lift_start = lift_bottom;

void updateLift(){ //TODONT: Add constraints on max and min
    motor[liftL] = lift_speed_constant*atan(lift_slow_constant*(lift_position-lift_start-nMotorEncoder[liftL]*lift_cm_per_tick));//this atan is totally arbitrary and was only chosen because gives a good curve(the motor will be at an approx. const. speed far from the wanted point and will slow down near the wanted point)
    motor[liftR] = motor[liftL];
}

float offset = 0;

void calibrateGyro()
{
    const int itts = 500;
    offset = 0;
    char load_display[16];
    for(int i = 0; i < itts; i++)
    {
        int line = 3;
        sprintf(load_display, "%1.3f complete", (float)(i)/itts);
        displayCenteredTextLine(line++, "Calibrating");
        displayCenteredTextLine(line++, "Please do not move");
        displayCenteredTextLine(line++, load_display);

        offset += SensorValue[gyro];

        wait1Msec(5);
    }
    offset /= itts;
}

const float drive_cm_per_tick = (2*PI*WHEEL_RADIUS)/encoderticks;
void driveDist(float distance, int motor_vIs) //TODO: both motors separate
{
    distance -= 5.0;
    resetDriveEncoders();

    motor[driveR] = motor_vIs;
    motor[driveL] = motor_vIs;

    while(abs(nMotorEncoder[driveR]*drive_cm_per_tick) < distance);

    motor[driveR] = 0;
    motor[driveL] = 0;
}

#define robot_half_width 40.0/2.0//middle of the wheel to middle of the wheel

#define gyro_adjustment 60.0/65.0 //TODO: bash this
//RHR
void turnAngle(float degrees, int motor_vIs){//80 deg at 50 power = 90 deg turn, 40 deg at 50 power = 45 deg turn
    clearTimer(T1);
	motor[driveR] = motor_vIs;
	motor[driveL] = -(motor_vIs);
	float theta = 0;
	char namE[16];
    sprintf(namE, "Initial: %f", theta);
    displayCenteredTextLine(1, namE);
    while(abs(theta) < degrees)
	{
	    float dt = time1[T1]/(1000.0);
        clearTimer(T1);

        float omega = SensorValue[gyro]-offset;
        theta += dt*omega;//*gyro_adjustment; //rectangular approx.
        wait1Msec((8-T1)%8);
        char bad_at_names[16];
        sprintf(bad_at_names, "Current: %f", theta);
        displayCenteredTextLine(2, bad_at_names);

	}
	motor[driveR] = 0;
    motor[driveL] = 0;
    char bad_at_names[16];
    sprintf(bad_at_names, "Final: %f", theta);
    displayCenteredTextLine(3, bad_at_names);
}
