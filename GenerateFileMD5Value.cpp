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
#include "GenerateFileMD5Value.h"
#endif

GenerateFileMD5Value::GenerateFileMD5Value()
{
  CryptStart();
}

GenerateFileMD5Value::~GenerateFileMD5Value()
{
  CryptStop();
}

BOOL 
GenerateFileMD5Value::CryptStart()
{
  BOOL retVal = TRUE;
    if(CryptAcquireContext(&mCryptProvider, 
			   NULL, 
			   MS_ENHANCED_PROV, 
			   PROV_RSA_FULL, 
			   CRYPT_VERIFYCONTEXT) == 0)
    {
      if(GetLastError() == NTE_EXISTS)
      {
	if(CryptAcquireContext(&mCryptProvider, 
			       NULL, 
			       MS_ENHANCED_PROV, 
			       PROV_RSA_FULL, 
			       0) == 0)
	  retVal = FALSE;
      }
      else 
	retVal = FALSE;
    }
    return retVal;
}

void 
GenerateFileMD5Value::CryptStop()
{
    if(mCryptProvider) 
      CryptReleaseContext(mCryptProvider, 0);
    mCryptProvider = 0;
}

void 
GenerateFileMD5Value::MD5Init(MD5Context *ctx)
{

    CryptCreateHash(mCryptProvider, 
		    CALG_MD5, 
		    0, 
		    0, 
		    &ctx->hHash);
}


void 
GenerateFileMD5Value::MD5Update(MD5Context *ctx, unsigned char const *buf, unsigned int len)
{
    CryptHashData(ctx->hHash, buf, len, 0);
}

void 
GenerateFileMD5Value::MD5Final(MD5Context *ctx)
{
    DWORD dwCount = 16;
    CryptGetHashParam(ctx->hHash, HP_HASHVAL, ctx->digest, &dwCount, 0);
    if(ctx->hHash) 
      CryptDestroyHash(ctx->hHash);
    ctx->hHash = 0;
}

string 
GenerateFileMD5Value::CalculateMD5(string fileName)
{
  FILE* fInput = fopen(fileName.c_str(), "rb");

  if(!fInput)
  {
    throw(0);
  }

  MD5Context md5Hash;
  memset(&md5Hash, 0, sizeof(MD5Context));
  MD5Init(&md5Hash);

  unsigned char bBuffer[4096];
  while(!feof(fInput)){
    unsigned int nCount = (unsigned int)fread(bBuffer, sizeof(unsigned char), 4096, fInput);
    MD5Update(&md5Hash, bBuffer, nCount);
  }
  MD5Final(&md5Hash);
    
  fclose(fInput);

  // this or base64? 
  char value[1024];
  unsigned char b;
  int k = 0;
  for(int i = 0; i < 16; i++)
  {
    b = md5Hash.digest[i];
    for(int j = 4; j >= 0; j -= 4)
    {
      char c = ((char)(b >> j) & 0x0F);
      if(c < 10) c += '0';
      else c = ('a' + (c - 10));
      value[k] = c;
      k++;
    }
  }
  value[k] = '\0';

  return string(value);
	
}


