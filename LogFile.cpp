#ifndef __LOGFILE_H__
#include "LogFile.h"
#endif

LogFile::LogFile()
        :mIsOpen(FALSE),
	 mFile(NULL),
	 mLock(INVALID_HANDLE_VALUE)
{
  mFile.open(".\\TrDebug.log");
  mIsOpen = TRUE;
  mLock = CreateMutex(0, FALSE, 0);
}

LogFile::~LogFile()
{
  try
  {
    mFile.close();
    CloseHandle(mLock);
  }
  catch(...)
  {

  }
}

void
LogFile::Filename(CString filename)
{

}

void    
LogFile::Message(const char* msg)
{
  WaitForSingleObject(mLock, INFINITE);

  if (mIsOpen)
    mFile << msg << endl;

  ReleaseMutex(mLock);
}

LogFile gLogFile;
