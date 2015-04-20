#pragma config(Hubs,  S4, HTMotor,  HTMotor,  HTMotor,  HTServo)
#pragma config(Sensor, S2,     gyro,           sensorNone)
#pragma config(Sensor, S1,     US,             sensorSONAR)
#pragma config(Motor,  mtr_S4_C1_1,     driveL,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S4_C1_2,     motorE,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S4_C2_1,     launcher,      tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S4_C2_2,     liftL,         tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S4_C3_1,     driveR,        tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S4_C3_2,     intake,        tmotorTetrix, openLoop, reversed)
#pragma config(Servo,  srvo_S4_C4_1,    goal,                 tServoStandard)
#pragma config(Servo,  srvo_S4_C4_2,    shrub,                tServoContinuousRotation)
#pragma config(Servo,  srvo_S4_C4_3,    net,                  tServoStandard)
#pragma config(Servo,  srvo_S4_C4_4,    side_lock,            tServoStandard)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//
#pragma DebuggerWindows("debugStream")
#pragma DebuggerWindows("nxtLCDScreen")
#pragma DebuggerWindows("joystickGame")

#include "pathfinding.h"

task main()
{
    startTask(ultrasonic_loop);
    calibrateGyro();
    turnAngle(30, 50);
    turnToGoal(45, -50);
    clearTimer(T1);
    motor[driveL] = -40;
    motor[driveR] = -40;
    while(nMotorEncoder[driveR]*drive_cm_per_tick > -23 && time1[T1] < 4000){};//bash to figure out how close we need to be
    servo[goal] = goal_close;
    resetDriveEncoders();
    clearTimer(T1);
    while(nMotorEncoder[driveR]*drive_cm_per_tick > -7 && time1[T1] < 4000){};//bash, set times high for testing purposes.
    motor[driveR] = 0;
    motor[driveL] = 0;

}
