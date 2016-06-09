//  Copyright (c) 1999-2001 Network Computing Technologies, Inc.
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
//

#ifndef __CONFIG_H__
#define __CONFIG_H__


#ifdef _WIN32
#ifndef _WINDOWS
//#include <windows.h>
#endif
#endif

#include "compat.h"

#include <iostream>
#include <fstream>
using std::ofstream;

class Config
{

  private:

    bool         mGotConfig;

	std::fstream mConfigFile;
	std::streampos  mStreamPos;
    int          mLastReadCount;

    char         mLineBuffer[4096];
    char*        mLineBuf;

    char         mFileName[512];
    
    //    mutex_t      mLock;

    void init();

  protected:

    void         Rewind();

    const char*  Search(char* param);

    // returns how many match the param
    unsigned int SearchAll(char* param);

    // returns an individual one of the group
    const char*  SearchOne(char* param, int which);

    void         Lock();
    void         Unlock();


  public:

    Config();
    Config(const char* filename);
    ~Config();

    int          ConfigFileFound();

    void         Filename(const char* filename);

};
#endif
