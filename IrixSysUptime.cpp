#include <utmp.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <fcntl.h>
#include <nlist.h>
#include <iostream.h>
#include <unistd.h>
#include <stdlib.h>

#include "IrixSysUptime.h"

#ifndef UTMP_FILE
#define UTMP_FILE		"/etc/utmp"
#endif

IrixSysUptime::IrixSysUptime()
{

}

IrixSysUptime::~IrixSysUptime()
{

}

unsigned long
IrixSysUptime::SysUpTime()
{
  struct utmp*  ptr;
  struct utmp*  utmpBuff;
  struct stat   statBuff;
  int	        fd;
  size_t        amt;
  char*         cp;
  time_t        now = time(0);  // default to something
  unsigned long retVal = (unsigned long)now;

  if (stat(UTMP_FILE, &statBuff) < 0) 
  {
    return retVal;
  }
  amt = statBuff.st_size;

  if ((fd = open(UTMP_FILE, O_RDONLY)) < 0) 
  {
    return retVal;
  }

  utmpBuff = (struct utmp *)malloc(amt);
  if (read(fd, (char *)utmpBuff, amt) != amt) 
  {
    (void) close(fd);
    return retVal;
  }
  (void)close(fd);

  for (ptr = utmpBuff; ptr < utmpBuff + amt; ++ptr) 
    if (ptr->ut_type == BOOT_TIME) 
    {
      retVal = ((unsigned long)difftime(now, ptr->ut_time)) * 100;
      break;
    }

  (void)free(utmpBuff);
  return retVal;
}

