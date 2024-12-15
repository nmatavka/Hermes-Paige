#include "Paige.h"
#include "pgMemMgr.h"
#include "pgText.h"
#include "defprocs.h" // Required for CommandCharInfo

extern paige_rec_ptr paigeDoc;

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

#include "Paige.h"
#include "pgMemMgr.h"
#include "pgText.h"
#include "defprocs.h" // Required for CommandCharInfo

extern paige_rec_ptr paigeDoc;

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

long GetCharacterType(pg_ref pg, long offset, long mask_bits) {
    if (pg) {
        return pgCharType(pg, offset, mask_bits);
    }
    return 0;
}

pg_short_t GetCharacterBytes(pg_ref pg, long offset, pg_char_ptr char_bytes) {
    if (pg) {
        return pgCharByte(pg, offset, char_bytes);
    }
    return 0;
}

void PaginateNow(pg_ref pg, long paginate_to, short use_best_guess) {
    if (pg) {
        pgPaginateNow(pg, paginate_to, use_best_guess);
    }
}

pg_boolean FindStyleInfo(pg_ref pg, long* begin_position, long* end_position, style_info_ptr match_style, style_info_ptr mask, style_info_ptr AND_mask) {
    if (pg) {
        return pgFindStyleInfo(pg, begin_position, end_position, match_style, mask, AND_mask);
    }
    return FALSE;
}

pg_char_ptr ExamineText(pg_ref pg, long offset, text_ref* text, long* length) {
    if (pg) {
        return pgExamineText(pg, offset, text, length);
    }
    return NULL;
}

void InvalidateSelection(pg_ref pg, long select_from, long select_to) {
    if (pg) {
        pgInvalSelect(pg, select_from, select_to);
    }
}

pg_boolean GetHighlightRegion(pg_ref pg, select_pair_ptr range, memory_ref select_list, shape_ref rgn) {
    if (pg && rgn) {
        return pgGetHiliteRgn(pg, range, select_list, rgn);
    }
    return FALSE;
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

void PaginateNow(pg_ref pg, long paginate_to, short use_best_guess) {
    if (pg) {
        pgPaginateNow(pg, paginate_to, use_best_guess);
    }
}

pg_boolean FindStyleInfo(pg_ref pg, long* begin_position, long* end_position, style_info_ptr match_style, style_info_ptr mask, style_info_ptr AND_mask) {
    if (pg) {
        return pgFindStyleInfo(pg, begin_position, end_position, match_style, mask, AND_mask);
    }
    return FALSE;
}

pg_char_ptr ExamineText(pg_ref pg, long offset, text_ref* text, long* length) {
    if (pg) {
        return pgExamineText(pg, offset, text, length);
    }
    return NULL;
}

void InvalidateSelection(pg_ref pg, long select_from, long select_to) {
    if (pg) {
        pgInvalSelect(pg, select_from, select_to);
    }
}

pg_boolean GetHighlightRegion(pg_ref pg, select_pair_ptr range, memory_ref select_list, shape_ref rgn) {
    if (pg && rgn) {
        return pgGetHiliteRgn(pg, range, select_list, rgn);
    }
    return FALSE;
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
