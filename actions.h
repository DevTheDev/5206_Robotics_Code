/*
Function library for autonomous operations
Tele-op should be done in-line unless it needs to do something that might be needed in tele-op
*/
#include "consts.h"
#include "misc.h"

#include "3rd Party Sensor Drivers\drivers\hitechnic-irseeker-v2.h"

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
const float lift_90 = 57.0;//62.0; needs to be lowered for new net
const float lift_120 = 85.0;

float lift_position = 0;//the desired lift position in cm, is the position at the start of teleop, max = 32.5 cm

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

bool seeIR(tHTIRS2 * irseeker)
{
    readSensor(irseeker);
    return irseeker->acDirection == 6;
}

void calibrateGyro()
{
    const int itts = 500;
    offset = 0;
    char load_display[16];
    for(int i = 0; i < itts; i++)
    {
        int line = 3;
        sprintf(load_display, "%3.0f'/. complete", (float)(i)*100.0/itts);
        displayCenteredTextLine(line++, "Calibrating");
        displayCenteredTextLine(line++, "Please don't move");
        displayCenteredTextLine(line++, load_display);

        offset += SensorValue[gyro];

        wait1Msec(5);
    }
    offset /= itts;
}

void calibrateGyroWithLinearRegression()
{
    char load_display[16];

    float sigma_x = 0;
    float sigma_xx = 0;
    float sigma_y = 0;
    float sigma_xy = 0;
    const int n = 500;

    clearTimer(T1);

    float theta = 0;

    float new_time = time1[T1]/1000.0;
    float old_time = new_time;
    for(int i = 0; i < n; i++)
    {
        int line = 3;
        sprintf(load_display, "%1.3f complete", (float)(i)/n);
        displayCenteredTextLine(line++, "Calibrating");
        displayCenteredTextLine(line++, "Please do not move");
        displayCenteredTextLine(line++, load_display);
        float omega = SensorValue[gyro];
        old_time = new_time;
        new_time = time1[T1]/1000.0;
        float dt = new_time-old_time;
        theta += dt*omega;

        sigma_x += new_time;
        sigma_xx += sq(new_time);
        sigma_y += theta;

        sigma_xy += new_time*theta;

        wait1Msec(5);
    }
    offset = (n*sigma_xy - sigma_x*sigma_y)
               / (n*sigma_xx - sq(sigma_x)); //slope of the least squares fit
}

float US_dist = 0.0;
float US_dist_temp = US_dist;
task ultrasonic_loop()
{
    float US_error_est = 1.0;
    US_dist_temp = 0.0;
    US_dist = 0.0;

    for ever
    {
        kalmanUpdate(&US_dist_temp, &US_error_est, SensorValue[US], 4.0, 10.0);
        US_dist = US_dist_temp; //just to be safe with multitasking, should check if a incomplete update is possible
    }
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

#define gyro_adjustment 8.0/9.0

//RHR
void turnAngle(float degrees, int motor_vIs){//80 deg at 50 power = 90 deg turn, 40 deg at 50 power = 45 deg turn
    clearTimer(T1);
    motor[driveR] = motor_vIs;
    motor[driveL] = -(motor_vIs);
    float theta = 0;
    char gyro_value[16];
    sprintf(gyro_value, "Initial: %f", theta);
    displayCenteredTextLine(1, gyro_value);
    while(abs(theta) < degrees)
    {
        float dt = time1[T1]/(1000.0);
        clearTimer(T1);

        float omega = SensorValue[gyro]-offset;
        theta += dt*omega;//*gyro_adjustment; //rectangular approx.
        wait1Msec((8-T1)%8);
        sprintf(gyro_value, "Current: %f", theta);
        displayCenteredTextLine(2, gyro_value);

    }
    motor[driveR] = 0;
    motor[driveL] = 0;
    sprintf(gyro_value, "Final: %f", theta);
    displayCenteredTextLine(3, gyro_value);
}

//normal turnAngle, but with a kalman filter on the gyro
#define degrees_per_second_per_unit_motor_vIs 1
void kalmanTurnAngle(float degrees, int motor_vIs)
{
    clearTimer(T1);
    motor[driveR] = motor_vIs;
    motor[driveL] = -(motor_vIs);
    float theta = 0;
    float omega = 0;
    float error_est = 0;
    while(abs(theta) < degrees)
    {
        float dt = time1[T1]/(1000.0);
        clearTimer(T1);

        kalmanUpdate(&omega, &error_est, SensorValue[gyro] - offset, motor_vIs*degrees_per_second_per_unit_motor_vIs, 1.0, 1.0);
        theta += dt*omega;
    }
    motor[driveL] = 0;
    motor[driveR] = 0;
}

void startLauncher(float launcher_speed)
{
    motor[launcher] = launcher_speed;
}

void stopLauncher()
{
    int launcher_speed = motor[launcher]; //WARNING: Make sure reading the motor speed works

    clearTimer(T4);
    while(time1[T4] < launcher_slow_time)
    {
        motor[launcher] = (float)(clamp(lerp((float)time1[T4]/launcher_slow_time, launcher_speed, 0.0), 0.0, launcher_speed));
    }
}
