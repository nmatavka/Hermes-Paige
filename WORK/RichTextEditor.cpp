#include <windows.h>
#include "Paige.h"
#include "PGHEADER/PGBASICS.H"
#include "PGHEADER/PGCLIPBD.H"
#include "PGHEADER/PGSELECT.H"
#include "pgMemMgr.h"
#include "pgTraps.h"
#include "pgHLevel.h"

short m_KeyModifiers = 0; // Declare the key modifiers variable
HINSTANCE hInst;
paige_rec_ptr paigeDoc;

// Function prototypes
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void pgDrawScrollProc(paige_rec_ptr pg, shape_ref update_rgn, co_ordinate_ptr scroll_pos, pg_boolean post_call) {
    if (pg && update_rgn) {
        HDC hdc = GetDC((HWND)pg->port.window);
        if (hdc) {
            // Repaint the uncovered area using the update region
            FillRgn(hdc, (HRGN)update_rgn, (HBRUSH)(COLOR_WINDOW + 1));
            ReleaseDC((HWND)pg->port.window, hdc);
        }
    }
}
}
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

void InitPaige(HWND hwnd);

void SetPageAttributes(pg_ref pg, long attributes) {
    if (pg) {
        UseMemory(pg);
        pg->doc_info.attributes = attributes;
        UnuseMemory(pg);
    }
}

long GetPageAttributes(pg_ref pg) {
    if (pg) {
        UseMemory(pg);
        long attributes = pg->doc_info.attributes;
        UnuseMemory(pg);
        return attributes;
    }
    return 0;
}

void SetPageColor(pg_ref pg, color_value_ptr color) {
    pgSetPageColor(pg, color);
}

void GetPageColor(pg_ref pg, color_value_ptr color) {
    pgGetPageColor(pg, color);
}

void SetTransparentColor(pg_globals_ptr globals, color_value trans_color) {
    globals->trans_color = trans_color;
}

color_value GetTransparentColor(pg_globals_ptr globals) {
    return globals->trans_color;
}

void ErasePageArea(pg_ref pg, shape_ref vis_area) {
    pgErasePageArea(pg, vis_area);
}

void FillPageAreaWithBackground(pg_ref pg, shape_ref vis_area) {
    if (pg) {
        color_value current_color;
        GetPageColor(pg, &current_color);
        pgErasePageArea(pg, vis_area);
    }
}

void OffsetShape(shape_ref the_shape, long h, long v) {
    pgOffsetShape(the_shape, h, v);
}

void InsetShape(shape_ref the_shape, long h, long v) {
    pgInsetShape(the_shape, h, v);
}

pg_short_t PointInShape(shape_ref the_shape, co_ordinate_ptr point, co_ordinate_ptr offset_extra, co_ordinate_ptr inset_extra, pg_scale_ptr scaling) {
    return pgPtInShape(the_shape, point, offset_extra, inset_extra, scaling);
}

pg_short_t SectionRectInShape(shape_ref the_shape, rectangle_ptr rect, rectangle_ptr sect_rect) {
    return pgSectRectInShape(the_shape, rect, sect_rect);
}

void ShapeBounds(shape_ref the_shape, rectangle_ptr bounds) {
    pgShapeBounds(the_shape, bounds);
}

pg_boolean EmptyShape(shape_ref the_shape) {
    return pgEmptyShape(the_shape);
}

pg_boolean EqualShapes(shape_ref shape1, shape_ref shape2) {
    return pgEqualShapes(shape1, shape2);
}

pg_boolean SectionShape(shape_ref shape1, shape_ref shape2, shape_ref result_shape) {
    return pgSectShape(shape1, shape2, result_shape);
}

void DifferenceShape(shape_ref shape1, shape_ref shape2, shape_ref result_shape) {
    pgDiffShape(shape1, shape2, result_shape);
}

void EraseShape(pg_ref pg, shape_ref the_shape, pg_scale_ptr scale_factor, co_ordinate_ptr offset_extra, rectangle_ptr vis_bounds) {
    pgEraseShape(pg, the_shape, scale_factor, offset_extra, vis_bounds);
}

void OffsetAreas(pg_ref pg, long h, long v, pg_boolean offset_page, pg_boolean offset_vis, pg_boolean offset_exclude) {
    pgOffsetAreas(pg, h, v, offset_page, offset_vis, offset_exclude);
}

