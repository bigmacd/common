#ifndef __CLIENTSOCKET_H__
#define __CLIENTSOCKET_H__

#ifdef _WIN32
#include <winsock2.h>
#endif

#ifndef _WIN32
#include "compat.h"
#endif

#ifdef _USE_PACKETLIB
class Packet;
#endif

class ClientSocket
{
  private:

    int          mPort;
    SOCKET       mSocket;
    bool         mConnected;
    SOCKADDR_IN  mSaddr;


    int          mStatus;

  protected:


  public:

    ClientSocket (char* host, int port);
    ~ClientSocket ( );

    bool    Connected();

    void    Connect();

    unsigned int Send(const char* message,
		      unsigned int messageLength);

#ifdef _USE_PACKETLIB
    unsigned int Send(Packet* p);
#endif

    unsigned int Receive(char* buffer, 
			 unsigned int bufferSize);

    int          Status() { return mStatus; }

    void         Block();
    void         UnBlock();
};
    
#endif

