//  Copyright (c) 1997-2001 Network Computing Technologies, Inc.
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

#ifndef __TRAPRCVRTHREAD_H__
#define __TRAPRCVRTHREAD_H__

#include "Thread.h"

#ifdef _WIN32
//#include "winsock.h"
#endif

#include "compat.h"

class Packet;

class TrapRcvrThread : public Thread
{


  private:

    int            mPort;
    long           mErrorCode;
    BOOL           mDoInforms;

  protected:

    virtual void   Add(Packet* packet) = 0;
    virtual void   Add(unsigned char* buf, 
		       unsigned int length,
		       unsigned long peerAddr) = 0;

  public:

    TrapRcvrThread(int port, BOOL doInforms);
    ~TrapRcvrThread();

    virtual void        main();

    long                ErrorCode() { return mErrorCode; }

};

#endif