shape_ref CreateShape(pg_globals_ptr globals, rectangle_ptr rect) {
    return pgRectToShape(globals, rect);
}

void SetShapeToRectangle(shape_ref the_shape, rectangle_ptr rect) {
    pgSetShapeRect(the_shape, rect);
}

void AddRectangleToShape(shape_ref the_shape, rectangle_ptr rect) {
    pgAddRectToShape(the_shape, rect);
}

void DisposeShape(shape_ref the_shape) {
    pgDisposeShape(the_shape);
}

void ConvertRectToRectangle(Rect PG_FAR *r, rectangle_ptr pg_rect) {
    RectToRectangle(r, pg_rect);
}

void ConvertRectangleToRect(rectangle_ptr pg_rect, co_ordinate_ptr offset, Rect PG_FAR *r) {
    RectangleToRect(pg_rect, offset, r);
}
void SetPointSize(paige_rec_ptr doc, long point_size, pg_boolean redraw) {
    if (doc) {
        select_pair selection;
        pgGetSelection(doc, &selection.begin, &selection.end);
        pgSetPointSize(doc, point_size, &selection, redraw);
    }
}

pg_boolean GetPointSize(paige_rec_ptr doc, long* point_size) {
    if (doc) {
        select_pair selection;
        pgGetSelection(doc, &selection.begin, &selection.end);
        return pgGetPointSize(doc, point_size, &selection);
    }
    return FALSE;
}

void SetFontByName(paige_rec_ptr doc, const char* fontName, pg_boolean redraw) {
    if (doc) {
        select_pair selection;
        pgGetSelection(doc, &selection.begin, &selection.end);
        pgSetFontByName(doc, (LPSTR)fontName, &selection, redraw);
    }
}

pg_boolean GetFontByName(paige_rec_ptr doc, char* fontName) {
    if (doc) {
        return pgGetFontByName(doc, (LPSTR)fontName);
    }
    return FALSE;
}

void SetIndents(paige_rec_ptr doc, long left_indent, long right_indent, long first_line_indent) {
    if (doc) {
        pgSetIndents(doc, left_indent, right_indent, first_line_indent);
    }
}

void SetSelection(paige_rec_ptr doc, long begin_sel, long end_sel, short modifiers, pg_boolean show_hilite) {
    if (doc) {
        pgSetSelection(doc, begin_sel, end_sel, modifiers, show_hilite);
    }
}

void GetSelection(paige_rec_ptr doc, long* begin_sel, long* end_sel) {
    if (doc) {
        pgGetSelection(doc, begin_sel, end_sel);
    }
}

void GetIndents(paige_rec_ptr doc, long* left_indent, long* right_indent, long* first_line_indent) {
    if (doc) {
        pgGetIndents(doc, left_indent, right_indent, first_line_indent);
    }
}

void ExtendSelection(paige_rec_ptr doc, long amount_ext, short modifiers, pg_boolean show_hilite) {
    if (doc) {
        pgExtendSelection(doc, amount_ext, modifiers, show_hilite);
    }
}

void ShapeToSelections(paige_rec_ptr doc, shape_ref the_shape, memory_ref selections) {
    if (doc && the_shape && selections) {
        pgShapeToSelections(doc, the_shape, selections);
    }
}

void SelectToShape(paige_rec_ptr doc, memory_ref select_shape, pg_boolean show_hilite) {
    if (doc && select_shape) {
        pgSelectToShape(doc, select_shape, show_hilite);
    }
}

void CalculateScrollPixels(paige_rec_ptr doc, short h_verb, short v_verb, long* h_pixels, long* v_pixels) {
    pgScrollUnitsToPixels(doc, h_verb, v_verb, TRUE, FALSE, h_pixels, v_pixels);
}

void UpdateScrollbars(paige_rec_ptr doc, HWND hWnd) {
    short h_value, v_value, max_h, max_v;

    if (pgGetScrollValues(doc, &h_value, &v_value, &max_h, &max_v)) {
        if (max_v < 1) max_v = 1; // Ensure scrollbar doesn't disappear
        SetScrollRange(hWnd, SB_VERT, 0, max_v, FALSE);
        SetScrollRange(hWnd, SB_HORZ, 0, max_h, FALSE);
        SetScrollPos(hWnd, SB_VERT, v_value, TRUE);
        SetScrollPos(hWnd, SB_HORZ, h_value, TRUE);
    }
}
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


