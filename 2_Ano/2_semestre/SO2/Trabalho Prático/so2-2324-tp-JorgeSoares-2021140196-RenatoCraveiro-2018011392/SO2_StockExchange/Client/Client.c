#include "../SO2_StockExchange/utils.h"

typedef struct {
    HANDLE hPipe;
    BOOL shouldRun;
    HANDLE hEvent; // Event to signal threads to exit
    HANDLE hResponseEvent;  // Event to signal when a response is received
} ThreadData;

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

BOOL connectToServer(HANDLE* hPipe) {
    _tprintf(TEXT("Waiting for server."));
    while (1) {
        *hPipe = CreateFile(
            MY_STOCK_PIPE,
            GENERIC_READ | GENERIC_WRITE,
            0,
            NULL,
            OPEN_EXISTING,
            FILE_FLAG_OVERLAPPED,
            NULL);

        if (*hPipe != INVALID_HANDLE_VALUE) {        
            DWORD dwMode = PIPE_READMODE_MESSAGE;
            if (!SetNamedPipeHandleState(
                *hPipe,
                &dwMode,
                NULL,
                NULL)) {
                CloseHandle(*hPipe);  // Close handle on failure
                continue;
            } else {
                _tprintf(TEXT("\n\n[SUCCESS] Connected to server to Login.\n"));
                return TRUE; // Connection successful
            }
        }

        DWORD lastError = GetLastError();
        if (lastError == ERROR_PIPE_BUSY || lastError == ERROR_FILE_NOT_FOUND) {
            // Retry connection after a delay
            if (!WaitNamedPipe(MY_STOCK_PIPE, 1000)) {
                _tprintf(TEXT("."));
                Sleep(1000); // Wait before retrying
            }
        } else {
            _tprintf(TEXT("Could not open pipe. GLE=%d\n"), lastError);
            Sleep(1000); // Wait before retrying
        }
    }
    return FALSE;
}

DWORD WINAPI keyboardInputThread(LPVOID lpParam) {
    ThreadData* data = (ThreadData*)lpParam;
    TCHAR command[BUFFER_SIZE];
    OVERLAPPED olWrite = {0};
    olWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL); // Event for overlapped I/O

    while (data->shouldRun) {
        _tprintf(TEXT("\n\n\n( listc | buy <company_name> <number_of_shares> | sell <company_name> <number_of_shares> | balance | exit )\n> "));

        _fgetts(command, BUFFER_SIZE, stdin);

        if (command[0] == '\n') continue;
        command[_tcslen(command) - 1] = '\0'; // Remove newline

        if ((_tcscmp(command, TEXT("clc")) == 0) || (_tcscmp(command, TEXT("cls")) == 0) || (_tcscmp(command, TEXT("clear")) == 0)) {
            clearScreen();
            continue;
        }
        DWORD bytesWritten;
        BOOL writePending = FALSE;
        if (!WriteFile(data->hPipe, command, _tcslen(command) * sizeof(TCHAR), &bytesWritten, &olWrite)) {
            DWORD lastError = GetLastError();
            if (lastError == ERROR_IO_PENDING) {
                writePending = TRUE;
            } else {
                _tprintf(TEXT("WriteFile to pipe failed. GLE=%d\n"), lastError);
                data->shouldRun = FALSE; // Stop the process if write fails
                SetEvent(data->hEvent); // Signal other threads to exit
                break;
            }
        }

        if (writePending) {
            WaitForSingleObject(olWrite.hEvent, INFINITE);
            if (!GetOverlappedResult(data->hPipe, &olWrite, &bytesWritten, FALSE)) {
                _tprintf(TEXT("WriteFile to pipe failed. GLE=%d\n"), GetLastError());
                data->shouldRun = FALSE; // Stop the process if write fails
                SetEvent(data->hEvent); // Signal other threads to exit
                break;
            }
        }

        // Wait for the response from the server
        WaitForSingleObject(data->hResponseEvent, INFINITE);
        ResetEvent(data->hResponseEvent); // Reset the event for the next command

        if (_tcscmp(command, TEXT("exit")) == 0) {
            data->shouldRun = FALSE; // Exit command stops the thread
            SetEvent(data->hEvent); // Signal other threads to exit
        }

        
    }

    CloseHandle(olWrite.hEvent);
    return 0;
}

