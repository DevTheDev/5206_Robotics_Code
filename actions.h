#include "robot.c"
#include "joystickdriver.c"
#include "3rd Party Sensor Drivers/drivers/hitechnic-eopd.h"
#include "3rd Party Sensor Drivers/drivers/hitechnic-irseeker-v2.h"


/**
 * Reset the motor encoders
 */
void reset()
{
    nMotorEncoder[RightDr] = 0;
    //nMotorEncoder[LeftDr] = 0;
}

/**
 * Pause the robot in one spot
 */
void pause()
{
    motor[LeftDr] = 0;
    motor[RightDr] = 0;
}

/**
 * Stop the robot and wait for time to end
 */
void hold()
{
    pause();

    //Wait for time to end
    while(true) {}
}

/**
 * Bring the robot to a complete stop
 */
void stopRobot() {
    motor[LeftDr] = DCstop;
    motor[RightDr] = DCstop;
    motor[PaddleMtr] = DCstop;
    motor[LiftFlagMtr] = DCstop;
    servo[LeftIntake] = intakestop;
    servo[RightIntake] = intakestop;
}

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
 * Drive a set distance
 * inches: number of inches to move
 * speed: speed to traval at (negative values will cause the robot to drive backwards)
 */
void move(float inches, int speed)
{
    int rotations = inches * 1440 / robot.wheel.circumference * robot.wheel.dRatio;

    reset();

    drive(speed);
    if(inches > 0){
        while (nMotorEncoder[RightDr] < rotations) {}
    }
    else{
        while (nMotorEncoder[RightDr] > rotations) {}
    }
    pause();
}

/**
 * Preform a point turn
 * degrees: number of degrees to turn - positive values turn right, negative values turn left
 * speed: speed to travel at
 */
void point(float degrees, int speed)
{
	int counts = (degrees / 360.0) * (robot.wheel.dRatio * robot.wheel.around / robot.wheel.circumference) * (-encoderticks);
	// counts = (proportion of circle turned) * (number of rotations per full turn) * (encoderticks per wheel rotation)
	
	reset();

	//nMotorEncoderTarget[RightDr] = counts;

	if(degrees > 0)
	{
		turn(speed);
		while(nMotorEncoder[RightDr] > counts) {}
	}
	else
	{
		turn(-speed)
		while(nMotorEncoder[RightDr] < counts) {}
	}

	pause();
}

/**
 * Preform a swing turn
 * degrees: number of degrees to turn - positive values turn right, negative values turn left
 * speed: speed to travel at
 */
void swing(int degrees, int speed)
{
	int counts = 2 * -encoderticks * robot.wheel.around * robot.wheel.dRatio * degrees / (robot.wheel.circumference*360.0);

	reset();

	//nMotorEncoderTarget[RightDr] = counts;

	if(degrees > 0)
	{
		motor[RightDr] = -speed;
		while(nMotorEncoder[RightDr] > counts){
		}
	}
	else
	{
		motor[RightDr] = speed;
		while(nMotorEncoder[RightDr] < counts){
		}
	}

	pause();
}

/**
 * Score the blocks that the robot is carring
 */
void scoreBlocks()
{
    motor[PaddleMtr] = 10;
    wait1Msec(2000);
    motor[PaddleMtr] = -10;
    wait1Msec(2000);
    motor[PaddleMtr] = DCstop;
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

/**
 * TODO: Give description of what this fuction does
 */
bool EOPDDetect(tSensors EOPD, int eopdetect) {
        return(HTEOPDreadProcessed(EOPD) >= eopdetect);
}

/////////////////////////////////////////
///             TELE-OP               ///
/////////////////////////////////////////

void singleJoyDrive();
void doublejoyDrive();
void PaddleAndIntake();
void liftandflag();
void onePaddleTurn(int speed);

typedef struct{
	float x;
	float y;
} vec2;

/**
	if the joystick is inside a circle of radius threshhold set x and y to 0
*/
vec2 clampToThreshhold(float x, float y){
	vec2 out;
	if(x*x + y*y <= threshhold*threshhold){
		out.x = 0;
		out.y = 0;
	}else{
		out.x = x;
		out.y = x;
	}
	return out;
}

///////////////////
/*****************/
/*     DRIVE     */
/*****************/
///////////////////
// Single Joy Drive //
void singleJoyDrive() {

        vec2 joyVec = clampToThreshhold(joystick.joy1_x1, joystick.joy1_y1);
        if(joyVec.x != 0 || joyVec,y != 0) {
        	float turnRatio = (joyVec.x+/*range of joystick*/)/(/*range of joystick*/-joyVec.x);//TODO: division by zero safety
          motor[LeftDr] = joyVec.y*turnRatio*constdrivereg;
          motor[RightDr] = joyVec.y*(1/turnRatio)*constdrivereg;
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

// PADDLE + INTAKE //
void PaddleAndIntake() {
        if(joystick.joy1_TopHat==2) {
                (joy1Btn(btnA)==1) ? onePaddleTurn(paddlespeedslow) : onePaddleTurn(paddlespeedreg);
        }
        else if(joystick.joy1_TopHat==6) {
                (joy1Btn(btnA)==1) ? onePaddleTurn(-paddlespeedslow) : onePaddleTurn(-paddlespeedreg);
        }

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

void initializeRobotnext() {
        HTEOPDsetShortRange(PaddleEOPD);
}
