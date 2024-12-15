#include <windows.h>
#include "Paige.h"
#include "PGHEADER/PGBASICS.H"
#include "PGHEADER/PGCLIPBD.H"
#include "PGHEADER/PGSELECT.H"

// Global variables
HINSTANCE hInst;
paige_rec_ptr paigeDoc;

// Function prototypes
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void InitPaige(HWND hwnd);
void SetFontStyle(paige_rec_ptr doc, const char* fontName, short fontSize, short fontStyle);
void CleanupPaige();
void CopyText();
void PasteText();
void CutText();
void UndoAction();

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASSEX wcex;
    HWND hWnd;
    MSG msg;

    hInst = hInstance;

    // Register window class
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = "RichTextEditor";
    wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

    RegisterClassEx(&wcex);

    // Create window
    hWnd = CreateWindow("RichTextEditor", "Basic Rich Text Editor", WS_OVERLAPPEDWINDOW,
                        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

    if (!hWnd) {
        return FALSE;
        SetFontStyle(paigeDoc, "Arial", 12, 0); // Example: Set default font
        pgShutdown(&m_Globals);
        pgMemShutdown(m_MemoryGlobals);
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    // Main message loop
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

void CopyText() {
    if (paigeDoc) {
        pgPrepareUndo(paigeDoc, undo_copy, NULL);
        pgCopyToClipboard(paigeDoc, NULL, 0, best_way);
    }
}

void PasteText() {
    if (paigeDoc) {
        pg_ref scrap = pgNewScrap();
        pgPrepareUndo(paigeDoc, undo_paste, scrap);
        pgPasteFromClipboard(paigeDoc, NULL, 0, best_way);
    }
}

void CutText() {
    if (paigeDoc) {
        pgPrepareUndo(paigeDoc, undo_cut, NULL);
        pgCutToClipboard(paigeDoc, NULL, 0, best_way);
    }
}

void UndoAction() {
    if (paigeDoc) {
        pgPrepareUndo(paigeDoc, undo_undo, NULL);
        pgUndo(paigeDoc, 1, best_way);
    }
}

void SetFontStyle(paige_rec_ptr doc, const char* fontName, short fontSize, short fontStyle) {
    if (doc) {
        style_info style;
        pgGetStyleInfo(doc, NULL, &style, NULL, best_way);

        // Set font name
        strncpy(style.font_name, fontName, sizeof(style.font_name) - 1);
        style.font_name[sizeof(style.font_name) - 1] = '\0';

        // Set font size
        style.point = fontSize;

        // Set font style (e.g., bold, italic)
        style.styles = fontStyle;

        pgSetStyleInfo(doc, NULL, &style, best_way);
    }
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_CREATE:
        InitPaige(hWnd);
            break;
        case ID_EDIT_COPY:
            CopyText();
            break;
        case ID_EDIT_PASTE:
            PasteText();
            break;
        case ID_EDIT_CUT:
            CutText();
            break;
        case ID_EDIT_UNDO:
            UndoAction();
            break;
    case WM_DESTROY:
        CleanupPaige();
        PostQuitMessage(0);
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // Add code to render Paige content here
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_COMMAND:
        // Handle menu commands for changing font, style, etc.
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

void InitPaige(HWND hwnd) {
    // Initialize Paige document
    pg_globals_ptr globals = /* Initialize or retrieve your pg_globals_ptr */;
    HWND hwndFocus = GetFocus();
    shape_ref vis_area = pgRectToShape(globals, /* Define your visible area rectangle */);
    shape_ref page_area = pgRectToShape(globals, /* Define your page area rectangle */);

    paigeDoc = pgNew(globals, (generic_var)hwndFocus, vis_area, page_area, MEM_NULL, 0);
    if (paigeDoc) {
        // Set up Paige document properties here
    }

    pgDisposeShape(vis_area);
    pgDisposeShape(page_area);
}

void CleanupPaige() {
    if (paigeDoc) {
        pgDisposeDoc(paigeDoc);
    }
}
