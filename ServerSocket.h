#include <winsock.h>

class ServerSocket
{
   public:
      ServerSocket (int port);
     ~ServerSocket ( );

      SOCKET accept();

   private:
      const int    port_;
      SOCKET       hLstnSock_;
};
    
