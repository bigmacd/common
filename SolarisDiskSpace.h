#ifndef __SOLARISDISKSPACE_H__
#define __SOLARISDISKSPACE_H__

#include <stdio.h>
#include <string.h>

typedef struct _diskpart
{
  char device[256];
  char path[256];
  _diskpart()
  {
    memset(device, 0, 256);
    memset(path, 0, 256);
  }
} diskpart;

class SolarisDiskSpace
{

  private:

    diskpart**        mDiskPart;

    int               mNumberOfPartitions;

  protected:

  public:

    SolarisDiskSpace();
    ~SolarisDiskSpace();


    void NumberOfPartitions(int number);
    void PartitionName(int number, char* name);
    

    char* DeviceName(int partitionNumber);
    int   Percent(int partitionNumber);
    long  Available(int partitionNumber);

};

#endif
