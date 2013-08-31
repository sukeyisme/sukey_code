#pragma once
DWORD WINAPI RealTimeDataThread(LPVOID CallbackObject);
WPARAM MessageLoop();
void ThreadOnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
#define WM_TERMINATE 100
#define WM_SILENTTERMINATE 101
#define WM_REFRESH 102
#define WM_GETSTOCKINFO 103