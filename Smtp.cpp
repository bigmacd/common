#ifndef __SMTP_H__
#include "Smtp.h"
#endif

#include "Registry.h"
#include <time.h>
//#include <fstream.h>
#include "Base64.h"

#include <iostream.h>

#include <fstream>
using namespace std;

static char* EOL = "\r\n"; // network end of line

// states
#define S_LOGIN 1
#define S_ID 2
#define S_MAILFROM 3
#define S_RCPTTO 4
#define S_DATA 5
#define S_DATA2 6
#define S_QUIT 7

#define S_BEGIN_AUTH    10
#define S_AUTH_USERNAME 20
#define S_AUTH_PASSWORD 30


Smtp::Smtp()
     :Thread("Smtp"),
      mPacket(NULL),
      mSocket(NULL)
{

}

Smtp::~Smtp()
{
  if (mSocket != NULL)
    delete mSocket;
}

  //Smtp::Smtp(Packet* packet, char* emailIds)
Smtp::Smtp(Packet* packet, CString emailIds)
     :Thread("Smtp"),
      mPacket(packet),
      mSocket(NULL),
      mEmailIds(emailIds)
{

}


void
Smtp::main()
{
  char          computerName[128];
  unsigned long size = sizeof(computerName);
  GetComputerName(computerName, &size);

  try
  {
    Registry registry;
    BOOL doLogging = registry.smtpLogging();

    CString currDir = registry.cwd();
    currDir += "\\TrapRcvr.smtp.log";

    std::ofstream ofile;
    ofile.open(currDir, ios::out | ios::app);

    registry.ReOpen("Email");
    unsigned long count = registry.getCount();

    ofile << "processing " << count << " email destinations" << endl;

    for (unsigned long x = 0; x < count; x++)
    {
      registry.ReOpen(x);
      int id = registry.emailId();
      CString c;
      c.Format("%d", id);
      c = c.Right(6);
      if (mEmailIds.Find(c) == -1)
      {
	ofile << "email id:" << id << " not part of this action" << endl;
	continue;
      }

      ofile << "email id:" << id << " part of this action" << endl;      
      ofile << "destination is " << registry.smtpServerHost() << endl;
      ofile << "destination port is " << registry.smtpPort() << endl;

      mSocket = new ClientSocket(registry.smtpServerHost().GetBuffer(0),
				 registry.smtpPort());

      char sendBuff[1024];
      char recvBuff[1024];
      int state = S_LOGIN;
      //      int state = S_DATA2;
      BOOL done = FALSE;
      int retries = 0;

      while (mSocket->Connected() && !done)
      //      while (!done)
      {
	memset(sendBuff, 0, 1024);
	memset(recvBuff, 0, 1024);
	mSocket->Receive(recvBuff, 1024);
	
	ofile << "Received:<" << recvBuff << '>' << endl;
	switch (state)
	{
	  case S_LOGIN:
	  {
	    ofile << "in login state" << endl;
	    if (!strstr(recvBuff, "220"))
	    {
	      ofile << "retrying login state" << endl;
	      if (retries++ < 3)
		Sleep(500 * retries);
	      else
	      {
		ofile << "giving up" << endl;
		done = TRUE;
	      }
	    }
	    else
	    {
	      if (registry.authNeeded())
		state = S_BEGIN_AUTH;
	      else
		state++;
	      retries = 0;
	      sprintf(sendBuff, "HELO %s%s", 
		      computerName, EOL);
	    }
	    break;
	  } // case S_LOGIN

   	  case S_ID:
	  {
	    ofile << "in ID state" << endl;
	    char* code = "250";
	    if (registry.authNeeded())
	      code = "235";
	    if (!strstr(recvBuff, code)) 
	    {
	      ofile << "retrying ID state" << endl;
	      if (retries++ < 3)
		Sleep(500 * retries);
	      else
	      {
		ofile << "giving up" << endl;
		done = TRUE;
	      }
	    }
	    else
	    {	      
	      state++;
	      retries = 0;
	      sprintf(sendBuff, "MAIL FROM: <%s>%s",
		      registry.from().GetBuffer(0),
		      EOL);
	    }
	      break;
	  } // case S_ID

	  case S_MAILFROM:
	  {
	    ofile << "in MAILFROM state" << endl;
	    if (!strstr(recvBuff, "250"))
	    {
	      ofile << "retrying MAILFROM state" << endl;
	      if (retries++ < 3)
		Sleep(500 * retries);
	      else
	      {
		ofile << "giving up" << endl;
		done = TRUE;
	      }
	    }
	    else
	    {
	      state++;
	      retries = 0;
	      sprintf(sendBuff, "RCPT TO: %s%s",
		      registry.to().GetBuffer(0),
		      EOL);
	    }
	    break;
	  } // case S_MAILFROM
	  
	  case S_RCPTTO:
	  {
	    ofile << "in RCPTTO state" << endl;
	    if (!strstr(recvBuff, "250"))
	    {
	      ofile << "retrying RCPTTO state" << endl;
	      if (retries++ < 3)
		Sleep(500 * retries);
	      else
	      {
		ofile << "giving up" << endl;
		done = TRUE;
	      }
	    }
	    else
	    {
	      state++;
	      retries = 0;
	      sprintf(sendBuff, "DATA%s", EOL);
	    }
	    break;
	  } // case S_RCPTTO
	  
	  case S_DATA:
	  {
	    ofile << "in DATA state" << endl;
	    if (!strstr(recvBuff, "354"))
	    {
	      ofile << "retrying DATA state" << endl;
	      if (retries++ < 3)
		Sleep(500 * retries);
	      else
	      {
		ofile << "giving up" << endl;
		done = TRUE;
	      }
	    }
	    else
	    {
	      state++;
	      retries = 0;
	      char* dateF = DateFormat();
	      CString realSubject = ExpandKeywords(registry.subject());
	      sprintf(sendBuff, 
		      "From: %s%sTo: %s%sSubject: %s%sDate: %s%sComment: Automated Sender%sX-Mailer: www.ncomtech.com%s%s",
		      registry.from().GetBuffer(0), EOL,
		      registry.to().GetBuffer(0), EOL,
		      //  registry.subject().GetBuffer(0), EOL,
		      realSubject, EOL,
		      dateF, EOL, EOL, EOL, EOL);
	      free(dateF);
	    }
	    break;
	  } // case S_DATA

	  case S_DATA2:
	  {
	    ofile << "in DATA2 state" << endl;
	    state++;
	    ////////////////////////////////////////
	    retries = 0;
	    CString realMessage;
	    CString Message = registry.message();
	    if (mPacket == NULL)
	      realMessage = Message;
	    else
	    {
	      /////5.50//////
	      realMessage = ExpandKeywords(Message);
	      /////5.50//////
	    }
	    ///////////////////////////////////////
	    sprintf(sendBuff, "%s%s%c%s",
		    realMessage.GetBuffer(0),
		    //  registry.message().GetBuffer(0),
		    EOL, '.', EOL);
	    break;
	  } // case S_DATA2

	  case S_QUIT:
	  {
	    ofile << "in QUIT state" << endl;
	    if (strstr(recvBuff, "250"))
	      sprintf(sendBuff, "QUIT%s", EOL);
	    done = TRUE;
	    break;
	  } // case S_QUIT

   	  case S_BEGIN_AUTH:
	  {
	    ofile << "in BEGIN_AUTH state" << endl;
	    if (!strstr(recvBuff, "250")) 
	    {
	      ofile << "retrying BEGIN_AUTH state" << endl;
	      if (retries++ < 3)
		Sleep(500 * retries);
	      else
	      {
		ofile << "giving up" << endl;
		done = TRUE;
	      }
	    }
	    else
	    {	      
	      state = S_AUTH_USERNAME;
	      retries = 0;
	      sprintf(sendBuff, "AUTH LOGIN%s",
		      EOL);
	    }
	      break;
	  } // case S_BEGIN_AUTH

	  case S_AUTH_USERNAME:
	  {
	    ofile << "in AUTHUN state" << endl;
	    if (!strstr(recvBuff, "334"))
	    {
	      ofile << "retrying AUTHUN state" << endl;
	      if (retries++ < 3)
		Sleep(500 * retries);
	      else
	      {
		ofile << "giving up" << endl;
		done = TRUE;
	      }
	    }
	    else
	    {
	      state = S_AUTH_PASSWORD;
	      retries = 0;
	      CBase64 cb6;
	      cb6.Decode(recvBuff + 4);
	      const char* tMsg = cb6.DecodedMessage();
	      if (!strcmp(tMsg, "Username:"))
	      {
		//		CBase64 cb62;
		//		cb62.Encode(registry.authUser().GetBuffer(0));
		sprintf(sendBuff, "%s%s",
			//			cb62.EncodedMessage(),
			registry.authUser().GetBuffer(0),
			EOL);
	      }
	      else
		done = TRUE;
	    }
	    break;
	  } // case S_AUTH_USERNAME

	  case S_AUTH_PASSWORD:
	  {
	    ofile << "in AUTHPW state" << endl;
	    if (!strstr(recvBuff, "334"))
	    {
	      ofile << "retrying AUTHPW state" << endl;
	      if (retries++ < 3)
		Sleep(500 * retries);
	      else
	      {
		ofile << "giving up" << endl;
		done = TRUE;
	      }
	    }
	    else
	    {
	      state = S_ID;
	      retries = 0;
	      CBase64 cb6;
	      cb6.Decode(recvBuff + 4);
	      const char* tMsg = cb6.DecodedMessage();
	      if (!strcmp(tMsg, "Password:"))
	      {
		//		CBase64 cb62;
		//		cb62.Encode(registry.authJuniper().GetBuffer(0));
		sprintf(sendBuff, "%s%s",
			//			cb62.EncodedMessage(),
			registry.authJuniper().GetBuffer(0),
			EOL);
	      }
	      else
		done = TRUE;
	    }
	    break;
	  } // case S_AUTH_PASSWORD

	} // switch (state)
	ofile << "sending:<" << sendBuff << '>' << endl;
	mSocket->Send(sendBuff, strlen(sendBuff));
	Sleep(2500);
      } // while (mSocket->Connected() && !done)
    } // end of for loop
    ofile.close();
  } // try
  catch(...)
  {

  }
  Thread::Exit();
} // ::main()

