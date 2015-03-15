#pragma config(Hubs,  S4, HTMotor,  HTMotor,  HTMotor,  HTServo)
#pragma config(Motor,  mtr_S4_C1_1,     intake,        tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S4_C1_2,     driveL,        tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S4_C2_1,     launcher,      tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S4_C2_2,     liftL,         tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S4_C3_1,     driveR,        tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S4_C3_2,     liftR,         tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S4_C4_1,    goal,                 tServoStandard)
#pragma config(Servo,  srvo_S4_C4_2,    shrub,                tServoContinuousRotation)
#pragma config(Servo,  srvo_S4_C4_3,    net,                  tServoStandard)
#pragma config(Servo,  srvo_S4_C4_4,    wheel,                tServoContinuousRotation)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

//include out libraries first
#include "misc.h"
#include "consts.h"
#include "joystickdriver.c"

// Joystick buttons
#define btnX 1
#define btnA 2
#define btnB 3
#define btnY 4

#define btnLB 5
#define btnRB 6
#define btnLT 7
#define btnRT 8

//Back and Start
#define btnBack 9
#define btnStart 10

//D-Pad buttons
#define tophat_x0y1 0
#define tophat_x1y1 1
#define tophat_x1y0 2
#define tophat_x1yN1 3
#define tophat_x0yN1 4
#define tophat_xN1yN1 5
#define tophat_xN1y0 6
#define tophat_xN1y1 7

//Drive Constants
#define threshold 0.1
#define min_drive 15
#define bezier_drive_control 10
#define max_drive 80 //revert to 80 if driving is strange

//Launcher Constants
#define max_launcher 100
#define unjam_wait 200

//========================Control Functions=========================
static unsigned short prev1Btns = 0;
static unsigned short toggle1Btns = 0;

bool joy1press(unsigned short btn){
    return (((joystick.joy1_Buttons & (~prev1Btns)) >> (btn-1))&1);
}

bool joy1toggle(unsigned short btn){
    return (bool)((toggle1Btns >> (btn-1))&1); //the "&1" should make it 0 or 1 no reason to != 0 it
}

static unsigned short prev2Btns = 0;
static unsigned short toggle2Btns = 0;

bool joy2press(unsigned short btn){
    return (((joystick.joy2_Buttons & (~prev2Btns)) >> (btn-1))&1);
}

bool joy2toggle(unsigned short btn){
    return (bool)((toggle2Btns >> (btn-1))&1); //the "&1" should make it 0 or 1 no reason to != 0 it
}

float thresholdify(float a){
    if(a > threshold){
        return (a-threshold)/(1-threshold);
    }
    else if(a < -threshold){
        return (a+threshold)/(1-threshold);
    }
    return 0;
}

float deadzone(float a){
    if(a > 127.0*threshold){
        return (a-127.0*threshold)/(1.0-threshold);
    }
    else if(a < -127.0*threshold){
        return (a+127.0*threshold)/(1.0-threshold);
    }
    return 0;
}

void stopLauncher()//Duplicated from actions
{
    int launcher_speed = motor[launcher]; //WARNING: Make sure reading the motor speed works

    clearTimer(T4);
    while(time1[T4] < launcher_slow_time)
    {
        motor[launcher] = (float)(clamp(lerp((float)time1[T4]/launcher_slow_time, launcher_speed, 0.0), 0.0, launcher_speed));
    }
}

void allStop()
{
    motor[driveL] = 0;
    motor[driveR] = 0;
    motor[intake] = 0;
    motor[liftL] = 0;
    motor[liftR] = 0;
    stopLauncher();
    servo[shrub] = servo_stop; //Probably want to make this do something special so that we know we're dcd
    servo[goal] = goal_open;
    servo[net] = net_open; //Both should be "safe" positions
}

void checkConnection()
{
    nNoMessageCounterLimit = 750; //4ms per check (3 seconds after disconnect)
    if(bDisconnected == 1)
    {
       allStop();
    }
}
//=============================Control==============================

// Drive Control
#define slow_button joy1press(btnA)
#define fast_button joy1press(btnY)
#define single_joystick_drive 1
#if single_joystick_drive
#define left_drive_control (joystick.joy1_y1 + joystick.joy1_x1)/127.0
#define rght_drive_control (joystick.joy1_y1 - joystick.joy1_x1)/127.0
#else //dual joystick drive
#define left_drive_control joystick.joy1_y1/127.0
#define rght_drive_control joystick.joy1_y2/127.0
#endif

// Lift Control
#define lift_control (joystick.joy2_y2)/127.0

//Intake Control
#define intake_control (joy1toggle(btnRB) || joy2Btn(btnRB))
#define intake_back_control joy2Btn(btnBack)

//Launcher Control
#define launcher_control (joy1toggle(btnRT) || joy2Btn(btnRT))
#define launcher_force_unjam joy2Btn(btnBack)
#define launcher_unjam joy2Btn(btnY)

//Net Control
#define net_pressed joy1press(btnLB)
#define net_close_btn joy2Btn(btnX)
#define net_open_btn joy2Btn(btnB)
#define net_small_btn joy1Btn(btnB)

