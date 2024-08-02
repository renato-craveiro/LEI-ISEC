#include <windows.h>
#include <tchar.h>
#include <io.h>
#include <fcntl.h>
#include <stdio.h>
#define TAM 200
int _tmain(int argc, TCHAR* argv[]) {
	HKEY chave;
	TCHAR chave_nome[TAM], par_nome[TAM], par_valor[TAM], buff_in[TAM], final_key[TAM],val_type[TAM],val_date[TAM], agoratwo[TAM];
	LSTATUS res;
	DWORD opt,dword_data,bftam;

	DWORD disp,vtype,iter=0;
	/* ... Mais variáveis ... */
#ifdef UNICODE
	_setmode(_fileno(stdin), _O_WTEXT);
	_setmode(_fileno(stdout), _O_WTEXT);
	_setmode(_fileno(stderr), _O_WTEXT);
#endif
	fflush(stdout);
	fflush(stdin);
	_tprintf(_TEXT("Insira o nome da chave a criar/abrir\n> "));
	_fgetts(buff_in, TAM, stdin);
	buff_in[_tcslen(buff_in) - 1] = '\0';
	_stprintf_s(final_key,TAM, TEXT("Software\\AULA\\%s"),buff_in);
	_tprintf(_TEXT("Nome da chave %s\n"), final_key);

	if (RegCreateKeyEx(HKEY_CURRENT_USER, final_key, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &chave, &disp) != ERROR_SUCCESS) {
		_tprintf(_T("DEU BOSTINHA AMIGUINHO.... (%d)\n"),GetLastError());
		return -1;
	}
	else {
		if (disp == REG_OPENED_EXISTING_KEY) {
			_tprintf(_T("ABERTA COM SUSSEXO (%s)\n"), final_key);
		}
		if (disp == REG_CREATED_NEW_KEY) {
			_tprintf(_T("%s - mais uma chave camelo...\n"), final_key);
		}

	}

	while (TRUE) {
		_tprintf(_T("O que queres fazer agora engraçadinho?\n1-criar par-valor\n2-consultar\n3-apagar um gajo?\n4-listar tudo???\n5-apagar o que para aqui foi feito!\n0-sair disto mas é....\n\n> "));
		_fgetts(buff_in, TAM, stdin);
		buff_in[_tcslen(buff_in) - 1] = '\0';
		opt = _wtoi(buff_in);
		switch (opt) {
		case 1:
			_tprintf(TEXT("NOME: "));
			_fgetts(buff_in, TAM, stdin);
			buff_in[_tcslen(buff_in) - 1] = '\0';
			_tprintf(TEXT("TIPO (1-REG_BINARY | 2-REG_DWORD | 3-REG_SZ): "));
			_fgetts(val_type, TAM, stdin);
			val_type[_tcslen(val_type) - 1] = '\0';
			vtype = _wtoi(val_type);
			_tprintf(TEXT("nome: %s, tipo: %d\n\n"), buff_in, vtype);
			switch (vtype) {
			case 1:
				if (RegSetValueEx(chave, buff_in, 0, REG_BINARY, TRUE, sizeof(TRUE)) != ERROR_SUCCESS) {
					_tprintf(TEXT("Não consegui inserir dados :( (%d)...\n"), GetLastError());
				}
				break;

			case 2:
				dword_data = 123;
				if (RegSetValueEx(chave, buff_in, 0, &dword_data, sizeof(dword_data), sizeof(123)) != ERROR_SUCCESS) {
					_tprintf(TEXT("Não consegui inserir dados :( (%d)...\n"), GetLastError());
				}
				break;
			case 3:
				_tprintf(TEXT("DADOS: "));
				_fgetts(val_date, TAM, stdin);
				val_date[_tcslen(val_date) - 1] = '\0';
				if (RegSetValueEx(chave, buff_in, 0, REG_SZ, (LPBYTE) val_date,  _tcslen(val_date)*sizeof(TCHAR)) != ERROR_SUCCESS) {
					_tprintf(TEXT("Não consegui inserir dados :( (%d)...\n"),GetLastError());
				}
				break;
			default:
				break;
			}

			


			break;
		case 2:
			/*
			LSTATUS RegGetValue(
HKEY hkey,
LPCTSTR lpSubKey,
LPCTSTR lpValue,
DWORD dwFlags,
LPDWORD pdwType,
PVOID pvData,
LPDWORD pcbData
);*/
			_tprintf(TEXT("NOME: "));
			_fgetts(buff_in, TAM, stdin);
			buff_in[_tcslen(buff_in) - 1] = '\0';
			DWORD bfsize = TAM;
			TCHAR agora[TAM];

			_tprintf(TEXT("%s\n\n"), buff_in);
			if (RegQueryValueEx(chave, buff_in, NULL, NULL, (LPBYTE)agora, &bfsize) != ERROR_SUCCESS) {
				_tprintf(_T("Não consigo :(...\n"));
				_tprintf(_T("isto? %s\n"), agora);
			}
			else {
				_tprintf(_T("isto? %s\n"), agora);
			}
			break;
		case 3:
			_tprintf(TEXT("NOME: "));
			_fgetts(buff_in, TAM, stdin);
			buff_in[_tcslen(buff_in) - 1] = '\0';
			if (RegDeleteValue(chave, buff_in)!=ERROR_SUCCESS) {
				_tprintf(TEXT("NAO CONSIGO :(...\n"));
			}
			else
				_tprintf(TEXT("feito.\n"));

			break;

		case 4:
			iter = 0;
			bftam = TAM;
			while (RegEnumValue(chave, iter, agoratwo, &bftam, NULL, NULL, NULL, NULL) == ERROR_SUCCESS) {
				_tprintf(TEXT("PAR-VALOR: %s\n"), agoratwo);
				iter++;
				bftam = TAM;
			}
			//while (regenum... ,i, varRetorno,...)
			//	i++

			break;
		case 5:
			RegCloseKey(chave);
			if (RegDeleteKeyEx(HKEY_CURRENT_USER, final_key, KEY_WOW64_32KEY, 0) != ERROR_SUCCESS) {
				_tprintf("erro...\n");
			}
			else {
				_tprintf("muerta!\n");
				return 0;
			}

			break;
		case 0:
			RegCloseKey(chave);
			return 0;
			break;
		default:
			_tprintf(TEXT("ESSA MERDA NÃO EXISTE (desculpa :( )....\n\n> "));
			break;

		}

	}

	
	/* ... as várias alíneas ...*/
	RegCloseKey(chave);
	
	return 0;
}