#include <sys/types.h>
#include <sys/time.h>
#include <iostream.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/sysctl.h>

#include "FreeBSDSysUptime.h"


FreeBSDSysUptime::FreeBSDSysUptime()
{

}

FreeBSDSysUptime::~FreeBSDSysUptime()
{

}

unsigned long
FreeBSDSysUptime::SysUpTime()
{
  time_t        now = time(0);  // default to something
  unsigned long retVal = (unsigned long)now;

  static int request[2] = { CTL_KERN, KERN_BOOTTIME };
  struct timeval tv;
  size_t len = sizeof(tv);
  sysctl(request, 2, &tv, &len, NULL, 0);
  retVal = ((unsigned long)difftime(now, tv.tv_sec)) * 100;
  return retVal;

}

