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
#pragma config(Servo,  srvo_S4_C4_2,    gate,                 tServoStandard)
#pragma config(Servo,  srvo_S4_C4_3,    shrub,                tServoContinuousRotation)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "menu.h"
#include "consts.h"
#include "actions.h"
#include "joystickdriver.c"

//TODO: debug stream

task lift() //TODO: move this to actions?
{
    for ever
    {
        updateLift();
    }
}

#define gate_closed 118
#define gate_open 10

const float lift_positions[] = {lift_bottom, lift_30, lift_60, lift_90, lift_120};

typedef enum _lift_pos
{
    lift_pos_bottom,
    lift_pos_30,
    lift_pos_60,
    lift_pos_90,
    lift_pos_120,
    lift_pos_count,
} lift_pos;

typedef enum _start_pos
{
    start_pos_ramp_backward,
    start_pos_count,
} start_pos;

typedef enum _center_rot
{
    center_rot_not_found,
    center_rot_NS,
    center_rot_NESW,
    center_rot_EW,
    center_rot_count,
} center_rot;

bool seeIR()
{
    return /*TODO: IR sensor reading*/0;
}

center_rot driveDistAndScanForIR(float distance, int motor_vIs)
{
    center_rot center_rotation = center_rot_not_found;

    motor[driveL] = motor_vIs;
    motor[driveR] = motor_vIs;
    while(1)
    {
        const float IR_spacing_in_cm = 10.0;
        const float distance_to_EW_IR = 50.0;

        if(seeIR() && center_rotation == center_rot_not_found)
        {
            float spacings_past_EW = nMotorEncoder[driveL]*drive_cm_per_tick - distance_to_EW_IR;
            spacings_past_EW /= IR_spacing_in_cm;

            center_rotation = round(spacings_past_EW);
            //TODO(maybe): set to center_rot_not_found if the center_rotation is outside the range
            center_rotation = clamp(center_rotation, 1, center_rot_count);
        }
        center_rotation = center_rot_EW; //DEBUG

        if(absv(absv(nMotorEncoder[driveL]*drive_cm_per_tick)-distance) < 0.0){
            return center_rotation;
        }
    }
}

