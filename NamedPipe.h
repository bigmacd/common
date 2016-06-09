#ifndef __NAMEDPIPE_H__
#define __NAMEDPIPE_H__

#include "windows.h"

class NamedPipe
{

  private:

    HANDLE      mNamedPipeHandle;

  protected:

  public:

    NamedPipe();
    NamedPipe(BOOL client);
    
    ~NamedPipe();


    BOOL     Get(unsigned char* message, unsigned int& length);
    void     Put(unsigned char* message, unsigned int length);

};
#endif
