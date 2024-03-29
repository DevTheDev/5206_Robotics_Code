#ifndef _consts
#define _consts

/*!
 * consts.c
 * store global consts used in one place
 */
typedef unsigned char uint8;

#define pi PI

#define launcher_slow_time 1000.0 //How long the launcher slows for after button release

//Servo Values
#define servo_stop 127 //Stopping point for continuous servos

#define goal_close 60
#define goal_open 210

#define side_close 146
#define side_open 80

#define net_close 11
#define net_open 47
#define net_endgame 25
#define net_small 27

// Robot specs
#define WHEEL_RADIUS 2*2.54 //The radius of the wheels, for distance calculation (converted to cm)
#define encoderticks 1440 //the number of encoder ticks in a rotation

#endif
