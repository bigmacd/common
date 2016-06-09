#include "ClientSocket.h"
#endif

#include <iostream>

#ifdef _USE_PACKETLIB
#include <Packet.h>
#endif

ClientSocket::ClientSocket(char* host, int port)
             :mPort(port),
              mSocket(INVALID_SOCKET),
	      mConnected(FALSE),
	      mStatus(0)
{

  //   SOCKADDR_IN sAddr;

   mSocket = socket(AF_INET, SOCK_STREAM, 0);
   if (mSocket == INVALID_SOCKET)
   {
     mStatus = WSAGetLastError();
     return;
   }

   mSaddr.sin_family      = PF_INET;
   mSaddr.sin_port        = htons(mPort);  

#ifdef _LINUX
   struct in_addr inaddr;
   if (inet_aton(host, &inaddr))
     mSaddr.sin_addr.s_addr = inaddr.s_addr;
#else
   unsigned long inaddr = inet_addr(host);

   if (inaddr != INADDR_NONE)
     mSaddr.sin_addr.s_addr = inaddr;
#endif
   else
   {
     HOSTENT* hostent = gethostbyname(host);
     if (hostent != NULL)
     {
       memcpy((char*)&(mSaddr.sin_addr.s_addr), 
       	      hostent->h_addr,
	      //       	      (const void*)*(hostent->h_addr),
       	      hostent->h_length);
     }
     else
     {
       mStatus = WSAGetLastError();
       return;
     }
   }

   Connect();
}

void
ClientSocket::Connect()
{
  if (mConnected == FALSE)
  {
    if (!connect(mSocket, 
		 (struct sockaddr *)&mSaddr, 
		 sizeof(mSaddr)))
    {
      mConnected = TRUE;
      unsigned long on = 1;
      ioctlsocket(mSocket, FIONBIO, &on);	      
    }
    else
      mStatus = WSAGetLastError();
  }
}


ClientSocket::~ClientSocket()
{  
  if (mSocket != INVALID_SOCKET)
    closesocket(mSocket);
}


bool    
ClientSocket::Connected()
{
  return mConnected;
}

#ifdef _USE_PACKETLIB
unsigned int 
ClientSocket::Send(Packet* p)
{
  int retVal = 0;
  Block();
  try
  {
    unsigned long len = p->TotalLength();
    unsigned char* t = new unsigned char[len];
    memset(t, 0, len);
    p->Build(t);
    retVal = send(mSocket,
		  (const char*)t,
		  len,
		  0);
    delete [] t;
  }
  catch(...)
  {
    retVal = 0;
  }

  mStatus = WSAGetLastError();

  UnBlock();

  return (unsigned int)retVal;
}
#endif

unsigned int 
ClientSocket::Send(const char* message,
		   unsigned int messageLength)

{
  Block();
  int retVal = send(mSocket,
		    message,
		    messageLength,
		    0);
  if (retVal < 0)
    retVal = 0;

  mStatus = WSAGetLastError();

  UnBlock();

  return (unsigned int)retVal;
}

unsigned int 
ClientSocket::Receive(char* buffer, 
		      unsigned int bufferSize)
{

  int retVal = recv(mSocket, buffer, bufferSize, 0);
  if (retVal < 0)
    retVal = 0;

  mStatus = WSAGetLastError();
  return (unsigned int)retVal;
}

void         
ClientSocket::Block()
{
  unsigned long on = 0;
  ioctlsocket(mSocket, FIONBIO, &on);	      
}

void         
ClientSocket::UnBlock()
{
  unsigned long on = 1;
  ioctlsocket(mSocket, FIONBIO, &on);	      
}



