#ifndef __LINUXSYSUPTIME_H__
#include "LinuxSysUptime.h"
#endif

#include <stdio.h>
#include <iostream.h>

LinuxSysUpTime::LinuxSysUpTime()
{

}

LinuxSysUpTime::~LinuxSysUpTime()
{

}

unsigned long 
LinuxSysUpTime::SysUpTime()
{
   FILE *in = fopen ("/proc/uptime", "r");
   unsigned long retVal = 0;
   long a, b;

   if (in) 
   {
     if (fscanf (in, "%ld.%ld", &a, &b) == 2)
       retVal = a * 100 + b;
     fclose (in);
   }
   return retVal;
}

