#include <windows.h>
#include "Paige.h"
#include "PGHEADER/PGBASICS.H"
#include "PGHEADER/PGCLIPBD.H"
#include "PGHEADER/PGSELECT.H"

short m_KeyModifiers = 0; // Declare the key modifiers variable
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
    case WM_KEYDOWN:
        {
            pg_globals globals = /* Retrieve your pg_globals */;
            pg_short_t verb;

            switch (wParam) {
                case VK_SHIFT:
                    m_KeyModifiers |= EXTEND_MOD_BIT;
                    break;
                case VK_CONTROL:
                    m_KeyModifiers |= CONTROL_MOD_BIT;
                    break;
                case VK_LEFT:
                    SendMessage(hWnd, WM_CHAR, globals->left_arrow_char, 0);
                    break;
                case VK_UP:
                    SendMessage(hWnd, WM_CHAR, globals->up_arrow_char, 0);
                    break;
                case VK_RIGHT:
                    SendMessage(hWnd, WM_CHAR, globals->right_arrow_char, 0);
                    break;
                case VK_DOWN:
                    SendMessage(hWnd, WM_CHAR, globals->down_arrow_char, 0);
                    break;
                case VK_HOME:
                    verb = (m_KeyModifiers & CONTROL_MOD_BIT) ? home_caret : begin_line_caret;
                    if (m_KeyModifiers & EXTEND_MOD_BIT) verb |= EXTEND_CARET_FLAG;
                    pgSetCaretPosition(paigeDoc, verb, TRUE);
                    pgScrollToView(paigeDoc, CURRENT_POSITION, 0, 0, TRUE, bits_emulate_or);
                    break;
                case VK_END:
                    verb = (m_KeyModifiers & CONTROL_MOD_BIT) ? doc_bottom_caret : end_line_caret;
                    if (m_KeyModifiers & EXTEND_MOD_BIT) verb |= EXTEND_CARET_FLAG;
                    pgSetCaretPosition(paigeDoc, verb, TRUE);
                    pgScrollToView(paigeDoc, CURRENT_POSITION, 0, 0, TRUE, bits_emulate_or);
                    break;
                case VK_PRIOR:
                    SendMessage(hWnd, WM_VSCROLL, SB_PAGEUP, 0);
                    break;
                case VK_NEXT:
                    SendMessage(hWnd, WM_VSCROLL, SB_PAGEDOWN, 0);
                    break;
                case VK_DELETE:
                    if (m_KeyModifiers & EXTEND_MOD_BIT) {
                        long start, end;
                        pg_ref scrap;
                        pgGetSelection(paigeDoc, &start, &end);
                        if (start != end) {
                            scrap = pgCut(paigeDoc, &start, &end);
                            if (scrap) {
                                OpenClipboard();
                                pgPutScrap(scrap, 0, pg_void_scrap);
                                CloseClipboard();
                                pgDispose(scrap);
                            }
                        }
                    } else {
                        SendMessage(hWnd, WM_CHAR, globals->fwd_delete_char, 0);
                    }
                    break;
                case VK_INSERT:
                    if (m_KeyModifiers & CONTROL_MOD_BIT) {
                        long start, end;
                        pg_ref scrap;
                        pgGetSelection(paigeDoc, &start, &end);
                        if (start != end) {
                            scrap = pgCopy(paigeDoc, NULL);
                            if (scrap) {
                                OpenClipboard();
                                pgPutScrap(scrap, 0, pg_void_scrap);
                                CloseClipboard();
                                pgDispose(scrap);
                            }
                        }
                    } else if (m_KeyModifiers & EXTEND_MOD_BIT) {
                        pg_ref scrap = MEM_NULL;
                        OpenClipboard();
                        scrap = pgGetScrap(globals, 0, HookEmbedProc);
                        CloseClipboard();
                        if (scrap) {
                            pgPaste(paigeDoc, scrap, CURRENT_POSITION, false, best_way);
                            pgDispose(scrap);
                        }
                    }
                    pgScrollToView(paigeDoc, CURRENT_POSITION, 0, 0, TRUE, bits_emulate_or);
                    break;
            }
        }
        break;
    case WM_KEYUP:
        switch (wParam) {
            case VK_SHIFT:
                m_KeyModifiers &= (~EXTEND_MOD_BIT);
                break;
            case VK_CONTROL:
                m_KeyModifiers &= (~CONTROL_MOD_BIT);
                break;
        }
        break;
    case WM_CHAR:
        {
            pg_char the_char = (pg_char)wParam;
            pgInsert(paigeDoc, &the_char, 1, CURRENT_POSITION, key_insert_mode, m_KeyModifiers, best_way);
            pgScrollToView(paigeDoc, CURRENT_POSITION, 0, 0, TRUE, bits_emulate_or);
        }
        break;
        CleanupPaige();
        PostQuitMessage(0);
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            pgDisplay(paigeDoc, NULL, MEM_NULL, MEM_NULL, NULL, direct_or);
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
    RECT clientRect;
    GetClientRect(hwnd, &clientRect);
    rectangle visRect = {{clientRect.top, clientRect.left}, {clientRect.bottom, clientRect.right}};
    shape_ref vis_area = pgRectToShape(globals, &visRect);
    shape_ref page_area = pgRectToShape(globals, &visRect);

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
