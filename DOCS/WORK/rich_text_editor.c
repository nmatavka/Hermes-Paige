#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "Paige.h"
#include "pgHLevel.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_PAINT:
            // Handle painting here
            break;
        case WM_KEYDOWN:
            // Handle key events for text input
            break;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    WNDCLASS wc = {0};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "RichTextEditor";

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(0, "RichTextEditor", "Rich Text Editor", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH, WINDOW_HEIGHT, NULL, NULL, hInstance, NULL);

    ShowWindow(hwnd, nShowCmd);

    // Initialize HERMES Paige
    pg_globals globals;
    pgMemStartup(&globals.mem_globals, 0);
    pgInit(&globals, &globals.mem_globals);

    // Main message loop
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // Cleanup
    pgMemShutdown(&globals.mem_globals);
    return 0;
}
