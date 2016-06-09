#ifndef __NAMEDPIPE_H__
#include "NamedPipe.h"
#endif

#include "Security.h"

#include <iostream.h>
NamedPipe::NamedPipe()
{
  Security s;
  SECURITY_ATTRIBUTES sa = s.SecurityAttributes();

  mNamedPipeHandle = CreateNamedPipe("\\\\.\\pipe\\TrapRcvr",
				     PIPE_ACCESS_OUTBOUND,
				     PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_NOWAIT,
				     10,
				     32767,
				     32767,
				     0,
				     &sa);
  if (mNamedPipeHandle == INVALID_HANDLE_VALUE)
    cout << "could not create <" << GetLastError() << ">" << endl;
  else
    cout << "create was successful" << endl;
}

NamedPipe::NamedPipe(BOOL client)
{
#if 0
  mNamedPipeHandle = CreateNamedPipe("\\\\.\\pipe\\TrapRcvr",
				     PIPE_ACCESS_INBOUND,
				     PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_NOWAIT,
				     10,
				     32767,
				     32767,
				     0,
				     NULL);
#endif
  mNamedPipeHandle = CreateFile("\\\\.\\pipe\\TrapRcvr",
				GENERIC_READ,
				FILE_SHARE_READ,
				NULL,
				OPEN_EXISTING,
				FILE_ATTRIBUTE_NORMAL,
				NULL);
  if (mNamedPipeHandle == INVALID_HANDLE_VALUE)
  {
    int x = GetLastError();
    if (x == 99)
      ;
  }
} 

NamedPipe::~NamedPipe()
{
  try
  { 
    CloseHandle(mNamedPipeHandle);
  }
  catch(...) {}
}

BOOL
NamedPipe::Get(unsigned char* message, unsigned int& length)
{
  BOOL retVal = FALSE;
  unsigned long bytesAvail = 0;
  retVal = PeekNamedPipe(mNamedPipeHandle,
			 NULL,
			 0,
		         NULL,
			 &bytesAvail,
			 NULL);
  if (!retVal || (retVal && bytesAvail == 0))
    return FALSE;

  unsigned long read = 0;
  retVal = ReadFile(mNamedPipeHandle,
		    message,
		    length,
		    &read,
		    NULL);
  length = (unsigned int)read;

  return retVal;
}

void
NamedPipe::Put(unsigned char* message, unsigned int length)
{
  unsigned long len = 0;
  if (!WriteFile(mNamedPipeHandle,
			  message,
			  length,
			  &len,
			  NULL))
    cout << "write file failed <" << GetLastError() << ">" << endl;
  else
    cout << "write file was successful" << endl;
}
