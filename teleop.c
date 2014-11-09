#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Motor,  mtr_S1_C1_1,     lf,            tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C1_2,     lb,            tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C2_1,     rf,            tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C2_2,     rb,            tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_1,     intake,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_2,     launcher,      tmotorTetrix, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

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
//===================================================================

float thresholdify(float a){
	if(a >= threshold){
		return (a-threshold)/(1-threshold);
	}
	else if(a <= -threshold){
		return (a+threshold)/(1-threshold);
	}
	return 0;
}

float clampDrive(float a){
	if(abs(a) <= threshold){
		return 0;
	}
	return max_drive*(a-threshold)/(1-threshold);
}

task main()
{
	while(1){
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
				motor[lf] = l;
				motor[lb] = l;
				motor[rf] = r;
				motor[rb] = r;
			}
			else{
				motor[lf] = 0;
				motor[lb] = 0;
				motor[rf] = 0;
				motor[rb] = 0;
			}
		}
		#else
		{//dual joystick drive //square clamping
			int l = clampDrive(left_drive_control);
			int r = clampDrive(rght_drive_control);
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
	}
}