DWORD WINAPI serverListenThread(LPVOID lpParam) {
    ThreadData* data = (ThreadData*)lpParam;
    TCHAR buffer[BUFFER_SIZE];
    DWORD bytesRead;
    OVERLAPPED olRead = {0};
    olRead.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

    while (data->shouldRun) {
        BOOL readPending = FALSE;
        if (!ReadFile(data->hPipe, buffer, sizeof(buffer), &bytesRead, &olRead)) {
            DWORD lastError = GetLastError();
            if (lastError == ERROR_IO_PENDING) {
                readPending = TRUE;
            } else if (lastError == ERROR_PIPE_NOT_CONNECTED) {
                _tprintf(TEXT("Server has closed the pipe or error occurred. GLE=%d\n"), lastError);
                data->shouldRun = FALSE;
                SetEvent(data->hEvent); // Signal other threads to exit
                break;
            } else {
                _tprintf(TEXT("ReadFile failed with GLE=%d\n"), lastError);
                data->shouldRun = FALSE;
                SetEvent(data->hEvent); // Signal other threads to exit
                break;
            }
        }

        if (readPending) {
            WaitForSingleObject(olRead.hEvent, INFINITE);
            if (!GetOverlappedResult(data->hPipe, &olRead, &bytesRead, FALSE)) {
                _tprintf(TEXT("ReadFile failed with GLE=%d\n"), GetLastError());
                data->shouldRun = FALSE;
                SetEvent(data->hEvent); // Signal other threads to exit
                break;
            }
        }

        buffer[bytesRead / sizeof(TCHAR)] = '\0';

        if (_tcscmp(buffer, TEXT("exit")) == 0) {
            data->shouldRun = FALSE;
            SetEvent(data->hEvent); // Signal other threads to exit
            break;
        } else {
            _tprintf(TEXT("\n\t%s\n"), buffer);
            // Signal that a response has been received
            SetEvent(data->hResponseEvent);
        }
    }

    CloseHandle(olRead.hEvent);
    return 0;
}


void authenticateUser(ThreadData* data) {
    TCHAR username[MAX_TCHAR + 1];
    TCHAR password[MAX_TCHAR + 1];
    TCHAR credentials[2 * MAX_TCHAR + 2];  // For username and password
    DWORD bytesWritten;
    OVERLAPPED olWrite = { 0 };
    BOOL writeResult;
    HANDLE hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);  // Overlapped I/O

    if (!hEvent) {
        _tprintf(TEXT("Failed to create event.\n"));
        return;
    }
    olWrite.hEvent = hEvent;

    _tprintf(TEXT("Enter username: "));
    if (!_fgetts(username, MAX_TCHAR, stdin)) {
        _tprintf(TEXT("Failed to read username.\n"));
        CloseHandle(hEvent);
        return;
    }
    username[_tcslen(username) - 1] = '\0';  // Remove newline

    _tprintf(TEXT("Enter password: "));
    if (!_fgetts(password, MAX_TCHAR, stdin)) {
        _tprintf(TEXT("Failed to read password.\n"));
        CloseHandle(hEvent);
        return;
    }
    password[_tcslen(password) - 1] = '\0';  // Remove newline

    _snwprintf_s(credentials, BUFFER_SIZE, _TRUNCATE, TEXT("%s %s"), username, password);

    writeResult = WriteFile(data->hPipe, credentials, _tcslen(credentials) * sizeof(TCHAR), &bytesWritten, &olWrite);
    if (!writeResult) {
        if (GetLastError() == ERROR_IO_PENDING) {
            WaitForSingleObject(olWrite.hEvent, INFINITE);
            if (!GetOverlappedResult(data->hPipe, &olWrite, &bytesWritten, FALSE)) {
                _tprintf(TEXT("Failed to complete write operation. GLE=%d\n"), GetLastError());
                data->shouldRun = FALSE;
                SetEvent(data->hEvent); // Signal other threads to exit
            }
        } else {
             _tprintf(TEXT("Initial write failed. GLE=%d\n"), GetLastError());
             data->shouldRun = FALSE; // Stop the process if authentication write fails
             SetEvent(data->hEvent); // Signal other threads to exit
        }
    }

    CloseHandle(hEvent);  // Ensure handle is closed in all paths
}

