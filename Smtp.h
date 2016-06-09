#define __SMTP_H__

#include "Thread.h"
#include "Packet.h"
#include "ClientSocket.h"

class Smtp : public Thread
{

  private:

    Packet*        mPacket;
    ClientSocket*  mSocket;
    //    char*          mEmailIds;
    CString        mEmailIds;
    char*          DateFormat();
    CString        ExpandKeywords(CString message);

  protected:



  public:

    Smtp();
    Smtp(Packet* packet, CString emailIds);
    ~Smtp();

    virtual void main();

};

#endif
