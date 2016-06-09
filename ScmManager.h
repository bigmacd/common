
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


