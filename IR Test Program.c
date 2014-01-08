#pragma config(Hubs,  S1, HTMotor,  HTServo,  HTMotor,  none)
#pragma config(Sensor, S2,     PaddleEOPD,     sensorAnalogActive)
#pragma config(Sensor, S3,     AutoIR,         sensorHiTechnicIRSeeker1200)
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
#include "actions.h"
task main()
{

clearScreen();
while (true){
int IRValue = SensorValue[AutoIR];
nxtDisplayTextLine(3, "%d", IRValue);
}
}
