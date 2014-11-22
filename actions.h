/*
Function library for autonomous operations
Tele-op should be done in-line unless it needs to do something that might be needed in tele-op
*/

float lift_position;//the desired lift position in cm//0 is the position at the start of teleop

const float cm_per_tick = 180/ticks/PI;//the number of cm the lift raises when the lift motors rotate 1 encoder tick
const float lift_speed_constant = 10.0/90.0;
const float lift_slow_constant = 1.0/2.0;
void updateLift(){
    motor[liftL] = lift_speed_constant*atan(*(lift_position-nMotorEncoder[leftL]*cm_per_tick));//this atan is totally arbitrary and was only chosen because gives a good curve(the motor will be at an approx. const. speed far from the wanted point and will slow down near the wanted point)
    motor[liftR] = lift_speed_constant*atan(*(lift_position-nMotorEncoder[leftR]*cm_per_tick));
}
