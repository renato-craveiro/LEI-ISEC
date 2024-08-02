#include "utils.h"


typedef struct _GlobalData {
    ControlData* cdata; // Shared memory and synchronization objects
    Company* companies;
    User* users;
    int nCompanies;
    int nUsers;
    HANDLE hSemaphore; // Semaphore to limit the number of concurrent users
    HANDLE hMutex;     // Mutex for synchronization
    BOOL onPause;       // Flag to pause stock transactions
    HANDLE hPipe;      // Temporary Pipe handle for testing
    int nSecondsPause;
    int activeUsers;   // Current number of active users
    HANDLE hPauseThread; // Handle to the pause thread
    HANDLE hShm;
    HANDLE hWaitableTimer; // Handle to the waitable timer
    HANDLE hMutexPipe; // Mutex to control access to the pipe
    TCHAR latestTransaction[MSGTEXT_SZ]; // Latest transaction data
    TCHAR* lastCompanyTrName; // Last company transaction name
} GlobalData;


typedef struct _UserWithGlobalData {
    User* user;
    GlobalData* gd;
} UserWithGlobalData;


DWORD WINAPI ClientSessionThread(LPVOID lpParam);
TCHAR* listCompanies(GlobalData* gd);
TCHAR* userBuys(UserWithGlobalData* ugd, TCHAR* companyName, int numShares);
TCHAR* userSells(UserWithGlobalData* ugd, TCHAR* companyName, int numShares);
void updateStockValueDif(Company* c, float difPerc, BOOL raise, HANDLE* hMutex, TCHAR* latestTransaction, ControlData* cd);
void saveCompaniesToFile(Company* c, int nCompanies);
void sendData(ControlData* pcd, TCHAR* toSend);
void sendInfoNewBoard(GlobalData* gd);

DWORD WINAPI ClientSessionThread(LPVOID lpParam) {
    UserWithGlobalData* ugd = (UserWithGlobalData*)lpParam;
    HANDLE hPipe = ugd->user->hPipe;
    TCHAR buffer[BUFFER_SIZE];
    DWORD bytesRead, bytesWritten;
    BOOL success;

    // _tprintf(TEXT("[DEBUG] ClientSessionThread started for user: %s\n"), ugd->user->name);

    while (ugd->gd->cdata->threadMustContinue) {
        success = ReadFile(hPipe, buffer, BUFFER_SIZE, &bytesRead, NULL);
        if (!success || bytesRead == 0) {
            // _tprintf(TEXT("[DEBUG] User %s disconnected or error occurred. GLE=%d\n"), ugd->user->name, GetLastError());
            break;  // Handle disconnect
        }

        // Null-terminate the string received
        buffer[bytesRead / sizeof(TCHAR)] = '\0';
        // _tprintf(TEXT("[DEBUG] User %s sent: %s\n"), ugd->user->name, buffer);

        // Parse command
        TCHAR* token;
        TCHAR* context = NULL;
        token = _tcstok_s(buffer, TEXT(" "), &context);

        if (token == NULL) continue;

        if (_tcscmp(token, TEXT("exit")) == 0) {
            const TCHAR* msg = TEXT("[SERVER]: Exiting...");
            ugd->user->online = FALSE;
            WriteFile(hPipe, msg, (lstrlen(msg) + 1) * sizeof(TCHAR), &bytesWritten, NULL);
            break;
        } else if (_tcscmp(token, TEXT("login")) == 0) {
            TCHAR* username = _tcstok_s(NULL, TEXT(" "), &context);
            TCHAR* password = _tcstok_s(NULL, TEXT(" "), &context);
            if (username && password) {
                // Handle login logic
                const TCHAR* msg = TEXT("[SERVER]: Login successful.");
                WriteFile(hPipe, msg, (lstrlen(msg) + 1) * sizeof(TCHAR), &bytesWritten, NULL);
            } else {
                const TCHAR* msg = TEXT("[SERVER]: Invalid login command format.");
                WriteFile(hPipe, msg, (lstrlen(msg) + 1) * sizeof(TCHAR), &bytesWritten, NULL);
            }
        } else if (_tcscmp(token, TEXT("listc")) == 0) {
            // Handle list companies logic
            TCHAR* companyList = listCompanies(ugd->gd);
            TCHAR buffer[BUFFER_SIZE * 10];
            // Prepend "[SERVER]: " to the companyList string
            _stprintf_s(buffer, BUFFER_SIZE * 10, TEXT("[SERVER]: %s"), companyList);

            WriteFile(hPipe, buffer, (lstrlen(buffer) + 1) * sizeof(TCHAR), &bytesWritten, NULL);
        } else if (_tcscmp(token, TEXT("buy")) == 0) {
            TCHAR* companyName = _tcstok_s(NULL, TEXT(" "), &context);
            TCHAR* shares = _tcstok_s(NULL, TEXT(" "), &context);
            if (companyName && shares) {
                int numShares = _tstoi(shares);
                if(ugd->gd->onPause){
					const TCHAR* msg = TEXT("[SERVER]: Stock exchange is paused. No operation has been made.");
					WriteFile(hPipe, msg, (lstrlen(msg) + 1) * sizeof(TCHAR), &bytesWritten, NULL);
					continue;
				}
                // Handle buy logic
                const TCHAR* msg = userBuys(ugd, companyName, numShares);

                WriteFile(hPipe, msg, (lstrlen(msg) + 1) * sizeof(TCHAR), &bytesWritten, NULL);

                saveCompaniesToFile(ugd->gd->companies, ugd->gd->nCompanies);

            } else {
                const TCHAR* msg = TEXT("[SERVER]: Invalid buy command format.");
                WriteFile(hPipe, msg, (lstrlen(msg) + 1) * sizeof(TCHAR), &bytesWritten, NULL);
            }
        } else if (_tcscmp(token, TEXT("sell")) == 0) {
            TCHAR* companyName = _tcstok_s(NULL, TEXT(" "), &context);
            TCHAR* shares = _tcstok_s(NULL, TEXT(" "), &context);
            if (companyName && shares) {
                if (ugd->gd->onPause) {
                    const TCHAR* msg = TEXT("[SERVER]: Stock exchange is paused. No operation has been made.");
                    WriteFile(hPipe, msg, (lstrlen(msg) + 1) * sizeof(TCHAR), &bytesWritten, NULL);
                    continue;
                }
                int numShares = _tstoi(shares);
                // Handle sell logic
                const TCHAR* msg = userSells(ugd, companyName, numShares);
                WriteFile(hPipe, msg, (lstrlen(msg) + 1) * sizeof(TCHAR), &bytesWritten, NULL);
                saveCompaniesToFile(ugd->gd->companies, ugd->gd->nCompanies);

            } else {
                const TCHAR* msg = TEXT("[SERVER]: Invalid sell command format.");
                WriteFile(hPipe, msg, (lstrlen(msg) + 1) * sizeof(TCHAR), &bytesWritten, NULL);
            }
        } else if (_tcscmp(token, TEXT("balance")) == 0) {
            // Handle balance logic
            TCHAR msg[BUFFER_SIZE];
            _stprintf_s(msg, BUFFER_SIZE, TEXT("[SERVER]: Here is your balance: %.2f$"), ugd->user->balance);

            if(ugd->user->nCompanies > 0)
            {
                _tcscat_s(msg, BUFFER_SIZE, TEXT("\n\tYour companies: \n"));
                for (int i = 0; i < ugd->user->nCompanies; i++)
                {
                    _stprintf_s(msg, BUFFER_SIZE, TEXT("%s\n\t\t%s: %d shares\n"), msg, ugd->user->companies[i].name, ugd->user->companies[i].nShares);
                }
            }

            WriteFile(hPipe, msg, (lstrlen(msg) + 1) * sizeof(TCHAR), &bytesWritten, NULL);
        } else {
            const TCHAR* msg = TEXT("[SERVER]: Unknown command.");
            WriteFile(hPipe, msg, (lstrlen(msg) + 1) * sizeof(TCHAR), &bytesWritten, NULL);
        }
    }

    // Release semaphore slot
    ReleaseSemaphore(ugd->gd->hSemaphore, 1, NULL);

    if (hPipe != INVALID_HANDLE_VALUE && hPipe != NULL) {
        DisconnectNamedPipe(hPipe);
        CloseHandle(hPipe);
        hPipe = NULL;
    }

    // _tprintf(TEXT("[DEBUG] ClientSessionThread ended for user: %s\n"), ugd->user->name);
    return 0;
}


