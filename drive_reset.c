#pragma config(Hubs,  S4, HTMotor,  HTMotor,  HTMotor,  HTServo)
#pragma config(Sensor, S2,     gyro,           sensorNone)
#pragma config(Sensor, S1,     US,             sensorSONAR)
#pragma config(Motor,  mtr_S4_C1_1,     driveL,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S4_C2_1,     launcher,      tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S4_C2_2,     liftL,         tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S4_C3_1,     driveR,        tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S4_C3_2,     intake,        tmotorTetrix, openLoop, reversed)
#pragma config(Servo,  srvo_S4_C4_1,    goal,                 tServoStandard)
#pragma config(Servo,  srvo_S4_C4_2,    shrub,                tServoContinuousRotation)
#pragma config(Servo,  srvo_S4_C4_3,    net,                  tServoStandard)
#pragma config(Servo,  srvo_S4_C4_4,    side_lock,            tServoStandard)

#include "menu.h"
task main()
{
	clearScreen();
	while(1)
	{
		char displayr[16], displayl[16];
		int drive_r_vIs = 20;
		int drive_l_vIs = 20;
		sprintf(displayr, "DriveR: %i", drive_r_vIs);
		sprintf(displayl, "DriveL: %i", drive_l_vIs);
		bool driveR_pressed = 0;
		bool driveL_pressed = 0;
		if(doMenuItem(displayr))
		{
			driveR_pressed = 1;
		}
		if(doMenuItem(displayl))
		{
			driveL_pressed = 1;
		}
		if(doMenuItem("inc driveR") && time1[T2] >= 200)
		{
    	clearTimer(T2);
      drive_r_vIs += 5;
    }
    if(doMenuItem("dec driveR") && time1[T2] >= 200)
    {
    	clearTimer(T2);
    	drive_r_vIs -= 5;
    }
    if(doMenuItem("inc driveL") && time1[T2] >= 200)
    {
    	clearTimer(T2);
    	drive_l_vIs += 5;
    }
    if(doMenuItem("dec driveL") && time1[T2] >= 200)
    {
    	clearTimer(T2);
    	drive_l_vIs -= 5;
    }
    motor[driveR] = drive_r_vIs*driveR_pressed;
    motor[driveL] = drive_l_vIs*driveL_pressed;
		updateMenu();
   }
}
