#ifndef __NTSYSUPTIME_H__
#define __NTSYSUPTIME_H__

//#define SYSUPTIME "\\System\\System Up Time" 

//#include "perfmon.h"


//class NtSysUpTime : public CPerfMon
class NtSysUpTime
{

  private:

  //    int      mCounter;

  protected:

  public:

    NtSysUpTime();
    //    virtual ~NtSysUpTime();
    ~NtSysUpTime();

    unsigned long SysUpTime();

};
#endif
