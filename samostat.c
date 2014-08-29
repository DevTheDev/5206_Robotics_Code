#pragma platform(FTC)         // Only works for "FTC" platform. Generate error on other platforms.
#pragma autoStartTasks        // Automatically start this task when the main user program starts.

typedef struct
{
  int nLine;
  int nSizeOfMessage;
  string message;  // 20 byte char
} TSamanathaStatusMessage;
TSamanathaStatusMessage status;

const TMailboxIDs kQueueID = mailbox2;

////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// "Macro" to get a non-volatile copy of the last joystick settings so
//
////////////////////////////////////////////////////////////////////////////////////////////////////////

bool readSamanthaStatus()
{
  int nSizeOfMessage;
  int i;

  TFileIOResult nBTCmdRdErrorStatus;
  const int kMaxSizeOfMessage = sizeof(TSamanathaStatusMessage);
  ubyte tempBuffer[kMaxSizeOfMessage];

   // Check to see if a message is available.
   nSizeOfMessage = cCmdMessageGetSize(kQueueID);

   if (nSizeOfMessage <= 0)
   {
     return false;
   }

    if (nSizeOfMessage > sizeof(tempBuffer))
      nSizeOfMessage = sizeof(tempBuffer);

    nBTCmdRdErrorStatus = cCmdMessageRead(tempBuffer, nSizeOfMessage, kQueueID);

    if (nBTCmdRdErrorStatus != ioRsltSuccess)
    {
       return true;
    }

    status.nLine = (int)tempBuffer[0];
    status.nSizeOfMessage = (int)nSizeOfMessage;

    //    StringFromChars(samanthaStatusCopy.message,tempBuffer);
    status.message ="";
    for(i=0; i<20; i++)
      status.message += tempBuffer[i+1];

    return true;
}

task main()
{
  eraseDisplay();
  nxtDisplayCenteredTextLine(2, "Acquiring...");

  while (true)
  {
    if (readSamanthaStatus())
    {
       nxtDisplayTextLine(status.nLine, "%s", status.message);
    }
  }
}
