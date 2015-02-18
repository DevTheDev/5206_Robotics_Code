#pragma config(Hubs,  S4, HTMotor,  HTMotor,  HTMotor,  HTServo)
#pragma config(Sensor, S3,     ,               sensorHiTechnicIRSeeker1200)
#pragma config(Sensor, S4,     ,               sensorI2CMuxController)
#pragma config(Motor,  mtr_S4_C1_1,     intake,        tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S4_C1_2,     launcher,      tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S4_C2_1,     driveL,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S4_C2_2,     liftL,         tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S4_C3_1,     driveR,        tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S4_C3_2,     liftR,         tmotorTetrix, openLoop, encoder)
#pragma config(Servo,  srvo_S4_C4_1,    goal,                 tServoStandard)
#pragma config(Servo,  srvo_S4_C4_3,    gate,                 tServoStandard)
#pragma config(Servo,  srvo_S4_C4_2,    shrub,                tServoContinuousRotation)

#include "actions.h"
#include "consts.h"

task main()
{
    turnAngle(pi/4, 80);
    wait1Msec(500);
    turnAngle(pi/4, 80);
    wait1Msec(500);
    turnAngle(pi/4, 80);
    wait1Msec(500);
    turnAngle(pi/4, 80);
    wait1Msec(500);
    return;
driveDist(144*2.54/6, 80);
driveDist(-144*2.54/6, -80);
}
