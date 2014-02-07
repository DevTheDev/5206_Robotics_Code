#pragma config(Hubs,  S1, HTMotor,  HTServo,  HTMotor,  none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     PaddleEOPD,     sensorAnalogActive)
#pragma config(Sensor, S3,     AutoIR,         sensorI2CCustom)
#pragma config(Sensor, S4,     PaddleLIGHT,    sensorLightActive)
#pragma config(Motor,  mtr_S1_C1_1,     LiftFlagMtr,   tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C1_2,     PaddleMtr,     tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C3_1,     RightDr,       tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C3_2,     LeftDr,        tmotorTetrix, openLoop, reversed)
#pragma config(Servo,  srvo_S1_C2_1,    LeftIntake,           tServoContinuousRotation)
#pragma config(Servo,  srvo_S1_C2_2,    RightIntake,          tServoContinuousRotation)
#pragma config(Servo,  srvo_S1_C2_3,    servo3,               tServoNone)
#pragma config(Servo,  srvo_S1_C2_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S1_C2_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C2_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "JoystickDriver.c"
#include "robot.c"
//#include "consts.c"
#include "3rd Party Sensor Drivers/drivers/hitechnic-eopd.h"
#include "3rd Party Sensor Drivers/drivers/hitechnic-irseeker-v2.h"

/**
* Drive until told elsewhere to stop
* speed: the speed to drive at (negative speed will cause the robot to go backwards)
*/
void drive(int speed) {
        motor[LeftDr] = speed;
        motor[RightDr] = speed;
}

void turn(int speed) {
        motor[LeftDr] = speed;
        motor[RightDr] = -speed;
}

        /**
        * TODO: Give description of what this fuction does
        */
        bool EOPDDetect(tSensors EOPD, int eopdetect) {
                return(HTEOPDreadProcessed(EOPD) >= eopdetect);
        }


/**
* TODO: Give description of what this fuction does
*/
bool joy1sector(int type) {
        switch(type) {
                case 1:
                        if (joystick.joy1_y1 > abs(joystick.joy1_x1) || joystick.joy1_y1 <- abs(joystick.joy1_x1)) {
                                return true;
                        }
                        else
                        {
                                return false;
                        }
                        break;
                case 2:
                        if (joystick.joy1_x1 > abs(joystick.joy1_y1) || joystick.joy1_x1<-abs(joystick.joy1_y1))
                        {
                                return true;
                        }
                        else
                        {
                                return false;
                        }
                        break;
                default:
                        return false;
        }
}


/////////////////////////////////////////
///             TELE-OP               ///
/////////////////////////////////////////

void singleJoyDrive();
void doublejoyDrive();
void PaddleAndIntake();
void liftandflag();
void onePaddleTurn(int speed);


///////////////////
/*****************/
/*     DRIVE     */
/*****************/
///////////////////
// Single Joy Drive //
void singleJoyDrive() {
        // FORWARD / BACKWARD
        if(abs(joystick.joy1_y1) > threshhold && joy1sector(1)) {
                          motor[LeftDr] = joystick.joy1_y1*constdrivereg;
                    motor[RightDr] = joystick.joy1_y1*constdrivereg;
    }

    // TURN
    else if(abs(joystick.joy1_x1) > threshhold && joy1sector(2)) {
                          motor[LeftDr] = joystick.joy1_x1*constdrivereg;
                          motor[RightDr] = -joystick.joy1_x1*constdrivereg;
    }

    else if(joy1Btn(9)==1) {
            drive(80);
            wait1Msec(60);
            drive(-80);
            wait1Msec(60);
            turn(80);
            wait1Msec(60);
            turn(-80);
            wait1Msec(60);
            drive(80);
            wait1Msec(60);
            drive(0);
    }

    // NO MOVEMENT
    else {
      motor[LeftDr] = DCstop;
      motor[RightDr] = DCstop;
    }
}

