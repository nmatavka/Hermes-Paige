#include "Paige.h"
#include "pgMemMgr.h"
#include "pghtext.h"

extern paige_rec_ptr paigeDoc;

PG_PASCAL void HyperlinkCallback(paige_rec_ptr pg, pg_hyperlink_ptr hypertext, short command, short modifiers, long position, pg_char_ptr URL) {
    // Call the standard callback first to get default behaviour:
    pgStandardSourceCallback(pg, hypertext, command, modifiers, position, URL);
    switch(command) {
        case hyperlink_mousedown_verb:
            // Handle mouse down event
            break;
        case hyperlink_doubleclick_verb:
            // Handle double click event
            break;
        case hyperlink_mouseup_verb:
            // Handle mouse up event
            break;
        case hyperlink_delete_verb:
            // Handle link deletion
            break;
    }
}

long FindHyperlinkSourceByURL(paige_rec_ptr doc, long start_pos, long* end_pos, const char* url, pg_boolean partial_ok, pg_boolean case_insensitive, pg_boolean scroll_to) {
    if (doc) {
        return pgFindHyperlinkSource(doc, start_pos, end_pos, (pg_char_ptr)url, partial_ok, case_insensitive, scroll_to);
    }
    return -1;
}

long FindHyperlinkTargetByURL(paige_rec_ptr doc, long start_pos, long* end_pos, const char* url, pg_boolean partial_ok, pg_boolean case_insensitive, pg_boolean scroll_to) {
    if (doc) {
        return pgFindHyperlinkTarget(doc, start_pos, end_pos, (pg_char_ptr)url, partial_ok, case_insensitive, scroll_to);
    }
    return -1;
}

long FindHyperlinkSourceByID(paige_rec_ptr doc, long start_pos, long* end_pos, long id_num, pg_boolean scroll_to) {
    if (doc) {
        return pgFindHyperlinkSourceByID(doc, start_pos, end_pos, id_num, scroll_to);
    }
    return -1;
}

long FindHyperlinkTargetByID(paige_rec_ptr doc, long start_pos, long* end_pos, long id_num, pg_boolean scroll_to) {
    if (doc) {
        return pgFindHyperlinkTargetByID(doc, start_pos, end_pos, id_num, scroll_to);
    }
    return -1;
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

long SetHyperlinkSource(paige_rec_ptr doc, const char* url, const char* keyword_display, long start, long end, ht_callback callback, long type, long id_num, short state1_style, short state2_style, short state3_style, short draw_mode) {
    if (doc) {
        select_pair selection = {start, end};
        return pgSetHyperlinkSource(doc, &selection, (pg_char_ptr)url, (pg_char_ptr)keyword_display, callback, type, id_num, state1_style, state2_style, state3_style, draw_mode);
    }
    return 0;
}

long SetHyperlinkTarget(paige_rec_ptr doc, const char* url, long start, long end, ht_callback callback, long type, long id_num, short display_style, short draw_mode) {
    if (doc) {
        select_pair selection = {start, end};
        return pgSetHyperlinkTarget(doc, &selection, (pg_char_ptr)url, callback, type, id_num, display_style, draw_mode);
    }
    return 0;
}

void SetHypertextLink(paige_rec_ptr doc, const char* url, long start, long end, pg_boolean redraw) {
    if (doc) {
        pg_hyperlink link;
        link.applied_range.begin = start;
        link.applied_range.end = end;
        link.url = (pg_char_ptr)url;
        link.state1_style = pgNewHyperlinkStyle(doc, 0, 0, STYLE_COLOR_INTENSITY, X_UNDERLINE_BIT, FALSE);
        pgAddHyperlink(doc, &link, redraw);
    }
}

pg_boolean GetHypertextLink(paige_rec_ptr doc, long position, char* url, long url_size) {
    if (doc && url) {
        pg_hyperlink link;
        if (pgGetHyperlink(doc, position, &link)) {
            strncpy(url, (const char*)link.url, url_size);
            return TRUE;
        }
    }
    return FALSE;
}

void SetParagraphShading(paige_rec_ptr doc, long shading_color, pg_boolean redraw) {
    if (doc) {
        select_pair selection;
        pgGetSelection(doc, &selection.begin, &selection.end);
        par_info info;
        pgGetParInfo(doc, &selection, &info);
        info.table.border_shading = shading_color;
        pgSetParInfo(doc, &selection, &info, NULL, redraw);
    }
}

pg_boolean GetParagraphShading(paige_rec_ptr doc, long* shading_color) {
    if (doc && shading_color) {
        select_pair selection;
        pgGetSelection(doc, &selection.begin, &selection.end);
        par_info info;
        if (pgGetParInfo(doc, &selection, &info)) {
            *shading_color = info.table.border_shading;
            return TRUE;
        }
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

void SetScaling(pg_ref pg, pg_scale_ptr scale_factor, short draw_mode) {
    if (pg && scale_factor) {
        pgSetScaling(pg, scale_factor, draw_mode);
    }
}

void GetScaling(pg_ref pg, pg_scale_ptr scale_factor) {
    if (pg && scale_factor) {
        pgGetScaling(pg, scale_factor);
    }
}

void ScaleLong(long scale_factor, long origin, long PG_FAR *value) {
    if (value) {
        pgScaleLong(scale_factor, origin, value);
    }
}

void ScalePt(pg_scale_ptr scale_factor, co_ordinate_ptr amount_offset, co_ordinate_ptr pt) {
    if (scale_factor && pt) {
        pgScalePt(scale_factor, amount_offset, pt);
    }
}

void ScaleRect(pg_scale_ptr scale_factor, co_ordinate_ptr amount_offset, rectangle_ptr rect) {
    if (scale_factor && rect) {
        pgScaleRect(scale_factor, amount_offset, rect);
    }
}

void ScaleRectToRect(pg_scale_ptr scale_factor, rectangle_ptr src_rect, rectangle_ptr target_rect, co_ordinate_ptr offset_extra) {
    if (scale_factor && src_rect && target_rect) {
        pgScaleRectToRect(scale_factor, src_rect, target_rect, offset_extra);
    }
}

long PrintToPage(pg_ref pg, graf_device_ptr target, long starting_position, rectangle_ptr page_rect, short draw_mode) {
    if (pg && page_rect) {
        return pgPrintToPage(pg, target, starting_position, page_rect, draw_mode);
    }
    return 0;
}

short NumPages(pg_ref pg, rectangle_ptr page_rect) {
    if (pg && page_rect) {
        return pgNumPages(pg, page_rect);
    }
    return 0;
}

long FindPage(pg_ref pg, short page_num, rectangle_ptr page_rect) {
    if (pg && page_rect) {
        return pgFindPage(pg, page_num, page_rect);
    }
    return 0;
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
