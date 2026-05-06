#include "PAIGE.H"
#include "DEFPROCS.H"
#include "MACHINE.H"
#include "PGERRORS.H"
#include "PGEXCEPS.H"
#include "PGOSUTL.H"
#include "PGREGION.H"
#include "PGSELECT.H"
#include "PGSHAPES.H"
#include "PGTRAPS.H"
#include "PGUTILS.H"
#include "PGMODERNNATIVE.H"

#ifdef POSIX_PLATFORM

#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstring>
#include <vector>

namespace {

bool empty_rect(const Rect &rect)
{
	return rect.left >= rect.right || rect.top >= rect.bottom;
}

bool empty_paige_rect(const rectangle &rect)
{
	return rect.top_left.h >= rect.bot_right.h || rect.top_left.v >= rect.bot_right.v;
}

Rect intersect_rect(const Rect &left, const Rect &right)
{
	Rect result;
	result.left = std::max(left.left, right.left);
	result.top = std::max(left.top, right.top);
	result.right = std::min(left.right, right.right);
	result.bottom = std::min(left.bottom, right.bottom);
	return result;
}

bool intersects(const Rect &left, const Rect &right)
{
	return !empty_rect(intersect_rect(left, right));
}

rectangle rect_to_paige(const Rect &rect)
{
	rectangle result;
	result.top_left.h = rect.left;
	result.top_left.v = rect.top;
	result.bot_right.h = rect.right;
	result.bot_right.v = rect.bottom;
	return result;
}

Rect paige_to_rect(const rectangle *rect)
{
	Rect result = {0, 0, 0, 0};
	if (rect) {
		result.left = rect->top_left.h;
		result.top = rect->top_left.v;
		result.right = rect->bot_right.h;
		result.bottom = rect->bot_right.v;
	}
	return result;
}

void init_global_char(pg_char_ptr global_char, pg_short_t value)
{
	global_char[0] = static_cast<pg_char>(value);
	global_char[1] = 0;
	global_char[2] = 0;
	global_char[3] = 0;
}

pg_short_t global_char_to_int(pg_char_ptr global_char)
{
	return global_char ? static_cast<pg_short_t>(global_char[0]) : 0;
}

short point_size_from_style(const style_info_ptr style)
{
	if (!style || !style->point)
		return static_cast<short>(DEF_POINT_SIZE);
	return static_cast<short>(std::max<long>(1, style->point >> 16));
}

short default_char_width(const style_info_ptr style)
{
	const short point = point_size_from_style(style);
	long width = style && style->char_width ? (style->char_width >> 16) : 0;
	if (width <= 0)
		width = std::max<long>(1, point / 2);
	return static_cast<short>(width);
}

long measure_run(style_info_ptr style, pg_char_ptr data, long length, long slop, long num_spaces,
		pg_text_int PG_FAR *positions)
{
	long total = 0;
	const long space_extra = num_spaces ? slop / num_spaces : 0;
	const short width = default_char_width(style);

	for (long index = 0; index < length; ++index) {
		long char_width = width;
		if (data && (data[index] == TAB_CHAR))
			char_width = width * 4;
		else if (data && data[index] == ' ')
			char_width += space_extra;

		total += char_width;
		if (positions)
			positions[index] = static_cast<pg_text_int>(std::max<long>(0, total));
	}

	return total;
}

void make_graf_device(const pg_globals_ptr globals, const generic_var the_port,
		size_t machine_ref, graf_device_ptr device)
{
	if (!device)
		return;

	std::memset(device, 0, sizeof(graf_device));
	device->machine_var = the_port;
	device->machine_ref = machine_ref;
	device->clip_rgn = pgCreateRgn();
	device->scroll_rgn = pgCreateRgn();
	device->resolution = (72L << 16) | 72L;
	device->scale.scale = 0x00010000L;
	device->bk_color.red = device->bk_color.green = device->bk_color.blue = 0xFFFF;

	if (globals)
		device->graf_stack = MemoryAlloc(globals->mem_globals, sizeof(port_preserve), 0, 4);
}

bool is_word_break(pg_char ch, pg_globals_ptr globals)
{
	return ch == ' ' || ch == '\t' || ch == global_char_to_int(globals->hyphen_char)
			|| ch == globals->line_wrap_char || ch == globals->soft_line_char;
}

} // namespace

