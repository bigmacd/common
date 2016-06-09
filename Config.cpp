#ifndef __CONFIG_H__
#include "Config.h"
#endif

#ifdef _WIN32
#include <istream>
#endif

#if defined(_HPUX) || defined(_LINUX)
#include <fstream>
#include <iostream>
#endif

Config::Config()
       :mGotConfig(FALSE)
{

}

Config::Config(const char* filename)
       :mGotConfig(FALSE)
{
  Filename(filename);
}

void
Config::init()
{
#if defined(_HPUX) 
  mConfigFile.open(mFileName, ios::in);
#else
  mConfigFile.open(mFileName, std::ios::in | std::ios::out);
#endif 
  if (mConfigFile.good())
    mGotConfig = TRUE;

  int x = 0;//mutex_init(&mLock, USYNC_THREAD, NULL);
  if (x != 0)
    mGotConfig = FALSE;
}

void
Config::Filename(const char* filename)
{
  memset(mFileName, 0, 512);
  //strncpy(mFileName, filename, 511);
  strncpy_s(mFileName, 511, filename, 511);
  init();
}

Config::~Config()
{
  mConfigFile.close();
  //  mutex_destroy(&mLock);
}

int
Config::ConfigFileFound()
{
  return mGotConfig;
}

void
Config::Lock()
{
  //  mutex_lock(&mLock);
}

void
Config::Unlock()
{
  //  mutex_unlock(&mLock);
}

void
Config::Rewind()
{
#if defined(_HPUX) || defined(_LINUX)
  //  mConfigFile.close();
  //  mConfigFile.open(mFileName);
  mConfigFile.clear();
  mConfigFile.seekg(0);
  //  mConfigFile.seekg(0);
  //  mConfigFile.clear();
#else
  mConfigFile.clear();
  mConfigFile.seekg(0);
#endif
}

const char*
Config::Search(char* param)
{
  mLineBuf = NULL;
    
  Rewind();

  while (!mConfigFile.eof())
  {
    memset(mLineBuffer, 0, 4096);
    mConfigFile.getline(mLineBuffer, 4096);
    if (mLineBuffer[0] == '#')
      continue;
    if (mLineBuf = strstr(mLineBuffer, param))
    {
      mStreamPos = mConfigFile.tellg();
      mLastReadCount = mConfigFile.gcount();
      mStreamPos -= (mLastReadCount + 1);
      mLineBuf += strlen(param);
      while (*mLineBuf == ' ' || *mLineBuf == '\t')
	mLineBuf++;
      break;
    }
  }
  return (const char*)mLineBuf;
}

unsigned int
Config::SearchAll(char* param)
{
  unsigned int retVal = 0;
  Rewind();

  while (!mConfigFile.eof())
  {
    memset(mLineBuffer, 0, 4096);
    mConfigFile.getline(mLineBuffer, 4096);
    if (mLineBuffer[0] == '#')
      continue;
    if (strstr(mLineBuffer, param) != NULL)
      retVal++;
  }
  return retVal;
}

const char*
Config::SearchOne(char* param, int which)
{
  mLineBuf = NULL;
    
  Rewind();

  while (!mConfigFile.eof())
  {
    memset(mLineBuffer, 0, 4096);
    mConfigFile.getline(mLineBuffer, 4096);
    if (mLineBuffer[0] == '#')
      continue;
    if (mLineBuf = strstr(mLineBuffer, param))
    {
      mStreamPos = mConfigFile.tellg();
      mLastReadCount = mConfigFile.gcount();
      mStreamPos -= (mLastReadCount + 1);
      mLineBuf += strlen(param);
      while (*mLineBuf == ' ' || *mLineBuf == '\t')
	mLineBuf++;
      if (!which--)
        break;
    }
  }
  return (const char*)mLineBuf;
}