DWORD WINAPI serverResponseThread(LPVOID lpParam) {
    ThreadData* data = (ThreadData*)lpParam;
    TCHAR buffer[BUFFER_SIZE];
    DWORD bytesRead;
    OVERLAPPED olRead = {0};
    olRead.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL); 

    if (olRead.hEvent == NULL) {
        _tprintf(TEXT("CreateEvent failed with GLE=%d\n"), GetLastError());
        return 1;
    }

    BOOL awaitingConfirmation = TRUE;
    BOOL onWaitingList = FALSE;
    BOOL loginAllowed = TRUE;

    DWORD bytesWritten;
    OVERLAPPED olWrite = { 0 };
    olWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

    if (olWrite.hEvent == NULL) {
        _tprintf(TEXT("CreateEvent failed with GLE=%d\n"), GetLastError());
        CloseHandle(olRead.hEvent);
        return 1;
    }

    if (!WriteFile(data->hPipe, TEXT("START"), 10 * sizeof(TCHAR), &bytesWritten, &olWrite)) {
        if (GetLastError() == ERROR_IO_PENDING) {
            WaitForSingleObject(olWrite.hEvent, INFINITE);
            if (!GetOverlappedResult(data->hPipe, &olWrite, &bytesWritten, FALSE)) {
                _tprintf(TEXT("WriteFile failed with GLE=%d\n"), GetLastError());
                CloseHandle(olWrite.hEvent);
                CloseHandle(olRead.hEvent);
                data->shouldRun = FALSE; // Stop the process if write fails
                SetEvent(data->hEvent); // Signal other threads to exit
                return 1;
            }
        } else {
            _tprintf(TEXT("WriteFile failed with GLE=%d\n"), GetLastError());
            CloseHandle(olWrite.hEvent);
            CloseHandle(olRead.hEvent);
            data->shouldRun = FALSE; // Stop the process if write fails
            SetEvent(data->hEvent); // Signal other threads to exit
            return 1;
        }
    }

    while (data->shouldRun) {
        if (awaitingConfirmation) {
            if (!ReadFile(data->hPipe, buffer, sizeof(buffer), &bytesRead, &olRead)) {
                DWORD lastError = GetLastError();
                if (lastError == ERROR_IO_PENDING) {
                    WaitForSingleObject(olRead.hEvent, INFINITE);
                    if (!GetOverlappedResult(data->hPipe, &olRead, &bytesRead, FALSE)) {
                        _tprintf(TEXT("ReadFile failed with GLE=%d\n"), GetLastError());
                        data->shouldRun = FALSE; // Exit the loop if read fails
                        break;
                    }
                } else if (lastError == ERROR_BROKEN_PIPE) {
                    _tprintf(TEXT("Server has closed the pipe or error occurred. %d\n"), lastError);
                    data->shouldRun = FALSE; // Exit the loop if server disconnects
                    break;
                } else {
                    _tprintf(TEXT("ReadFile failed with GLE=%d\n"), lastError);
                    data->shouldRun = FALSE; // Exit the loop if read fails
                    break;
                }
            }

            buffer[bytesRead / sizeof(TCHAR)] = '\0'; // Null-terminate the string

            if (loginAllowed && _tcscmp(buffer, LOGIN_ALLOWED) == 0) {
                authenticateUser(data);
                loginAllowed = FALSE;
                continue;
            } else {
                if (_tcscmp(buffer, LOGIN_SUCCESS) == 0) {
                    _tprintf(TEXT("\n\n\tYou are now connected. Start entering commands.\n"));
                    awaitingConfirmation = FALSE; // Move on to regular command phase
                    onWaitingList = FALSE; // Reset waiting list status
                }

                if (onWaitingList) {
                    _tprintf(TEXT("."));
                    Sleep(1000); // Wait before retrying
                } else {
                    if (_tcscmp(buffer, LOGIN_WAIT) == 0) {
                        _tprintf(TEXT("\n\n\tAll slots are full. You are placed on a waiting list.\nPlease wait."));
                        onWaitingList = TRUE;
                    } else if (_tcscmp(buffer, LOGIN_FAIL) == 0) {
                        _tprintf(TEXT("\n\n\tYour credentials are incorrect\n\n\n"));
                        data->shouldRun = FALSE; // Exit the loop and close the client
                        break;
                    }
                }
            }

        } else {
            // Normal command processing
            HANDLE hThreadInput = CreateThread(NULL, 0, keyboardInputThread, data, 0, NULL);
            HANDLE hThreadListen = CreateThread(NULL, 0, serverListenThread, data, 0, NULL);
            
            if (hThreadInput == NULL || hThreadListen == NULL) {
                _tprintf(TEXT("Failed to create input thread. GLE=%d\n"), GetLastError());
                data->shouldRun = FALSE;
                break;
            }

            // Wait for the input thread to complete
            HANDLE handles[] = { hThreadInput, hThreadListen, data->hEvent };
            WaitForMultipleObjects(3, handles, FALSE, INFINITE);

            CloseHandle(hThreadInput);
            CloseHandle(hThreadListen);
        }
    }

    // Ensure we signal other threads to exit if they haven't already
    SetEvent(data->hEvent);

    CloseHandle(olWrite.hEvent);
    CloseHandle(olRead.hEvent);

    // Check if the pipe handle is valid before closing
    if (data->hPipe != INVALID_HANDLE_VALUE) {
        DisconnectNamedPipe(data->hPipe);
        CloseHandle(data->hPipe);
        data->hPipe = INVALID_HANDLE_VALUE; // Invalidate the handle after closing
    }

    return 0;
}

