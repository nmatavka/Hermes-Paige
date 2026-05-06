#ifndef PGMODERN_NATIVE_H
#define PGMODERN_NATIVE_H

#include "PAIGE.H"
#include "PGSCRAP.H"

#ifdef POSIX_PLATFORM

#ifdef __cplusplus

#include <vector>

struct pg_modern_region {
	std::vector<rectangle> rects;
};

struct pg_native_text_metrics {
	long width;
	short ascent;
	short descent;
	short leading;
	pg_fixed average_width;
};

bool pgNativeInitFont(paige_rec_ptr pg, font_info_ptr font);
bool pgNativeIsRealFont(pg_globals_ptr globals, font_info_ptr font, pg_boolean use_alternate);
void pgNativeStyleMetrics(paige_rec_ptr pg, style_info_ptr style, font_info_ptr font);
void pgNativeInstallFont(paige_rec_ptr pg, style_info_ptr style, font_info_ptr font,
		style_info_ptr composite_style, short style_overlay, pg_boolean include_offscreen);
void pgNativeDisposeStyle(style_info_ptr style);
long pgNativeMeasureText(paige_rec_ptr pg, style_walk_ptr walker, style_info_ptr style,
		font_info_ptr font, pg_char_ptr data, size_t length, long slop, long num_spaces,
		long PG_FAR *positions);
void pgNativeDrawText(paige_rec_ptr pg, style_walk_ptr walker, pg_char_ptr data,
		pg_short_t offset, pg_short_t length, draw_points_ptr draw_position,
		long extra, short draw_mode);
void pgNativeDrawHilite(paige_rec_ptr pg, shape_ref rgn);
void pgNativeDrawCursor(paige_rec_ptr pg, t_select_ptr select, short verb);
void pgNativeClipGrafDevice(paige_rec_ptr pg, pg_region clip_region);
void pgNativeEraseRect(pg_globals_ptr globals, rectangle_ptr rect,
		pg_scale_ptr scaling, co_ordinate_ptr offset_extra);
pg_region pgNativeScrollRect(paige_rec_ptr pg, rectangle_ptr rect, long distance_h,
		long distance_v, rectangle_ptr affected_area, short draw_mode);
void pgNativePrepareOffscreen(paige_rec_ptr pg, rectangle_ptr target_area,
		rectangle_ptr real_bits_target, co_ordinate_ptr offset_adjust,
		long text_offset, point_start_ptr line_start, short draw_mode);
pg_boolean pgNativeFinishOffscreen(paige_rec_ptr pg, long text_offset,
		point_start_ptr line_start, co_ordinate_ptr new_offset,
		rectangle_ptr new_target, short draw_mode);
pg_boolean pgNativeIsCaretTime(paige_rec_ptr pg);
short pgNativeGetCharWidth(paige_rec_ptr pg, style_info_ptr style, pg_char the_char);
void pgNativeDrawSpecialUnderline(paige_rec_ptr pg, co_ordinate from_pt,
		short distance, style_info_ptr style, short draw_bits);

pg_boolean pgNativeClipboardWrite(pg_os_type native_format, int scrap_type,
		memory_ref dataref, pg_boolean clear_scrap);
memory_ref pgNativeClipboardRead(pgm_globals_ptr mem_globals, pg_os_type native_format,
		short PG_FAR *scrap_type, long PG_FAR *embed_type, size_t PG_FAR *raw_data_size,
		pg_boolean load_real_data);
pg_boolean pgNativeClipboardAvailable(pg_os_type native_format);
void pgNativeClipboardClear(void);

#endif

#endif

#endif
