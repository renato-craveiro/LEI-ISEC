#include <windows.h>
#include <tchar.h>
#include <io.h>
#include <fcntl.h>
#include <stdio.h>

#define TAM 200


typedef struct {
	int sum;
	int loLim;
	int upLim;
	//BOOLEAN continue;
}ThreadData;


typedef struct { 
	ThreadData *tData;
	int threadsSize;
}ThreadsContinue;


int isPrime(int n);
int isEven(int n);
DWORD WINAPI SumPrimeNumbers(LPVOID lpParam);
DWORD WINAPI SumEvenNumbers(LPVOID lpParam);





int _tmain(int argc, TCHAR* argv[]) {
	ThreadData tDataEven,tDataPrime;
	HANDLE hThreadArray[2];
	int sumEven = 0, sumPrime = 0, i;
	
#ifdef UNICODE
	_setmode(_fileno(stdin), _O_WTEXT);
	_setmode(_fileno(stdout), _O_WTEXT);
	_setmode(_fileno(stderr), _O_WTEXT);
#endif
	tDataEven.loLim = 500;
	tDataEven.upLim = 3000;
	tDataPrime.loLim = 700;
	tDataPrime.upLim = 7000;

	//tDataEven.continue = TRUE 
	//&tDataEven
	
	//CREATE THREAD TO USER INPUT. when "chega" change structs tData->continue=FALSE
	//ThreadsContinue struct to warn other threads

	hThreadArray[0] = NULL;
	hThreadArray[1] = NULL;
	hThreadArray[0] = CreateThread(NULL, 0, SumPrimeNumbers, &sumPrime, 0, NULL);
	if (hThreadArray[0] == NULL) {
		_tprintf(_T("Sum Prime thread failed!"));
	}
	hThreadArray[1] = CreateThread(NULL, 0, SumEvenNumbers, &sumEven, 0, NULL);
	if (hThreadArray[1] == NULL) {
		_tprintf(_T("Sum Even thread failed!"));
	}
	
	while (1) {
		if (WaitForMultipleObjects(2, hThreadArray, TRUE, 500) == WAIT_TIMEOUT) {
			_tprintf(_T("AINDA A ESPERA....\n"));
		}
		else {
			break;
		}
	}

	for (i = 0; i < 2; i++) {
		if (hThreadArray[i] != NULL)
			CloseHandle(hThreadArray[i]);
	}
	
	
	_tprintf(_T("Sum Results.\nPrime: %d\nEven: %d\n\n"),sumPrime,sumEven);

	return 0;
}


DWORD WINAPI SumPrimeNumbers(LPVOID lpParam) {
	int i;
	int count = 0;
	int* sum = (int*)lpParam;
	//tDataEven->sum... //tDataEven->loLim/upLim...

	
	for (i = 1; i <= 1000; i++) {
		
		if (isPrime(i)) {
			//_tprintf(_T("[PRIME]: num a somar %d\n"), i);
			*sum += i;
			count++;
			if (count % 15 == 0)
				Sleep(1000);
		}
	}

	return 0;
}



DWORD WINAPI SumEvenNumbers(LPVOID lpParam) {
	int i;
	int count = 0;
	int* sum = (int*)lpParam;

	for (i = 1; i <= 1000; i++) {
		
		if (isEven(i)) {
			//_tprintf(_T("[EVEN]: num a somar %d\n"), i);
			*sum += i;
			count++;
			if (count % 200 == 0)
				Sleep(1000);
		}
	}

	return 0;
}



int isPrime(int n)
{
	int i;
	if (n <= 1)
		return 0;

	for (i = 2; i < n; i++)
		if (n % i == 0)
			return 0;

	return 1;
}

int isEven(int n)
{
	int i;
	if (n <= 1)
		return 0;

	if (n % 2 == 0)
		return 1;

	return 0;
}
