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

#include "joystickdriver.c"

task main()
{
    int distance = 150;
    int angle = 40;
    int vIs = 100;
    bool lift = 1;
    bool confirmed = 0;
    bool calibrated = 0;

    servo[net] = net_close;
    servo[goal] = goal_open;
    servo[shrub] = 127;
    clearScreen();

    while(confirmed == 0){
        char bat[16];
        sprintf(bat, "Bat: %f V", externalBattery/1000.0);
        displayMenuItem(bat);
        menu_size++;
        char dist[16];
        sprintf(dist, "Dist Inc: %i cm", distance);
        if(doMenuItem(dist) && time1[T2] >= 200){
            clearTimer(T2);
            distance += 5;
        }
        if(doMenuItem("Dist Dec") && time1[T2] >= 200){
            clearTimer(T2);
            distance -= 5;
        }
        sprintf(dist, "Angle Inc: %i cm", angle);
        if(doMenuItem(dist) && time1[T2] >= 200){
            clearTimer(T2);
            angle += 5;
        }
        if(doMenuItem("Angle Dec") && time1[T2] >= 200){
            clearTimer(T2);
            angle -= 5;
        }
        char power[16];
        sprintf(power, "Power Inc: %i ", vIs);
        if(doMenuItem(power) && time1[T2] >= 200){
            clearTimer(T2);
            vIs += 5;
        }
        if(doMenuItem("Power Dec") && time1[T2] >= 200){
            clearTimer(T2);
            vIs -= 5;
        }

        if(doMenuItem((calibrated) ? "Calibrate*" : "Calibrate")){
            clearScreen();
            displayCenteredTextLine(3, "Waiting for");
            displayCenteredTextLine(4, "you to move");
            displayCenteredTextLine(5, "...");

            wait1Msec(2000);

            calibrateGyro();
            playSoundFile("Calibrated.rso");
            calibrated = 1;
        }

        if(doMenuItem("Confirm")){
            confirmed = 1;
        }
        updateMenu(soundBlip);
    }

    if(!calibrated){
        displayCenteredTextLine(2, "Calibrating");
        displayCenteredTextLine(3, "Waiting for");
        displayCenteredTextLine(4, "you to move");
        displayCenteredTextLine(5, "...");
        wait1Msec(2000);
        clearScreen();

        calibrateGyro();
        playSoundFile("Calibrated.rso");
    }

    while(externalBattery == -1){
        playSoundFile("RobotOn.rso");
        while(bSoundActive);
        wait1Msec(2000);
    }

    displayCenteredBigTextLine(3, "Ready!");
    playSoundFile("Ready.rso");

    waitForStart();

    servo[shrub] = 227;
    turnAngle(angle, vIs);
    driveDist(distance, vIs);
}