User* AuthenticateUser(TCHAR* credentials, User* users, int userCount) {
    TCHAR* token;
    TCHAR* context = NULL;
    TCHAR receivedName[MAX_TCHAR];
    TCHAR receivedPassword[MAX_TCHAR];

    // Split credentials
    token = _tcstok_s(credentials, TEXT(" "), &context);
    if (token == NULL) return NULL;
    _tcscpy_s(receivedName, MAX_TCHAR, token);

    token = _tcstok_s(NULL, TEXT(" "), &context);
    if (token == NULL) return NULL;
    _tcscpy_s(receivedPassword, MAX_TCHAR, token);

    // Authenticate against user list
    for (int i = 0; i < userCount; i++) {
        if (_tcscmp(users[i].name, receivedName) == 0 && _tcscmp(users[i].password, receivedPassword) == 0) {
            users[i].online = TRUE; // Mark user as online
            //users[i].nCompanies = 0; // Reset user's companies
            
            // _tprintf(TEXT("[DEBUG] User %s authenticated successfully.\n"), users[i].name);
            return &users[i];  // Credentials match
        }
    }

    return NULL;  // No match found
}

DWORD WINAPI ConnectionHandlerThread(LPVOID lpParam) {
    GlobalData* gd = (GlobalData*)lpParam;
    HANDLE hPipe;
    DWORD threadID;
    TCHAR credentials[1024];  // Assuming credentials are sent as a string
    DWORD bytesRead;

    OVERLAPPED olConnect = {0};
    olConnect.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

    if (olConnect.hEvent == NULL) {
        _tprintf(TEXT("CreateEvent failed with GLE=%d\n"), GetLastError());
        return 1;
    }

    while (gd->cdata->threadMustContinue) {
        hPipe = CreateNamedPipe(
            MY_STOCK_PIPE,
            PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED,
            PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
            PIPE_UNLIMITED_INSTANCES,
            BUFFER_SIZE,
            BUFFER_SIZE,
            0, NULL);

        if (hPipe == INVALID_HANDLE_VALUE) {
            _tprintf(TEXT("Failed to create pipe instance: %d\n"), GetLastError());
            continue;
        }

        if (ConnectNamedPipe(hPipe, &olConnect) == 0) {
            DWORD lastError = GetLastError();
            if (lastError == ERROR_IO_PENDING) {
                WaitForSingleObject(olConnect.hEvent, INFINITE);
            } else if (lastError == ERROR_PIPE_CONNECTED) {
                // Client connected before we could call ConnectNamedPipe
                // This is not an error
                ResetEvent(olConnect.hEvent);
            } else {
                _tprintf(TEXT("ConnectNamedPipe failed with GLE=%d\n"), lastError);
                CloseHandle(hPipe);
                continue;
            }
        }

        // At this point, the pipe is connected
        WaitForSingleObject(gd->hMutexPipe, INFINITE);

        OVERLAPPED olRead = {0};
        olRead.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
        if (olRead.hEvent == NULL) {
            _tprintf(TEXT("CreateEvent failed with GLE=%d\n"), GetLastError());
            CloseHandle(hPipe);
            ReleaseMutex(gd->hMutexPipe);
            continue;
        }

        if (!ReadFile(hPipe, credentials, sizeof(credentials), &bytesRead, &olRead)) {
            DWORD lastError = GetLastError();
            if (lastError == ERROR_IO_PENDING) {
                WaitForSingleObject(olRead.hEvent, INFINITE);
                if (!GetOverlappedResult(hPipe, &olRead, &bytesRead, FALSE)) {
                    _tprintf(TEXT("ReadFile failed with GLE=%d\n"), GetLastError());
                    DisconnectNamedPipe(hPipe);
                    CloseHandle(hPipe);
                    CloseHandle(olRead.hEvent);
                    ReleaseMutex(gd->hMutexPipe); // Release mutex
                    continue;
                }
            } else {
                _tprintf(TEXT("ReadFile failed with GLE=%d\n"), lastError);
                DisconnectNamedPipe(hPipe);
                CloseHandle(hPipe);
                CloseHandle(olRead.hEvent);
                ReleaseMutex(gd->hMutexPipe); // Release mutex
                continue;
            }
        }

        credentials[bytesRead / sizeof(TCHAR)] = '\0';
        DWORD bytesWritten;

        if (WaitForSingleObject(gd->hSemaphore, INFINITE) == WAIT_OBJECT_0) {
            const TCHAR* allowMessage = LOGIN_ALLOWED;
            WriteFile(hPipe, allowMessage, (lstrlen(allowMessage) + 1) * sizeof(TCHAR), &bytesWritten, NULL);

            if (!ReadFile(hPipe, credentials, sizeof(credentials), &bytesRead, &olRead)) {
                DWORD lastError = GetLastError();
                if (lastError == ERROR_IO_PENDING) {
                    WaitForSingleObject(olRead.hEvent, INFINITE);
                    if (!GetOverlappedResult(hPipe, &olRead, &bytesRead, FALSE)) {
                        _tprintf(TEXT("ReadFile failed with GLE=%d\n"), GetLastError());
                        ReleaseSemaphore(gd->hSemaphore, 1, NULL);
                        DisconnectNamedPipe(hPipe);
                        CloseHandle(hPipe);
                        CloseHandle(olRead.hEvent);
                        ReleaseMutex(gd->hMutexPipe); // Release mutex
                        continue;
                    }
                } else {
                    ReleaseSemaphore(gd->hSemaphore, 1, NULL);
                    DisconnectNamedPipe(hPipe);
                    CloseHandle(hPipe);
                    CloseHandle(olRead.hEvent);
                    ReleaseMutex(gd->hMutexPipe); // Release mutex
                    continue;
                }
            }

            credentials[bytesRead / sizeof(TCHAR)] = '\0';
            User* incomingUser = AuthenticateUser(credentials, gd->users, gd->nUsers);

            if (incomingUser) {
                incomingUser->hPipe = hPipe;
                UserWithGlobalData* ugd = (UserWithGlobalData*)malloc(sizeof(UserWithGlobalData));  // Allocate memory on heap
                if (ugd) {
                    ugd->user = incomingUser;
                    ugd->gd = gd;

                    const TCHAR* acceptedMessage = LOGIN_SUCCESS;
                    WriteFile(hPipe, acceptedMessage, (lstrlen(acceptedMessage) + 1) * sizeof(TCHAR), &bytesWritten, NULL);

                    CreateThread(NULL, 0, ClientSessionThread, ugd, 0, &threadID);
                } else {
                    _tprintf(TEXT("Memory allocation failed for user data.\n"));
                    ReleaseSemaphore(gd->hSemaphore, 1, NULL);
                    DisconnectNamedPipe(hPipe);
                    CloseHandle(hPipe);
                    CloseHandle(olRead.hEvent);
                    ReleaseMutex(gd->hMutexPipe); // Release mutex
                }
            } else {
                const TCHAR* authFailMsg = LOGIN_FAIL;
                WriteFile(hPipe, authFailMsg, (lstrlen(authFailMsg) + 1) * sizeof(TCHAR), &bytesWritten, NULL);
                ReleaseSemaphore(gd->hSemaphore, 1, NULL);
                DisconnectNamedPipe(hPipe);
                CloseHandle(hPipe);
                CloseHandle(olRead.hEvent);
                ReleaseMutex(gd->hMutexPipe); // Release mutex
            }
        } else {
            const TCHAR* waitMessage = LOGIN_WAIT;
            WriteFile(hPipe, waitMessage, (lstrlen(waitMessage) + 1) * sizeof(TCHAR), &bytesWritten, NULL);
            DisconnectNamedPipe(hPipe);
            CloseHandle(hPipe);
            CloseHandle(olRead.hEvent);
            ReleaseMutex(gd->hMutexPipe); // Release mutex
        }
    }

    CloseHandle(olConnect.hEvent);
    return 0;
}

