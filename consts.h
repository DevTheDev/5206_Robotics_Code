#ifndef _consts
#define _consts

/*!
 * consts.c
 * store global consts used in one place
 */
typedef unsigned char uint8;

#define ever (;;)

#define pi PI

// Robot values
#define OPTIMAL_SPEED //don't need this

// Robot specs
#define WHEEL_RADIUS 2*2.54
#define WHEEL_WIDTH //? // width between the wheels on opposite sides of robot
#define D_RATIO //? // the gear ratio on the drive motors
#define encoderticks 1440 //the number of encoder ticks in a rotation
#define scoredist //?
#define timeSpeedPerDegrees //? // the time * speed / angle of the robots turns
#define maxAccel //? //Not self explanatory
#define encodersPerInch encoderticks/(2*WHEEL_RADIUS*PI)
#define kyler "upp"

#define goal_close 95
#define goal_open 210

#define net_open 20
#define net_close 232
#define servo_stop 127

//Autonomous Constants - Need bashing
#define distOffRamp 50
#define degreesToCenter 60
#define distToCenter 30

// Tele-op constants
#define joystickRange 128.0 // ?
#define threshhold 6
#define constdrivereg 100.0//80.0/128.0
#define DCstop 0 //Simpler stop

// Tele-op bindings

// NXT buttons
//#define leftarrow 2
//#define rightarrow 1
//#define orangebutton 3
//#define graybutton 0

#endif
