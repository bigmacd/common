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