int _tmain(int argc, TCHAR* argv[]) {
    ThreadData data = { NULL, TRUE, NULL };
    data.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL); // Event to signal threads to exit
    data.hResponseEvent = CreateEvent(NULL, TRUE, FALSE, NULL); // Event to signal when a response is received

    if (data.hEvent == NULL || data.hResponseEvent == NULL) {
        _tprintf(TEXT("Failed to create event. GLE=%d\n"), GetLastError());
        if (data.hEvent) CloseHandle(data.hEvent);
        if (data.hResponseEvent) CloseHandle(data.hResponseEvent);
        return 1;
    }

    if (!connectToServer(&data.hPipe)) {
        _tprintf(TEXT("Unable to connect to the server.\n"));
        CloseHandle(data.hEvent);
        CloseHandle(data.hResponseEvent);
        return 1;
    }

    HANDLE hLoginThread = CreateThread(NULL, 0, serverResponseThread, &data, 0, NULL);

    if (hLoginThread == NULL) {
        _tprintf(TEXT("Failed to create threads. GLE=%d\n"), GetLastError());
        data.shouldRun = FALSE;
        SetEvent(data.hEvent); // Signal other threads to exit
        CloseHandle(data.hEvent);
        CloseHandle(data.hResponseEvent);
        CloseHandle(data.hPipe);
        return 1;
    }

    // Wait for threads to complete or for the event to be signaled
    HANDLE handles[] = { hLoginThread, data.hEvent };
    WaitForMultipleObjects(2, handles, FALSE, INFINITE);

    // Cleanup
    CloseHandle(hLoginThread);
    CloseHandle(data.hEvent);
    CloseHandle(data.hResponseEvent);

    // Check if the pipe handle is valid before closing
    if (data.hPipe != INVALID_HANDLE_VALUE) {
        DisconnectNamedPipe(data.hPipe);
        CloseHandle(data.hPipe);
        data.hPipe = INVALID_HANDLE_VALUE; // Invalidate the handle after closing
    }

    return 0;
}
