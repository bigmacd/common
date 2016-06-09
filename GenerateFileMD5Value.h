//  Copyright (c) 2011 Network Computing Technologies, Inc.
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

#ifndef __GENERATEFILEMD5VALUE_H__
#define __GENERATEFILEMD5VALUE_H__

#include "stdafx.h"
#include <iostream>
#include <windows.h>
#include <wincrypt.h>
#include <fstream>
#include <string.h>
#include <stdio.h>
#include "shlobj.h"
#include <direct.h>
#include <strsafe.h>

#include <string>
using namespace std;

typedef struct {
    unsigned char digest[16];
    unsigned long hHash;
} MD5Context;

class GenerateFileMD5Value 
{
  private:

  HCRYPTPROV           mCryptProvider;

  protected:

  BOOL                 CryptStart();
  void                 CryptStop();

  void                 MD5Init(MD5Context *ctx);
  void                 MD5Update(MD5Context *ctx, 
				 unsigned char const *buffer, 
				 unsigned int len);
  void                 MD5Final(MD5Context *ctx);

  public:

  GenerateFileMD5Value();
  ~GenerateFileMD5Value();

  string CalculateMD5(string FileName);

};
#endif
