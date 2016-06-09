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




