/*
Sound Tester/Loader

RobotC requires the *.rso file to exist on the brick to be accessed.
However, RobotC won't use a file system to find the sound files(i.e. no "Sound Files/actual file.rso".)
To load a sound file on the brick:
1. Move sound file into same folder as the program
2. Download this program onto the brick
3. Move the sound file into storage, to keep the main directory clean
*/

task main()
{
playSoundFile(".rso"); //Insert sound file here
while(bSoundActive);
}