extern "C" {

PG_PASCAL (pg_region) pgCreateRgn(void)
{
	return new pg_modern_region;
}

PG_PASCAL (void) pgSectRgn(pg_region rgn1, pg_region rgn2, pg_region rgn3)
{
	if (!rgn3)
		return;

	rgn3->rects.clear();
	if (!rgn1 || !rgn2)
		return;

	for (const auto &left : rgn1->rects) {
		for (const auto &right : rgn2->rects) {
			Rect section = intersect_rect(paige_to_rect(&left), paige_to_rect(&right));
			if (!empty_rect(section))
				rgn3->rects.push_back(rect_to_paige(section));
		}
	}
}

PG_PASCAL (void) pgUnionRgn(pg_region rgn1, pg_region rgn2, pg_region rgn3)
{
	if (!rgn3)
		return;

	rgn3->rects.clear();
	if (rgn1)
		rgn3->rects.insert(rgn3->rects.end(), rgn1->rects.begin(), rgn1->rects.end());
	if (rgn2)
		rgn3->rects.insert(rgn3->rects.end(), rgn2->rects.begin(), rgn2->rects.end());
}

PG_PASCAL (void) pgSubRgn(pg_region rgn1, pg_region rgn2, pg_region rgn3)
{
	if (!rgn3)
		return;

	rgn3->rects.clear();
	if (!rgn1)
		return;
	if (!rgn2) {
		rgn3->rects = rgn1->rects;
		return;
	}

	for (const auto &candidate : rgn1->rects) {
		bool overlapped = false;
		for (const auto &clip : rgn2->rects) {
			if (intersects(paige_to_rect(&candidate), paige_to_rect(&clip))) {
				overlapped = true;
				break;
			}
		}
		if (!overlapped)
			rgn3->rects.push_back(candidate);
	}
}

PG_PASCAL (void) pgDisposeRgn(pg_region rgn)
{
	delete rgn;
}

PG_PASCAL (pg_boolean) pgEmptyRgn(pg_region rgn)
{
	return static_cast<pg_boolean>(!rgn || rgn->rects.empty());
}

PG_PASCAL (void) pgSetEmptyRgn(pg_region rgn)
{
	if (rgn)
		rgn->rects.clear();
}

PG_PASCAL (void) pgCopyRgn(pg_region rgn1, pg_region rgn2)
{
	if (!rgn2)
		return;
	rgn2->rects = rgn1 ? rgn1->rects : std::vector<rectangle>();
}

PG_PASCAL (void) pgRectToRgn(pg_region rgn, Rect PG_FAR *r)
{
	if (!rgn)
		return;

	rgn->rects.clear();
	if (r && !empty_rect(*r))
		rgn->rects.push_back(rect_to_paige(*r));
}

PG_PASCAL (void) pgOffsetRgn(pg_region rgn, short h, short v)
{
	if (!rgn)
		return;

	for (auto &rect : rgn->rects) {
		rect.top_left.h += h;
		rect.bot_right.h += h;
		rect.top_left.v += v;
		rect.bot_right.v += v;
	}
}

PG_PASCAL (void) pgInsetRgn(pg_region rgn, short dh, short dv)
{
	if (!rgn)
		return;

	for (auto &rect : rgn->rects) {
		rect.top_left.h += dh;
		rect.bot_right.h -= dh;
		rect.top_left.v += dv;
		rect.bot_right.v -= dv;
	}
	rgn->rects.erase(std::remove_if(rgn->rects.begin(), rgn->rects.end(), empty_paige_rect), rgn->rects.end());
}

PG_PASCAL (int) EmptyRgn(RgnHandle rgn)
{
	return pgEmptyRgn(rgn);
}

PG_PASCAL (void) InsetRgn(RgnHandle rgn, short dh, short dv)
{
	pgInsetRgn(rgn, dh, dv);
}

PG_PASCAL (void) pgMachineInit(pg_globals_ptr globals)
{
	auto *global_vars = globals;

	global_vars->max_offscreen = MAX_OFFSCREEN;
	global_vars->max_block_size = MAX_TEXTBLOCK;
	global_vars->def_tab_space = DEF_TAB_SPACE;
	global_vars->minimum_line_width = DEF_MIN_WIDTH;
	global_vars->line_wrap_char = CR_CHAR;
	global_vars->soft_line_char = SOFT_CR_CHAR;
	global_vars->tab_char = TAB_CHAR;
	global_vars->soft_hyphen_char = SOFT_HYPHEN;
	global_vars->bs_char = DELETE_CHAR;
	global_vars->ff_char = FF_CHAR;
	global_vars->container_brk_char = CONTAINER_BRK_CHAR;
	global_vars->left_arrow_char = LEFT_ARROW;
	global_vars->right_arrow_char = RIGHT_ARROW;
	global_vars->up_arrow_char = UP_ARROW;
	global_vars->down_arrow_char = DOWN_ARROW;
	global_vars->text_brk_char = TEXT_BRK_CHAR;
	global_vars->fwd_delete_char = FWD_DELETE_CHAR;
	global_vars->machine_specific = CURRENT_OS;

	init_global_char(global_vars->hyphen_char, HYPHEN_CHAR);
	init_global_char(global_vars->decimal_char, DECIMAL_CHAR);
	init_global_char(global_vars->cr_invis_symbol, INVIS_CR);
	init_global_char(global_vars->lf_invis_symbol, INVIS_LF);
	init_global_char(global_vars->tab_invis_symbol, INVIS_TAB);
	init_global_char(global_vars->end_invis_symbol, INVIS_END);
	init_global_char(global_vars->cont_invis_symbol, INVIS_BREAK);
	init_global_char(global_vars->pbrk_invis_symbol, INVIS_PBREAK);
	init_global_char(global_vars->space_invis_symbol, INVIS_SPACE);
	init_global_char(global_vars->elipse_symbol, ELIPSE_SYMBOL);
	init_global_char(global_vars->flat_single_quote, APOSTROPHE);
	init_global_char(global_vars->flat_double_quote, FLAT_QUOTE_CHAR);
	init_global_char(global_vars->left_single_quote, LEFT_SINGLE_QUOTE);
	init_global_char(global_vars->right_single_quote, RIGHT_SINGLE_QUOTE);
	init_global_char(global_vars->left_double_quote, LEFT_DOUBLE_QUOTE);
	init_global_char(global_vars->right_double_quote, RIGHT_DOUBLE_QUOTE);
	init_global_char(global_vars->unknown_char, UNSUPPORTED_CHAR);
	init_global_char(global_vars->bullet_char, BULLET_CHAR);

	pgInitDefaultFont(global_vars, &global_vars->def_font);
	pgInitDefaultStyle(global_vars, &global_vars->def_style, &global_vars->def_font);
	pgInitDefaultPar(global_vars, &global_vars->def_par);

	global_vars->def_bk_color.red = global_vars->def_bk_color.green = global_vars->def_bk_color.blue = 0xFFFF;
	global_vars->trans_color = global_vars->def_bk_color;
	global_vars->system_version = PAIGE_OS;
	global_vars->color_enable = TRUE;
	global_vars->bullet_size = 5;
	global_vars->alpha_widths = MemoryAlloc(global_vars->mem_globals, sizeof(pg_text_int), 0, 128);

	make_graf_device(global_vars, nullptr, 0, &global_vars->offscreen_port);
	global_vars->offscreen_enable = FALSE;
	global_vars->offscreen_exclusion = pgRectToShape(global_vars->mem_globals, nullptr);
}

PG_PASCAL (void) pgMachineShutdown(const pg_globals_ptr globals)
{
	if (!globals)
		return;

	pgCloseDevice(globals, &globals->offscreen_port);
	DisposeNonNilMemory(globals->offscreen_exclusion);
	DisposeNonNilMemory(globals->alpha_widths);
	DisposeNonNilMemory(globals->pg_list);
}

PG_PASCAL (void) pgInitDefaultDevice(const pg_globals_ptr globals, graf_device_ptr device)
{
	pgInitDevice(globals, nullptr, 0, device);
}

PG_PASCAL (void) pgInitDefaultFont(const pg_globals_ptr globals, font_info_ptr font)
{
	(void)globals;
	if (!font)
		return;

	font->environs = FONT_GOOD;
	font->platform = PAIGE_GRAPHICS;
	font->char_type = 0;
	font->code_page = 0;
	font->language = 0;

	if (!font->name[0]) {
#ifdef HAIKU_PLATFORM
		const char *name = "Noto Sans";
#else
		const char *name = "Helvetica";
#endif
		size_t length = std::min<size_t>(std::strlen(name), FONT_SIZE - 1);
		font->name[0] = static_cast<pg_char>(length);
		for (size_t index = 0; index < length; ++index)
			font->name[index + 1] = static_cast<pg_char>(name[index]);
		font->name[length + 1] = 0;
	}
}

PG_PASCAL (void) pgInitDevice(const pg_globals_ptr globals, const generic_var the_port,
		size_t machine_ref, graf_device_ptr device)
{
	make_graf_device(globals, the_port, machine_ref, device);
}

PG_PASCAL (void) pgCloseDevice(const pg_globals_ptr globals, const graf_device_ptr device)
{
	(void)globals;
	if (!device)
		return;

	DisposeNonNilMemory(device->graf_stack);
	if (device->clip_rgn)
		pgDisposeRgn(device->clip_rgn);
	if (device->scroll_rgn)
		pgDisposeRgn(device->scroll_rgn);

	const_cast<graf_device_ptr>(device)->graf_stack = MEM_NULL;
	const_cast<graf_device_ptr>(device)->clip_rgn = REGION_NULL;
	const_cast<graf_device_ptr>(device)->scroll_rgn = REGION_NULL;
}

PG_PASCAL (short) pgInsertQuery(paige_rec_ptr pg, pg_char_ptr the_char, short charsize)
{
	(void)pg;
	(void)the_char;
	(void)charsize;
	return static_cast<short>(key_insert_mode);
}

PG_PASCAL (pg_word) pgCharClassProc(paige_rec_ptr pg, pg_char_ptr the_char, short charsize,
		style_info_ptr style, font_info_ptr font)
{
	(void)pg;
	(void)charsize;
	(void)style;
	(void)font;
	if (!the_char)
		return 0;
	return 0;
}

PG_PASCAL (void) pgDrawHiliteProc(paige_rec_ptr pg, shape_ref rgn)
{
#if defined(MACOS_PLATFORM) || defined(HAIKU_PLATFORM)
	pgNativeDrawHilite(pg, rgn);
#else
	(void)pg;
	(void)rgn;
#endif
}

PG_PASCAL (void) pgDrawCursorProc(paige_rec_ptr pg, t_select_ptr select, short verb)
{
#if defined(MACOS_PLATFORM) || defined(HAIKU_PLATFORM)
	pgNativeDrawCursor(pg, select, verb);
#else
	(void)select;
	if (!pg)
		return;

	switch (verb) {
		case hide_cursor:
		case deactivate_cursor:
		case dont_draw_cursor:
			pg->flags &= ~CARET_BIT;
			break;
		case show_cursor:
		case toggle_cursor:
		case restore_cursor:
		case update_cursor:
			if (!(pg->flags & NO_CARET_BIT))
				pg->flags |= CARET_BIT;
			break;
		default:
			break;
	}
#endif
}

PG_PASCAL (void) pgInitFont(paige_rec_ptr pg, font_info_ptr info)
{
	if (!info || !info->name[0])
		return;

	if (!info->platform)
		info->platform = PAIGE_GRAPHICS;

	info->environs &= ~(FONT_USES_ALTERNATE | FONT_NOT_AVAIL | FONT_BEST_GUESS);
	pgFixFontName(info);

	short name_length = static_cast<short>(FONT_NAME_SIZE(info->name) + 1);
	if (name_length > FONT_SIZE) {
		name_length = FONT_SIZE - 1;
		info->name[0] = static_cast<pg_char>(name_length);
	}

	pgFillBlock(&info->name[name_length], (FONT_SIZE - name_length) * sizeof(pg_char), 0);

	if (!pg || pgIsRealFont(pg->globals, info, FALSE))
		info->environs |= FONT_GOOD;
	else
		info->environs |= FONT_NOT_AVAIL | FONT_BEST_GUESS;

#if defined(MACOS_PLATFORM) || defined(HAIKU_PLATFORM)
	pgNativeInitFont(pg, info);
#endif
}

PG_PASCAL (void) pgStyleInitProc(paige_rec_ptr pg, style_info_ptr style, font_info_ptr font)
{
	(void)font;
	if (!style)
		return;

	const short point = point_size_from_style(style);
	style->ascent = static_cast<short>((point * 3) / 4);
	style->descent = static_cast<short>(std::max(1, point - style->ascent));
	style->leading = (pg && (pg->flags & NO_DEFAULT_LEADING)) ? 0 : 1;
	style->char_width = static_cast<pg_fixed>(std::max(1, point / 2)) << 16;
	style->right_overhang = 0;
	style->left_overhang = 0;

#if defined(MACOS_PLATFORM) || defined(HAIKU_PLATFORM)
	pgNativeStyleMetrics(pg, style, font);
#endif
}

PG_PASCAL (void) pgInstallFont(paige_rec_ptr pg, style_info_ptr the_style,
		font_info_ptr the_font, style_info_ptr composite_style, short style_overlay, pg_boolean include_offscreen)
{
#if defined(MACOS_PLATFORM) || defined(HAIKU_PLATFORM)
	pgNativeInstallFont(pg, the_style, the_font, composite_style, style_overlay, include_offscreen);
#else
	(void)pg;
	(void)the_font;
	(void)style_overlay;
	(void)include_offscreen;
	if (composite_style && the_style)
		pgBlockMove(the_style, composite_style, sizeof(style_info));
#endif
}

PG_PASCAL (void) pgDeleteStyleProc(paige_rec_ptr pg, pg_globals_ptr globals,
		short reason_verb, format_ref all_styles, style_info_ptr style)
{
	(void)pg;
	(void)globals;
	(void)reason_verb;
	(void)all_styles;
#if defined(MACOS_PLATFORM) || defined(HAIKU_PLATFORM)
	pgNativeDisposeStyle(style);
#else
	(void)style;
#endif
}

PG_PASCAL (void) pgDupStyleProc(paige_rec_ptr src_pg, paige_rec_ptr target_pg,
		short reason_verb, format_ref all_styles, style_info_ptr style)
{
	(void)src_pg;
	(void)target_pg;
	(void)reason_verb;
	(void)all_styles;
#if defined(MACOS_PLATFORM) || defined(HAIKU_PLATFORM)
	if (style) {
		style->machine_var = 0;
		style->machine_var2 = 0;
	}
#else
	(void)style;
#endif
}

PG_PASCAL (void) pgSaveStyleProc(paige_rec_ptr pg, style_info_ptr style_to_save)
{
	(void)pg;
	if (style_to_save) {
		style_to_save->machine_var = 0;
		style_to_save->machine_var2 = 0;
	}
}

PG_PASCAL (void) pgMeasureProc(paige_rec_ptr pg, style_walk_ptr walker,
		pg_char_ptr data, size_t length, pg_short_t slop, long PG_FAR *positions,
		short PG_FAR *types, short measure_verb, size_t current_offset, pg_boolean scale_widths,
		short call_order)
{
	(void)measure_verb;
	(void)current_offset;
	(void)scale_widths;
	(void)call_order;

	style_info_ptr style = walker ? walker->cur_style : (pg ? &pg->globals->def_style : nullptr);

#if defined(MACOS_PLATFORM) || defined(HAIKU_PLATFORM)
	font_info_ptr font = walker ? walker->cur_font : (pg ? &pg->globals->def_font : nullptr);
	const long num_spaces = data ? std::count(data, data + length, static_cast<pg_char>(' ')) : 0;
	pgNativeMeasureText(pg, walker, style, font, data, length, slop, num_spaces, positions);
#else
	long total = 0;
	const short width = default_char_width(style);

	for (size_t index = 0; index < length; ++index) {
		pg_char ch = data ? data[index] : 0;
		long char_width = (ch == TAB_CHAR) ? width * 4 : width;
		if (ch == ' ' && slop)
			char_width += slop;
		total += char_width;

		if (positions)
			positions[index] = total;

		if (types) {
			long info = pgCharInfoProc(pg, walker, data, 0, 0, length, index, ALL_INFO_BITS);
			types[index] = static_cast<short>(info & (CTL_CHAR_BITS | WORD_BREAK_BIT | WORD_SEL_BIT | BLANK_BIT));
		}
	}
#endif

	if (types) {
		for (size_t index = 0; index < length; ++index) {
			long info = pgCharInfoProc(pg, walker, data, 0, 0, length, index, ALL_INFO_BITS);
			types[index] = static_cast<short>(info & (CTL_CHAR_BITS | WORD_BREAK_BIT | WORD_SEL_BIT | BLANK_BIT));
		}
	}
}

PG_PASCAL (long) pgCharInfoProc(paige_rec_ptr pg, style_walk_ptr style_walker,
		pg_char_ptr data, size_t block_offset, size_t offset_begin, size_t offset_end, size_t char_offset,
		long mask_bits)
{
	(void)block_offset;
	(void)offset_begin;
	(void)offset_end;

	if (!pg || !data)
		return 0;
	if (style_walker && style_walker->cur_style && (pg->flags & NO_HIDDEN_TEXT_BIT)
			&& style_walker->cur_style->styles[hidden_text_var])
		return 0;

	const pg_char ch = data[char_offset];
	long result = 0;

	if (ch == ' ')
		result |= BLANK_BIT | WORD_BREAK_BIT | WORD_SEL_BIT;
	if (is_word_break(ch, pg->globals))
		result |= WORD_BREAK_BIT | WORD_SEL_BIT;
	if (ch == pg->globals->line_wrap_char)
		result |= CTL_BIT | PAR_SEL_BIT;
	if (ch == pg->globals->soft_line_char)
		result |= CTL_BIT | LINE_SEL_BIT;
	if (ch == pg->globals->tab_char)
		result |= CTL_BIT | TAB_BIT | WORD_BREAK_BIT | WORD_SEL_BIT;
	if (ch == pg->globals->container_brk_char)
		result |= CTL_BIT | CONTAINER_BRK_BIT;
	if (ch == pg->globals->ff_char)
		result |= CTL_BIT | PAGE_BRK_BIT;
	if (ch >= '0' && ch <= '9')
		result |= NUMBER_BIT;
	if (ch == global_char_to_int(pg->globals->decimal_char))
		result |= DECIMAL_CHAR_BIT;
	if (ch >= 'A' && ch <= 'Z')
		result |= UPPER_CASE_BIT | EUROPEAN_BIT;
	if (ch >= 'a' && ch <= 'z')
		result |= LOWER_CASE_BIT | EUROPEAN_BIT;
	if (ch == APOSTROPHE)
		result |= SINGLE_QUOTE_BIT | FLAT_QUOTE_BIT;
	if (ch == FLAT_QUOTE_CHAR)
		result |= FLAT_QUOTE_BIT;
	if (ch > 0x7F)
		result |= NON_ROMAN_BIT;

	return result & mask_bits;
}

PG_PASCAL (void) pgDrawProc(paige_rec_ptr pg, style_walk_ptr walker, pg_char_ptr data,
		pg_short_t offset, pg_short_t length, draw_points_ptr draw_position,
		long extra, short draw_mode)
{
#if defined(MACOS_PLATFORM) || defined(HAIKU_PLATFORM)
	pgNativeDrawText(pg, walker, data, offset, length, draw_position, extra, draw_mode);
#else
	(void)pg;
	(void)walker;
	(void)data;
	(void)offset;
	(void)extra;
	(void)draw_mode;
	if (draw_position)
		draw_position->from.h += length * default_char_width(walker ? walker->cur_style : nullptr);
#endif
}

PG_PASCAL (void) pgSpecialCharProc(paige_rec_ptr pg, style_walk_ptr walker, pg_char_ptr data,
		pg_short_t offset, pg_short_t length, draw_points_ptr draw_position,
		long extra, short draw_mode)
{
	pgDrawProc(pg, walker, data, offset, length, draw_position, extra, draw_mode);
}

PG_PASCAL (void) pgTabDrawProc(paige_rec_ptr pg, style_walk_ptr walker, tab_stop_ptr tab,
		draw_points_ptr draw_position)
{
	(void)pg;
	(void)tab;
	if (draw_position)
		draw_position->from.h += default_char_width(walker ? walker->cur_style : nullptr) * 4;
}

PG_PASCAL (void) pgIdleProc(paige_rec_ptr pg, short verb)
{
	(void)pg;
	(void)verb;
}

PG_PASCAL (void) pgSetGrafDevice(paige_rec_ptr pg, short verb, graf_device_ptr device,
		color_value_ptr bk_color)
{
	(void)bk_color;
	if (!pg || !device)
		return;

	if (verb == set_pg_device) {
		if (!device->graf_stack)
			device->graf_stack = MemoryAlloc(pg->globals->mem_globals, sizeof(port_preserve), 0, 4);
		auto *preserve = static_cast<port_preserve *>(AppendMemory(device->graf_stack, 1, TRUE));
		if (preserve) {
			preserve->last_device = pg->globals->current_port;
			preserve->clip_rgn = device->clip_rgn;
			UnuseMemory(device->graf_stack);
		}
		pg->globals->current_port = device;
		++device->access_ctr;
	}
	else {
		if (device->graf_stack && GetMemorySize(device->graf_stack)) {
			size_t last = GetMemorySize(device->graf_stack) - 1;
			auto *preserve = static_cast<port_preserve *>(UseMemoryRecord(device->graf_stack, last, 1, FALSE));
			pg->globals->current_port = preserve ? preserve->last_device : nullptr;
			UnuseMemory(device->graf_stack);
			DeleteMemory(device->graf_stack, last, 1);
		}
		else
			pg->globals->current_port = nullptr;
	}
}

PG_PASCAL (generic_var) pgGetPlatformDevice(graf_device_ptr the_device)
{
	return the_device ? the_device->machine_var : nullptr;
}

PG_PASCAL (void) pgReleasePlatformDevice(graf_device_ptr the_device)
{
	(void)the_device;
}

PG_PASCAL (void) pgClipGrafDevice(paige_rec_ptr pg, short clip_verb, shape_ref alternate_vis)
{
	if (!pg)
		return;

	if ((pg->flags & PRINT_MODE_BIT) || (pg->doc_info.attributes & NO_CLIP_REGIONS))
		return;

	pg_scale_factor scaler = pg->port.scale;
	rectangle vis_bounds;
	pg_region clip_region = pg->port.clip_rgn;
	shape_ref use_vis = alternate_vis ? alternate_vis : pg->vis_area;
	pg_boolean scale_vis_area = (pg_boolean)((pg->flags & SCALE_VIS_BIT) != 0);

	pgShapeBounds(use_vis, &vis_bounds);
	if (scale_vis_area)
		pgScaleRect(&scaler, nullptr, &vis_bounds);

	Rect vis_r = paige_to_rect(&vis_bounds);

	if (clip_verb == clip_with_none_verb) {
		clip_region = pgCreateRgn();
		pgRectToRgn(clip_region, &vis_r);
	}
	else if (pgVisRegionChanged(pg, TRUE)) {
		pgRectToRgn(clip_region, &vis_r);

		if (!(pg->doc_info.attributes & NO_CLIP_PAGE_AREA)
				&& (pg->doc_info.attributes & BOTTOM_FIXED_BIT)) {
			pg_region intersect_region = pgCreateRgn();
			pgBuildPageRegion(pg, &scaler, intersect_region);
			pgSectRgn(clip_region, intersect_region, clip_region);
			pgDisposeRgn(intersect_region);
		}

		if (!pgEmptyShape(pg->exclude_area)) {
			pg_region intersect_region = pgCreateRgn();
			if (pgBuildExclusionRegion(pg, &scaler, intersect_region))
				pgSubRgn(clip_region, intersect_region, clip_region);
			pgDisposeRgn(intersect_region);
		}
	}

	if (pg->port.scroll_rgn && !pgEmptyRgn(pg->port.scroll_rgn)) {
		pg_region intersect_region = pgCreateRgn();
		pgSectRgn(clip_region, pg->port.scroll_rgn, intersect_region);
#if defined(MACOS_PLATFORM) || defined(HAIKU_PLATFORM)
		pgNativeClipGrafDevice(pg, intersect_region);
#endif
		pgDisposeRgn(intersect_region);
	}
	else {
#if defined(MACOS_PLATFORM) || defined(HAIKU_PLATFORM)
		pgNativeClipGrafDevice(pg, clip_region);
#endif
	}

	if (clip_verb == clip_with_none_verb)
		pgDisposeRgn(clip_region);
}

PG_PASCAL (void) pgSetMeasureDevice(paige_rec_ptr pg)
{
	(void)pg;
}

PG_PASCAL (void) pgUnsetMeasureDevice(paige_rec_ptr pg)
{
	(void)pg;
}

PG_PASCAL (void) pgPrintDeviceChanged(paige_rec_ptr pg)
{
	(void)pg;
}

PG_PASCAL (void) pgPrepareOffscreen(paige_rec_ptr pg, rectangle_ptr target_area,
		rectangle_ptr real_bits_target, co_ordinate_ptr offset_adjust,
		long text_offset, point_start_ptr line_start, short draw_mode)
{
#if defined(MACOS_PLATFORM) || defined(HAIKU_PLATFORM)
	pgNativePrepareOffscreen(pg, target_area, real_bits_target, offset_adjust, text_offset, line_start, draw_mode);
#else
	(void)pg;
	(void)text_offset;
	(void)line_start;
	(void)draw_mode;
	if (real_bits_target && target_area)
		*real_bits_target = *target_area;
	if (offset_adjust)
		offset_adjust->h = offset_adjust->v = 0;
#endif
}

PG_PASCAL (pg_boolean) pgFinishOffscreen(paige_rec_ptr pg, long text_offset,
		point_start_ptr line_start, co_ordinate_ptr new_offset,
		rectangle_ptr new_target, short draw_mode)
{
#if defined(MACOS_PLATFORM) || defined(HAIKU_PLATFORM)
	return pgNativeFinishOffscreen(pg, text_offset, line_start, new_offset, new_target, draw_mode);
#else
	(void)pg;
	(void)text_offset;
	(void)line_start;
	(void)new_offset;
	(void)new_target;
	(void)draw_mode;
	return FALSE;
#endif
}

PG_PASCAL (void) pgScaleGrafDevice(paige_rec_ptr pg)
{
	(void)pg;
}

PG_PASCAL (pg_region) pgScrollRect(paige_rec_ptr pg, rectangle_ptr rect, long distance_h,
		long distance_v, rectangle_ptr affected_area, short draw_mode)
{
#if defined(MACOS_PLATFORM) || defined(HAIKU_PLATFORM)
	return pgNativeScrollRect(pg, rect, distance_h, distance_v, affected_area, draw_mode);
#else
	(void)pg;
	(void)distance_h;
	(void)distance_v;
	(void)draw_mode;
	if (affected_area && rect)
		*affected_area = *rect;

	pg_region result = pgCreateRgn();
	Rect r = paige_to_rect(rect);
	pgRectToRgn(result, &r);
	return result;
#endif
}

PG_PASCAL (void) pgEraseRect(pg_globals_ptr globals, rectangle_ptr rect,
		pg_scale_ptr scaling, co_ordinate_ptr offset_extra)
{
#if defined(MACOS_PLATFORM) || defined(HAIKU_PLATFORM)
	pgNativeEraseRect(globals, rect, scaling, offset_extra);
#else
	(void)globals;
	(void)rect;
	(void)scaling;
	(void)offset_extra;
#endif
}

PG_PASCAL (pg_short_t) pgMeasureText(paige_rec_ptr pg, short measure_verb, pg_char_ptr data,
		long length, long slop, long num_spaces, pg_text_int PG_FAR *positions,
		style_walk_ptr walker)
{
	(void)pg;
	(void)measure_verb;
#if defined(MACOS_PLATFORM) || defined(HAIKU_PLATFORM)
	std::vector<long> native_positions;
	long *native_position_data = nullptr;
	if (positions && length > 0) {
		native_positions.resize(static_cast<size_t>(length));
		native_position_data = native_positions.data();
	}
	const long measured = pgNativeMeasureText(pg, walker, walker ? walker->cur_style : nullptr,
			walker ? walker->cur_font : nullptr, data, length, slop, num_spaces, native_position_data);
	if (positions) {
		for (long index = 0; index < length; ++index)
			positions[index] = static_cast<pg_text_int>(native_positions[static_cast<size_t>(index)]);
	}
	return static_cast<pg_short_t>(measured);
#else
	return static_cast<pg_short_t>(measure_run(walker ? walker->cur_style : nullptr, data, length, slop, num_spaces, positions));
#endif
}

PG_PASCAL (pg_short_t) pgMeasureText32(paige_rec_ptr pg, short measure_verb, pg_char_ptr data,
		long length, long slop, long num_spaces, pg_text_int PG_FAR *positions,
		style_walk_ptr walker)
{
	return pgMeasureText(pg, measure_verb, data, length, slop, num_spaces, positions, walker);
}

PG_PASCAL (pg_short_t) pgMeasureText16(paige_rec_ptr pg, short measure_verb, pg_char_ptr data,
		long length, long slop, long num_spaces, pg_text_int PG_FAR *positions,
		style_walk_ptr walker)
{
	return pgMeasureText(pg, measure_verb, data, length, slop, num_spaces, positions, walker);
}

PG_PASCAL (short) pgScalePointSize(paige_rec_ptr pg, style_walk_ptr walker,
		pg_char_ptr text, long length, pg_boolean PG_FAR *did_scale)
{
	(void)pg;
	(void)text;
	(void)length;
	if (did_scale)
		*did_scale = FALSE;
	return point_size_from_style(walker ? walker->cur_style : nullptr);
}

PG_PASCAL (short) pgSystemDirection(pg_globals_ptr globals)
{
	(void)globals;
	return left_right_direction;
}

PG_PASCAL (pg_boolean) pgIsRealFont(pg_globals_ptr globals, font_info_ptr font, pg_boolean use_alternate)
{
#if defined(MACOS_PLATFORM) || defined(HAIKU_PLATFORM)
	return pgNativeIsRealFont(globals, font, use_alternate);
#else
	(void)globals;
	(void)use_alternate;
	if (font)
		font->environs |= FONT_GOOD;
	return TRUE;
#endif
}

PG_PASCAL (pg_fixed) pgPointsizeToScreen(pg_ref pg, pg_fixed pointsize)
{
	(void)pg;
	return pointsize;
}

PG_PASCAL (pg_fixed) pgScreenToPointsize(pg_ref pg, pg_fixed screensize)
{
	(void)pg;
	return screensize;
}

PG_PASCAL (void) pgOSToPgColor(const pg_plat_color_value PG_FAR *os_color, color_value_ptr pg_color)
{
	if (!os_color || !pg_color)
		return;

	uint32_t color = *os_color;
	pg_color->red = static_cast<unsigned short>(((color >> 16) & 0xFF) * 257);
	pg_color->green = static_cast<unsigned short>(((color >> 8) & 0xFF) * 257);
	pg_color->blue = static_cast<unsigned short>((color & 0xFF) * 257);
	pg_color->alpha = static_cast<pg_short_t>((color >> 24) & 0xFF);
}

PG_PASCAL (void) pgColorToOS(const color_value_ptr pg_color, pg_plat_color_value PG_FAR *os_color)
{
	if (!pg_color || !os_color)
		return;

	*os_color = (static_cast<uint32_t>(pg_color->alpha & 0xFF) << 24)
			| (static_cast<uint32_t>(pg_color->red >> 8) << 16)
			| (static_cast<uint32_t>(pg_color->green >> 8) << 8)
			| static_cast<uint32_t>(pg_color->blue >> 8);
}

PG_PASCAL (void) pgTransLiterate(pg_char_ptr text, long length, pg_char_ptr target,
		pg_boolean do_uppercase)
{
	if (!text || !target)
		return;

	for (long index = 0; index < length; ++index) {
		pg_char ch = text[index];
		if (ch <= 0x7F)
			ch = static_cast<pg_char>(do_uppercase ? std::toupper(static_cast<unsigned char>(ch))
					: std::tolower(static_cast<unsigned char>(ch)));
		target[index] = ch;
	}
}

PG_PASCAL (void) pgOpenPrinter(paige_rec_ptr pg_rec, graf_device_ptr print_dev,
		long first_position, rectangle_ptr page_rect)
{
	(void)first_position;
	(void)page_rect;
	if (pg_rec && print_dev)
		pgInitDevice(pg_rec->globals, nullptr, 0, print_dev);
}

PG_PASCAL (void) pgClosePrinter(paige_rec_ptr pg_rec, graf_device_ptr print_dev)
{
	if (pg_rec && print_dev)
		pgCloseDevice(pg_rec->globals, print_dev);
}

PG_PASCAL (pg_boolean) pgIsCaretTime(paige_rec_ptr pg)
{
#if defined(MACOS_PLATFORM) || defined(HAIKU_PLATFORM)
	return pgNativeIsCaretTime(pg);
#else
	(void)pg;
	return TRUE;
#endif
}

PG_PASCAL (short) pgGetCharWidth(paige_rec_ptr pg_rec, style_info_ptr style, pg_char the_char)
{
#if defined(MACOS_PLATFORM) || defined(HAIKU_PLATFORM)
	return pgNativeGetCharWidth(pg_rec, style, the_char);
#else
	(void)pg_rec;
	if (the_char == TAB_CHAR)
		return static_cast<short>(default_char_width(style) * 4);
	return default_char_width(style);
#endif
}

PG_PASCAL (void) pgDrawSpecialUnderline(paige_rec_ptr pg, Point from_pt,
		short distance, style_info_ptr style, short draw_bits)
{
#if defined(MACOS_PLATFORM) || defined(HAIKU_PLATFORM)
	co_ordinate native_point;
	native_point.h = from_pt.h;
	native_point.v = from_pt.v;
	pgNativeDrawSpecialUnderline(pg, native_point, distance, style, draw_bits);
#else
	(void)pg;
	(void)from_pt;
	(void)distance;
	(void)style;
	(void)draw_bits;
#endif
}

PG_PASCAL (pg_fixed) pgMultiplyFixed(pg_fixed fix1, pg_fixed fix2)
{
	return static_cast<pg_fixed>((static_cast<long long>(fix1) * static_cast<long long>(fix2)) >> 16);
}

PG_PASCAL (pg_fixed) pgDivideFixed(pg_fixed fix1, pg_fixed fix2)
{
	if (!fix2)
		return 0;
	return static_cast<pg_fixed>((static_cast<long long>(fix1) << 16) / fix2);
}

PG_PASCAL (pg_fixed) pgFixedRatio(short n, short d)
{
	if (!d)
		return 0;
	return static_cast<pg_fixed>((static_cast<long long>(n) << 16) / d);
}

PG_PASCAL (void) SetFontCharWidths(pg_ref pg, style_info_ptr style, int PG_FAR *charwidths)
{
	(void)pg;
	(void)style;
	(void)charwidths;
}

} // extern "C"

#endif