void clearScreen() {
	COORD topLeft = { 0, 0 };
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO screen;
	DWORD written;

	GetConsoleScreenBufferInfo(consoleHandle, &screen);
	FillConsoleOutputCharacter(consoleHandle, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written);
	FillConsoleOutputAttribute(consoleHandle, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE,
		screen.dwSize.X * screen.dwSize.Y, topLeft, &written);
	SetConsoleCursorPosition(consoleHandle, topLeft);
}

TCHAR* listCompanies(GlobalData* gd) {
    static TCHAR buffer[BUFFER_SIZE * 10]; // Adjust size as needed
    TCHAR temp[BUFFER_SIZE];

    _tcscpy_s(buffer, BUFFER_SIZE * 10, TEXT("Company List:\n"));
    WaitForSingleObject(gd->cdata->hRWMutex, INFINITE);
    for (int i = 0; i < gd->nCompanies; i++) {
        _stprintf_s(temp, BUFFER_SIZE, TEXT("\nName: %s\n\tShare Count: %d\n\tShare Value: %.2f $\n"),
            gd->companies[i].name,
            gd->companies[i].nShares,
            gd->companies[i].value);
        _tcscat_s(buffer, BUFFER_SIZE * 10, temp);
    }
    ReleaseMutex(gd->cdata->hRWMutex);

    return buffer;
}


void listUsers(GlobalData* gd) {
	_tprintf(TEXT("User List:\n"));
	WaitForSingleObject(gd->cdata->hRWMutex, INFINITE);
	for (int i = 0; i < gd->nUsers; i++) {
		_tprintf(TEXT("\nName: %s\n"), gd->users[i].name);
		_tprintf(TEXT("\tPassword: "));
		for(int j = 0; j < _tcslen(gd->users[i].password); j++)
			_tprintf(TEXT("*"));

		_tprintf(TEXT("\n\tBalance: %.2f $\n"), gd->users[i].balance);

		if (gd->users[i].online)
			_tprintf(TEXT("\tOnline\n"));
		else
			_tprintf(TEXT("\tOffline\n"));
	}
	ReleaseMutex(gd->cdata->hRWMutex);
}

BOOL addCompany(GlobalData*gd, TCHAR* name, int nShares, float value) {
	
	WaitForSingleObject(gd->cdata->hRWMutex, INFINITE);
	Company* companies = gd->companies;
	int nCompanies = gd->nCompanies;

	if(nCompanies>=MAX_COMPANIES){
		ReleaseMutex(gd->cdata->hRWMutex);
		_tprintf(TEXT("[WARNING] Maximum number of companies reached.\n"));
		return FALSE;
	}
	
	if (nCompanies >= MAX_COMPANIES) {
		ReleaseMutex(gd->cdata->hRWMutex);
		_tprintf(TEXT("[WARNING] Maximum number of companies reached.\n"));
		return FALSE;
	}
	for(int i = 0; i < nCompanies; i++){
		
		if(_tcscmp(companies[i].name, name) == 0){
			ReleaseMutex(gd->cdata->hRWMutex);
			_tprintf(TEXT("[ERROR] Company %s already exists!\n"), name);
			return FALSE;
		}
	}
	_tcscpy_s(companies[nCompanies].name, MAX_TCHAR, name);
	companies[nCompanies].nShares = nShares;
	companies[nCompanies].value = value;
	gd->nCompanies++;
	ReleaseMutex(gd->cdata->hRWMutex);

	return TRUE;
}

