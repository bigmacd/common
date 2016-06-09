

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



