#pragma config(Hubs,  S4, HTMotor,  HTMotor,  HTMotor,  HTServo)
#pragma config(Motor,  mtr_S4_C1_1,     driveL,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S4_C1_2,     motorE,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S4_C2_1,     launcher,      tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S4_C2_2,     liftL,         tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S4_C3_1,     driveR,        tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S4_C3_2,     intake,        tmotorTetrix, openLoop, reversed)
#pragma config(Servo,  srvo_S4_C4_1,    goal,                 tServoStandard)
#pragma config(Servo,  srvo_S4_C4_2,    shrub,                tServoContinuousRotation)
#pragma config(Servo,  srvo_S4_C4_3,    net,                  tServoStandard)
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
#define max_drive 100 //revert to 80 if driving is strange

//D-Pad Distances
#define distance_shift 10
#define distance_back 15
#define distance_forward sqrt(sq(distance_shift) + sq(distance_back))
#define theta_shift atan2(distance_shift,distance_back)//Evidently this is a known bug in robotc, atan doesn't like fractions
#define robot_half_width 20.0

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
//Dpad functions
void runMotors(int rmotor_vis, int lmotor_vis)
{
    motor[driveR] = lmotor_vis;
    motor[driveL] = rmotor_vis;
}
const float drive_cm_per_tick = (2*PI*WHEEL_RADIUS)/encoderticks;//Should this be in consts?
bool dpadDrive(int distance)
{
    return (abs(nMotorEncoder[driveR]*drive_cm_per_tick) <= distance);
}

bool dpadTurn(float angle)
{
    return (abs(nMotorEncoder[driveR]*0.65*drive_cm_per_tick) <= angle*robot_half_width);
}

int getLauncherPower()
{
    return (int) clamp(1350/externalBattery * 100, 90, 100);
}
//=============================Control==============================

// Drive Control
#define slow_button joy1press(btnA)
#define fast_button joy1press(btnY)
#define single_joystick_drive 1
#if single_joystick_drive
#if 1 //NORMAL DRIVE
#define left_drive_control (joystick.joy1_y1 + joystick.joy1_x1)/127.0
#define rght_drive_control (joystick.joy1_y1 - joystick.joy1_x1)/127.0
#else
#define left_drive_control (-joystick.joy1_y1 + joystick.joy1_x1)/127.0
#define rght_drive_control (-joystick.joy1_y1 - joystick.joy1_x1)/127.0
#endif
#else //dual joystick drive
#define left_drive_control joystick.joy1_y1/127.0
#define rght_drive_control joystick.joy1_y2/127.0
#endif

//D-Pad Control
#define joy1x0y1   (joystick.joy1_TopHat == 0 && last_dpad != 0)
#define joy1x1y1   (joystick.joy1_TopHat == 1 && last_dpad != 1)
#define joy1x1y0   (joystick.joy1_TopHat == 2 && last_dpad != 2)
#define joy1x1yN1  (joystick.joy1_TopHat == 3 && last_dpad != 3)
#define joy1x0yN1  (joystick.joy1_TopHat == 4 && last_dpad != 4)
#define joy1xN1yN1 (joystick.joy1_TopHat == 5 && last_dpad != 5)
#define joy1xN1y0  (joystick.joy1_TopHat == 6 && last_dpad != 6)
#define joy1xN1y1  (joystick.joy1_TopHat == 7 && last_dpad != 7)

// Lift Control
#define lift_control (joystick.joy2_y2)/127.0

//Intake Control
#define intake_toggle_button btnRB
#define intake_control (joy1toggle(btnRB) || joy2Btn(btnRB))
#define intake_back_control joy2Btn(btnBack)

//Launcher Control
#define launcher_toggle_button btnRT
#define launcher_control (joy1toggle(launcher_toggle_button) || joy2Btn(btnRT))
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

#define dpad_toggle joy1toggle(btnBack)

//=========================Initialization===========================
//Servo Initializations
bool goal_down = 1;
bool net_down = 0;

float speed_mod = 0.8;

