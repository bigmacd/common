#ifndef __HPUXSYSUPTIME_H__
#include "HpuxSysUpTime.h"
#endif

#include <stdio.h>
#include <iostream.h>

#include <sys/pstat.h>

HpuxSysUpTime::HpuxSysUpTime()
{

}

HpuxSysUpTime::~HpuxSysUpTime()
{

}

unsigned long 
HpuxSysUpTime::SysUpTime()
{
  unsigned long  retVal = 0;

  time_t t = time(0);
  struct pst_static pst;
  if (pstat_getstatic(&pst, sizeof(pst), (size_t)1, 0) != -1)
    retVal = ((unsigned long)difftime(t, pst.boot_time)) * 100;
  return retVal;
}
