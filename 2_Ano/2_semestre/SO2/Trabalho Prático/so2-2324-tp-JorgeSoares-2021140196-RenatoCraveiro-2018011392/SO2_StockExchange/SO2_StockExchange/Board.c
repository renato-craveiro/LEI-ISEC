#include "utils.h"

typedef struct {
	ControlData* cdata;
	int numCompaniesOnDisplay;
	int actualCompaniesN;
	Company* companies;
} BoardData;




BOOL initBrdMemAndSync(ControlData* cdata) {

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

	cdata->sharedMsg = (SharedMsg*)MapViewOfFile(
		cdata->hMapFile,
		FILE_MAP_ALL_ACCESS,
		0,
		0,
		MSFBUFSIZE
	);

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


BOOL getCompaniesFromFile(Company* c, BoardData* bd) {
	HANDLE hFile;
	DWORD dwBytesRead;
	TCHAR buffer[4096]; // Buffer to store read data

	hFile = CreateFile(
		COMPANIESFILE,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);

	if (hFile == INVALID_HANDLE_VALUE) {
		_tprintf(TEXT("[WARNING] Error opening file. Error code: %d\n"), GetLastError());
		return FALSE;
	}

	if (!ReadFile(hFile, buffer, sizeof(buffer), &dwBytesRead, NULL)) {
		_tprintf(TEXT("[WARNING] Error reading file. Error code: %d\n"), GetLastError());
		CloseHandle(hFile);
		return FALSE;
	}

	CloseHandle(hFile);

	buffer[dwBytesRead / sizeof(TCHAR)] = TEXT('\0');

	TCHAR* linesContext = NULL;
	TCHAR* tokenContext = NULL;
	TCHAR* lines = _tcstok_s(buffer, TEXT("\n"), &linesContext);
	int aux;

	for (int i = 0; lines != NULL; i++) {
		TCHAR* token = _tcstok_s(lines, TEXT(" "), &tokenContext);
		_tcscpy_s(c[i].name, MAX_TCHAR, token);
		token = _tcstok_s(NULL, TEXT(" "), &tokenContext);
		c[i].nShares = _tstoi(token);
		token = _tcstok_s(NULL, TEXT("\n"), &tokenContext);
		c[i].value = _tstof(token);
		lines = _tcstok_s(NULL, TEXT("\n"), &linesContext);
		aux = i;
	}

	bd->actualCompaniesN = aux + 1;

	if(bd->actualCompaniesN > bd->numCompaniesOnDisplay) 
		bd->actualCompaniesN = bd->numCompaniesOnDisplay;

	//reorder Companies by value
	
	Company auxC;
	for (int i = 0; i < bd->actualCompaniesN; i++) {
		for (int j = i + 1; j < bd->actualCompaniesN; j++) {
			if (c[i].value < c[j].value) {
				auxC = c[i];
				c[i] = c[j];
				c[j] = auxC;
			}
		}
	}

	

	//print ordered companies
	_tprintf(TEXT("\nTop Companies:\n\n"));
	for (int i = 0; i < bd->actualCompaniesN; i++) {
		_tprintf(TEXT("\t%s | %d stocks | %.2f $ each\n"), c[i].name, c[i].nShares, c[i].value);
	}

	return TRUE;
}





DWORD WINAPI receiveMsg(LPVOID p) {
	BoardData* bdata = (BoardData*)p;
	//ControlData* pcd = (ControlData*)p;
	SharedMsg msg;
	//while (1) {
	//	if (!pcd->threadMustContinue) return 0;

	//	// Wait for new message or termination signal
	//	HANDLE handles[2] = { pcd->hnewMsg, pcd->hExitEvent }; // Add termination event
	//	DWORD waitResult = WaitForMultipleObjects(2, handles, FALSE, INFINITE);

	//	if (waitResult == WAIT_OBJECT_0) { // New message received
	//		WaitForSingleObject(pcd->hRWMutex, INFINITE);
	//		CopyMemory(&msg, pcd->sharedMsg, sizeof(SharedMsg));
	//		ReleaseMutex(pcd->hRWMutex);

	//		_tprintf(TEXT("%s\n"), msg.szMessage);
	//		// Reset the event to signal that the message has been processed
	//		ResetEvent(pcd->hnewMsg);
	//	}
	//	else if (waitResult == WAIT_OBJECT_0 + 1) { // Termination signal received
	//		return 0;
	//	}
	//	else {
	//		_tprintf(TEXT("Error %d waiting for event (WaitForSingleObject)\n"), GetLastError());
	//		return 1;
	//	}
	//}

	while (bdata->cdata->threadMustContinue) {
		WaitForSingleObject(bdata->cdata->hnewMsg, INFINITE);

		WaitForSingleObject(bdata->cdata->hRWMutex, INFINITE);
		CopyMemory(&msg, bdata->cdata->sharedMsg, sizeof(SharedMsg));
		ReleaseMutex(bdata->cdata->hRWMutex);

		for(int i =0; i < bdata->actualCompaniesN; i++) {
			if (_tcscmp(msg.userName, bdata->companies[i].name) == 0) {
				_tprintf(TEXT("[TRANSACTION FROM %s] %s\n\n"), msg.userName, msg.szMessage);

				getCompaniesFromFile(bdata->companies, bdata);

				_tprintf(TEXT("\n"));
				break;
			}
		}

		if (_tcsncmp(msg.szMessage, TEXT("exit"), 4) == 0) {
			bdata->cdata->threadMustContinue = FALSE;
		}

		//_tprintf(TEXT("Received [%s]: %s\n"), msg.userName, msg.szMessage);
		ResetEvent(bdata->cdata->hnewMsg); // Make sure to reset the event

	}
	return 0;
}






int _tmain(int argc, LPTSTR argv[]) {
	HANDLE hThread;
	BoardData bdata;
	ControlData cdata;
    bdata.cdata = &cdata;
	Company companies[MAX_COMPANIES];
	//DWORD threadId;

#ifdef UNICODE
	_setmode(_fileno(stdin), _O_WTEXT);
	_setmode(_fileno(stdout), _O_WTEXT);
#endif // UNICODE


	if (!initBrdMemAndSync(&cdata)) {
		return 1;
	}

	// Check if there's at least one argument
	if (argc > 1) {
		// Convert the first argument to an integer
		bdata.numCompaniesOnDisplay = _ttoi(argv[1]);

		if (bdata.numCompaniesOnDisplay > MAX_COMPANIES) {
			bdata.numCompaniesOnDisplay = MAX_COMPANIES;
		}
	} else {
		bdata.numCompaniesOnDisplay = MAX_COMPANIES;
	}

	cdata.threadMustContinue = TRUE;

	cdata.hExitEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (cdata.hExitEvent == NULL) {
		_tprintf(TEXT("[Error %d creating event (CreateEvent])\n"), GetLastError());
		// ... (close handles and exit) 
		CloseHandle(cdata.hnewMsg);
		CloseHandle(cdata.hRWMutex);
		UnmapViewOfFile(cdata.sharedMsg);
		CloseHandle(cdata.hMapFile);

		return 1;
	}
	hThread = CreateThread(
		NULL,
		0,
		receiveMsg,
		&bdata,
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
	_tprintf(TEXT("[Welcome to SO2 Stock Exchange Board]\n"));
	bdata.companies = companies;
	if (getCompaniesFromFile(companies, &bdata)) {
		// _tprintf(TEXT("[Companies loaded successfully]\n"));
		_tprintf(TEXT("\n\n"));
	}
	else {
		_tprintf(TEXT("[Error loading companies]\n"));
		return 1;
	}


	// Signal the thread to exit
	//SetEvent(cdata.hExitEvent);
	

	//_tprintf(TEXT("Exiting...\n"));

	WaitForSingleObject(hThread, INFINITE);

	CloseHandle(hThread);
	CloseHandle(cdata.hnewMsg);
	CloseHandle(cdata.hRWMutex);
	UnmapViewOfFile(cdata.sharedMsg);
	CloseHandle(cdata.hMapFile);

	return 0;
}