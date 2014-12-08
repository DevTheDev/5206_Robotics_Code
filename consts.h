/*!
 * consts.c
 * store global consts used in one place
 */
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