BOOL updateStockValue(GlobalData* gd, TCHAR* name, float value) {
	WaitForSingleObject(gd->cdata->hRWMutex, INFINITE);
	Company* companies = gd->companies;
	int nCompanies = gd->nCompanies;

	for (int i = 0; i < nCompanies; i++) {
		if (_tcscmp(companies[i].name, name) == 0) {
			companies[i].value = value;
			ReleaseMutex(gd->cdata->hRWMutex);
			return TRUE;
		}
	}
	ReleaseMutex(gd->cdata->hRWMutex);

    _tprintf(TEXT("[ERROR] Company %s not found!\n"), name);
    return FALSE;
}

VOID CALLBACK TimerAPCProc(LPVOID lpArg, DWORD dwTimerLowValue,DWORD dwTimerHighValue){

	GlobalData* gd =(GlobalData*)lpArg;
	// Formal parameters not used in this example.
	UNREFERENCED_PARAMETER(dwTimerLowValue);
	UNREFERENCED_PARAMETER(dwTimerHighValue);

	// _tprintf(TEXT("\n[INFO] Pause ended\n\n"));
	gd->onPause = FALSE;


}

DWORD WINAPI ThreadPause(LPVOID lpParam) {

	GlobalData *gd = (GlobalData*)lpParam;

	//int* gd = (int*)lpParam;
	int sec = gd->nSecondsPause;

	HANDLE          hTimer;
	BOOL            bSuccess;
	__int64         qwDueTime;
	LARGE_INTEGER   liDueTime;

	hTimer = CreateWaitableTimer(
		NULL,                   // Default security attributes
		FALSE,                  // Create auto-reset timer
		TEXT("MyTimer"));       // Name of waitable timer
	if (hTimer != NULL)
	{
		__try
		{
			gd->onPause = TRUE;
			// Create an integer that will be used to signal the timer 
			// sec seconds from now.
			qwDueTime = -1 * (sec) * _SECOND;

			// Copy the relative time into a LARGE_INTEGER.
			liDueTime.LowPart = (DWORD)(qwDueTime & 0xFFFFFFFF);
			liDueTime.HighPart = (LONG)(qwDueTime >> 32);

			bSuccess = SetWaitableTimer(
				hTimer,           // Handle to the timer object
				&liDueTime,       // When timer will become signaled
				0,//2000,             // Periodic timer interval of 2 seconds
				TimerAPCProc,     // Completion routine
				gd,//&MyData,          // Argument to the completion routine
				FALSE);          // Do not restore a suspended system

			if (bSuccess)
			{
					SleepEx(INFINITE,     // Wait forever
						TRUE);       // Put thread in an alertable state
			}
			else
			{
				printf("SetWaitableTimer failed with error %d\n", GetLastError());
			}

		}
		__finally
		{
			CloseHandle(hTimer);
		}
	}
	else
	{
		printf("CreateWaitableTimer failed with error %d\n", GetLastError());
	}

	return 0;


}

BOOL pauseStockExchg(DWORD nSeconds, GlobalData *gd){
	int sec = nSeconds;
	gd->nSecondsPause = sec;


	//_tprintf(TEXT("[DEBUG] stock exchange PAUSED for %d seconds\n"), sec);
	gd->hPauseThread = CreateThread(NULL, 0, ThreadPause, gd, 0, NULL);
	if (gd->hPauseThread == NULL) {
		fprintf(stderr, "[ERROR] Failed to create pause thread\n");
		return FALSE;
	}
	return TRUE;


}