void doublejoyDrive() {
        (abs(joystick.joy1_y1) > threshhold) ? motor[LeftDr]=joystick.joy1_y1*constdrivereg : motor[LeftDr]=DCstop;
        (abs(joystick.joy1_y2) > threshhold) ? motor[RightDr]=joystick.joy1_y2*constdrivereg : motor[RightDr]=DCstop;
}


#define paddleLightThresh 50

void onePaddleTurn(int speed) {
        motor[PaddleMtr]=speed;
        wait1Msec(250);
        ClearTimer(T2);
        while(SensorValue[PaddleLIGHT]<paddleLightThresh && time1[T2]<2000){
                singleJoyDrive();
          liftandflag();
        }
        ClearTimer(T2);
        while(SensorValue[PaddleLIGHT]>paddleLightThresh && time1[T2]<2000){
                singleJoyDrive();
          liftandflag();
        }
        motor[PaddleMtr]=DCstop;
        wait1Msec(100);
        motor[PaddleMtr]=-paddlespeedreg;
        wait1Msec(50);
        motor[PaddleMtr]=DCstop;
}

// PADDLE + INTAKE //
void PaddleAndIntake() {
        /*if(joystick.joy1_TopHat==2) {
                (joy1Btn(btnA)==1) ? onePaddleTurn(paddlespeedslow) : onePaddleTurn(paddlespeedreg);
        }
        else if(joystick.joy1_TopHat==6) {
                (joy1Btn(btnA)==1) ? onePaddleTurn(-paddlespeedslow) : onePaddleTurn(-paddlespeedreg);
        }*/

        // INTAKE ALGORITHM
        if(joy1Btn(btnY)==1) {
                servo[LeftIntake]=leftintakefwd;
                servo[RightIntake]=rightintakefwd;

                if(EOPDDetect(PaddleEOPD,farblockconst)==1) {
                        motor[LiftFlagMtr]=100;
                        wait1Msec(100);
                        motor[LiftFlagMtr]=0;
                        wait1Msec(100);
                        motor[LiftFlagMtr]=-100;
                        wait1Msec(120);
                        motor[LiftFlagMtr]=DCstop;
                }
        }

        else {
                // INTAKE
                if(joy1Btn(btnRT)==1) {
                        servo[LeftIntake]=leftintakefwd;
                        servo[RightIntake]=rightintakefwd;
                }

                else if(joy1Btn(btnLT)==1) {
                        servo[LeftIntake]=leftintakebck;
                        servo[RightIntake]=rightintakebck;
                }

                else {
                        servo[LeftIntake]=intakestop;
                        servo[RightIntake]=intakestop;
                }

                // PADDLE
                if(abs(joystick.joy1_y2)>threshhold) {
                        motor[PaddleMtr]=joystick.joy1_y2 * paddleratio;
                }
                else if(joy1Btn(btnRB)==1) {
                        motor[PaddleMtr]=paddlespeedreg;
                }
                else if(joy1Btn(btnLB)==1) {
                        motor[PaddleMtr]=-paddlespeedreg;
                }
                else {
                        motor[PaddleMtr]=DCstop;
                }
        }
}

// LIFT AND FLAG MECHANISM //
void liftandflag()
{
        if(abs(joystick.joy2_y2)>threshhold) {
                motor[LiftFlagMtr]=joystick.joy2_y2*constdrivereg;
        }
        else if(joy2Btn(btnA)==1) {
                motor[LiftFlagMtr]=100;
                wait1Msec(500);
                motor[LiftFlagMtr]=DCstop;
        }
        else {
                motor[LiftFlagMtr]=DCstop;
        }
}

task main()
{
	initializeRobot();
	//        initializeRobotnext();

	waitForStart(); // wait for start of tele-op phase

	while(true) {
	  getJoystickSettings(joystick);
	  singleJoyDrive();
	  //PaddleAndIntake();
	  liftandflag();
	}
}