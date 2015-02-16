/*
Sound Tester/Loader

RobotC requires the *.rso file to exist on the brick to be accessed. However,
RobotC won't use a file system to find the sound files. (i.e. no "Sound Files/actual file.rso")

*/

task main()
{
playSoundFile("");
while(bSoundActive);
}
