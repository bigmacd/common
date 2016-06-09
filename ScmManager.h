//  Copyright (c) 2002 Network Computing Technologies, Inc.
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


#ifndef __SCMMANAGER_H__
#define __SCMMANAGER_H__
#endif

#include <windows.h>
#include <winsvc.h>

class ScmManager
{

  private:

    SC_HANDLE      mScManager;
    SC_HANDLE      mService;

    unsigned char* mServiceInfo;
    unsigned long  mNumberOfServices;

  protected:

  public:

    int             mError;


    ScmManager();
    ScmManager(char* service);
    ~ScmManager();

    BOOL          ServiceIsRunning();
    BOOL          ShutdownService();
    BOOL          StartupService();

    unsigned long EnumerateServices();
    char*         GetServiceName(unsigned long which);
    char*         GetDisplayName(unsigned long which);
    BOOL          IsRunning(unsigned long which);
    void          Reset();
};