int new_launcher_position = 0;
int old_launcher_position = 0;
int jam_time = 0;
int launcher_time = 0;
int unjam_time = 0;
bool intake_on = 1;


//D-Pad Toggles (Probably a sloppy way of doing it)
bool driving_forward = 0;
bool driving_backward = 0;
bool turning_left = 0;
bool turning_right = 0;
bool shifting_left = 0;
bool shifting_right = 0;
bool back_shifting_right = 0;
bool back_shifting_left = 0;
int shifting_state = 0;
int last_dpad = -1;
bool dpad_active = 0;
//==================================================================
void allStop()
{
    joystick.joy1_x1 = 0;
    joystick.joy1_y1 = 0;
    joystick.joy2_y1 = 0;
    toggle1Btns = 0;
    servo[shrub] = 25*(sin((float)time1[T1]/1000.0)) + 127;//Sin wave means DC'd regular for epicycle
    goal_down = 0;
    net_down = 0; //Both should be "safe" positions
}

bool checkConnection()
{
    nNoMessageCounterLimit = 750; //4ms per check (3 seconds after disconnect)
    if(bDisconnected == 1)
    {
        allStop();
        return 1;
    }
    return 0;
}

void evaluateTurn(int vIs)
{
    dpad_active = true;
    if(dpadTurn(pi/2))
    {
        runMotors(-vIs, vIs);
    }
    else if(!dpadTurn(pi/2))
    {
        runMotors(0, 0);
        nMotorEncoder[driveR] = 0;
        turning_right = false;
        turning_left = false;
        dpad_active = false;
    }
}

void evaluateDistance(int vIs, float distance)
{
    dpad_active = true;
    if(dpadDrive(distance))
    {
        runMotors(vIs, vIs);
    }
    else if(!dpadDrive(distance))
    {
        runMotors(0, 0);
        nMotorEncoder[driveR] = 0;
        driving_forward = false;
        driving_backward = false;
        dpad_active = false;
    }
}

void evaluateShift(int rturn_vIs, int drive_vIs)
{

    dpad_active = true;
    switch (shifting_state)
    {
        case 1:
            if(dpadDrive(distance_back))
            {
                runMotors(drive_vIs, drive_vIs);
            }
            else if(!dpadTurn(distance_back))
            {
                runMotors(0,0);
                nMotorEncoder[driveR] = 0;
                shifting_state++;
            }
            break;
        case 2:
            if(dpadTurn(theta_shift))
            {
                runMotors(-rturn_vIs, rturn_vIs);
            }
            else if(!dpadTurn(theta_shift))
            {
                runMotors(0,0);
                nMotorEncoder[driveR] = 0;
                shifting_state++;
            }
            break;
        case 3:
            if(dpadDrive(distance_forward))
            {
                runMotors(drive_vIs, drive_vIs);
            }
            else if(!dpadDrive(distance_forward))
            {
                runMotors(0,0);
                nMotorEncoder[driveR] = 0;
                shifting_state++;
            }
            break;
         case 4:
            if(dpadTurn(theta_shift))
            {
                runMotors(rturn_vIs, -rturn_vIs);
            }
            else if(!dpadTurn(theta_shift))
            {
                    runMotors(0,0);
                    shifting_right = false;
                    shifting_left = false;
                    back_shifting_right = false;
                    back_shifting_left = false;
                    dpad_active = false;
            }
            break;
        }
    }


