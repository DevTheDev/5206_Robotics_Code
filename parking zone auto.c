#pragma config(Sensor, S2,     gyro,           sensorNone)
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
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "actions.h"
#include "consts.h"
#include "menu.h"

#include "joystickdriver.c"
#include "3rd Party Sensor Drivers\drivers\hitechnic-irseeker-v2.h"

//TODO: debug stream

task lift() //TODO: move this to actions?
{
    for ever
    {
        updateLift();
    }
}


bool seeIR(tHTIRS2 * irseeker)
{
    readSensor(irseeker);
    return irseeker->acDirection == 6;
}

task main()
{
    clearScreen();//Turns off built in diagnostics and clears the display

    //IR Initialization
    tHTIRS2 irseeker;
    initSensor(&irseeker, S3);
    irseeker.mode = DSP_1200;

    //Servo Initialization
    servo[net] = net_close;
    servo[goal] = goal_open;
    servo[shrub] = servo_stop;
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

    //Begin raising lift
    startTask(lift);
    lift_position = lift_120;

    driveDist(40, 80);//weird stuff happens if you wait after this
    turnAngle(80, 50);
    uint8 n_turns = 0;

    {
        int motor_vIs = 20;
        int distance = 56;//Length of a section of the octagon

        resetDriveEncoders();

        for ever
        {
            if(seeIR(&irseeker))
            {
                break;
            }

            motor[driveL] = motor_vIs;
            motor[driveR] = motor_vIs;
            if(nMotorEncoder[driveR]*drive_cm_per_tick > distance)
            {
                if(n_turns == 1)
                {
                    driveDist(26, motor_vIs);
                }
                else
                {
                    driveDist(19, motor_vIs);
                }

                if(n_turns == 3){
                turnAngle(35, -50);
                }
                else{
                turnAngle(38, -50);
                }

                resetDriveEncoders();
                if(++n_turns >= 3)
                {
                    break;
                }
            }
        }
    }

    if(n_turns == 2)
    {
        driveDist(9, -20);
    }
    else if(n_turns == 1)
    {
        driveDist(7, -20);
    }
    else
    {
    }
        turnAngle(75, 50);

    wait1Msec(5000);
    if(n_turns == 1){
        driveDist(10, -30);
    }
    else{
        driveDist(15, -30);
    }
    wait1Msec(500);
    servo[net] = net_open;
    wait1Msec(6000);
    driveDist(20, 30);
    if(n_turns == 0)
    {
        turnAngle(40, -50);
    }
    else
    {
        turnAngle(80, -50);
    }
    lift_position = lift_90;
    wait1Msec(7000);
    return;

    switch(n_turns)
    {
        case 0:
        {
            driveDist(10, -80);
            turnAngle(80, 50);
            driveDist(90, -80);
            turnAngle(80, 50);
            driveDist(150, -80);
            turnAngle(40, -50);
        }
        break;
    }

    driveDist(85, -80);
    servo[goal] = goal_close;

    driveDist(85, 80);
    turnAngle(40, 50);
    driveDist(240, 80);
    turnAngle(80, 50);
    driveDist(90, -80);
}