//Goal Lock Control
#define goal_open_btn joy2Btn(btnLB)
#define goal_close_btn joy2Btn(btnLT)
#define goal_toggle joy1press(btnLT)
#define goal_wheel_forward joy1Btn(btnStart)
#define goal_wheel_backward joy1Btn(btnBack)

//=========================Initialization===========================
//Servo Initializations
bool goal_down = 1;
bool net_down = 0;

float speed_mod = 0.8;

int new_launcher_position = 0;
int old_launcher_position = 0;
int jam_time = 0;
int launcher_time = 0;

//==================================================================
task main()
{
    waitForStart();
    servo[shrub] = servo_stop;
    clearTimer(T3); //unjam timer
    clearTimer(T4); //global timer
    while(1){
        //Control Processing
        prev1Btns = joystick.joy1_Buttons;
        prev2Btns = joystick.joy2_Buttons;
        getJoystickSettings(joystick);
        toggle1Btns = (toggle1Btns^((joystick.joy1_Buttons) & (~prev1Btns)));//invert toggle1Btns when the button goes from low to high
        toggle2Btns = (toggle2Btns^((joystick.joy2_Buttons) & (~prev2Btns)));//invert toggle2Btns when the button goes from low to high

//===============================Drive===============================
#if single_joystick_drive
        {//circle clamping
            float magnitude = sqrt(left_drive_control*left_drive_control+rght_drive_control*rght_drive_control);//the magnitude of the joystick vector
            if(magnitude >= threshold){
                //maximum speed*(the magnitude of the joystick mapped from threshold to 1 to 0 to 1)*(the normalized joystick)
                //= maximum speed*(fraction speed)*(direction)
                float speed = (magnitude-threshold)/(1-threshold);
                speed = speed_mod*quadBezier(speed, min_drive, bezier_drive_control, max_drive);
                int left_vIs = speed*(left_drive_control/magnitude);
                int right_vIs = speed*(rght_drive_control/magnitude);
                motor[driveL] = left_vIs;
                motor[driveR] = right_vIs;
            }
            else{
                motor[driveL] = 0;
                motor[driveR] = 0;
            }
            if(fast_button){
                if(speed_mod != 1.25){
                    speed_mod = 1.25;
                }
                else{
                    speed_mod = 1;
                }
            }
            else if(slow_button){
                if(speed_mod != 0.25){
                    speed_mod = 0.25;
                }
                else{
                    speed_mod = 1;
                }
            }
        }

#else
        {//dual joystick drive //square clamping
            int l = max_drive*thresholdify(left_drive_control);
            int r = max_drive*thresholdify(rght_drive_control);
            motor[driveL] = l;
            motor[driveR] = r;
        }
#endif

        //===============================Intake==============================
        if(intake_back_control)
        {
            motor[intake] = -55;
        }
        else
        {
            motor[intake] = intake_control*50;
        }

        //==============================Launcher=============================

        int time = time1[T4];
        if(launcher_unjam || time-jam_time >= unjam_wait)
        {
            jam_time = time;
            clearTimer(T3);
        }

        if(time1[T3] < 500 && time1[T4] > 500)
        {
            jam_time = time;
            if(time1[T3] < 200)
            {
                motor[launcher] = -40;
            }
        }
        else
        {
            float new_launcher_power = lerp((time-launcher_time)/launcher_slow_time, max_launcher, 0);
            if(new_launcher_power < 0)
            {
                new_launcher_power = 0;
            }

            if(launcher_control)
            {

                old_launcher_position = new_launcher_position;
                new_launcher_position = nMotorEncoder[launcher];

                if(new_launcher_position-old_launcher_position > 0)
                {
                    jam_time = time;
                }

                launcher_time = time;
                new_launcher_power = max_launcher;
            }
            else
            {
                jam_time = time;
            }

            motor[launcher] = new_launcher_power;
        }

        //================================Lift===============================
        float lift_vel = deadzone(joystick.joy2_y1)*100.0/128.0;
        motor[liftL] = lift_vel;
        motor[liftR] = lift_vel;

        //===========================Goal Mechanism==========================
        if(goal_open_btn){
            goal_down = 0;
        }
        if(goal_close_btn){
            goal_down = 1;
        }
        if(goal_toggle)
        {
            goal_down = !goal_down;
        }

        servo[goal] = goal_open + (goal_close-goal_open)*goal_down;

        if(goal_wheel_forward){
            servo[wheel] = servo_stop + 128;
        }
        else if(goal_wheel_backward){
            servo[wheel] = 0;
        }
        else{
            servo[wheel] = servo_stop;
        }


        //===========================Net Mechanism==========================
        if(net_open_btn){
            net_down = 0;
        }
        if(net_close_btn){
            net_down = 1;
        }
        if(net_small_btn){
            net_down = 2;
        }
        if(net_pressed)
        {
            net_down = !net_down;
        }

        const unsigned short net_positions[3] = {net_open, net_close, net_small};

        servo[net] = net_positions[net_down];

        //===============================Shrub===============================
        servo[shrub] = 100*joy2toggle(btnStart)*(sin((float)time1[T1]/1000.0))+127;
        //==========================Low Battery Notification=================
        /*if(externalBattery < 14000){
        playSound(soundException);
        }*/
        //Prevent damage upon fcs failure, MAKE SURE THIS IS WORKING. IT COULD COST US A MATCH
        checkConnection();
    }
}
