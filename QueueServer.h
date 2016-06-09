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
#define __QUEUESERVER_H__

//#include "stdafx.h"
#include "windows.h"
#include <stdio.h>

class QueueServer
{

  private:

    HANDLE      mMailSlotHandle;

  protected:

  public:

    QueueServer(const char* type, const char* name, BOOL nonNull = FALSE);
    ~QueueServer();

    BOOL  Get(unsigned char* message, unsigned int& length);
    BOOL  MessageAvailable();

#ifdef _USE_PACKETLIB
    BOOL  Get(Packet* p);
#endif

};
#endif