void callBolsafunction(TCHAR* command, GlobalData* gd) {
    //_tprintf(TEXT("Command received: %s\n"), command);

    if ((_tcscmp(command, TEXT("shutdown")) == 0) || (_tcscmp(command, TEXT("close")) == 0)) {
        _tprintf(TEXT("[WARNING] Server shutting down...\n"));

        // Notify all clients to exit
        WaitForSingleObject(gd->hMutex, INFINITE);  // Synchronize access to gd->users

        for (int i = 0; i < gd->nUsers; i++) {
            if (gd->users[i].online) {
                DWORD bytesWritten;
                const TCHAR* exitMsg = TEXT("exit");

                ControlData* cdata = gd->cdata;
                sendData(cdata, exitMsg, TEXT("SERVER"));
                CancelWaitableTimer(gd->hWaitableTimer);
                gd->onPause = FALSE;

                if (gd->users[i].hPipe != INVALID_HANDLE_VALUE && gd->users[i].hPipe != NULL) {
                    if (!WriteFile(gd->users[i].hPipe, exitMsg, (lstrlen(exitMsg) + 1) * sizeof(TCHAR), &bytesWritten, NULL)) {
                        _tprintf(TEXT("WriteFile failed for user %s with GLE=%d\n"), gd->users[i].name, GetLastError());
                    }
                } else {
                    _tprintf(TEXT("Invalid handle for user %s\n"), gd->users[i].name);
                }

                gd->users[i].online = FALSE;
            }
        }

        ReleaseMutex(gd->hMutex);  // Release the mutex after updating gd->users

        // Signal the exit event to shut down the server
        gd->cdata->threadMustContinue = FALSE;
        SetEvent(gd->cdata->hExitEvent);

        system("pause");
        exit(0);
    } else if (_tcscmp(command, TEXT("listc")) == 0) {
        TCHAR* companyList = listCompanies(gd);
        ControlData* cdata = gd->cdata;
        sendData(cdata, companyList, TEXT("listc"));

        _tprintf(TEXT("%s\n"), companyList);
    } else if (_tcscmp(command, TEXT("users")) == 0) {
        listUsers(gd);
    } else if (_tcsncmp(command, TEXT("addc"), 4) == 0) {
        TCHAR* token, * nextToken = NULL;
        TCHAR name[MAX_TCHAR];
        int nShares;
        float value;

        int count = 0;
        for (int i = 0; i < _tcslen(command); i++) {
            if (command[i] == ' ') count++;
        }

        if (count < 3) {
            _tprintf(TEXT("[ERROR] Invalid number of arguments!\n(addc [name - String] [n_stock - Integer] [value_per_stock - float]\n"));
            return;
        }

        token = _tcstok_s(command, TEXT(" "), &nextToken);
        token = _tcstok_s(NULL, TEXT(" "), &nextToken);
        _tcscpy_s(name, MAX_TCHAR, token);
        token = _tcstok_s(NULL, TEXT(" "), &nextToken);

        if (!isdigit(token[0])) {
            _tprintf(TEXT("[ERROR] Invalid number of shares!\n"));
            return;
        }
        nShares = _tstoi(token);
        token = _tcstok_s(NULL, TEXT(" "), &nextToken);
        if (!isdigit(token[0])) {
            _tprintf(TEXT("[ERROR] Invalid value per stock!\n"));
            return;
        }
        value = _tstof(token);

        if (addCompany(gd, name, nShares, value)) {
            _tprintf(TEXT("[SUCCESS] Company %s added successfully!\n"), name);
            saveCompaniesToFile(gd->companies, gd->nCompanies);
        } else {
            _tprintf(TEXT("[ERROR] Company %s could not be added!\n"), name);
        }
    } else if (_tcsncmp(command, TEXT("stock"), 5) == 0) {
        TCHAR* token, * nextToken = NULL;
        TCHAR name[MAX_TCHAR];
        float value;

        int count = 0;
        for (int i = 0; i < _tcslen(command); i++) {
            if (command[i] == ' ') count++;
        }

        if (count < 2) {
            _tprintf(TEXT("[ERROR] Invalid number of arguments!\n(stock [name - String] [value - float]\n"));
            return;
        }

        token = _tcstok_s(command, TEXT(" "), &nextToken);
        token = _tcstok_s(NULL, TEXT(" "), &nextToken);
        _tcscpy_s(name, MAX_TCHAR, token);
        token = _tcstok_s(NULL, TEXT(" "), &nextToken);

        if (!isdigit(token[0])) {
            _tprintf(TEXT("[ERROR] Invalid value!\n"));
            return;
        }
        value = _tstof(token);

        if (updateStockValue(gd, name, value)) {
            _tprintf(TEXT("[SUCCESS] Stock value for %s updated successfully!\n"), name);
        } else {
            _tprintf(TEXT("[ERROR] Stock value for %s could not be updated!\n"), name);
        }
    } else if (_tcsncmp(command, TEXT("pause"), 5) == 0) {
        if (gd->onPause) {
            _tprintf(TEXT("[ERROR] Stock exchange is already paused!\n"));
            return;
        }

        TCHAR* token, * nextToken = NULL;
        TCHAR name[MAX_TCHAR];
        int value;

        int count = 0;
        for (int i = 0; i < _tcslen(command); i++) {
            if (command[i] == ' ') count++;
        }

        if (count < 1) {
            _tprintf(TEXT("[ERROR] Invalid number of arguments!\n(pause [n_seconds - Integer]\n"));
            return;
        }

        token = _tcstok_s(command, TEXT(" "), &nextToken);
        token = _tcstok_s(NULL, TEXT(" "), &nextToken);

        if (!isdigit(token[0])) {
            _tprintf(TEXT("[ERROR] Invalid value!\n"));
            return;
        }
        value = _tstof(token);

        if (pauseStockExchg(value, gd)) {
            _tprintf(TEXT("[PAUSED] Stock exchange paused for %d seconds!\n"), value);
        } else {
            _tprintf(TEXT("[ERROR] Stock exchange could not be paused!\n"));
        }
    } else if (_tcscmp(command, TEXT("help")) == 0) {
        _tprintf(TEXT("Commands list:\n"));
        _tprintf(TEXT("\tlistc - List companies\n\t\tusage: listc\n"));
        _tprintf(TEXT("\tusers - List users\n\t\tusage: users\n"));
        _tprintf(TEXT("\taddc - Add company\n\t\tusage: [name - String] [n_stock - Integer] [value_per_stock - float]\n"));
        _tprintf(TEXT("\tstock - Stock value\n\t\tusage: stock [name - String] [value - float]\n"));
        _tprintf(TEXT("\tclose - Close server\n"));
    } else if ((_tcscmp(command, TEXT("clc")) == 0) || (_tcscmp(command, TEXT("cls")) == 0) || (_tcscmp(command, TEXT("clear")) == 0)) {
        clearScreen();
    } else {
        _tprintf(TEXT("Invalid command\n"));
    }
}

/*

Server commands loginc:

DONE:

	ok = addc - add company - addc [name] [nShares] [value]
	ok = listc - list companies - listc
	ok = stock - stock value - stock [name] [value]
	ok = users - list users - users
	working = pause - pause stock transactions - pause [time_seconds] 
	working = MUTEXES applied (not sure if the best practice)

NOT DONE:
close - close server & notify clients + boards - close


*/


DWORD WINAPI th_kb(LPVOID lpParam) {


    TCHAR command[MSGTEXT_SZ];
    GlobalData* gd = (GlobalData*)lpParam;

	

	_tprintf(TEXT("\n---Server Admin Console---\n\n\ttype 'help' for all the commands.\n\n"));
	DWORD dwWaitResult;
	gd->hWaitableTimer= CreateWaitableTimer(NULL, FALSE, NULL);
	if (gd->hWaitableTimer == NULL) {
		_ftprintf(stderr, TEXT("Error creating waitable timer: %d\n"), GetLastError());
		return 1;
	}

    while (gd->cdata->threadMustContinue) {

        _tprintf(TEXT("\n> "));
        _fgetts(command, MSGTEXT_SZ, stdin);
        command[_tcslen(command) - 1] = '\0';


        callBolsafunction(command, gd);
	}
	return 0;
}


DWORD WINAPI th_board(LPVOID lpParam) {
    // Monitor and update shared memory for board
    // while (gd->cdata->threadMustContinue) {
    //     // Analyze company data and update shared memory
    //     Sleep(1000); // Simulate work
    // }
    return 0;
}


