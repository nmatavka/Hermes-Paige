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

void ChangeHyperlinkSource(paige_rec_ptr doc, long position, select_pair_ptr selection, const char* url, ht_callback callback, short display_style, short draw_mode) {
    if (doc) {
        pgChangeHyperlinkSource(doc, position, selection, (pg_char_ptr)url, callback, display_style, draw_mode);
    }
}

void ChangeHyperlinkTarget(paige_rec_ptr doc, long position, select_pair_ptr selection, const char* url, ht_callback callback, short display_style, short draw_mode) {
    if (doc) {
        pgChangeHyperlinkTarget(doc, position, selection, (pg_char_ptr)url, callback, display_style, draw_mode);
    }
}

long PointInHyperlinkSource(paige_rec_ptr doc, co_ordinate_ptr point) {
    if (doc) {
        return pgPtInHyperlinkSource(doc, point);
    }
    return -1;
}

long PointInHyperlinkTarget(paige_rec_ptr doc, co_ordinate_ptr point) {
    if (doc) {
        return pgPtInHyperlinkTarget(doc, point);
    }
    return -1;
}

void SetHyperlinkSourceState(paige_rec_ptr doc, long position, short state, pg_boolean redraw) {
    if (doc) {
        pgSetHyperlinkSourceState(doc, position, state, redraw);
    }
}

void SetHyperlinkTargetState(paige_rec_ptr doc, long position, short state, pg_boolean redraw) {
    if (doc) {
        pgSetHyperlinkTargetState(doc, position, state, redraw);
    }
}

void SetHyperlinkCallback(paige_rec_ptr doc, ht_callback source_callback, ht_callback target_callback) {
    if (doc) {
        pgSetHyperlinkCallback(doc, source_callback, target_callback);
    }
}

void DeleteHyperlinkSource(paige_rec_ptr doc, long position, pg_boolean redraw) {
    if (doc) {
        pgDeleteHyperlinkSource(doc, position, redraw);
    }
}

void DeleteHyperlinkTarget(paige_rec_ptr doc, long position, pg_boolean redraw) {
    if (doc) {
        pgDeleteHyperlinkTarget(doc, position, redraw);
    }
}

pg_boolean GetSourceURL(paige_rec_ptr doc, long position, char* url, short max_size) {
    if (doc) {
        return pgGetSourceURL(doc, position, (pg_char_ptr)url, max_size);
    }
    return FALSE;
}

pg_boolean GetTargetURL(paige_rec_ptr doc, long position, char* url, short max_size) {
    if (doc) {
        return pgGetTargetURL(doc, position, (pg_char_ptr)url, max_size);
    }
    return FALSE;
}

long GetSourceID(paige_rec_ptr doc, long position) {
    if (doc) {
        return pgGetSourceID(doc, position);
    }
    return 0;
}

long GetTargetID(paige_rec_ptr doc, long position) {
    if (doc) {
        return pgGetTargetID(doc, position);
    }
    return 0;
}

pg_boolean GetHyperlinkSourceInfo(paige_rec_ptr doc, long position, pg_boolean closest_one, pg_hyperlink_ptr hyperlink) {
    if (doc) {
        return pgGetHyperlinkSourceInfo(doc, position, closest_one, hyperlink);
    }
    return FALSE;
}

pg_boolean GetHyperlinkTargetInfo(paige_rec_ptr doc, long position, pg_boolean closest_one, pg_hyperlink_ptr hyperlink) {
    if (doc) {
        return pgGetHyperlinkTargetInfo(doc, position, closest_one, hyperlink);
    }
    return FALSE;
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
