#define __PING_H__

#include "..\common\thread.h"

class Ping : public Thread
{


  private:

  
    unsigned long mTimeout;
    char*         mIpAddress;
  

  protected:




  public:


    Ping(const char* ipAddress, unsigned long timeout = 10000);
  
    ~Ping();


    virtual void   main(void);
};

#endif