// Function to read the NCLIENTES value from the registry
DWORD GetMaxClientsFromRegistry() {
    HKEY hKey;
    DWORD maxClients = MAX_USERS_ONLINE;
    DWORD dataSize = sizeof(DWORD);
    DWORD valueType;

    if (RegOpenKeyEx(HKEY_CURRENT_USER, REGISTRY_NCLIENTES_KEY, 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        if (RegQueryValueEx(hKey, REGISTRY_NCLIENTES_VALUE, NULL, &valueType, (LPBYTE)&maxClients, &dataSize) == ERROR_SUCCESS) {
            if (valueType != REG_DWORD) {
                maxClients = MAX_USERS_ONLINE;
            }
        }
        RegCloseKey(hKey);
    }

    return maxClients;
}

BOOL initMemAndSync(GlobalData* gd) {
    gd->cdata->hMapFile = CreateFileMapping(
        INVALID_HANDLE_VALUE,
        NULL,
        PAGE_READWRITE,
        0,
        MSFBUFSIZE,
        SHM_NAME
    );

    if (gd->cdata->hMapFile == NULL) {
        _tprintf(TEXT("Error %d opening file (CreateFileMapping)\n"), GetLastError());
        return FALSE;
    }

    gd->cdata->sharedMsg = (SharedMsg*)MapViewOfFile(
        gd->cdata->hMapFile,
        FILE_MAP_ALL_ACCESS,
        0,
        0,
        MSFBUFSIZE
    );

    if (gd->cdata->sharedMsg == NULL) {
        _tprintf(TEXT("Error %d opening file (MapViewOfFile)\n"), GetLastError());
        CloseHandle(gd->cdata->hMapFile);
        return FALSE;
    }

    gd->cdata->hRWMutex = CreateMutex(
        NULL,
        FALSE,
        MUTEXT_NAME
    );

    if (gd->cdata->hRWMutex == NULL) {
        _tprintf(TEXT("Error %d creating mutex (CreateMutex)\n"), GetLastError());
        UnmapViewOfFile(gd->cdata->sharedMsg);
        CloseHandle(gd->cdata->hMapFile);
        return FALSE;
    }

    gd->cdata->hnewMsg = CreateEvent(
        NULL,
        TRUE,
        FALSE,
        EVENT_NAME
    );

    if (gd->cdata->hnewMsg == NULL) {
        _tprintf(TEXT("Error %d creating event (CreateEvent)\n"), GetLastError());
        UnmapViewOfFile(gd->cdata->sharedMsg);
        CloseHandle(gd->cdata->hMapFile);
        CloseHandle(gd->cdata->hRWMutex);
        return FALSE;
    }

    DWORD maxClients = GetMaxClientsFromRegistry();
    gd->hSemaphore = CreateSemaphore(NULL, maxClients, maxClients, NULL);
    if (!gd->hSemaphore) return FALSE;

    gd->hMutex = CreateMutex(NULL, FALSE, NULL);
    if (!gd->hMutex) return FALSE;

    gd->nUsers = 0;

    return TRUE;
}


BOOL getUsersFromFile(User* u, GlobalData* gd) {
    HANDLE hFile;
    DWORD dwBytesRead;
    TCHAR buffer[4096]; // Buffer to store read data

    hFile = CreateFile(
        USERSFILE,
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
        _tprintf(TEXT("Error reading file. Error code: %d\n"), GetLastError());
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
        _tcscpy_s(u[i].name, MAX_TCHAR, token);

        token = _tcstok_s(NULL, TEXT(" "), &tokenContext);
        _tcscpy_s(u[i].password, MAX_TCHAR, token);

        token = _tcstok_s(NULL, TEXT(" "), &tokenContext);
        u[i].balance = _tstof(token);

        u[i].online = FALSE;

        lines = _tcstok_s(NULL, TEXT("\n"), &linesContext);
        aux = i;
    }
    gd->nUsers = aux + 1;

    return TRUE;
}

BOOL getCompaniesFromFile(Company* c, GlobalData* gd) {
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

    gd->nCompanies = aux + 1;

    return TRUE;
}





DWORD WINAPI receiveMsg(LPVOID p) {
	GlobalData* gd = (GlobalData*)p;
	SharedMsg msg;
	while (1) {
		WaitForSingleObject(gd->cdata->hnewMsg, INFINITE);
		if (!gd->cdata->threadMustContinue) return 0;

		WaitForSingleObject(gd->cdata->hRWMutex, INFINITE);
		CopyMemory(&msg, gd->cdata->sharedMsg, sizeof(SharedMsg));
		ReleaseMutex(gd->cdata->hRWMutex);

        if(_tcsncmp(msg.szMessage, TEXT("new_board"), 4) == 0){
            sendInfoNewBoard(gd);
		}

		_tprintf(TEXT("Received [%s]: %s\n"), msg.userName, msg.szMessage);
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
		/*if (!GetUserName(infoBuf, &bufCharCount))
			_tprintf(TEXT("GetUserName"));*/

		_tcscpy_s(msg.userName, MSGTEXT_SZ, TEXT("Bolsa"));

		WaitForSingleObject(pcd->hRWMutex, INFINITE);
		CopyMemory(pcd->sharedMsg, &msg, sizeof(SharedMsg));
		ReleaseMutex(pcd->hRWMutex);

		if (_tcsncmp(msg.szMessage, TEXT("exit"), 4) == 0) {
			pcd->threadMustContinue = 0;
		}

		SetEvent(pcd->hnewMsg);
		//Sleep(500);
		ResetEvent(pcd->hnewMsg);

		if (!pcd->threadMustContinue)
			break;
	}
}


void sendData(ControlData* pcd,TCHAR *toSend, TCHAR *infoOrigin) {
	SharedMsg msg;
	TCHAR  infoBuf[MSGTEXT_SZ] = { '\0' };
	DWORD  bufCharCount = MSGTEXT_SZ;
	
	bufCharCount = MSGTEXT_SZ;
	/*if (!GetUserName(infoBuf, &bufCharCount))
		_tprintf(TEXT("GetUserName"));*/

	_tcscpy_s(msg.userName, MSGTEXT_SZ, infoOrigin);
	_tcscpy_s(msg.szMessage, MSGTEXT_SZ, toSend);

	WaitForSingleObject(pcd->hRWMutex, INFINITE);
	CopyMemory(pcd->sharedMsg, &msg, sizeof(SharedMsg));
	ReleaseMutex(pcd->hRWMutex);

	SetEvent(pcd->hnewMsg);
	//Sleep(500);
	ResetEvent(pcd->hnewMsg);
}


int _tmain(int argc, LPTSTR argv[]) {
    // _tprintf(TEXT("Server NOVO!\n"));
    GlobalData gd;
    ControlData cdata;
    gd.cdata = &cdata;
	gd.onPause = FALSE;
	gd.cdata->threadMustContinue = TRUE;

    User users[MAX_USERS];
    Company companies[MAX_COMPANIES];
    HANDLE hUniqueMutex;

    

#ifdef UNICODE
    _setmode(_fileno(stdin), _O_WTEXT);
    _setmode(_fileno(stdout), _O_WTEXT);
#endif // UNICODE

    // Create a named mutex to ensure only one instance of the program is running
    hUniqueMutex = CreateMutex(NULL, TRUE, TEXT("Global\\UniqueMutexName"));
    if (hUniqueMutex == NULL) {
        _tprintf(TEXT("CreateMutex error: %d\n"), GetLastError());
        return 1;
    }

    if (GetLastError() == ERROR_ALREADY_EXISTS) {
        _tprintf(TEXT("Another instance of this program is already running.\n"));
        CloseHandle(hUniqueMutex);
        return 1;
    }

    _tprintf(TEXT("Starting server...\n"));

    if (initMemAndSync(&gd))
        _tprintf(TEXT("[SUCCESS] Shared memory and synchronization objects initialized successfully!\n"));
    else {
        _tprintf(TEXT("[ERROR] Shared memory and synchronization objects could not be initialized!\n"));
        CloseHandle(hUniqueMutex);
        return 1;
    }

    if (getUsersFromFile(users, &gd))
        _tprintf(TEXT("[SUCCESS] Users loaded from file successfully!\n"));
    else
        _tprintf(TEXT("[ERROR] Users could not be loaded from file!\n"));

    if (getCompaniesFromFile(companies, &gd))
        _tprintf(TEXT("[SUCCESS] Companies loaded from file successfully!\n"));
    else
        _tprintf(TEXT("[ERROR] Companies could not be loaded from file!\n"));

    gd.users = users;
    gd.companies = companies;

    for(int i = 0; i < gd.nUsers;i++)
        gd.users[i].nCompanies  = 0;

    // Create exit event
    cdata.hExitEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    if (cdata.hExitEvent == NULL) {
        _tprintf(TEXT("Error %d creating event (CreateEvent)\n"), GetLastError());
        CloseHandle(hUniqueMutex);
        return 1;
    }
    // Start threads
    HANDLE hThreadKb = CreateThread(NULL, 0, th_kb, &gd, 0, NULL);
    HANDLE hThreadBoard = CreateThread(NULL, 0, th_board, NULL, 0, NULL);
    HANDLE hThreadListenClient = CreateThread(NULL, 0, ConnectionHandlerThread, &gd, 0, NULL);
    //HANDLE hThreadReceiveMsg = CreateThread(NULL, 0, receiveMsg, &gd, 0, NULL);

	gd.cdata = &cdata;

    if (hThreadListenClient == NULL || hThreadKb == NULL || hThreadBoard == NULL) {
        _tprintf(TEXT("Error %d creating thread (CreateThread)\n"), GetLastError());
        CloseHandle(cdata.hnewMsg);
        CloseHandle(cdata.hRWMutex);
        UnmapViewOfFile(cdata.sharedMsg);
        CloseHandle(cdata.hMapFile);
        CloseHandle(hUniqueMutex);
        return 1;
    }

    // Wait for threads to complete (typically on server shutdown)
    WaitForSingleObject(hThreadListenClient, INFINITE);
    WaitForSingleObject(hThreadKb, INFINITE);
    WaitForSingleObject(hThreadBoard, INFINITE);
    WaitForSingleObject(cdata.hExitEvent, INFINITE);

	gd.hWaitableTimer = CreateWaitableTimer(NULL, TRUE, NULL);
	if (gd.hWaitableTimer == NULL) {
		_tprintf(TEXT("Error %d creating waitable timer (CreateWaitableTimer)\n"), GetLastError());
		CloseHandle(cdata.hnewMsg);
		CloseHandle(cdata.hRWMutex);
		UnmapViewOfFile(cdata.sharedMsg);
		CloseHandle(cdata.hMapFile);
        CloseHandle(hUniqueMutex);
		return 1;
	}
	

    // Cleanup
    CloseHandle(hThreadKb);
    CloseHandle(hThreadBoard);
    CloseHandle(hThreadListenClient);

    CloseHandle(cdata.hExitEvent);
    CloseHandle(cdata.hnewMsg);
    CloseHandle(cdata.hRWMutex);
    UnmapViewOfFile(cdata.sharedMsg);
    CloseHandle(cdata.hMapFile);
    
    // Release the unique mutex
    CloseHandle(hUniqueMutex);

    return 0;
}




TCHAR* userBuys(UserWithGlobalData* ugd, TCHAR* companyName, int numShares) {

    WaitForSingleObject(ugd->gd->hMutex, INFINITE);
	GlobalData* gd = ugd->gd;
	User* user = ugd->user;
	Company* companies = gd->companies;
	int nCompanies = gd->nCompanies;

	for (int i = 0; i < nCompanies; i++) {
		if (_tcscmp(companies[i].name, companyName) == 0) {
			float totalCost = companies[i].value * numShares;

            if (companies[i].nShares < numShares) {
                ReleaseMutex(ugd->gd->hMutex);
				return TEXT("[SERVER]: Insufficient shares on requested company.");
            }

			if (user->balance >= totalCost) {
				
                for(int j=0; j < user->nCompanies; j++){
					if(_tcscmp(user->companies[j].name, companyName) == 0){
                        
                        float nsh = (float) companies[i].nShares;
                        float stockDifPercentage = 1.0 - ((companies[i].nShares - numShares) / nsh);


                        user->balance -= totalCost;
                        companies[i].nShares -= numShares;
						user->companies[j].nShares += numShares;
                        _stprintf_s(ugd->gd->latestTransaction, MSGTEXT_SZ, TEXT("Last Transaction: user %s bought %d stocks from %s (%.2f $ each)"), ugd->user->name, numShares, companyName,companies[i].value);
                        ugd->gd->lastCompanyTrName = companyName;
                        sendData(ugd->gd->cdata, ugd->gd->latestTransaction, ugd->gd->lastCompanyTrName);

                        updateStockValueDif(&companies[i], stockDifPercentage, TRUE, ugd->gd->hMutex,ugd->gd->latestTransaction,ugd->gd->cdata);    
                        //_tprintf(TEXT("%s"), ugd->gd->latestTransaction);
                        sendData(ugd->gd->cdata, ugd->gd->latestTransaction, companyName);
                        
                        saveCompaniesToFile(gd->companies, gd->nCompanies);
                        ReleaseMutex(ugd->gd->hMutex);
                       
                        

                        
						return TEXT("[SERVER]: Purchase successful.");
					}
				}
                if (user->nCompanies < MAX_COMPANIES) {
                    float nsh = (float)companies[i].nShares;
                    float stockDifPercentage = 1.0 - ((companies[i].nShares - numShares) / nsh);
                    user->balance -= totalCost;
                    companies[i].nShares -= numShares;
                    _tcscpy_s(user->companies[user->nCompanies].name, MAX_TCHAR, companyName);
					user->companies[user->nCompanies].nShares = numShares;
					user->nCompanies++;

                    _stprintf_s(ugd->gd->latestTransaction, MSGTEXT_SZ, TEXT("Last Transaction: user %s bought %d stocks from %s (%.2f $ each)"), ugd->user->name, numShares, companyName, companies[i].value);
                    ugd->gd->lastCompanyTrName = companyName;
                    sendData(ugd->gd->cdata, ugd->gd->latestTransaction, ugd->gd->lastCompanyTrName);

                    updateStockValueDif(&companies[i], stockDifPercentage, TRUE, ugd->gd->hMutex, ugd->gd->latestTransaction, ugd->gd->cdata);
                    //_tprintf(TEXT("%s"), ugd->gd->latestTransaction);
                    sendData(ugd->gd->cdata, ugd->gd->latestTransaction, companyName);
                    
                    saveCompaniesToFile(gd->companies, gd->nCompanies);
                    ReleaseMutex(ugd->gd->hMutex);
					return TEXT("[SERVER]: Purchase successful.");
				} else {
					ReleaseMutex(ugd->gd->hMutex);
					return TEXT("[SERVER]: Maximum number of companies reached.");
                }
                
				//return TEXT("[SERVER]: Purchase successful.");
			} else {
                ReleaseMutex(ugd->gd->hMutex);
				return TEXT("[SERVER]: Insufficient funds.");
			}
		}
	}
	ReleaseMutex(ugd->gd->hMutex);

	return TEXT("[SERVER]: Company not found.");
}

TCHAR* userSells(UserWithGlobalData* ugd, TCHAR* companyName, int numShares) {
    WaitForSingleObject(ugd->gd->hMutex, INFINITE);
	GlobalData* gd = ugd->gd;
	User* user = ugd->user;
	Company* companies = gd->companies;
	int nCompanies = gd->nCompanies;

	for (int i = 0; i < nCompanies; i++) {
		if (_tcscmp(companies[i].name, companyName) == 0) {
			float totalCost = companies[i].value * numShares;

			for (int j = 0; j < user->nCompanies; j++) {
				if (_tcscmp(user->companies[j].name, companyName) == 0) {
					if (user->companies[j].nShares >= numShares) {

                        float nsh = (float)companies[i].nShares;
                        float stockDifPercentage;
                        if (numShares > companies[i].nShares) {
                            nsh = (float)numShares;
                            stockDifPercentage =  ((numShares - companies[i].nShares) / nsh);
                            //_tprintf(TEXT("[DEBUG] StockDifPercentage: %f\n"), stockDifPercentage);

                        }
                        else {
                            stockDifPercentage = 1.0 - ((companies[i].nShares - numShares) / nsh);
                        }


						user->balance += totalCost;
						companies[i].nShares += numShares;
						user->companies[j].nShares -= numShares;

                        if (user->companies[j].nShares == 0) {
							for (int k = j; k < user->nCompanies - 1; k++) {
								_tcscpy_s(user->companies[k].name, MAX_TCHAR, user->companies[k + 1].name);
								user->companies[k].nShares = user->companies[k + 1].nShares;
							}
							user->nCompanies--;
						}

                        _stprintf_s(ugd->gd->latestTransaction, MSGTEXT_SZ, TEXT("Last Transaction: user %s sold %d stocks from %s (%.2f $ each)"), ugd->user->name, numShares, companyName, companies[i].value);
                        ugd->gd->lastCompanyTrName = companyName;

                        sendData(ugd->gd->cdata, ugd->gd->latestTransaction, ugd->gd->lastCompanyTrName);


                        updateStockValueDif(&companies[i], stockDifPercentage, FALSE,  ugd->gd->hMutex, ugd->gd->latestTransaction, ugd->gd->cdata);
                        //_tprintf(TEXT("%s"), ugd->gd->latestTransaction);
                        
                        sendData(ugd->gd->cdata, ugd->gd->latestTransaction, ugd->gd->lastCompanyTrName);
                       
                        saveCompaniesToFile(gd->companies, gd->nCompanies);
                        ReleaseMutex(ugd->gd->hMutex);


						return TEXT("[SERVER]: Sale successful.");
					} else {
                        ReleaseMutex(ugd->gd->hMutex);
						return TEXT("[SERVER]: Insufficient shares.");
					}
				}
			}
            ReleaseMutex(ugd->gd->hMutex);
			return TEXT("[SERVER]: User does not own shares of this company.");
		}
	}
    ReleaseMutex(ugd->gd->hMutex);
	return TEXT("[SERVER]: Company not found.");
}


void updateStockValueDif(Company* c, float difPerc, BOOL raise, HANDLE* hMutex, TCHAR * latestTransaction, ControlData *cd) {
    if(raise){
        //_tprintf(TEXT("[DEBUG] Stock value before raise: %.2f\n Diff.: %f\nval times dif %f"), c->value,difPerc,(c->value * difPerc));
		
        c->value += c->value * difPerc;
        // _tprintf(TEXT("[INFO] Stock value for %s raised to %.2f\n"),c->name, c->value);
        
	} else {
		c->value -= c->value * difPerc;
        // _tprintf(TEXT("[INFO] Stock value for %s lowered to %.2f\n"), c->name, c->value);
	}
    _stprintf_s(latestTransaction, MSGTEXT_SZ, TEXT("Value per stock: %.2f $ (%d stock remaining)"), c->value, c->nShares);

    
}


void saveCompaniesToFile(Company* companies, int nCompanies) {
	HANDLE hFile;
	DWORD dwBytesWritten;
	TCHAR buffer[4096]; // Buffer to store read data

	hFile = CreateFile(
		COMPANIESFILE,
		GENERIC_WRITE,
		FILE_SHARE_WRITE,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);

	if (hFile == INVALID_HANDLE_VALUE) {
		_tprintf(TEXT("[WARNING] Error opening file. Error code: %d\n"), GetLastError());
		return;
	}

	for (int i = 0; i < nCompanies; i++) {
		_stprintf_s(buffer, 4096, TEXT("%s %d %.2f\n"), companies[i].name, companies[i].nShares, companies[i].value);
		if (!WriteFile(hFile, buffer, _tcslen(buffer) * sizeof(TCHAR), &dwBytesWritten, NULL)) {
			_tprintf(TEXT("[WARNING] Error writing to file. Error code: %d\n"), GetLastError());
			CloseHandle(hFile);
			return;
		}
	}

	CloseHandle(hFile);
}

void sendInfoNewBoard(GlobalData* gd) {
    TCHAR toSendBuffer[MSGTEXT_SZ];
    for(int i = 0; i< gd->nCompanies; i++){
		TCHAR buffer[MSGTEXT_SZ];
		_stprintf_s(buffer, MSGTEXT_SZ, TEXT("%s %d %.2f\n"), gd->companies[i].name, gd->companies[i].nShares, gd->companies[i].value);
        _tcscat_s(toSendBuffer, MSGTEXT_SZ, buffer);
	}
    toSendBuffer[_tcslen(toSendBuffer) - 1] = '\0';

    sendData(gd->cdata, toSendBuffer, TEXT("new_board_answer"));
}