#include "NtSysUpTime.h"
#endif

#include <windows.h>
#include <winbase.h>


NtSysUpTime::NtSysUpTime()
{
  // Initialize();
  //mCounter = AddCounter(SYSUPTIME);
}

NtSysUpTime::~NtSysUpTime()
{
  //  Uninitialize();
}

unsigned long 
NtSysUpTime::SysUpTime()
{
  unsigned long timeticks = GetTickCount();
  timeticks = timeticks / 10;
  //  CollectQueryData();
  //  unsigned long sysUpTime = GetCounterValue(mCounter);
  
  // little tweak to mimic the real sysUptime
  //  sysUpTime -= 60;
  //  sysUpTime *= 100;
  //  return sysUpTime;
  return timeticks;
}

