#include "ScmManager.h"
#include <iostream>
using namespace std;

int main(int argc, char** argv);
void Usage();

#define STARTACTION "--start"
#define STOPACTION "--stop"

#define NOACTION 0
#define STARTUP 1
#define SHUTDOWN 2

int 
main(int argc, char** argv)
{

  if (argc != 3)
  {
    Usage();
    return 0;
  }

  int action = NOACTION;
  int whichArg = 0;

  while (1)
  {
    if (!strcmp(argv[1], STARTACTION))
    {
      whichArg = 2;
      action = STARTUP;
      break;
    }
    if (!strcmp(argv[2], STARTACTION))
    {
      whichArg = 1;
      action = STARTUP;
      break;
    }
    if (!strcmp(argv[1], STOPACTION))
    {
      whichArg = 2;
      action = SHUTDOWN;    
      break;
    }
    if (!strcmp(argv[2], STOPACTION))
    { 
      whichArg = 1;
      action = SHUTDOWN;
      break;
    }
    break;
  }

  if (action == NOACTION)
  {
    Usage();
    return 0;
  }

  ScmManager scmManager(argv[whichArg]);
  if (scmManager.mError)
  {
    cout << "There was an error opening the service" << endl;
    cout << "the error code was " << scmManager.mError << endl;
    return 0;
  }

  if (action == STARTUP)
  {
    scmManager.StartupService();
  }
  else
  {
    if (!scmManager.ServiceIsRunning())
    {
      cout << "Request to stop service will not work because service is not running" << endl;
      return 0;
    }
    else
    {
	  int i;
      scmManager.ShutdownService();

      for (i = 0; i < 10; i++)
      {
	Sleep(1000);
	if (!scmManager.ServiceIsRunning())
	  break;
      }
      if (i == 20)
	cout << "Service does not seem to be responding to shutdown request." << endl;
    }
  }

  return 0;

}

void
Usage()
{

  cout << "Usage: scm <service name> <--start | --stop>" << endl;
  cout << "parameters can be specified in any order" << endl;

}
