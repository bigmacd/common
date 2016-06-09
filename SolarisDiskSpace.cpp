#include "SolarisDiskSpace.h"
#endif

#include <sys/mntent.h>
#include <sys/mnttab.h>
#include <sys/stat.h>
#include <sys/statvfs.h>
#include <errno.h>

#include <iostream.h>

SolarisDiskSpace::SolarisDiskSpace()
                 :mDiskPart(NULL),
		  mNumberOfPartitions(0)
{

}

SolarisDiskSpace::~SolarisDiskSpace()
{

}

void 
SolarisDiskSpace::NumberOfPartitions(int number)
{
  if (!mNumberOfPartitions && number > 0 && number < 11)
  {
    mNumberOfPartitions = number;
    mDiskPart = new diskpart* [number];
    for (int i = 0; i < number; i++)
      mDiskPart[i] = new diskpart;
  }
}

void 
SolarisDiskSpace::PartitionName(int number, char* name)
{
  FILE* mntfp;
  if (name != NULL && number > 0 && number <= mNumberOfPartitions)
  {

    struct mnttab mntent;
    mntfp = fopen ("/etc/mnttab", "r");

    strncpy(mDiskPart[number - 1]->path, name, 255);
    if (mntfp != NULL)
    { 
      while (getmntent (mntfp, &mntent) == 0)
	if (strcmp (name, mntent.mnt_mountp) == 0) 
	{
	  strncpy(mDiskPart[number - 1]->device, mntent.mnt_special, 255);
	  break;
	}
      fclose (mntfp);
    }
  }
}

int 
SolarisDiskSpace::Percent(int partitionNumber)
{
  int retVal = 100;
  struct statvfs vfs;

  if (partitionNumber > 0 && partitionNumber <= mNumberOfPartitions)
  {
    if (statvfs(mDiskPart[partitionNumber - 1]->path, &vfs) != -1) 
    {
      retVal = vfs.f_bavail <= 0 ? 100 :
	(int) ((double) (vfs.f_blocks - vfs.f_bfree) /
	       (double) (vfs.f_blocks - (vfs.f_bfree - vfs.f_bavail)) * 100.0 + 0.5);

    }
  }
  return retVal;
}

long 
SolarisDiskSpace::Available(int partitionNumber)
{
  long retVal = 0;
  struct statvfs vfs;

  if (partitionNumber > 0 && partitionNumber <= mNumberOfPartitions)
  {
    if (statvfs (mDiskPart[partitionNumber - 1]->path, &vfs) != -1) 
    {
      retVal = vfs.f_bavail * (vfs.f_bsize / 1024);
      if (vfs.f_frsize > 255)
	retVal = vfs.f_bavail * (vfs.f_frsize / 1024);
      
    }
  }
  return retVal;
}

char* 
SolarisDiskSpace::DeviceName(int partitionNumber)
{
  char* retVal = NULL;
  if (partitionNumber > 0 && partitionNumber <= mNumberOfPartitions)
    retVal = mDiskPart[partitionNumber - 1]->device;
  return retVal;
}

