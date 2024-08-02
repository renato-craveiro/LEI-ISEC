#include <windows.h>
#include <tchar.h>
#include <fcntl.h>
#include <io.h>
#include <stdio.h>
#define MAX 256



int _tmain(int argc, LPTSTR argv[]) {
    TCHAR str[MAX], result[MAX] = TEXT("Olá! Este programa é para aceitar UNICODE. Insira \'fim\' para sair\n"), buffer[MAX];
    unsigned int i;
    //UNICODE: Por defeito, a consola Windows não processa caracteres wide. 
    //A maneira mais fácil para ter esta funcionalidade é chamar _setmode:
#ifdef UNICODE 
    _setmode(_fileno(stdin), _O_WTEXT);

    
    _setmode(_fileno(stdout), _O_WTEXT);
#endif

    GetModuleFileName(NULL, buffer, MAX);
    _tprintf(TEXT("O meu executável é: %s\n"), buffer);
    TCHAR* env;
    STARTUPINFO infoself = { sizeof(infoself) };
    //ZeroMemory(info, sizeof(STARTUPINFO));
    PROCESS_INFORMATION processInfoself;
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    TCHAR text[256];
    int vezes;
    GetModuleFileName(NULL, text, 256);

    if (argc == 1)
        vezes = 3;
    else
        vezes = _ttoi(argv[1]);

    if (vezes > 0) {
        _tprintf(TEXT("Faltam %d execucoes\n"), vezes);
        vezes--;
        _stprintf_s(text, 256, TEXT("%s %d"), text, vezes);
        ZeroMemory(&si, sizeof(STARTUPINFO));
        si.cb = sizeof(STARTUPINFO);
        if (!CreateProcess(NULL, text, NULL, NULL, FALSE, FALSE, NULL, NULL, &si, &pi))
            _tprintf(TEXT("Erro ao lancar processo."));
        ExitProcess(0);
    }

    TCHAR vars;

    do {
        _tprintf(result);
        fflush(stdin);
        _fgetts(str, MAX, stdin);
        //Retirar \n
        str[_tcslen(str) - 1] = '\0';
        



        STARTUPINFO info = { sizeof(info) };
        //ZeroMemory(info, sizeof(STARTUPINFO));
        PROCESS_INFORMATION processInfo;
        if (CreateProcess(NULL, str, NULL, NULL, TRUE, 0, NULL, NULL, &info, &processInfo))
        {
            WaitForSingleObject(processInfo.hProcess, INFINITE);
            CloseHandle(processInfo.hProcess);
            CloseHandle(processInfo.hThread);
        }




        //Maiúsculas
        for (i = 0; i < _tcslen(str); i++)
            str[i] = _totupper(str[i]);
        _stprintf_s(result, MAX, TEXT("Frase:%s, Tamanho:%d\n"), str, _tcslen(str));
    } while (_tcsicmp(TEXT("FIM"), str));
    return 0;
}