CString
Smtp::ExpandKeywords(CString message)
{
  CString realMessage;
  realMessage.Empty();

  char* str = message.GetBuffer(0);
  int len = strlen(str);
  while (len > 0)
  {
    BOOL zero = FALSE;	      
    char* end = str;
    if (*end != '%')
    {
      while (len > 0)
      {
	end++; len--;
	if (*end == '%')
	{
	  *end = 0;
	  zero = TRUE;
	  break;
	}
      }
    }
    // check to see if a predefined
    // argument was used
    char specific[32];
    char generic[32];
    if (!strncmp(str, "%SENDERIP%", 10))
    {
      len -= 10; end += 10;

      Registry r;
      if (r.reverseLookup())
      {
	unsigned long addr = inet_addr(mPacket->SenderIP());
	struct hostent* hp = gethostbyaddr((const char*)&addr, 
					   sizeof(unsigned long), 
					   AF_INET);
	if (hp == NULL)
	{
	  realMessage += mPacket->SenderIP();
	  realMessage += "-u";
	}
	else
	  realMessage += hp->h_name;
      }
      else
	realMessage += (char*)mPacket->SenderIP();
    }
    else
      if (!strncmp(str, "%SENDEROID%", 11))
      {
	len -= 11; end += 11;
	realMessage += (char*)mPacket->SenderOID();
      }
      else
	if (!strncmp(str, "%COMMUNITY%", 11))
	{
	  len -= 11; end += 11;
	  realMessage += 
	    (char*)mPacket->Community();
	}
	else
	  if (!strncmp(str, "%GENERICTYPE%", 13))
	  {
	    len -= 13; end += 13;
	    sprintf(generic, 
		    "%d",
		    mPacket->GenericTrapType());
	    realMessage += generic;
	  }
	  else
	    if (!strncmp(str, "%SPECIFICTYPE%", 14))
	    {
	      len -= 14; end += 14;			
	      sprintf(specific,
		      "%d",
		      mPacket->SpecificTrapType());
	      realMessage += specific;
	    }
	    else
	      if (!strncmp(str, "%VBOID", 6))
	      {
		char* percentSign = strchr(str + 1, '%');
		// there has to be more
		if (percentSign > (str + 6))
		{
		  if (percentSign <= (str + len))
		  {
		    *percentSign = 0;
		    int which = atoi(str + 6);
		    int vblen = mPacket->VbListLength();
		    if (which > 0 && which <= vblen)
		      realMessage += mPacket->VbOID(which);
		    len -= ((percentSign - str) + 1);
		    end += ((percentSign - str) + 1);
		    *percentSign = '%';
		  }
		  else
		    len = 0;  // when too far
		}
		else
		{ len -= 7; end += 7; }
	      }
	      else
		if (!strncmp(str, "%VBDATA", 7))
		{
		  char* percentSign = strchr(str + 1, '%');
		  // there has to be more
		  if (percentSign > (str + 7))
		  {
		    if (percentSign <= (str + len))
		    {
		      *percentSign = 0;
		      int which = atoi(str + 7);
		      int vblen = mPacket->VbListLength();
		      if (which > 0 && which <= vblen)
			realMessage += mPacket->VbData(which);
		      len -= ((percentSign - str) + 1);
		      end += ((percentSign - str) + 1);
		      *percentSign = '%';
		    }
		    else
		      len = 0;  // when too far
		  }
		  else
		  { len -= 8; end += 8; }
		}
		else
		{
		  if (end == str && *end == '%')
		  {
				// found a misspelled % demarc 
				// find the end
		    end = strchr(str + 1, '%');
		    if (end == NULL)
		      len = 0; // no closing %, get out
		    else
		    {
		      end++; // past the closing %
		      len -= end - str;
		    }
		  }
		  else
		    realMessage += str;
		}
    if (zero == TRUE)
      *end = '%';
    str = end;
  } // while (len)


  // version 6.0 - allow for embedded CR and LF
  // convert the string "\r" or "\n" to '\r' or '\n'
  // will have to add a space too (two characters)
  char* charstr = realMessage.GetBuffer(0);
  char* specChar;
  while ((specChar = strstr(charstr, "\\r")) != NULL)
  {
    *specChar++ = ' ';
    *specChar++ = '\r';
  }
  charstr = realMessage.GetBuffer(0);
  while ((specChar = strstr(charstr, "\\R")) != NULL)
  {
    *specChar++ = ' ';
    *specChar++ = '\r';
  }
  charstr = realMessage.GetBuffer(0);
  while ((specChar = strstr(charstr, "\\n")) != NULL)
  {
    *specChar++ = ' ';
    *specChar++ = '\n';
  }
  charstr = realMessage.GetBuffer(0);
  while ((specChar = strstr(charstr, "\\N")) != NULL)
  {
    *specChar++ = ' ';
    *specChar++ = '\n';
  }

  return realMessage;
}


