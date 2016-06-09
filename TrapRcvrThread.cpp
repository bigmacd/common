//  Copyright (c) 1997-2001 Network Computing Technologies, Inc.
//  All rights reserved.
// 
//  Redistribution and use of executable software is never 
//  permitted without the express written permission of 
//  Network Computing Technologies, Inc.
// 
//  Distribution of the source is never permitted without 
//  the express written permission of 
//  Network Computing Technologies, Inc.
// 
//  THIS SOFTWARE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
//  IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
//  WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.

#ifndef __TRAPRCVRTHREAD_H__
#include "TrapRcvrThread.h"
#endif

#include "Packet.h"
#include "TrapRcvrException.h"
#include "SnmpParser.h"
#include "UdpServer.h"
#include "V2TrapPdu.h"

//#include <fstream>
//using namespace std;

TrapRcvrThread::TrapRcvrThread(int port, BOOL doInforms)
               :Thread("TrapRcvr"),
		mPort(port),
		mErrorCode(0),
		mDoInforms(doInforms)
{
  // first get the socket for receiving traps
  // initial the winsock
#ifdef _WIN32
  WSADATA wsaData; 
  int err; 
  OSVERSIONINFO osVer;
  memset(&osVer, 0, sizeof(OSVERSIONINFO));
  osVer.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
  GetVersionEx(&osVer);
  if (osVer.dwPlatformId == VER_PLATFORM_WIN32_NT)
  {
    err = WSAStartup(0x202, &wsaData);
    if (err == WSAVERNOTSUPPORTED)
      err = WSAStartup(0x200, &wsaData);
    if (err == WSAVERNOTSUPPORTED)
      err = WSAStartup(0x101, &wsaData);
  }
  else
    if (osVer.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)
    {
      err = WSAStartup(0x200, &wsaData);
      if (err == WSAVERNOTSUPPORTED)
	err = WSAStartup(0x101, &wsaData);
    }
    else
      err = 99;
#endif
}

TrapRcvrThread::~TrapRcvrThread()
{
}

void
TrapRcvrThread::main()
{
  //  std::ofstream ofile("trthread.log");

  UdpServer udp(mPort);
  udp.Timeout(0);
  if (!udp.IsReady())
  {
    mErrorCode = udp.ErrorCode();
    return;
  }

  while (State() != Thread::TERMINATED)
  {
    //    ofile << "waiting for data" << endl;
    const char* buf = udp.Receive();
    if (buf != NULL)
    {
      try
      {
	//	ofile << "received " << udp.ReadLength() << " bytes" << endl;
	SnmpParser snmpparser((unsigned char*)buf, (unsigned int)udp.ReadLength());
	Packet* packet = snmpparser.packet();
	if (packet != NULL)
	{
	  //	  ofile << "received a valid snmp packet" << endl;
	  unsigned char type = packet->Type();
	  if (type == V1TRAP || type == V2TRAP || type == INFORMPDU)
	  {
	    //	    ofile << "received a valid snmp trap" << endl;
	    // packet->TimeStamp(time(0));
	    // packet->PeerAddress(udp.Peer()->sin_addr.s_addr);
	    Add((unsigned char*)buf, 
		(unsigned int)udp.ReadLength(),
		udp.Peer()->sin_addr.s_addr);
	  }
	  if (type == INFORMPDU)
	  {
	    //	    ofile << "received an snmp INFORM" << endl;
	    if (mDoInforms)
	    {
	      //	      ofile << "we are configured to generate inform responses" << endl;
	      V1Pdu* p = (V1Pdu*)(packet->pdu());
	      if (p != NULL)
	      {
		p->MakeResponsePdu();
		packet->ErrorIndex(0);
		packet->ErrorStatus(NOERROR);
		//		ofile << "sending inform response" << endl;
		udp.Send(packet);
	      } // if (p != NULL)
	      //	      else ofile << "failed to extract inform pdu" << endl;
	    } // if (doInforms)
	  } // if (type == INFORMPDU)
	  delete packet;
	} // if (packet != NULL)
	//	else ofile << "failed to parse snmp packet" << endl;
      }
      catch(...)
      {
	    
      }
    } // if (buf != NULL)
  } // while (State() != Thread::TERMINATED)
}

