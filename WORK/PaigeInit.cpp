#include <windows.h>
#include "Paige.h"
#include "pgMemMgr.h"

extern paige_rec_ptr paigeDoc;

void InitPaige(HWND hwnd) {
    // Initialize Paige document
    paigeDoc = pgNewShell(&globals->mem_globals);
    pgSetPort(paigeDoc, hwnd, NULL);
}

void CleanupPaige() {
    if (paigeDoc) {
        pgDispose(paigeDoc);
        paigeDoc = NULL;
    }
}

void InitializeTransColor(pg_globals_ptr globals, HWND hwnd) {
    if (globals) {
        HDC hdc = GetDC(hwnd);
        if (hdc) {
            COLORREF windowColor = GetBkColor(hdc);
            ReleaseDC(hwnd, hdc);
            globals->trans_color.red = GetRValue(windowColor);
            globals->trans_color.green = GetGValue(windowColor);
            globals->trans_color.blue = GetBValue(windowColor);
        }
    }
}
