#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  HTServo)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S4,     IR,             sensorI2CCustom)
#pragma config(Motor,  mtr_S1_C1_1,     driveR,        tmotorTetrix, openLoop, driveRight, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     driveL,        tmotorTetrix, openLoop, driveLeft, encoder)
#pragma config(Motor,  mtr_S1_C2_1,     launcher,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_2,     intake,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_1,     liftR,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_2,     liftL,        tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S1_C4_1,    goal,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_2,    gate,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

//include out libraries first
//#include "actions.h"//for lift functions
//TODO: move these to actions.h
float lift_position;//the desired lift position in cm//0 is the position at the start of teleop

const float cm_per_tick = 180/ticks/PI;//the number of cm the lift raises when the lift motors rotate 1 encoder tick
const float lift_speed_constant = 10.0/90.0;
const float lift_slow_constant = 1.0/2.0;
void updateLift(){
    motor[liftL] = lift_speed_constant*atan(*(lift_position-nMotorEncoder[leftL]*cm_per_tick));//this atan is totally arbitrary and was only chosen because gives a good curve(the motor will be at an approx. const. speed far from the wanted point and will slow down near the wanted point)
    motor[liftR] = lift_speed_constant*atan(*(lift_position-nMotorEncoder[leftR]*cm_per_tick));
}

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

#define threshold 0.1
#define max_drive 75
//=============================Controls=============================
static short prev1Btns;
#define joy1toggle(btn) (!joy1Btn(btn)) && (prev1Btns & btn)

// Drive Controls
#define single_joystick_drive 1
#if single_joystick_drive // single joystick drive
#define left_drive_control (joystick.joy1_y1 - joystick.joy1_x1)/128.0
#define rght_drive_control (joystick.joy1_y1 + joystick.joy1_x1)/128.0
#else // dual joystick drive
#define left_drive_control joystick.joy1_y1/128.0
#define rght_drive_control joystick.joy1_y2/128.0
#endif

//Buttons
#define intake_control joystick.joy1_y2/128.0
#define launcher_control joystick.joy1_y1
#define goal_control joy1toggle(btnX);
#define gate_control joy1toggle(btnY);
//===================================================================

float thresholdify(float a){
	if(a > threshold){
		return (a-threshold)/(1-threshold);
	}
	else if(a < -threshold){
		return (a+threshold)/(1-threshold);
	}
	return 0;
}

task main()
{
	while(1){
		clearTimer(T1)
		prev1Btns = joystick.joy1_Buttons;
 		getJoystickSettings(joystick);
//===============================Drive===============================
		#if single_joystick_drive
		{//single joystick drive //circle clamping
			float magnitude = sqrt(left_drive_control*left_drive_control+rght_drive_control*rght_drive_control);//the magnitude of the joystick vector
			if(magnitude >= threshold){
				//maximum speed*(the magnitude of the joystick mapped from threshold to 1 to 0 to 1)*(the normalized joystick)
			//= maximum speed*(fraction speed)*(direction)
				int l = max_drive*(magnitude-threshold)/(1-threshold)*(left_drive_control/magnitude);
				int r = max_drive*(magnitude-threshold)/(1-threshold)*(rght_drive_control/magnitude);
				motor[driveL] = l;
				motor[driveR] = r;
			}
			else{
				motor[driveL] = 0;
				motor[driveR] = 0;
			}
		}
		#else
		{//dual joystick drive //square clamping
			int l = max_drive*thresholdify(left_drive_control);
			int r = max_drive*thresholdify(rght_drive_control);
			motor[lf] = l;
			motor[lb] = l;
			motor[rf] = r;
			motor[rb] = r;
		}
		#endif
//===============================Intake==============================
		motor[intake] = thresholdify(intake_control)*100;
//==============================Launcher=============================
		motor[launcher] = launcher_control*100;
//================================Lift===============================
		lift_position += thresholdify(lift_control)*time1[t1]*1.5;//the rate in (units of lift_position)/ms
		updateLift();
//===========================Goal Mechanism==========================
		servo[goal] = 180*goal_control;
//===========================Gate Mechanism==========================
		servo[gate] = 180*gate_control;
	}
}
