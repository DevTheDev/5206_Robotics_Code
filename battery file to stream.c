#pragma DebuggerWindows("debugStream")
#pragma DebuggerWindows("nxtLCDScreen")

task main()
{
    TFileHandle file;
    TFileIOResult error;
    short file_size;
    OpenRead(file, error, "battery", file_size);
    if(error != 0)
    {
        playSound(soundException);
    }
    short battery = -1;
    while(battery != 0)
    {
        ReadShort(file, error, battery);
        if(error != 0)
        {
            playSound(soundBeepBeep);
        }
        writeDebugStreamLine("%i", battery);
    }
}
