#include <windows.h>
#include "Paige.h"
#include "PGHEADER/PGBASICS.H"
#include "PGHEADER/PGCLIPBD.H"
#include "PGHEADER/PGSELECT.H"
#include "pgMemMgr.h"

short m_KeyModifiers = 0; // Declare the key modifiers variable
HINSTANCE hInst;
paige_rec_ptr paigeDoc;

// Function prototypes
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void InitPaige(HWND hwnd);
void InitVirtualMemory(pg_globals_ptr globals, int tempFile);
void UninitVirtualMemory(int tempFile);
void SetFontStyle(paige_rec_ptr doc, const char* fontName, short fontSize, short fontStyle);
void CleanupPaige();
long GetAttributes();
pg_boolean SetAttributes(long attributes);
void CopyText();
void PasteText();
void CutText();
void DeleteText();
void UndoAction();
void RedoAction();

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

void InitVirtualMemory(pg_globals_ptr globals, int tempFile) {
    InitVirtualMemory(globals, NULL, tempFile);
}

void UninitVirtualMemory(int tempFile) {
    _close(tempFile);
}

void CopyText() {
    if (paigeDoc) {
        if (OpenClipboard(NULL)) {
            pg_ref scrap = pgCopy(paigeDoc, NULL);
            if (scrap) {
                pgPutScrap(scrap, RegisterClipboardFormat("HERMES Paige"), pg_void_scrap);
                pgDispose(scrap);
            }
            CloseClipboard();
        }
    }
#define MAX_UNDO_STACK 16

undo_ref undoStack[MAX_UNDO_STACK];
short undoStackIndex = 0;

void PrepareUndo(short verb, long insertSize = 0, long insertPosition = 0) {
    undo_ref newUndoRef = MEM_NULL;
    undo_ref previousUndoRef = (undoStackIndex > 0) ? undoStack[undoStackIndex - 1] : MEM_NULL;

    if (verb == undo_typing || verb == undo_fwd_delete || verb == undo_backspace) {
        if (previousUndoRef && pgUndoType(previousUndoRef) == verb) {
            newUndoRef = pgPrepareUndo(paigeDoc, verb, (void PG_FAR *)previousUndoRef);
        } else {
            newUndoRef = pgPrepareUndo(paigeDoc, verb, NULL);
        }
    } else if (verb == undo_insert) {
        newUndoRef = pgPrepareUndo(paigeDoc, verb, (void PG_FAR *)&insertSize);
    } else if (verb == undo_app_insert) {
        long insertParams[2] = {insertPosition, insertSize};
        newUndoRef = pgPrepareUndo(paigeDoc, verb, (void PG_FAR *)insertParams);
    } else if (verb == undo_page_change || verb == undo_vis_change || verb == undo_exclude_change || verb == undo_doc_info || verb == undo_embed_insert) {
        newUndoRef = pgPrepareUndo(paigeDoc, verb, NULL);
    }

    if (!previousUndoRef || newUndoRef != previousUndoRef) {
        pgSetUndoRefCon(newUndoRef, 12345); // Example: Set a custom reference value
        if (undoStackIndex < MAX_UNDO_STACK) {
            undoStack[undoStackIndex++] = newUndoRef;
        } else {
            pgDisposeUndo(undoStack[0]);
            memmove(undoStack, undoStack + 1, (MAX_UNDO_STACK - 1) * sizeof(undo_ref));
            undoStack[MAX_UNDO_STACK - 1] = newUndoRef;
        }
    }
}

void UndoAction() {
    if (paigeDoc && undoStackIndex > 0) {
        undo_ref lastUndoRef = undoStack[--undoStackIndex];
        short undoType = pgUndoType(lastUndoRef);
        // Optionally, use undoType to update UI or log the action
        undo_ref redoRef = pgUndo(paigeDoc, lastUndoRef, TRUE, best_way);
        pgDisposeUndo(lastUndoRef);
        if (undoStackIndex < MAX_UNDO_STACK) {
            undoStack[undoStackIndex++] = redoRef;
        }
    }
}

void PasteText() {
    if (paigeDoc) {
        OpenClipboard(NULL);
        if (pgScrapAvail(RegisterClipboardFormat("HERMES Paige"))) {
            pg_ref scrap = pgGetScrap(&globals->mem_globals, RegisterClipboardFormat("HERMES Paige"), NULL);
            if (scrap) {
                pgPaste(paigeDoc, scrap, CURRENT_POSITION, false, best_way);
                pgDispose(scrap);
            }
        }
        CloseClipboard();
    }
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

void DeleteText() {
    if (paigeDoc) {
        long start, end;
        pgGetSelection(paigeDoc, &start, &end);
        if (start != end) {
            pgDelete(paigeDoc, &start, best_way);
        }
    }
}
    if (paigeDoc) {
        undo_ref redoRef = pgUndo(paigeDoc, lastUndoRef, TRUE, best_way);
        pgDisposeUndo(lastUndoRef);
        if (undoStackIndex < MAX_UNDO_STACK) {
            undoStack[undoStackIndex++] = redoRef;
        }
    }
}

void RedoAction() {
    if (paigeDoc && undoStackIndex > 0) {
        undo_ref lastRedoRef = undoStack[--undoStackIndex];
        undo_ref undoRef = pgUndo(paigeDoc, lastRedoRef, TRUE, best_way);
        pgDisposeUndo(lastRedoRef);
        if (undoStackIndex < MAX_UNDO_STACK) {
            undoStack[undoStackIndex++] = undoRef;
        }
    }
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
        CleanupPaige();
        PostQuitMessage(0);
        break;
    case WM_SETFOCUS:
        pgSetHiliteStates(paigeDoc, activate_verb, no_change_verb, TRUE);
        break;
    case WM_KILLFOCUS:
        pgSetHiliteStates(paigeDoc, deactivate_verb, no_change_verb, TRUE);
        break;
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

void GetShapeAreas(pg_ref pg, shape_ref vis_area, shape_ref page_area, shape_ref exclude_area) {
    pgGetAreas(pg, vis_area, page_area, exclude_area);
}

pg_ref ClonePaigeObject(pg_ref pg) {
    return pgDuplicate(pg);
}

void SetExtraStruct(pg_ref pg, void* extra_struct, long ref_id) {
    pgSetExtraStruct(pg, extra_struct, ref_id);
}

void* GetExtraStruct(pg_ref pg, long ref_id) {
    return pgGetExtraStruct(pg, ref_id);
}

long GetUniqueExtraStructID(pg_ref pg) {
    return pgExtraUniqueID(pg);
}
    pgAreaBounds(pg, page_bounds, vis_bounds);
}
    pgSetDevicePalette(pg, (generic_var)hPalette);
}

HPALETTE GetDevicePalette(pg_ref pg) {
    return (HPALETTE)pgGetDevicePalette(pg);
}
    // Initialize Paige globals with custom defaults
    pg_globals_ptr globals = /* Initialize or retrieve your pg_globals_ptr */;
    
    // Set default style
    globals->def_style.point = 0x00090000; // Example: Set default font size to 9 points
    strncpy(globals->def_style.font_name, "Arial", sizeof(globals->def_style.font_name) - 1);
    globals->def_style.font_name[sizeof(globals->def_style.font_name) - 1] = '\0';
    
    // Set default paragraph format
    globals->def_par.def_tab_space = 32; // Example: Set default tab spacing to 32
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

long GetAttributes() {
    if (paigeDoc) {
        return pgGetAttributes(paigeDoc);
    }
    return 0;
}

pg_boolean SetAttributes(long attributes) {
    if (paigeDoc) {
        return pgSetAttributes(paigeDoc, attributes);
    }
    return FALSE;
}
    if (paigeDoc) {
        pgDisposeDoc(paigeDoc);
    }
}
