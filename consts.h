/*!
 * consts.c
 * store all the consts used in one place
 */

// Robot values
#define OPTIMAL_SPEED 75

// Robot specs
#define WHEEL_RADIUS 2
#define WHEEL_WIDTH 17.75 // width between the wheels on opposite sides of robot
#define D_RATIO 1 // the gear ratio on the drive motors
#define encoderticks 1440 //the number of encoder ticks in a rotation
#define scoredist 20
#define timeSpeedPerDegrees 10 // the time * speed / angle of the robots turns
#define maxAccel 1
#define encodersPerInch encoderticks/(2*WHEEL_RADIUS*PI)

// Joystick buttons
#define btnX 1
#define btnA 2
#define btnB 3
#define btnY 4

#define btnLB 5
#define btnRB 6
#define btnLT 7
#define btnRT 8

// Tele-op constants
#define joystickRange 128.0 // ?
#define threshhold 6
#define constdrivereg 100.0//80.0/128.0
#define nearblockconst 36
#define farblockconst 20
#define leftintakefwd 255
#define rightintakefwd 0
#define leftintakebck 0
#define rightintakebck 255
#define intakestop 0
#define intakeFast 100
#define intakeSpeedRatio (1.00/128)*50
#define paddlespeedreg 21
#define paddlespeedslow 4
#define DCstop 0 // full DC motor stop
#define paddleEOPDThresh 30
#define blockEOPDThresh 50
#define paddleratio 14.0/128.0
#define paddleIntakeTime 6000 // the time in ms the robot takes to intake a block after seeing a block
#define paddleWaitTime 1000 // the time in ms the robot waits after seeing a block to start the paddle
#define paddleFastTime 1000 // the time the paddle runs before slowing down for the sensor
#define maxLiftTime 10000 //The time it takes the lift to go all the way up or down
#define turnSensitivity 1 //the sensitivity of turning
#define flagSpeed 100
#define nomWait 20
#define omWait 500

// Tele-op bindings
#if normal_teleop//us driving the robot

#define driveX joystick.joy1_x1
#define driveY joystick.joy1_y1
#define turboAnalog joystick.joy2_y1
#define liftControl joystick.joy2_y2
#define turboButton1 joy1Btn(btnX)==1
#define turboButton2 joy1Btn(btnA)==1
#define turboButton3 joy1Btn(btnB)==1
#define flagButton joy2Btn(btnA)==1
#define flagBackButton joy2Btn(btnX)==1
#define paddleAutoForwardButton joy1Btn(btnRB) ==1
#define paddleAutoBackButton joy1Btn(btnLB) ==1
#define autoIntakeButton joy1Btn(btnY)==1
#define manualIntakeButton joy1Btn(btnRT)==1
#define manualOuttakeButton joy1Btn(btnLT)==1
#define manualDeployIntake joystick.joy2_TopHat==0
#define manualRetractIntake joystick.joy2_TopHat==4
#define manualPaddleChange 1                               //this needs to be checked

#define intakeOffset 10                                     //this constant need to be bashed

#else//people driving teleop

#define driveX joystick.joy1_x1
#define driveY joystick.joy1_y1
#define turboAnalog joystick.joy2_y1
#define liftControl joystick.joy2_y2
#define turboButton1 joy2Btn(btnX)==1
#define turboButton2 joy2Btn(btnA)==1
#define turboButton3 joy2Btn(btnB)==1
#define flagButton joystick.joy2_TopHat == 6
#define flagBackButton joystick.joy2_TopHat == 2
#define paddleAutoForwardButton joy2Btn(btnRB) ==1
#define paddleAutoBackButton joy2Btn(btnLB) ==1
#define autoIntakeButton 0 //- no longer needed
#define manualIntakeButton joy2Btn(btnRT)==1
#define manualOuttakeButton joy2Btn(btnLT)==1
#define manualDeployIntake joystick.joy2_TopHat==0
#define manualRetractIntake joystick.joy2_TopHat==4
#define manualPaddleChange 1
#endif

// NXT buttons
#define leftarrow 2
#define rightarrow 1
#define orangebutton 3
#define graybutton 0

// Autonomous
#define distToPend 6.0
#define maxBridgeDistance encoderticks*120.0
#define irZone 6
#define distanceBetweenPend 10
#define pendulumLength 48
#define lightBlueLower 19
#define lightBlueUpper 28
#define lightRedLower 40
#define lightRedUpper 50
#define bridgeWidth 48
#define nBS 6
#define distBetweenBS bridgeWidth/nBS
#define lightFileName "Aaaaarrrrrrggghhh.txt"
