<<<<<<< HEAD
=======
#ifndef _consts
#define _consts

>>>>>>> Kyler's-Code
/*!
 * consts.c
 * store global consts used in one place
 */
<<<<<<< HEAD
#define ever (;;)

// Robot values
#define OPTIMAL_SPEED //

// Robot specs
#define WHEEL_RADIUS //?
#define WHEEL_WIDTH //? // width between the wheels on opposite sides of robot
#define D_RATIO //? // the gear ratio on the drive motors
#define encoderticks 1440 //the number of encoder ticks in a rotation
#define scoredist //?
#define timeSpeedPerDegrees //? // the time * speed / angle of the robots turns
#define maxAccel //? //Not self explanatory
#define encodersPerInch encoderticks/(2*WHEEL_RADIUS*PI)
#define kyler "upp"
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
=======
typedef unsigned char uint8;

#define ever (;;)

#define pi PI

#define launcher_slow_time 1000.0 //How long the launcher slows for after button release

//Servo Values
#define servo_stop 127 //Stopping point for continuous servos

#define goal_close 95
#define goal_open 210

#define net_close 15
#define net_open 55
#define net_center 60

// Robot specs
#define WHEEL_RADIUS 2*2.54 //The radius of the wheels, for distance calculation (converted to cm)
#define encoderticks 1440 //the number of encoder ticks in a rotation

#endif
>>>>>>> Kyler's-Code
