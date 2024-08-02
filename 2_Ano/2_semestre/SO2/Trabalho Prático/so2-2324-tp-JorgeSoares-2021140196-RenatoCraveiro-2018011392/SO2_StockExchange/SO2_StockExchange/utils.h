#pragma once
#include <Windows.h>
#include <tchar.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>


#define SHM_NAME TEXT("BoardSHM")
#define MUTEXT_NAME TEXT("RWMUTEX")
#define EVENT_NAME TEXT("NEWBOARDMSG")

#define USERSFILE TEXT("users.txt")
#define COMPANIESFILE TEXT("companies.txt")

#define REGISTRY_NCLIENTES_KEY TEXT("SO2")
#define REGISTRY_NCLIENTES_VALUE TEXT("NCLIENTES")

#define MY_STOCK_PIPE TEXT("\\\\.\\pipe\\MyStockPipe")
#define BUFFER_SIZE 512 // Adjust size as needed


#define MAX_TCHAR 256
#define MAX_USERS 50
#define MAX_COMPANIES 10
#define MAX_COMPANY_PER_USER 5

#define MAX_USERS_ONLINE 2

#define MSGTEXT_SZ 1024

#define LOGIN_SUCCESS TEXT("LOGIN_SUCCESS")
#define LOGIN_FAIL TEXT("LOGIN_FAIL")
#define LOGIN_WAIT TEXT("LOGIN_WAIT")
#define LOGIN_ALLOWED TEXT("LOGIN_ALLOWED")

#define _SECOND 10000000

typedef struct _MSG {
	TCHAR szMessage[MSGTEXT_SZ];
	TCHAR userName[MSGTEXT_SZ];
}SharedMsg;

#define MSFBUFSIZE sizeof(SharedMsg)

typedef struct _ControlData {
	HANDLE hMapFile;
	SharedMsg* sharedMsg;
	BOOL threadMustContinue;
	HANDLE hnewMsg;
	HANDLE hExitEvent;
	HANDLE hRWMutex;
}ControlData;


typedef struct _Company {
	TCHAR name[MAX_TCHAR];
	double value;
	DWORD nShares;
}Company;

typedef struct _User {
	TCHAR name[MAX_TCHAR];
	TCHAR password[MAX_TCHAR];
	double balance;
	HANDLE hPipe;  // Handle for communication pipe for each user
	BOOL online;
	Company companies[MAX_COMPANY_PER_USER];
	int nCompanies;
}User;