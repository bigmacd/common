//  Copyright (c) 2001 Network Computing Technologies, Inc.
//  All rights reserved.
// 
//  Redistribution and use of executable software is never 
//  permitted without the express written permission of 
//  Network Computing Technologies, Inc.
// 
//  Distribution of the source is never permitted without 
//  the express written permission of 
//  Network Computing Technologies, Inc.
// 
//  THIS SOFTWARE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
//  IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
//  WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.

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