task main()
{
    clearScreen();

    tHTIRS2 irseeker;
    initSensor(&irseeker, S3);

    bool go_for_120 = 0;
    start_pos start_location = 0;
    lift_pos lift_goto = 2;//0 => bottom, 1 => 30 cm, ...

    servo[gate] = gate_closed; //init the servos

    //the config menu
    int wait_timer = 0;
    char* bleh;
    do
    {
        updateMenu();

        //doCycleMenuItem("ramp start", start_pos_count, (int *) &start_location);

        //doCycleMenuItem("bottom\0\30 cm\060 cm\090 cm\0120cm", lift_pos_count, (int *) &lift_goto);

        doToggleMenuItem("don't do 120\0do 120", go_for_120);

        sprintf(bleh, "increase wait %i", wait_timer);

        if(doMenuItem(bleh) && time1[T2] >= 500){
            clearTimer(T2);
            wait_timer += 500;
        }

        if(doMenuItem("decrease wait (msec)") && time1[T2] >=500){
            clearTimer(T2);
            wait_timer -= 500;
        }
    }
    while(!doMenuItem("confirm"));

    nxtDisplayCenteredBigTextLine(4, "Ready?!");

    wait1Msec(1000);//waitForStart();

    center_rot center_rotation = center_rot_not_found;

    //raise the lift
    startTask(lift);
    lift_position = lift_positions[lift_goto];

    wait1Msec(clamp(wait_timer, 0, 15000));

    switch(start_location)
    {
        //drive and grab the goal
        case start_pos_ramp_backward:
            switch(lift_goto)
            {
                default: case lift_pos_count: case lift_pos_120:
                    goto skip_first_launch;
                    break;

	            case lift_pos_30:
	            {
	                servo[goal] = 180; //open the goal lock
	                driveDistAndScanForIR(200, -40); //get off of ramp
	                turnAngle(pi/2, 80);
	                driveDist(30, -80);
	                turnAngle(-pi/2, 80);
	                driveDist(30, -80);
	                servo[goal] = 0;
	            }
	            break;

	            case lift_pos_60:
	            {
	                servo[goal] = 180; //open the goal lock
	                driveDistAndScanForIR(230, -40); //get off of ramp
	                servo[goal] = 0;
	            }
	            break;

	            case lift_pos_90:
	            {
	                servo[goal] = 180; //open the goal lock
	                driveDistAndScanForIR(230, -40); //get off of ramp
	                turnAngle(pi/2, 80);
	                driveDist(30, 80);
	                turnAngle(pi/4, 80);
	                driveDist(30*sqrt(2), -80);
	                servo[goal] = 0;
	            }
	            break;
	        }
    }

    //launch the preloaded balls //TODO: make launch balls a function
    //motor[launcher] = 100;
    wait1Msec(100);
    motor[intake] = 100;
    if(!go_for_120)
    {
        servo[gate] = gate_open;
        wait1Msec(2500);
    }
    motor[intake] = 0;
    //motor[launcher] = 0;

skip_first_launch:;

    if(go_for_120)
    {
        lift_position = lift_120; //TODO: might need to wait for the lift to extend

        //drive to center goal and score
        switch(lift_goto)
        {
            default: case lift_pos_count: case lift_pos_120:
            {
                //drive from the ramp
                switch(start_location)
                {
                    case start_pos_ramp_backward:
                    {
                        center_rotation = driveDistAndScanForIR(200, -80); //drive to a constant position for consistancy
                        //drive to center goal
                        switch(center_rotation) //level three nested switch //TODO: we need to go deeper
                        {
                            case center_rot_NS:
                            {
                                turnAngle(-pi/2, 80);
                                driveDist(150, 80);
                                turnAngle(pi/2, 80);
                                driveDist(30, 40);
                                turnAngle(pi/2, 80);
                                driveDist(10, 40);
                            }
                            break;

                            case center_rot_NESW:
                            {
                                turnAngle(pi/2, 80);
                                driveDist(200, -80);
                                turnAngle(-pi/4, 80);
                                driveDist(40, 40);
                            }
                            break;

                            case center_rot_EW:
                            {
                                turnAngle(-pi/2, 80);
                                driveDist(125, 80);
                                turnAngle(pi/2, 80);
                                driveDist(20, 40);
                            }
                            break;
                        }

                        //score both balls in the goal
                        //motor[launcher] = 100;
                        wait1Msec(100);
                        motor[intake] = 100;
                        servo[gate] = gate_open;
                        wait1Msec(2500);
                        motor[intake] = 0;
                        //motor[launcher] = 0;
                    }
                    break;
                }

                //motor[launcher] = 100;
                wait1Msec(100);
                motor[intake] = 100;
                servo[gate] = gate_open;
                wait1Msec(2500);
                motor[intake] = 0;
                //motor[launcher] = 0;

                goto skip_second_launch;
            }
            break;

            case lift_pos_30:
            {
                switch(center_rotation)
                {
                    case center_rot_NS:
                    {
                        turnAngle(pi/2, 80);
                        driveDist(150, 80);
                        turnAngle(pi/2, 80);
                        driveDist(40, 80);
                        turnAngle(pi/2, 80);
                        driveDist(60, 80);
                    }
                    break;

                    case center_rot_NESW:
                    {
                        turnAngle(-pi/2, 80);
                        driveDist(150, -80);
                        turnAngle(-pi/4, 80);
                        driveDist(100, 80);
                    }
                    break;

                    case center_rot_EW:
                    {
                        turnAngle(pi/2, 80);
                        driveDist(81, 80);
                        turnAngle(pi/2, 80);
                        driveDist(60, 80);
                    }
                    break;
                }
            }
            break;

            case lift_pos_60:
            {
                switch(center_rotation)
                {
                    case center_rot_NS:
                    {
                        turnAngle(pi/2, 80);
                        driveDist(200, 80);
                        turnAngle(pi/2, 80);
                        driveDist(40, 80);
                        turnAngle(pi/2, 80);
                        driveDist(60, 80);
                    }
                    break;

                    case center_rot_NESW:
                    {
                        turnAngle(-pi/2, 80);
                        driveDist(125, -80);
                        turnAngle(-pi/4, 80);
                        driveDist(60, 80);
                    }
                    break;

                    case center_rot_EW:
                    {
                        turnAngle(pi/2, 80);
                        driveDist(125, 80);
                        turnAngle(pi/2, 80);
                        driveDist(60, 80);
                    }
                    break;
                }
            }
            break;

            case lift_pos_90:
            {
                switch(center_rotation)
                {
                    case center_rot_NS:
                    {
                        driveDist(40, 80);
                        turnAngle(pi/4, 80);
                        driveDist(170, 80);
                        turnAngle(pi/2, 80);
                        driveDist(40, 80);
                        turnAngle(pi/2, 80);
                        driveDist(60, 80);
                    }
                    break;

                    case center_rot_NESW:
                    {
                        driveDist(40, 80);
                        turnAngle(3.0*pi/4, 80);
                        driveDist(170, 80);
                        turnAngle(-pi/4, 80);
                        driveDist(50, 80);
                    }
                    break;

                    case center_rot_EW:
                    {
                        driveDist(40, 80);
                        turnAngle(pi/4, 80);
                        driveDist(120, 80);
                        turnAngle(pi/2, 80);
                        driveDist(40, 80);
                    }
                    break;
                }

            }
            break;
        }

        //motor[launcher] = 100;
        wait1Msec(100);
        motor[intake] = 100;
        motor[intake] = 0;
        //motor[launcher] = 0;
    skip_second_launch:;
    }

    driveDist(10, 80);
    turnAngle(-pi/2, 80);
    driveDist(30, 80);
    driveDist(40, 100);

    switch(center_rotation)
    {
        case center_rot_NS:
        {
            turnAngle(-pi/2, 80);
            driveDist(90, 80);
        }
        break;

        case center_rot_NESW:
        {
            turnAngle(-pi/4, 80);
            driveDist(90, 80);
        }
        break;

        case center_rot_EW:
        {
            driveDist(90, 80);
        }
        break;
    }
}