#define MAX_UNDO_STACK 16

undo_ref undoStack[MAX_UNDO_STACK];
short undoStackIndex = 0;

void CopyText() {

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
        if (OpenClipboard(NULL)) {
            if (pgScrapAvail(RegisterClipboardFormat("HERMES Paige"))) {
                pg_ref scrap = pgGetScrap(&globals->mem_globals, RegisterClipboardFormat("HERMES Paige"), NULL);
                if (scrap) {
                    pgPaste(paigeDoc, scrap, CURRENT_POSITION, false, best_way);
                    pgDispose(scrap);
                }
            }
            CloseClipboard();
        }
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

void ApplyLogFontStyles(paige_rec_ptr doc, LOGFONT logFont) {
    if (doc) {
        // Set font by name
        pgSetFontByName(doc, logFont.lfFaceName, NULL, FALSE);

        // Set point size
        pgSetPointSize(doc, abs(logFont.lfHeight), NULL, FALSE);

        // Set style attributes
        long styleBits = 0;
        if (logFont.lfWeight == FW_BOLD) styleBits |= X_BOLD_BIT;
        if (logFont.lfItalic) styleBits |= X_ITALIC_BIT;
        if (logFont.lfUnderline) styleBits |= X_UNDERLINE_BIT;
        if (logFont.lfStrikeOut) styleBits |= X_STRIKEOUT_BIT;

        long setBits = (styleBits == X_PLAIN_TEXT) ? X_ALL_STYLES : styleBits;

        // Apply styles and redraw
        pgSetStyleBits(doc, styleBits, setBits, NULL, TRUE);
    }
}

void SetParagraphFormat(paige_rec_ptr doc, par_info_ptr info, par_info_ptr mask, pg_boolean redraw) {
    if (doc) {
        select_pair selection;
        pgGetSelection(doc, &selection.begin, &selection.end);
        pgSetParInfo(doc, &selection, info, mask, redraw);
    }
}

pg_boolean GetParagraphFormat(paige_rec_ptr doc, par_info_ptr info) {
    if (doc) {
        select_pair selection;
        pgGetSelection(doc, &selection.begin, &selection.end);
        return pgGetParInfo(doc, &selection, info);
    }
    return FALSE;
}

void GetDocumentInfo(pg_ref pg, pg_doc_ptr doc_info) {
    if (pg && doc_info) {
        pgGetDocInfo(pg, doc_info);
    }
}

void SetDocumentInfo(pg_ref pg, pg_doc_ptr doc_info, pg_boolean inval_text, short draw_mode) {
    if (pg && doc_info) {
        pgSetDocInfo(pg, doc_info, inval_text, draw_mode);
    }
}

void SetRepeatOffset(pg_ref pg, co_ordinate repeat_offset) {
    if (pg) {
        UseMemory(pg);
        pg->doc_info.repeat_offset = repeat_offset;
        UnuseMemory(pg);
    }
}

co_ordinate GetRepeatOffset(pg_ref pg) {
    if (pg) {
        UseMemory(pg);
        co_ordinate offset = pg->doc_info.repeat_offset;
        UnuseMemory(pg);
        return offset;
    }
    return {0, 0};
}

void SetRepeatSlop(pg_ref pg, long repeat_slop) {
    if (pg) {
        UseMemory(pg);
        pg->doc_info.repeat_slop = repeat_slop;
        UnuseMemory(pg);
    }
}

long GetRepeatSlop(pg_ref pg) {
    if (pg) {
        UseMemory(pg);
        long slop = pg->doc_info.repeat_slop;
        UnuseMemory(pg);
        return slop;
    }
    return 0;
}

void SetRepeatStop(pg_ref pg, long repeat_stop) {
    if (pg) {
        UseMemory(pg);
        pg->doc_info.repeat_stop = repeat_stop;
        UnuseMemory(pg);
    }
}

long GetRepeatStop(pg_ref pg) {
    if (pg) {
        UseMemory(pg);
        long stop = pg->doc_info.repeat_stop;
        UnuseMemory(pg);
        return stop;
    }
    return 0;
}

void SetPageModifyProc(pg_ref pg, void (PG_PASCAL *page_modify_proc)(paige_rec_ptr, long, rectangle_ptr)) {
    if (pg) {
        UseMemory(pg);
        pg->page_modify_proc = page_modify_proc;
        UnuseMemory(pg);
    }
}

pg_short_t NumContainers(pg_ref pg) {
    if (pg) {
        return pgNumContainers(pg);
    }
    return 0;
}

void InsertContainer(pg_ref pg, rectangle_ptr container, pg_short_t position, long ref_con, short draw_mode) {
    if (pg) {
        pgInsertContainer(pg, container, position, ref_con, draw_mode);
    }
}

void GetContainer(pg_ref pg, pg_short_t position, pg_boolean include_scroll, pg_boolean include_scale, rectangle_ptr container) {
    if (pg) {
        pgGetContainer(pg, position, include_scroll, include_scale, container);
    }
}

long GetContainerRefCon(pg_ref pg, pg_short_t position) {
    if (pg) {
        return pgGetContainerRefCon(pg, position);
    }
    return 0;
}

void SetContainerRefCon(pg_ref pg, pg_short_t position, long ref_con) {
    if (pg) {
        pgSetContainerRefCon(pg, position, ref_con);
    }
}

void RemoveContainer(pg_ref pg, pg_short_t position, short draw_mode) {
    if (pg) {
        pgRemoveContainer(pg, position, draw_mode);
    }
}

void ReplaceContainer(pg_ref pg, rectangle_ptr container, pg_short_t position, short draw_mode) {
    if (pg) {
        pgReplaceContainer(pg, container, position, draw_mode);
    }
}

void SwapContainers(pg_ref pg, pg_short_t container1, pg_short_t container2, short draw_mode) {
    if (pg) {
        pgSwapContainers(pg, container1, container2, draw_mode);
    }
}

pg_short_t PointInContainer(pg_ref pg, co_ordinate_ptr point, co_ordinate_ptr inset_extra) {
    if (pg) {
        return pgPtInContainer(pg, point, inset_extra);
    }
    return 0;
}

pg_short_t CharToContainer(pg_ref pg, long offset) {
    if (pg) {
        return pgCharToContainer(pg, offset);
    }
    return 0;
}

long ContainerToChar(pg_ref pg, pg_short_t position) {
    if (pg) {
        return pgContainerToChar(pg, position);
    }
    return -1;
}


pg_short_t NumExclusions(pg_ref pg) {
    if (pg) {
        return pgNumExclusions(pg);
    }
    return 0;
}

void InsertExclusion(pg_ref pg, rectangle_ptr exclusion, pg_short_t position, long ref_con, short draw_mode) {
    if (pg) {
        pgInsertExclusion(pg, exclusion, position, ref_con, draw_mode);
    }
}

void GetExclusion(pg_ref pg, pg_short_t position, pg_boolean include_scroll, pg_boolean include_scale, rectangle_ptr exclusion) {
    if (pg && position > 0 && position <= pgNumExclusions(pg)) {
        pgGetExclusion(pg, position, include_scroll, include_scale, exclusion);
    }
}

long GetExclusionRefCon(pg_ref pg, pg_short_t position) {
    if (pg && position > 0 && position <= pgNumExclusions(pg)) {
        return pgGetExclusionRefCon(pg, position);
    }
    return 0;
}

void SetExclusionRefCon(pg_ref pg, pg_short_t position, long ref_con) {
    if (pg && position > 0 && position <= pgNumExclusions(pg)) {
        pgSetExclusionRefCon(pg, position, ref_con);
    }
}

void RemoveExclusion(pg_ref pg, pg_short_t position, short draw_mode) {
    if (pg && position > 0 && position <= pgNumExclusions(pg)) {
        // Perform any necessary cleanup of refCon here before removal
        pgRemoveExclusion(pg, position, draw_mode);
    } else {
        // Handle invalid position case
        // Log or handle the error as needed
    }
}

void SwapExclusions(pg_ref pg, pg_short_t exclusion1, pg_short_t exclusion2, short draw_mode) {
    if (pg && exclusion1 > 0 && exclusion1 <= pgNumExclusions(pg) &&
        exclusion2 > 0 && exclusion2 <= pgNumExclusions(pg)) {
        pgSwapExclusions(pg, exclusion1, exclusion2, draw_mode);
    } else {
        // Handle invalid exclusion case
        // Log or handle the error as needed
    }
}

void ReplaceExclusion(pg_ref pg, rectangle_ptr exclusion, pg_short_t position, short draw_mode) {
    if (pg && position >= 1 && position <= pgNumExclusions(pg)) {
        pgReplaceExclusion(pg, exclusion, position, draw_mode);
    } else {
        // Handle invalid position case
        // Log or handle the error as needed
    }
}

void AttachParExclusion(pg_ref pg, long position, pg_short_t index, short draw_mode) {
    if (pg && index > 0 && index <= pgNumExclusions(pg)) {
        pgAttachParExclusion(pg, position, index, draw_mode);
    } else {
        // Handle invalid index case
        // Log or handle the error as needed
    }
}

long GetAttachedPar(pg_ref pg, pg_short_t exclusion) {
    if (pg && exclusion > 0 && exclusion <= pgNumExclusions(pg)) {
        return pgGetAttachedPar(pg, exclusion);
    }
    return -1;
}

void InsertExclusionShape(pg_ref pg, pg_short_t position, shape_ref exclude_shape, short draw_mode) {
    if (pg) {
        pgInsertExclusionShape(pg, position, exclude_shape, draw_mode);
    }
}

void SetPageMargins(pg_ref pg, rectangle_ptr margins) {
    if (pg) {
        UseMemory(pg);
        pg->doc_info.margins = *margins;
        UnuseMemory(pg);
    }
}

void GetPageMargins(pg_ref pg, rectangle_ptr margins) {
    if (pg) {
        UseMemory(pg);
        *margins = pg->doc_info.margins;
        UnuseMemory(pg);
    }
}

void SetScrollPosition(paige_rec_ptr doc, long h_pos, long v_pos) {
    if (doc) {
        UseMemory(doc);
        doc->scroll_position.h = h_pos;
        doc->scroll_position.v = v_pos;
        UnuseMemory(doc);
    }
}

void HandleWindowOriginChange(paige_rec_ptr doc, co_ordinate_ptr original_origin, co_ordinate_ptr new_origin) {
    if (doc) {
        pgWindowOriginChanged(doc, original_origin, new_origin);
    }
}

void Scroll(paige_rec_ptr doc, short h_verb, short v_verb, short draw_mode) {
    if (doc) {
        long h_pixels, v_pixels;
        CalculateScrollPixels(doc, h_verb, v_verb, &h_pixels, &v_pixels);
        SetScrollPosition(doc, h_pixels, v_pixels);
        pgScroll(doc, h_verb, v_verb, draw_mode);
    }
}

pg_boolean ScrollToView(paige_rec_ptr doc, long text_offset, short h_extra, short v_extra, short align_line, short draw_mode) {
    if (doc) {
        return pgScrollToView(doc, text_offset, h_extra, v_extra, align_line, draw_mode);
    }
    return FALSE;
}

pg_boolean GetTabBase(paige_rec_ptr doc, long* tab_base) {
    if (doc) {
        select_pair selection;
        pgGetSelection(doc, &selection.begin, &selection.end);
        return pgGetTabBase(doc, tab_base, &selection);
    }
    return FALSE;
}

void SetTab(paige_rec_ptr doc, long tab_value, pg_boolean redraw) {
    if (doc) {
        select_pair selection;
        pgGetSelection(doc, &selection.begin, &selection.end);
        pgSetTabs(doc, tab_value, &selection, redraw);
    }
}

void SetTabBase(paige_rec_ptr doc, long tab_base, pg_boolean redraw) {
    if (doc) {
        select_pair selection;
        pgGetSelection(doc, &selection.begin, &selection.end);
        pgSetTabBase(doc, tab_base, &selection, redraw);
    }
}

void SetDiscontinuousSelection(paige_rec_ptr doc, memory_ref select_list, long extra_offset, pg_boolean show_hilite) {
    if (doc && select_list) {
        pgSetSelectionList(doc, select_list, extra_offset, show_hilite);
    }
}

memory_ref GetDiscontinuousSelection(paige_rec_ptr doc, pg_boolean for_paragraph) {
    if (doc) {
        return pgGetSelectionList(doc, for_paragraph);
    }
    return MEM_NULL;
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
}

void SetDevicePalette(pg_ref pg, HPALETTE hPalette) {
    pgSetDevicePalette(pg, (generic_var)hPalette);
}

HPALETTE GetDevicePalette(pg_ref pg) {
    return (HPALETTE)pgGetDevicePalette(pg);
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
}
