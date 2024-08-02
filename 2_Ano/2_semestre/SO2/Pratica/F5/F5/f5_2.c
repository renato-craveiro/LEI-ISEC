#include <Windows.h>
#include <tchar.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <conio.h>

#define SHM_NAME TEXT("fmMsgSpace")
#define MUTEXT_NAME TEXT("RWMUTEX")
#define EVENT_NAME TEXT("NEWMSG")

#define MSGTEXT_SZ 100

typedef struct _MSG {
	TCHAR szMessage[MSGTEXT_SZ];
	TCHAR userName[MSGTEXT_SZ];
}SharedMsg;

#define MSFBUFSIZE sizeof(SharedMsg)

typedef struct _ControlData {
	HANDLE hMapFile;
	SharedMsg* sharedMsg;
	int threadMustContinue;
	HANDLE hnewMsg;
	HANDLE hRWMutex;
}ControlData;

BOOL initMemAndSync(ControlData* cdata) {

	cdata->hMapFile = CreateFileMapping(
		INVALID_HANDLE_VALUE,
		NULL,
		PAGE_READWRITE,
		0,
		MSFBUFSIZE,
		SHM_NAME
	);

	if (cdata->hMapFile == NULL) {
		_tprintf(TEXT("Error %d opening file (CreateFileMapping)\n"), GetLastError());
		return FALSE;
	}

	//----------------------------------------------


	cdata->sharedMsg = (SharedMsg*)MapViewOfFile(
		cdata->hMapFile,
		FILE_MAP_ALL_ACCESS,
		0,
		0,
		MSFBUFSIZE
	);

	//-----------------------

	if (cdata->sharedMsg == NULL) {
		_tprintf(TEXT("Error %d opening file (MapViewOfFile)\n"), GetLastError());
		CloseHandle(cdata->hMapFile);
		return FALSE;
	}

	cdata->hRWMutex = CreateMutex(
		NULL,
		FALSE,
		MUTEXT_NAME
	);

	if (cdata->hRWMutex == NULL) {
		_tprintf(TEXT("Error %d creating mutex (CreateMutex)\n"), GetLastError());
		UnmapViewOfFile(cdata->sharedMsg);
		CloseHandle(cdata->hMapFile);
		return FALSE;
	}

	cdata->hnewMsg = CreateEvent(
		NULL,
		TRUE,
		FALSE,
		EVENT_NAME
	);

	if (cdata->hnewMsg == NULL) {
		_tprintf(TEXT("Error %d creating event (CreateEvent)\n"), GetLastError());
		UnmapViewOfFile(cdata->sharedMsg);
		CloseHandle(cdata->hMapFile);
		CloseHandle(cdata->hRWMutex);
		return FALSE;
	}

	


	return TRUE;
}
DWORD WINAPI receiveMsg(LPVOID p) {
	ControlData* pcd = (ControlData*)p;
	SharedMsg msg;
	while (1) {
		WaitForSingleObject(pcd->hnewMsg, INFINITE);
		if(!pcd->threadMustContinue) return 0;

		WaitForSingleObject(pcd->hRWMutex, INFINITE);
		CopyMemory(&msg, pcd->sharedMsg, sizeof(SharedMsg));
		ReleaseMutex(pcd->hRWMutex);

		_tprintf(TEXT("Received [%s]: %s\n"),msg.userName, msg.szMessage);

		Sleep(1000);
	}
	return 0;
}


void sendMsg(ControlData* pcd) {
	SharedMsg msg;
	TCHAR  infoBuf[MSGTEXT_SZ] = { '\0' };
	DWORD  bufCharCount = MSGTEXT_SZ;
	while (1) {
		_getts_s(msg.szMessage, MSGTEXT_SZ);

		bufCharCount = MSGTEXT_SZ;
		if (!GetUserName(infoBuf, &bufCharCount))
			_tprintf(TEXT("GetUserName"));

		_tcscpy_s(msg.userName, MSGTEXT_SZ, infoBuf);

		WaitForSingleObject(pcd->hRWMutex, INFINITE);
		CopyMemory(pcd->sharedMsg, &msg, sizeof(SharedMsg));	
		ReleaseMutex(pcd->hRWMutex);

		if (_tcsncmp(msg.szMessage, TEXT("exit"), 4) == 0) {
			pcd->threadMustContinue = 0;
		}
		
		SetEvent(pcd->hnewMsg);
		Sleep(500);
		ResetEvent(pcd->hnewMsg);
		
		if(!pcd->threadMustContinue) 
			break;
	}
}

int _tmain(int argc, LPTSTR argv[]) {
	ControlData cdata;
	HANDLE hThread;
	//DWORD threadId;

#ifdef UNICODE
	_setmode(_fileno(stdin), _O_WTEXT);
	_setmode(_fileno(stdout), _O_WTEXT);
#endif // UNICODE





	

	if (!initMemAndSync(&cdata)) {
		return 1;
	}
	cdata.threadMustContinue = 1;
	hThread = CreateThread(
		NULL,
		0,
		receiveMsg,
		&cdata,
		0,
		NULL
	);

	if (hThread == NULL) {
		_tprintf(TEXT("Error %d creating thread (CreateThread)\n"), GetLastError());
		CloseHandle(cdata.hnewMsg);
		CloseHandle(cdata.hRWMutex);
		UnmapViewOfFile(cdata.sharedMsg);
		CloseHandle(cdata.hMapFile);
		return 1;
	}

	sendMsg(&cdata);

	_tprintf(TEXT("Exiting...\n"));

	WaitForSingleObject(hThread, INFINITE);

	CloseHandle(hThread);
	CloseHandle(cdata.hnewMsg);
	CloseHandle(cdata.hRWMutex);
	UnmapViewOfFile(cdata.sharedMsg);
	CloseHandle(cdata.hMapFile);

	return 0;
}	
