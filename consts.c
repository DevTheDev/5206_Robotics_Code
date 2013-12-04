// Robot values
#define OPTIMAL_SPEED 75

// ROBOT SPECS
#define WHEEL_RADIUS 2
#define WHEEL_WIDTH 17 // width between the wheels on opposite sides of robot
#define D_RATIO 1 // the gear ratio on the drive motors
#define encoderticks 1440
#define scoredist 20;

// Joystick Buttons
#define btnX 1
#define btnA 2
#define btnB 3
#define btnY 4

#define btnLB 5
#define btnRB 6
#define btnLT 7
#define btnRT 8

// Tele-Op Constants
#define joystickRange 128//?
#define threshhold 10
#define constdrivereg 80.0/128.0
#define nearblockconst 36
#define farblockconst 20
#define leftintakefwd 255
#define rightintakefwd 0
#define leftintakebck 0
#define rightintakebck 255
#define intakestop 127
#define paddlespeedreg 18
#define paddlespeedslow 8
#define DCstop 0
#define paddleLightThresh 50
#define paddleratio 14.0/128.0
#define paddleIntakeTime 6000;//the time in ms the robot takes to intake a block after seeing a block
#define paddleWaitTime 1000;//the time in ms the robot waits after seeing a block to start the paddle
#define paddleFastTime 1000;//the time the paddle runs before slowing down for the sensor

//Tele-Op Bindings
#define slowButton joy1Btn(btnA)==1
#define paddleBackButton joystick.joy1_TopHat==6
#define paddleForwardButton joystick.joy1_TopHat==2
#define autoIntakeButton joy1Btn(btnY)==1
#define manualIntakeButton joy1Btn(btnRT)==1
#define manualOuttakeButton joy1Btn(btnLT)==1

//nxt buttons
#define leftarrow 2
#define rightarrow 1
#define orangebutton 3
#define graybutton 0

//autonomous
#define distToPend 6.0
#define distToBridge 24.0