//==================================================================
task main()
{
    waitForStart();
    servo[shrub] = servo_stop;
    clearTimer(T3); //unjam timer

    while(1){
        //Control Processing
        prev1Btns = joystick.joy1_Buttons;
        prev2Btns = joystick.joy2_Buttons;
        last_dpad = joystick.joy1_TopHat;
        getJoystickSettings(joystick);
        toggle1Btns = (toggle1Btns^((joystick.joy1_Buttons) & (~prev1Btns)));//invert toggle1Btns when the button goes from low to high
        toggle2Btns = (toggle2Btns^((joystick.joy2_Buttons) & (~prev2Btns)));//invert toggle2Btns when the button goes from low to high

//===============================Drive===============================
#if single_joystick_drive
        if(!dpad_active){//circle clamping
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
        //===============================DPAD================================
        if(!dpad_toggle){//Allows for us to disable the dpad if for some reason the controller is messed up
            if(joystick.joy1_TopHat != -1)
            {
                nMotorEncoder[driveR] = 0;
            }

            if(driving_forward || joy1x0y1)
            {
                driving_forward = true;
                evaluateDistance(50, 10);
            }

            if(driving_backward || joy1x0yN1)
            {
                driving_backward = true;
                evaluateDistance(-50, 10);
            }

            if(turning_right || joy1x1y0)
            {
                turning_right = true;
                evaluateTurn(50);
            }
            if(turning_left || joy1xN1y0)
            {
                turning_left = true;
                evaluateTurn(-50);
            }

            if(joy1x1y1 || joy1xN1y1 || joy1xN1yN1 || joy1x1yN1)
            {
                shifting_state = 1;
            }

            if(shifting_right || joy1x1y1)
            {
                shifting_right = true;
                evaluateShift(50, 50);
            }
            if(shifting_left || joy1xN1y1)
            {
                shifting_left = true;
                evaluateShift(-50, 50);
            }
            if(back_shifting_right || joy1x1yN1)
            {
                back_shifting_right = true;
                evaluateShift(-50, -50);
            }
            if(back_shifting_left || joy1xN1yN1)
            {
                back_shifting_left = true;
                evaluateShift(50, -50);
            }
        }
        else if(dpad_toggle)
        {
            turning_left = false;
            turning_right = false;
            shifting_state = 0;
            shifting_left = false;
            shifting_right = false;
            back_shifting_left = false;
            back_shifting_right = false;
            dpad_active = false;
        }

        //==============================Launcher=============================

        int dt = time1[T4];
        clearTimer(T4);

        const int unjam_total = 500;

        if(launcher_unjam || jam_time >= unjam_wait)
        {
            jam_time = 0;
            unjam_time = unjam_total;
            intake_on = 0;
            nMotorEncoder[launcher] = 0;
        }

        if(unjam_time > 0)
        {
            motor[intake] = 0;
            if(nMotorEncoder[launcher] > -100)
            {
                motor[launcher] = -40;//Fix timer reset, stop intake when jammed
            }
            else
            {
                motor[launcher] = 0;
            }

            unjam_time -= dt;
        }
        else
        {
            //===============t================Intake==============================
	        if(intake_back_control)
	        {
	            motor[intake] = -55;
	        }
	        else
	        {
	            motor[intake] = intake_control*60;
	        }

            launcher_time -= dt;

            if(launcher_control)
            {

                old_launcher_position = new_launcher_position;
                new_launcher_position = nMotorEncoder[launcher];

                if(new_launcher_position-old_launcher_position <= 1)
                {
                    jam_time += dt;
                }
                else
                {
                    jam_time = 0;
                }

                launcher_time = launcher_slow_time;
            }
            else
            {
                jam_time = 0;
            }

            float new_launcher_power = lerp(((float)launcher_time)/launcher_slow_time, 0, getLauncherPower());
            if(new_launcher_power < 0)
            {
                new_launcher_power = 0;
            }

            motor[launcher] = new_launcher_power;
        }
        //================================Lift===============================
        float lift_vel = deadzone(joystick.joy2_y1)*100.0/128.0;
        motor[liftL] = lift_vel;
        //motor[liftR] = lift_vel;

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

        const unsigned short net_positions[3] = {net_open, net_close, net_endgame};

        servo[net] = net_positions[net_down];

        //===============================Shrub===============================
        if(!checkConnection())//Prevent damage upon fcs failure, MAKE SURE THIS IS WORKING. IT COULD COST US A MATCH
        {
        servo[shrub] = 100 * joy1toggle(btnX) + 127;
        }
        //==========================Low Battery Notification=================
        /*if(externalBattery < 14000){
        playSound(soundException);
        }*/
    }
}
