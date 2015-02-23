#pragma config(Hubs,  S4, HTMotor,  HTMotor,  HTMotor,  HTServo)
#pragma config(Sensor, S2,     gyro,           sensorNone)
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

task lift(){
    for ever
    {
        updateLift();
    }
}

#define goal_part 110
#define net_small 31


task main()
{
    servo[net] = net_close;
    servo[goal] = goal_open;
    servo[shrub] = 127;
    wait1Msec(100);//wait for everything to stop

    bool confirmed = 0;
    int wait_time = 0;
    bool calibrated = 0;
    bool parking_zone = 1;
    clearScreen();

    while(!confirmed){
        int right_lift = 0;
        int left_lift = 0;
        char bat[16];
        sprintf(bat, "Bat: %f V", externalBattery/1000.0);
        displayMenuItem(bat);
        menu_size++;
        if(doMenuItem("lift up")){
            left_lift = 50;
            right_lift = 50;
        }
        if(doMenuItem("lift down")){
            left_lift = -50;
            right_lift = -50;
        }
        motor[liftL] = left_lift;
        motor[liftR] = right_lift;

        char wait[16];
        sprintf(wait, "inc wait: %i", wait_time);
        if(doMenuItem(wait) && time1[T2] >= 200){
            clearTimer(T2);
            wait_time +=500;
        }
        if(doMenuItem("dec wait") && time1[T2] >= 200){
            clearTimer(T2);
            wait_time -= 500;
        }
        doToggleMenuItem("No PZ\0Yes PZ", parking_zone);
        if(doMenuItem("Calibrate")){
            wait1Msec(2000);
            calibrateGyro();
            playSoundFile("Calibrated.rso");
            calibrated = 1;
        }
        if(doMenuItem("Confirm")){
            confirmed = 1;
        }
        updateMenu();
    }
    wait1Msec(2000);
    if(!calibrated){
    calibrateGyro();
    playSoundFile("Calibrated.rso");
    }

    while(externalBattery == -1){
        playSoundFile("RobotOn.rso");
        while(bSoundActive);
        wait1Msec(2000);
    }
    playSoundFile("Ready.rso");
    waitForStart();

    wait1Msec(wait_time);
    resetLiftEncoders();
    startTask(lift);

    driveDist(160, -10);
    lift_position = lift_60;
    wait1Msec(3000);//minimize wasted time here for the lift
    resetDriveEncoders();
    motor[driveR] = -40;
    motor[driveL] = -40;
    while(nMotorEncoder[driveR]*drive_cm_per_tick > -63){};
    servo[goal] = goal_part;
    resetDriveEncoders();
    while(nMotorEncoder[driveR]*drive_cm_per_tick > -12){};
    motor[driveR] = 0;
    motor[driveL] = 0;
    wait1Msec(500);
    servo[net] = net_small;
    wait1Msec(750);//Time to score the ball in the 60
    turnAngle(130, 50); //Check angle and direction, may want to go further to get both back into the zone
    driveDist(65, -50);//Pushing goal towards PZservo
    lift_position = lift_90;
    turnAngle(3, 50); // bash angle, aligning with 90
    servo[goal] = goal_open;
    driveDist(35, 50);//Drive away from goal
    turnAngle(170, -50);
    wait1Msec(1000);//minimize wasted time here for the lift
    motor[driveR] = -40;
    motor[driveL] = -40;
    resetDriveEncoders();
    clearTimer(T1);
    while(nMotorEncoder[driveR]*drive_cm_per_tick > -63 && time1[T1] < 2000){};//wait1Msec(900);//bash to figure out how close we need to be
    servo[goal] = goal_close;
    resetDriveEncoders();
    clearTimer(T1);
    while(nMotorEncoder[driveR]*drive_cm_per_tick > -5 && time1[T1] < 2000){};//bash
    motor[driveR] = 0;
    motor[driveL] = 0;
    wait1Msec(750);
    servo[net] = net_open;
    wait1Msec(750);
    driveDist(40, 50); //Check distance
    turnAngle(3, -50);
    //turnAngle(10, -50);
    if(parking_zone){
        driveDist(250, 50);
        turnAngle(35, -50);
        driveDist(40, 50);
    }
    //turnAngle(160, 50); //Check angle
}
