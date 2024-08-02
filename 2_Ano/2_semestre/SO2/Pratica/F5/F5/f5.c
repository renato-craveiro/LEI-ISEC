#include <Windows.h>
#include <tchar.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>


#define FILENAME TEXT("letters.txt")
#define NUMBER_LETERS 26

int _1_tmain(int argc, _TCHAR* argv[])
{
	HANDLE hMapFile;
	HANDLE hFich;
	char* pBuf; //8 bits char
	char aux;
	TCHAR auxt;

#ifdef UNICODE
	_setmode(_fileno(stdin), _O_WTEXT);
	_setmode(_fileno(stdout), _O_WTEXT);
#endif // UNICODE
	
	_tprintf(TEXT("File inverter - 8 bits Chars\n"));

	_tprintf(TEXT("Creating file \n"));

	hFich = CreateFile(
		FILENAME, 
		GENERIC_READ | GENERIC_WRITE, 
		FILE_SHARE_READ | FILE_SHARE_WRITE, 
		NULL, 
		OPEN_EXISTING, 
		FILE_ATTRIBUTE_NORMAL, 
		NULL
	);

	if (hFich == INVALID_HANDLE_VALUE)
	{
		_tprintf(TEXT("Error %d opening file (CreateFile)\n"), GetLastError());
		//CloseHandle(hFich);
		return 1;
	}

	hMapFile = CreateFileMapping(
		hFich,
		NULL,
		PAGE_READWRITE,
		0,
		NUMBER_LETERS,
		NULL
	);

	if (hMapFile == NULL)
	{
		_tprintf(TEXT("Error %d opening file (CreateFileMapping)\n"), GetLastError());
		CloseHandle(hFich);
		return 1;
	}

	_tprintf(TEXT("MapViewOfFile\n"));

	pBuf = (char*)MapViewOfFile(
		hMapFile,
		FILE_MAP_READ | FILE_MAP_WRITE,
		0,
		0,
		NUMBER_LETERS
	);

	if (pBuf == NULL)
	{
		_tprintf(TEXT("Error %d opening file (MapViewOfFile)\n"), GetLastError());
		CloseHandle(hMapFile);
		CloseHandle(hFich);
		return 1;
	}

	_tprintf(TEXT("Updateing file in memory\n"));

	for (unsigned int i = 0; i < NUMBER_LETERS/2; i++)
	{
		aux = pBuf[i];
		pBuf[i] = pBuf[NUMBER_LETERS - i - 1];
		pBuf[NUMBER_LETERS - i - 1] = aux;
	}

	_tprintf(TEXT("Showing File inverted\n"));

	for (unsigned int i = 0; i < NUMBER_LETERS; i++)
	{
		auxt = pBuf[i];
		_tprintf(TEXT("%c"), auxt);
	}

	_tprintf(TEXT("\nFinito\n"));
	UnmapViewOfFile(pBuf);

	CloseHandle(hMapFile);	
	CloseHandle(hFich);

	return 0;

}