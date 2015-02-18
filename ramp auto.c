#pragma config(Hubs,  S4, HTMotor,  HTMotor,  HTMotor,  HTServo)
#pragma config(Sensor, S2,     gyro,           sensorNone)
#pragma config(Sensor, S4,     ,               sensorI2CMuxController)
#pragma config(Motor,  mtr_S4_C1_1,     intake,        tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S4_C1_2,     driveL,        tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S4_C2_1,     launcher,      tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S4_C2_2,     liftL,         tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S4_C3_1,     driveR,        tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S4_C3_2,     liftR,         tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S4_C4_1,    goal,                 tServoStandard)
#pragma config(Servo,  srvo_S4_C4_2,    shrub,                tServoContinuousRotation)
#pragma config(Servo,  srvo_S4_C4_3,    net,                  tServoStandard)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "actions.h"
#include "consts.h"

#include "joystickdriver.c"

task lift()
{
    for ever
    {
        updateLift();
    }
}


task main()
{
    //Servo Intialization
    servo[net] = net_close;
    servo[goal] = goal_open;
    servo[shrub] = 127;
    wait1Msec(100);//wait for everything to stop

    //Gyro Calibration
    calibrateGyro();
    eraseDisplay();
    displayCenteredTextLine(3, "Ready");
    playSoundFile("Calibrated.rso");

    while(externalBattery == -1){//Tells us if the robot is off
        playSoundFile("RobotOn.rso");
        while(bSoundActive);
        wait1Msec(2000);
    }
    playSoundFile("Ready.rso");
    waitForStart();

    resetLiftEncoders();
    startTask(lift);

    //Get off the ramp, and raise the lift to the correct height
    driveDist(150, -15);
    servo[shrub] = 227;
    lift_position = lift_60;
    wait1Msec(4000);
    driveDist(60, -80);
    servo[shrub] = 127;

    //Grab the goal (Done without distance for easier multitasking)
    motor[driveL] = -80;
    motor[driveR] = -80;
    wait1Msec(500);
    servo[goal] = goal_close;
    servo[shrub] = 227;
    wait1Msec(500);
    motor[driveL] = 0;
    motor[driveR] = 0;

    //Score in the goal
    wait1Msec(2000);
    servo[net] = net_open;
    wait1Msec(300);

    //goto parking zone, currently disabled
#if 0
    turnAngle(pi/4, -80);
    driveDist(75, 80);
    turnAngle(pi/4, 80);
    driveDist(190, 80);
#endif
}
