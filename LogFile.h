//  Copyright (c) 1997 Network Computing Technologies, Inc.
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



#ifndef __LOGFILE_H__
#define __LOGFILE_H__

#include <fstream>
#include <afxwin.h>

class LogFile
{

  private:

    BOOL       mIsOpen;
    ofstream   mFile;

    HANDLE     mLock;

  protected:



  public:


    LogFile();
    ~LogFile();

    void    Filename(CString filename);

    void    Message(const char* msg);

};

#endif



