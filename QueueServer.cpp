//  Copyright (c) 1997,1998 Network Computing Technologies, Inc.
//  All rights reserved.
// 
//  Redistribution and use of executable software is never 
//  permitted without the express written permission of 
//  Network Computing Technologies, Inc.
// 
//  Distribution of the source is never permitted without 
//  the express written permission of 
//  Network Computing Technologies, Inc.
// 
//  THIS SOFTWARE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
//  IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
//  WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.

#ifndef __QUEUESERVER_H__
#include "QueueServer.h"
#endif

QueueServer::QueueServer(const char* type, const char* name, BOOL nonNull)
{
  int len = strlen(type);
  len += strlen(name);
  len += 20;
  char* msName = new char [len];
  sprintf_s(msName, len - 1, "\\\\.\\mailslot\\%s\\%s", type, name);
  if (nonNull)
  {
    SECURITY_DESCRIPTOR sd;
    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(sa);
    InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION);
    SetSecurityDescriptorDacl(&sd, TRUE, NULL, TRUE);
    SetSecurityDescriptorGroup(&sd, NULL, TRUE);
    SetSecurityDescriptorOwner(&sd, NULL, TRUE);
    sa.lpSecurityDescriptor = &sd;
    sa.bInheritHandle = TRUE;
    mMailSlotHandle = CreateMailslot(msName,
				     0,
				     0,
				     &sa);
  }
  else
  {
    mMailSlotHandle = CreateMailslot(msName,
				     0,
				     0,
				     NULL);
  }
  delete [] msName;
}

#if 0
QueueServer::QueueServer(const char* type, const char* name)
{
  int len = strlen(type);
  len += strlen(name);
  len += 20;
  char* msName = new char [len];
  sprintf(msName, "\\\\.\\mailslot\\%s\\%s", type, name);
  mMailSlotHandle = CreateFile(msName
			       GENERIC_WRITE,
			       FILE_SHARE_READ,
			       NULL,
			       OPEN_EXISTING,
			       FILE_ATTRIBUTE_NORMAL,
			       NULL);
  delete [] msName;
}
#endif
 
QueueServer::~QueueServer()
{
  try
  { 
    CloseHandle(mMailSlotHandle);
  }
  catch(...) {}
}

BOOL  
QueueServer::MessageAvailable()
{
  BOOL retVal;
  unsigned long msgSize;
  unsigned long numMsgs;
  retVal = GetMailslotInfo(mMailSlotHandle,
			   NULL,
			   &msgSize,
			   &numMsgs,
			   NULL);
  if (retVal)
    if (msgSize == MAILSLOT_NO_MESSAGE)
      retVal = FALSE;

  return retVal;
}

BOOL
QueueServer::Get(unsigned char* message, unsigned int& length)
{
  BOOL retVal = FALSE;
  unsigned long msgSize;
  unsigned long numMsgs;
  retVal = GetMailslotInfo(mMailSlotHandle,
			   NULL,
			   &msgSize,
			   &numMsgs,
			   NULL);
  if (retVal)
  {
    if (msgSize != MAILSLOT_NO_MESSAGE)
    {
      length = (unsigned int)msgSize;
      unsigned long read = 0;
      retVal = ReadFile(mMailSlotHandle,
			message,
			length,
			&read,
			NULL);
    }
    else
      retVal = FALSE;
  }
  return retVal;
}

#ifdef _USE_PACKETLIB
BOOL  
QueueServer::Get(Packet* p)
{

}
#endif
