#include "ServerSocket.h"
ServerSocket::ServerSocket(int port)
             :port_     (port),
              hLstnSock_(SOCKET_ERROR)
{


   SOCKADDR_IN pstSockName;
   //   WSAStartup(WSA_VERSION, &stWSAData);

   hLstnSock_ = socket(AF_INET, SOCK_STREAM, 0);
   if (hLstnSock_ == INVALID_SOCKET)
     int err = WSAGetLastError();

   pstSockName.sin_family      = PF_INET;
   pstSockName.sin_port        = htons(port_);  
   pstSockName.sin_addr.s_addr = INADDR_ANY;

   if (bind(hLstnSock_,
	    (LPSOCKADDR)&pstSockName,
	    sizeof(struct sockaddr)))
     int berr = WSAGetLastError();

   //   unsigned long on = 1;
   unsigned long on = 0;
   ioctlsocket(hLstnSock_, FIONBIO, &on);	

   listen(hLstnSock_, 5);

}


ServerSocket::~ServerSocket()
{  
}


SOCKET
ServerSocket::accept()
{
   SOCKET      s;

   SOCKADDR_IN pstSockName;
   int         len = sizeof (SOCKADDR_IN);
   
   s = ::accept(hLstnSock_, (LPSOCKADDR)&pstSockName, &len);
   if (s == INVALID_SOCKET)
     int err = WSAGetLastError();

   return s;  
}

