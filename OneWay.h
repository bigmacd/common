
#ifdef __cplusplus
extern "C" {            
#endif	

// Module handle for OneWay.DLL
HINSTANCE		hInstDLL;
HINSTANCE		hInstSerialDLL;

// DLL name to load
#define ONEWAYDLL "OneWay.dll"
#define ONEWAYSERIALDLL "OneWaySerial.dll"

// Function Pointer Declarations
typedef bool  ( __stdcall *pAppSetupTrial )( char*, int );
typedef int   ( __stdcall *pAppStatus )( char* );
typedef int   ( __stdcall *pAppRegister )( char*, char*, char* );
typedef bool  ( __stdcall *pAppRemove )( char* );
typedef int   ( __stdcall *pIncTimesRun )( char* );
typedef int   ( __stdcall *pGetTimesRun )( char* );
typedef int   ( __stdcall *pSetTimesRun )( char*, int );
typedef bool  ( __stdcall *pSetEncrypted )( char*, char*, char* );
typedef char *( __stdcall *pGetEncrypted )( char*, char* );
typedef bool  ( __stdcall *pDelEncrypted )( char*, char* );
typedef char *( __stdcall *pDLLVersion )( void );
typedef bool  ( __stdcall *pGetSerialInfo )( char*, char*, int, int, int, int, int, int, int, int, int );

typedef char *( __stdcall *pGetSerial ) ( char*, int, int, int, int, int, int );

// Function Pointers
pAppSetupTrial   AppSetupTrial;
pAppStatus       AppStatus;
pAppRegister     AppRegister;
pAppRemove       AppRemove;
pIncTimesRun     IncTimesRun;
pGetTimesRun     GetTimesRun;
pSetTimesRun     SetTimesRun;
pSetEncrypted    SetEncrypted;
pGetEncrypted    GetEncrypted;
pDelEncrypted    DelEncrypted;
pDLLVersion      DLLVersion;
pGetSerialInfo   GetSerialInfo;

pGetSerial       GetSerial;

//Initialize OneWay.dll functions
bool InitOneWayDLL(void)
{
	hInstDLL = LoadLibrary(ONEWAYDLL);

	if (hInstDLL)
	{
		AppSetupTrial = (pAppSetupTrial) GetProcAddress(hInstDLL, "AppSetupTrial");
		AppStatus = (pAppStatus) GetProcAddress(hInstDLL, "AppStatus");
		AppRegister = (pAppRegister) GetProcAddress(hInstDLL, "AppRegister");
		AppRemove = (pAppRemove) GetProcAddress(hInstDLL, "AppRemove");
		IncTimesRun = (pIncTimesRun) GetProcAddress(hInstDLL, "IncTimesRun");
		GetTimesRun = (pGetTimesRun) GetProcAddress(hInstDLL, "GetTimesRun");
		SetTimesRun = (pSetTimesRun) GetProcAddress(hInstDLL, "SetTimesRun");
		SetEncrypted = (pSetEncrypted) GetProcAddress(hInstDLL, "SetEncrypted");
		GetEncrypted = (pGetEncrypted) GetProcAddress(hInstDLL, "GetEncrypted");
		DelEncrypted = (pDelEncrypted) GetProcAddress(hInstDLL, "DelEncrypted");
		DLLVersion = (pDLLVersion) GetProcAddress(hInstDLL, "DLLVersion");
		GetSerialInfo = (pGetSerialInfo) GetProcAddress(hInstDLL, "GetSerialInfo");

		return TRUE;	
	}
	else
		return FALSE;
}

// Remove references to OneWay.dll functions
void EndOneWayDLL(void)
{
    FreeLibrary(hInstDLL);
}
   
// Initialize OneWaySerial.dll functions
bool InitOneWaySerialDLL(void)
{
	hInstSerialDLL = LoadLibrary(ONEWAYSERIALDLL);

	if (hInstSerialDLL)
	{
		GetSerial = (pGetSerial) GetProcAddress(hInstSerialDLL, "GetSerial");
                
		return TRUE;	
	}
	else
   		return FALSE;
}

// Remove references to OneWaySerial.dll functions
void EndOneWaySerialDLL(void)
{
    FreeLibrary(hInstSerialDLL);
}
   
#ifdef __cplusplus
}
#endif
