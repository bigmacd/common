#include "Ping.h"
#endif


Ping::Ping(const char* ipAddress, unsigned long timeout)
  :Thread("Ping"),
   mTimeout(timeout),
   mHandle(INVALID_HANDLE_VALUE),
   mIpAddress(NULL)
{
  mHandle = IcmpCreateFile();  
  if (ipAddress != NULL)
  {
    mIpAddress = new char [strlen(ipAddress) + 1];
    strcpy(mIpAddress, ipAddress);
  }
}

Ping::~Ping()
{
  if (mHandle != INVALID_HANDLE_VALUE)
    IcmpCloseHandle(mHandle);
  if (mIpAddress != NULL)
    delete [] mIpAddress;
}

void
Ping::main()
{
  
}
