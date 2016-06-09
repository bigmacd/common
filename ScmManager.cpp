#ifndef __SCMANAGER_H__
#include "ScmManager.h"
#endif

//#include <iostream.h>

ScmManager::ScmManager()
           :mScManager(NULL),
	    mService(NULL),
	    mError(0),
	    mServiceInfo(NULL)
{
  mScManager = OpenSCManager(NULL,
			    NULL,
			    SC_MANAGER_ENUMERATE_SERVICE);
  if (mScManager == NULL)
    mError = GetLastError();
}

ScmManager::ScmManager(char* service)
           :mScManager(NULL),
	    mService(NULL),
	    mError(0),
	    mServiceInfo(NULL)
{
  mScManager = OpenSCManager(NULL,
			    NULL,
			    NULL);
  if (mScManager != NULL)
  {
    mService = OpenService(mScManager,
			   service,
			   SERVICE_ALL_ACCESS);
    if (mService == NULL)
      mError = GetLastError();
  }
  else
    mError = GetLastError();
}

ScmManager::~ScmManager()
{
  try
  {
    if (mService != NULL)
      CloseServiceHandle(mService);
    if (mScManager != NULL)
      CloseServiceHandle(mScManager);
    if (mServiceInfo != NULL)
      delete [] mServiceInfo;
  }
  catch(...)
  {}
}

BOOL
ScmManager::ServiceIsRunning()
{
  BOOL retVal = FALSE;

  SERVICE_STATUS scs;
  if (QueryServiceStatus(mService,
			 &scs))
    if (scs.dwCurrentState == SERVICE_RUNNING)
      retVal = TRUE;

  return retVal;
}

BOOL
ScmManager::ShutdownService()
{
  BOOL retVal = FALSE;

  SERVICE_STATUS scs;
  if (mService != NULL)
    if (ControlService(mService,
		       SERVICE_CONTROL_STOP,
		       &scs)) 
      retVal = TRUE;
  return retVal;
}

BOOL
ScmManager::StartupService()
{
  BOOL retVal = FALSE;
  if (mService != NULL)
    if (StartService(mService,
		     0,
		     NULL))
      retVal = TRUE;
  return retVal;
}



unsigned long
ScmManager::EnumerateServices()
{

  unsigned long bufferSize = 0;
  unsigned long bufferSizeNeeded = 0;
 
  if (mServiceInfo != NULL)
  {
    delete [] mServiceInfo;
    mServiceInfo = NULL;
  }

  BOOL status = EnumServicesStatus(mScManager,
				   SERVICE_WIN32,
				   SERVICE_STATE_ALL,
				   NULL,
				   bufferSize,
				   &bufferSizeNeeded,
				   &mNumberOfServices,
				   NULL);

  if (status == FALSE)
  {
    mError = GetLastError();
    //    PVOID errmsg;
    //    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, 
    //		  0, 
    //		  mError, 
    //		  0, 
    //		  (LPTSTR)&errmsg, 
    //		  0, 
    //		  NULL);
    //    cout << "error:" <<  (char*)errmsg << endl;
    //    LocalFree(errmsg);


    // do not return, because we are expecting a "need more data" error
  }


  bufferSize = bufferSizeNeeded + 0x10;
  mServiceInfo  = new unsigned char [bufferSize];

  status = EnumServicesStatus(mScManager,
			      SERVICE_WIN32,
			      SERVICE_ACTIVE,
			      (ENUM_SERVICE_STATUS*)mServiceInfo,
			      bufferSize,
			      &bufferSizeNeeded,
			      &mNumberOfServices,
			      NULL);

  if (status == FALSE)
  {
    mError = GetLastError();
    return 0;
  }

  return mNumberOfServices;
}


void    
ScmManager::Reset()
{
  if (mServiceInfo != NULL)
  {
    delete [] mServiceInfo;
    mServiceInfo = NULL;
  }
}


char*
ScmManager::GetServiceName(unsigned long which)
{
   ENUM_SERVICE_STATUS* serviceInfo = (ENUM_SERVICE_STATUS*)mServiceInfo;
   if (which >= 0 && which < mNumberOfServices)
     return serviceInfo[which].lpServiceName;
   
   return NULL;
}

char*
ScmManager::GetDisplayName(unsigned long which)
{
   ENUM_SERVICE_STATUS* serviceInfo = (ENUM_SERVICE_STATUS*)mServiceInfo;
   if (which >= 0 && which < mNumberOfServices)
     return serviceInfo[which].lpDisplayName;
   
   return NULL;
}

BOOL
ScmManager::IsRunning(unsigned long which)
{
  BOOL retVal = FALSE;
  ENUM_SERVICE_STATUS* serviceInfo = (ENUM_SERVICE_STATUS*)mServiceInfo;
  if (serviceInfo[which].ServiceStatus.dwCurrentState == SERVICE_RUNNING)
    retVal = TRUE;

  return retVal;
}

//    wpr_s(L"Prss Id \t : %04x (%d) \n", pInfo[i].ServiceStatusProcess.dwProcessId, pInfo[i].ServiceStatusProcess.dwProcessId);


