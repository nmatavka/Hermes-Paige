#ifndef UTILITIES_H
#define UTILITIES_H

void FindWordBoundaries(pg_ref pg, long offset, long* first_byte, long* last_byte, pg_boolean left_side, pg_boolean smart_select) {
    if (pg) {
        pgFindWord(pg, offset, first_byte, last_byte, left_side, smart_select);
    }
}

void FindCtlWordBoundaries(pg_ref pg, long offset, long* first_byte, long* last_byte, short left_side) {
    if (pg) {
        pgFindCtlWord(pg, offset, first_byte, last_byte, left_side);
    }
}

void FindParagraphBoundaries(pg_ref pg, long offset, long* first_byte, long* last_byte) {
    if (pg) {
        pgFindPar(pg, offset, first_byte, last_byte);
    }
}

void FindLineBoundaries(pg_ref pg, long offset, long* first_byte, long* last_byte) {
    if (pg) {
        pgFindLine(pg, offset, first_byte, last_byte);
    }
}

long GetNumberOfLines(pg_ref pg) {
    if (pg) {
        return pgNumLines(pg);
    }
    return 0;
}

long OffsetToLineNumber(pg_ref pg, long offset, pg_boolean line_end_has_precedence) {
    if (pg) {
        return pgOffsetToLineNum(pg, offset, line_end_has_precedence);
    }
    return 0;
}

void LineNumberToOffset(pg_ref pg, long line_num, long* begin_offset, long* end_offset) {
    if (pg) {
        pgLineNumToOffset(pg, line_num, begin_offset, end_offset);
    }
}

long GetNumberOfParagraphs(pg_ref pg) {
    if (pg) {
        return pgNumPars(pg);
    }
    return 0;
}

long OffsetToParagraphNumber(pg_ref pg, long offset) {
    if (pg) {
        return pgOffsetToParNum(pg, offset);
    }
    return 0;
}

void ParagraphNumberToOffset(pg_ref pg, long par_num, long* begin_offset, long* end_offset) {
    if (pg) {
        pgParNumToOffset(pg, par_num, begin_offset, end_offset);
    }
}

void LineNumberToBounds(pg_ref pg, long line_num, pg_boolean want_scrolled, pg_boolean want_scaled, pg_boolean line_end_has_precedence, rectangle_ptr bounds) {
    if (pg) {
        pgLineNumToBounds(pg, line_num, want_scrolled, want_scaled, line_end_has_precedence, bounds);
    }
}

void ParagraphNumberToBounds(pg_ref pg, long par_num, pg_boolean want_scrolled, pg_boolean want_scaled, rectangle_ptr bounds) {
    if (pg) {
        pgParNumToBounds(pg, par_num, want_scrolled, want_scaled, bounds);
    }
}

long GetCharacterType(pg_ref pg, long offset, long mask_bits) {
    if (pg) {
        return pgCharType(pg, offset, mask_bits);
    }
    return 0;
}

long FindCharacterType(pg_ref pg, long char_info, long* offset, pg_char_ptr the_byte) {
    if (pg && offset) {
        long result = pgFindCharType(pg, char_info, offset, the_byte);
        return result;
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

void EnableLineAndParagraphCounting(pg_ref pg) {
    if (pg) {
        long attributes = pgGetAttributes(pg);
        attributes |= COUNT_LINES_BIT;
        pgSetAttributes(pg, attributes);
    }
}

long GetChangeCounter(pg_ref pg) {
    if (pg) {
        return pgGetChangeCtr(pg);
    }
    return 0;
}

void SetChangeCounter(pg_ref pg, long ctr) {
    if (pg) {
        pgSetChangeCtr(pg, ctr);
    }
}

void GetTextRectangle(pg_ref pg, select_pair_ptr range, pg_boolean want_scroll, pg_boolean want_scaled, rectangle_ptr rect) {
    if (pg && rect) {
        pgTextRect(pg, range, want_scroll, want_scaled, rect);
    }
}

void GetCharacterRectangle(pg_ref pg, long position, short want_scrolled, short want_scaled, rectangle_ptr rect) {
    if (pg && rect) {
        pgCharacterRect(pg, position, want_scrolled, want_scaled, rect);
    }
}

long PointToCharacterOffset(pg_ref pg, co_ordinate_ptr point, co_ordinate_ptr offset_extra) {
    if (pg) {
        return pgPtToChar(pg, point, offset_extra);
    }
    return -1;
}

static long customChangeCounter = 0;

long GetCustomChangeCounter() {
    return customChangeCounter;
}

void SetCustomChangeCounter(long ctr) {
    customChangeCounter = ctr;
}

void GetMaxTextBounds(pg_ref pg, rectangle_ptr bounds, pg_boolean paginate) {
    if (pg && bounds) {
        pgMaxTextBounds(pg, bounds, paginate);
    }
}

#endif // UTILITIES_H