char* 
Smtp::DateFormat()
{
  // Thu, 20 May 1999 11:36:01 -0400

  time_t t;
  time(&t);

  struct tm* newTime = localtime(&t);
  char cTime[128];
  memset(cTime, 0, 128);
  strftime(cTime, 128, "%a, %d %b %Y %H:%M:%S" , newTime);


#if 0
  TIME_ZONE_INFORMATION tzi;
  unsigned long result = GetTimeZoneInformation(&tzi);
  if (result == TIME_ZONE_ID_STANDARD)
  {

  }
  else
  if (result == TIME_ZONE_ID_DAYLIGHT)
  {
  
  } 

  strcat(cTime, " %s", tz);
  
  if (senddate.getTimezoneOffset() < 0)
    formatted = formatted + "+";
  else
  formatted = formatted + "-";
  if (Math.abs(senddate.getTimezoneOffset())/60 < 10) 
    formatted = formatted + "0";
  formatted = formatted + 
              String.valueOf(Math.abs(senddate.getTimezoneOffset())/60);
  if (Math.abs(senddate.getTimezoneOffset())%60 < 10) 
    formatted = formatted + "0";
  formatted = formatted + 
              String.valueOf(Math.abs(senddate.getTimezoneOffset())%60);
#endif
  return strdup(cTime);
}



