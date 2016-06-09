#include "SolarisSysUpTime.h"
#endif

#include <stdio.h>
#include <iostream.h>

#include <kstat.h>

SolarisSysUpTime::SolarisSysUpTime()
{

}

SolarisSysUpTime::~SolarisSysUpTime()
{

}

unsigned long 
SolarisSysUpTime::SysUpTime()
{
  unsigned long  retVal = 0;

  kstat_ctl_t*   ksc = kstat_open();
  if (ksc != NULL) 
  {
    kstat_t* ks = kstat_lookup(ksc, 
			       "unix", 
			       -1, 
			       "system_misc");
    if (ks != NULL) 
    {
      kid_t kid = kstat_read(ksc, ks, NULL);
      if (kid != -1) 
      {
	kstat_named_t* named = 
	  (kstat_named_t*)kstat_data_lookup(ks, "lbolt");
	if (named != NULL) 
	  retVal = named->value.ul;
      }
    }
    kstat_close(ksc);
  }
  return retVal;
}
