#include <windows.h>
#include "Paige.h"
#include "PGHEADER/PGBASICS.H"
#include "PGHEADER/PGCLIPBD.H"
#include "PGHEADER/PGSELECT.H"
#include "pgMemMgr.h"
#include "pgTraps.h"
#include "pgTxrCPP.h"
#include "pgHLevel.h"
#include "pgTxr.h"

extern short m_KeyModifiers;
extern HINSTANCE hInst;
extern paige_rec_ptr paigeDoc;

// Function prototypes
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASSEX wcex;
    HWND hWnd;
    MSG msg;

    hInst = hInstance;

    // Define menu structure
    HMENU hMenu = CreateMenu();
    HMENU hFileMenu = CreatePopupMenu();
    HMENU hEditMenu = CreatePopupMenu();

    AppendMenu(hFileMenu, MF_STRING, ID_FILE_NEW, "New");
    AppendMenu(hFileMenu, MF_STRING, ID_FILE_OPEN, "Open");
    AppendMenu(hFileMenu, MF_STRING, ID_FILE_SAVE, "Save");
    AppendMenu(hFileMenu, MF_SEPARATOR, 0, NULL);
    AppendMenu(hFileMenu, MF_STRING, ID_FILE_EXIT, "Exit");

    AppendMenu(hEditMenu, MF_STRING, ID_EDIT_UNDO, "Undo");
    AppendMenu(hEditMenu, MF_STRING, ID_EDIT_REDO, "Redo");
    AppendMenu(hEditMenu, MF_SEPARATOR, 0, NULL);
    AppendMenu(hEditMenu, MF_STRING, ID_EDIT_CUT, "Cut");
    AppendMenu(hEditMenu, MF_STRING, ID_EDIT_COPY, "Copy");
    AppendMenu(hEditMenu, MF_STRING, ID_EDIT_PASTE, "Paste");
    AppendMenu(hEditMenu, MF_STRING, ID_EDIT_DELETE, "Delete");

    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hFileMenu, "File");
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hEditMenu, "Edit");
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
                        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, hMenu, hInstance, NULL);

    if (!hWnd) {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    // Initialize virtual memory
    int tempFile = _open("tempfile.tmp", _O_RDWR | _O_CREAT | _O_TEMPORARY, _S_IREAD | _S_IWRITE);
    if (tempFile != -1) {
        InitVirtualMemory(&globals->mem_globals, tempFile);
    }
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // Uninitialize virtual memory
    if (tempFile != -1) {
        UninitVirtualMemory(tempFile);
    }

    return (int)msg.wParam;
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
    case ID_EDIT_DELETE:
        DeleteText();
        break;
    case ID_EDIT_REDO:
        RedoAction();
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
            pgInsertPendingKeys(paigeDoc); // Ensure immediate insertion of pending keys
            pgScrollToView(paigeDoc, CURRENT_POSITION, 0, 0, TRUE, bits_emulate_or);
        }
        break;
    case WM_DESTROY:
        CleanupPaige();
        PostQuitMessage(0);
        break;
    case WM_SETFOCUS:
        pgSetHiliteStates(paigeDoc, activate_verb, no_change_verb, TRUE);
        break;
    case WM_KILLFOCUS:
        pgSetHiliteStates(paigeDoc, deactivate_verb, no_change_verb, TRUE);
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            pgDisplay(paigeDoc, NULL, MEM_NULL, MEM_NULL, NULL, direct_or);
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_HSCROLL:
        long h_pixels, v_pixels;
        switch (LOWORD(wParam)) {
            case SB_PAGELEFT:
                CalculateScrollPixels(paigeDoc, scroll_page, scroll_none, &h_pixels, &v_pixels);
                pgScroll(paigeDoc, scroll_page, scroll_none, best_way);
                break;
            case SB_LINELEFT:
                CalculateScrollPixels(paigeDoc, scroll_unit, scroll_none, &h_pixels, &v_pixels);
                pgScroll(paigeDoc, scroll_unit, scroll_none, best_way);
                break;
            case SB_PAGERIGHT:
                CalculateScrollPixels(paigeDoc, -scroll_page, scroll_none, &h_pixels, &v_pixels);
                pgScroll(paigeDoc, -scroll_page, scroll_none, best_way);
                break;
            case SB_LINERIGHT:
                CalculateScrollPixels(paigeDoc, -scroll_unit, scroll_none, &h_pixels, &v_pixels);
                pgScroll(paigeDoc, -scroll_unit, scroll_none, best_way);
                break;
            case SB_THUMBPOSITION:
            case SB_THUMBTRACK:
                short cur_h, cur_v, max_h, max_v;
                pgGetScrollValues(paigeDoc, &cur_h, &cur_v, &max_h, &max_v);
                pgSetScrollValues(paigeDoc, LOWORD(lParam), cur_v, TRUE, best_way);
                break;
        }
        UpdateScrollbars(paigeDoc, hWnd);
        break;

    case WM_VSCROLL:
        switch (LOWORD(wParam)) {
            case SB_PAGEDOWN:
                pgScroll(paigeDoc, scroll_none, scroll_page, best_way);
                break;
            case SB_LINEDOWN:
                pgScroll(paigeDoc, scroll_none, scroll_unit, best_way);
                break;
            case SB_PAGEUP:
                pgScroll(paigeDoc, scroll_none, -scroll_page, best_way);
                break;
            case SB_LINEUP:
                pgScroll(paigeDoc, scroll_none, -scroll_unit, best_way);
                break;
            case SB_TOP:
                pgScroll(paigeDoc, scroll_none, scroll_home, best_way);
                break;
            case SB_BOTTOM:
                pgScroll(paigeDoc, scroll_none, scroll_end, best_way);
                break;
            case SB_THUMBPOSITION:
            case SB_THUMBTRACK:
                pgGetScrollValues(paigeDoc, &cur_h, &cur_v, &max_h, &max_v);
                pgSetScrollValues(paigeDoc, cur_h, LOWORD(lParam), TRUE, best_way);
                break;
        }
        UpdateScrollbars(paigeDoc, hWnd);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
}
