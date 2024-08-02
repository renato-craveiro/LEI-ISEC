#include <windows.h>
#include <tchar.h>
#include <math.h>

#include <stdio.h>
#include <fcntl.h> 
#include <io.h>


// funcionalidade relacionada com temporização

static double PerfCounterFreq; // n ticks por seg.

void initClock() {
	LARGE_INTEGER aux;
	if (!QueryPerformanceFrequency(&aux))
		_tprintf(TEXT("\nSorry - No can do em QueryPerfFreq\n"));
	PerfCounterFreq = (double)(aux.QuadPart); // / 1000.0;
	_tprintf(TEXT("\nTicks por sec.%f\n"), PerfCounterFreq);
}

__int64 startClock() {
	LARGE_INTEGER aux;
	QueryPerformanceCounter(&aux);
	return aux.QuadPart;
}

double stopClock(__int64 from) {
	LARGE_INTEGER aux;
	QueryPerformanceCounter(&aux);
	return (double)(aux.QuadPart - from) / PerfCounterFreq;
}

// estrutura de dados para controlar as threads

typedef struct {
	int* sharedBlockCounter;
	//unsigned int lowerLimit;
	//unsigned int upperLimit;
	unsigned int* sharedCounter;
	CRITICAL_SECTION* sharedCriticalSection;
	HANDLE sharedEvent;
	// ...
//	int x; // remover este inteiro. Está aqui apenas para este código compilar
} TDados;

// função da(s) thread(s)
DWORD WINAPI CountMultiplesThree(LPVOID lpParam);
DWORD WINAPI UserCommands(LPVOID lpParam);

// número * máximo * de threads
// podem (e devem) ser menos
#define MAX_THREADS 20





int _tmain(int argc, TCHAR* argv[]) {

	// matriz de handles das threads
	HANDLE hThreads[MAX_THREADS];
	HANDLE commandThread;

	// Matriz de dados para as threads;
	TDados tdados[MAX_THREADS];

	// número efectivo de threads
	int numthreads;

	// limite superior
	unsigned int limsup;

	// variáveis para cronómetro
	__int64 clockticks;
	double duracao;

	int blockCounter = 0;

	unsigned int range;
	unsigned int inter;

	CRITICAL_SECTION criticalSection;
	HANDLE event;


	int i;

#ifdef UNICODE
	_setmode(_fileno(stdin), _O_WTEXT);
	_setmode(_fileno(stdout), _O_WTEXT);
#endif 

	if (!InitializeCriticalSectionAndSpinCount(&criticalSection, 400)) {
		_tprintf(TEXT("Error creating crit sct.\n"));
		return 0;
	}

	event = CreateEvent(NULL, TRUE, FALSE, NULL);

	if (event == NULL) {
		_tprintf(TEXT("Error creating the event\n"));
		return 0;
	}

	initClock();
	//_tprintf_s(TEXT("\nLimite sup. -> "));
	//_tscanf_s(TEXT("%u"), &limsup);
	_tprintf_s(TEXT("\nNum threads -> "));
	_tscanf_s(TEXT("%u"), &numthreads);
	if (numthreads > MAX_THREADS)
		numthreads = MAX_THREADS;

	// FAZER prepara e cria threads
	//       manda as threads começar FOR!!!!!!!!!!!!!!!
	for(i = 0; i < numthreads; i++) {
		//tdados[i].lowerLimit = i * (limsup / numthreads) + 1;
		//tdados[i].upperLimit = (i + 1) * (limsup / numthreads);
		tdados[i].sharedBlockCounter = &blockCounter;

		tdados[i].sharedCounter = (unsigned int*)malloc(sizeof(unsigned int));
		*(tdados[i].sharedCounter) = 0;
		tdados[i].sharedCriticalSection = &criticalSection;
		tdados[i].sharedEvent = event;
		hThreads[i] = CreateThread(NULL, 0, CountMultiplesThree, &tdados[i], 0, NULL);
	}


	clockticks = startClock();

	// FAZER aguarda / controla as threads 
	//       manda as threads parar

	SetEvent(event);
	WaitForMultipleObjects(numthreads, hThreads, TRUE, INFINITE);

	duracao = stopClock(clockticks);
	_tprintf(TEXT("\nSegundos=%f\n"), duracao);

	for (i = 0; i < numthreads; i++)
		CloseHandle(hThreads[i]);

	DeleteCriticalSection(&criticalSection);
	CloseHandle(event);

	// FAZER apresenta resultados

	// Cód. ref. para aguardar por uma tecla – caso faça falta
	 _tprintf(TEXT("\nFIM - Carregue numa tecla"));
	 _gettch();

	return 0;
}
// Este código é apenas uma ajuda para o exercício. Se quiser, mude-o


DWORD WINAPI CountMultiplesThree(LPVOID lpParam) {
	unsigned int i;
	TDados* data = (TDados*)lpParam;
	WaitForSingleObject(data->sharedEvent, INFINITE);

	//ResetEvent(data->sharedEvent);

	for (i = data->lowerLimit; i <= data->upperLimit; i++) {
		if (i % 3 == 0) {
			EnterCriticalSection(data->sharedCriticalSection);
			(*(data->sharedCounter))++;
			LeaveCriticalSection(data->sharedCriticalSection);
		}
	}
	return 0;

}