#include "PGMODERNNATIVE.H"

#if defined(POSIX_PLATFORM) && defined(HAIKU_PLATFORM)

#include "MACHINE.H"
#include "PGOSUTL.H"
#include "PGSHAPES.H"
#include "PGTRAPS.H"
#include "PGUTILS.H"

#include <Application.h>
#include <Bitmap.h>
#include <Clipboard.h>
#include <Font.h>
#include <Message.h>
#include <OS.h>
#include <Region.h>
#include <View.h>

#include <algorithm>
#include <cmath>
#include <cstring>
#include <string>

namespace {

const char *kPaigeNativeMime = "application/x-vnd.hermes-paige";
const char *kPaigeUnicodeMime = "text/plain;charset=utf-8";

short point_size_from_style(style_info_ptr style)
{
	if (!style || !style->point)
		return static_cast<short>(DEF_POINT_SIZE);
	return static_cast<short>(std::max<long>(1, style->point >> 16));
}

std::string utf8_from_paige(pg_char_ptr text, size_t length)
{
	std::string result;
	if (!text)
		return result;
	result.reserve(length);
	for (size_t index = 0; index < length; ++index) {
		uint32_t ch = text[index];
		if (ch < 0x80)
			result.push_back(static_cast<char>(ch));
		else if (ch < 0x800) {
			result.push_back(static_cast<char>(0xC0 | (ch >> 6)));
			result.push_back(static_cast<char>(0x80 | (ch & 0x3F)));
		}
		else {
			result.push_back(static_cast<char>(0xE0 | (ch >> 12)));
			result.push_back(static_cast<char>(0x80 | ((ch >> 6) & 0x3F)));
			result.push_back(static_cast<char>(0x80 | (ch & 0x3F)));
		}
	}
	return result;
}

std::string font_name(font_info_ptr font)
{
	if (!font || !font->name[0])
		return "Noto Sans";
	size_t length = 0;
	pg_char_ptr text = font->name;
	if (font->environs & NAME_IS_CSTR)
		length = pgCStrLength(font->name);
	else {
		length = std::min<size_t>(font->name[0], FONT_SIZE - 1);
		text = &font->name[1];
	}
	std::string result = utf8_from_paige(text, length);
	return result.empty() ? "Noto Sans" : result;
}

rgb_color rgb_from_paige(color_value_ptr color, unsigned char alpha = 255)
{
	rgb_color result = {0, 0, 0, alpha};
	if (color) {
		result.red = static_cast<unsigned char>(color->red >> 8);
		result.green = static_cast<unsigned char>(color->green >> 8);
		result.blue = static_cast<unsigned char>(color->blue >> 8);
	}
	return result;
}

BRect brect_from_paige(const rectangle *rect)
{
	if (!rect)
		return BRect(0, 0, -1, -1);
	return BRect(rect->top_left.h, rect->top_left.v, rect->bot_right.h, rect->bot_right.v);
}

BView *view_from_device(graf_device_ptr device)
{
	return device ? static_cast<BView *>(device->machine_var) : nullptr;
}

BView *view_from_pg(paige_rec_ptr pg)
{
	if (!pg)
		return nullptr;
	graf_device_ptr device = pg->globals && pg->globals->current_port ? pg->globals->current_port : &pg->port;
	return view_from_device(device);
}

BFont *cached_font(style_info_ptr style, font_info_ptr info)
{
	if (!style)
		return new BFont(be_plain_font);
	if (style->machine_var)
		return reinterpret_cast<BFont *>(style->machine_var);

	BFont *font = new BFont(be_plain_font);
	std::string name = font_name(info);
	font_family family;
	font_style face;
	std::memset(family, 0, sizeof(family));
	std::memset(face, 0, sizeof(face));
	std::strncpy(family, name.c_str(), sizeof(font_family) - 1);
	font->SetFamilyAndStyle(family, nullptr);
	font->SetSize(point_size_from_style(style));
	uint16 flags = 0;
	if (style->styles[bold_var])
		flags |= B_BOLD_FACE;
	if (style->styles[italic_var])
		flags |= B_ITALIC_FACE;
	if (flags)
		font->SetFace(flags);
	style->machine_var = reinterpret_cast<size_t>(font);
	return font;
}

float measure_text(BFont *font, pg_char_ptr text, size_t length)
{
	if (!font || !text || !length)
		return 0;
	std::string utf8 = utf8_from_paige(text, length);
	return font->StringWidth(utf8.c_str());
}

float measure_char(BFont *font, pg_char ch)
{
	if (ch == TAB_CHAR)
		return measure_char(font, static_cast<pg_char>(' ')) * 4.0f;
	if (ch == CR_CHAR || ch == SOFT_CR_CHAR || ch == FF_CHAR || ch == CONTAINER_BRK_CHAR)
		return 0;
	return measure_text(font, &ch, 1);
}

void fill_shape(BView *view, shape_ref shape, rgb_color color)
{
	if (!view || !shape)
		return;
	const size_t count = GetMemorySize(shape);
	if (!count)
		return;
	auto *rects = static_cast<rectangle_ptr>(UseMemory(shape));
	view->SetHighColor(color);
	const size_t first = count > 1 ? 1 : 0;
	for (size_t index = first; index < count; ++index)
		view->FillRect(brect_from_paige(&rects[index]));
	UnuseMemory(shape);
}

memory_ref memory_ref_from_bytes(pgm_globals_ptr globals, const void *bytes, size_t byte_size, pg_short_t rec_size)
{
	if (!globals || !bytes)
		return MEM_NULL;
	const size_t width = rec_size ? rec_size : 1;
	const size_t recs = (byte_size + width - 1) / width;
	memory_ref ref = MemoryAlloc(globals, rec_size, recs, 0);
	if (!ref)
		return MEM_NULL;
	void *target = UseMemory(ref);
	std::memcpy(target, bytes, byte_size);
	UnuseMemory(ref);
	return ref;
}

} // namespace

bool pgNativeInitFont(paige_rec_ptr pg, font_info_ptr font)
{
	(void)pg;
	if (!font)
		return false;
	font->platform = PAIGE_GRAPHICS;
	font->environs &= ~(FONT_NOT_AVAIL | FONT_BEST_GUESS);
	font->environs |= FONT_GOOD;
	return true;
}

bool pgNativeIsRealFont(pg_globals_ptr globals, font_info_ptr font, pg_boolean use_alternate)
{
	(void)globals;
	(void)use_alternate;
	if (font)
		font->environs |= FONT_GOOD;
	return font != nullptr;
}

void pgNativeStyleMetrics(paige_rec_ptr pg, style_info_ptr style, font_info_ptr font)
{
	(void)pg;
	if (!style)
		return;
	BFont *native = cached_font(style, font);
	font_height height;
	native->GetHeight(&height);
	style->ascent = static_cast<short>(std::ceil(height.ascent));
	style->descent = static_cast<short>(std::ceil(height.descent));
	style->leading = static_cast<short>(std::ceil(height.leading));
	style->char_width = static_cast<pg_fixed>(std::max<float>(1, measure_char(native, static_cast<pg_char>('W')))) << 16;
}

void pgNativeInstallFont(paige_rec_ptr pg, style_info_ptr style, font_info_ptr font,
		style_info_ptr composite_style, short style_overlay, pg_boolean include_offscreen)
{
	(void)include_offscreen;
	style_info imposed_style;
	font_info imposed_font;
	style_info_ptr use_style = style;
	font_info_ptr use_font = font;
	if (pg && style_overlay) {
		pgStyleSuperImpose(pg, style, &imposed_style, &imposed_font, style_overlay);
		imposed_style.machine_var = 0;
		imposed_style.machine_var2 = 0;
		use_style = &imposed_style;
		use_font = &imposed_font;
	}
	cached_font(use_style, use_font);
	pgNativeStyleMetrics(pg, use_style, use_font);
	if (composite_style) {
		pgBlockMove(use_style, composite_style, sizeof(style_info));
		composite_style->machine_var = 0;
		composite_style->machine_var2 = 0;
	}
	if (use_style == &imposed_style)
		pgNativeDisposeStyle(&imposed_style);
}

void pgNativeDisposeStyle(style_info_ptr style)
{
	if (!style || !style->machine_var)
		return;
	delete reinterpret_cast<BFont *>(style->machine_var);
	style->machine_var = 0;
	style->machine_var2 = 0;
}

long pgNativeMeasureText(paige_rec_ptr pg, style_walk_ptr walker, style_info_ptr style,
		font_info_ptr font, pg_char_ptr data, size_t length, long slop, long num_spaces,
		long PG_FAR *positions)
{
	(void)pg;
	BFont *native = cached_font(style, font ? font : (walker ? walker->cur_font : nullptr));
	const float space_extra = num_spaces ? static_cast<float>(slop) / static_cast<float>(num_spaces) : 0;
	float total = 0;
	for (size_t index = 0; index < length; ++index) {
		const pg_char ch = data ? data[index] : 0;
		float width = measure_char(native, ch);
		if (ch == static_cast<pg_char>(' '))
			width += space_extra;
		total += width;
		if (positions)
			positions[index] = static_cast<long>(std::lround(total));
	}
	return static_cast<long>(std::lround(total));
}

void pgNativeDrawText(paige_rec_ptr pg, style_walk_ptr walker, pg_char_ptr data,
		pg_short_t offset, pg_short_t length, draw_points_ptr draw_position,
		long extra, short draw_mode)
{
	if (!pg || !walker || !data || !draw_position || length <= 0)
		return;
	BView *view = view_from_pg(pg);
	style_info_ptr style = walker->cur_style;
	BFont *font = cached_font(style, walker->cur_font);
	const long spaces = std::count(data + offset, data + offset + length, static_cast<pg_char>(' '));
	const long width = pgNativeMeasureText(pg, walker, style, walker->cur_font, data + offset, length, extra, spaces, nullptr);
	if (view) {
		view->SetFont(font);
		if (style && !pgTransColor(pg->globals, &style->bk_color)) {
			view->SetHighColor(rgb_from_paige(&style->bk_color));
			view->FillRect(BRect(draw_position->from.h, draw_position->from.v - style->ascent,
					draw_position->from.h + width, draw_position->from.v + style->descent + style->leading));
		}
		rgb_color fg = style ? rgb_from_paige(&style->fg_color) : rgb_color{0, 0, 0, 255};
		if (draw_mode == grayish_or || draw_mode == grayish_bits_copy)
			fg.alpha = 128;
		view->SetHighColor(fg);
		std::string utf8 = utf8_from_paige(data + offset, length);
		view->DrawString(utf8.c_str(), BPoint(draw_position->from.h, draw_position->from.v));
		co_ordinate underline_pt;
		underline_pt.v = static_cast<short>(draw_position->from.v + 1);
		underline_pt.h = static_cast<short>(draw_position->from.h);
		const short underline_bits = style ? SpecialUnderline(style) : 0;
		if (style && (style->styles[underline_var] || underline_bits))
			pgNativeDrawSpecialUnderline(pg, underline_pt, static_cast<short>(width), style, underline_bits);
		if (style && style->styles[strikeout_var]) {
			underline_pt.v = static_cast<short>(draw_position->from.v - style->ascent / 3);
			pgNativeDrawSpecialUnderline(pg, underline_pt, static_cast<short>(width), style, 0);
		}
		if (style && style->styles[overline_var]) {
			underline_pt.v = static_cast<short>(draw_position->from.v - style->ascent);
			pgNativeDrawSpecialUnderline(pg, underline_pt, static_cast<short>(width), style, 0);
		}
	}
	draw_position->from.h += width;
}

void pgNativeDrawHilite(paige_rec_ptr pg, shape_ref rgn)
{
	fill_shape(view_from_pg(pg), rgn, rgb_color{64, 128, 255, 90});
}

void pgNativeDrawCursor(paige_rec_ptr pg, t_select_ptr select, short verb)
{
	if (!pg || !select)
		return;
	if (verb == hide_cursor || verb == deactivate_cursor || verb == dont_draw_cursor
			|| (pg->flags2 & NO_CARET_BIT) || (pg->flags & NO_EDIT_BIT)) {
		pg->flags &= ~CARET_BIT;
		return;
	}
	BView *view = view_from_pg(pg);
	if (view) {
		const float x = select->primary_caret ? select->primary_caret : select->original_pt.h;
		view->SetHighColor(0, 0, 0);
		view->StrokeLine(BPoint(x, select->original_pt.v - 14), BPoint(x, select->original_pt.v + 2));
	}
	pg->flags |= CARET_BIT;
}

void pgNativeClipGrafDevice(paige_rec_ptr pg, pg_region clip_region)
{
	BView *view = view_from_pg(pg);
	if (!view)
		return;
	BRegion region;
	if (clip_region) {
		for (const rectangle &rect : clip_region->rects)
			region.Include(brect_from_paige(&rect));
	}
	view->ConstrainClippingRegion(&region);
}

void pgNativeEraseRect(pg_globals_ptr globals, rectangle_ptr rect,
		pg_scale_ptr scaling, co_ordinate_ptr offset_extra)
{
	if (!globals || !rect || pgEmptyRect(rect))
		return;
	rectangle converted;
	pgScaleRectToRect(scaling, rect, &converted, offset_extra);
	BView *view = view_from_device(globals->current_port);
	if (!view)
		return;
	view->SetHighColor(rgb_from_paige(&globals->current_port->bk_color));
	view->FillRect(brect_from_paige(&converted));
}

pg_region pgNativeScrollRect(paige_rec_ptr pg, rectangle_ptr rect, long distance_h,
		long distance_v, rectangle_ptr affected_area, short draw_mode)
{
	(void)draw_mode;
	BView *view = view_from_pg(pg);
	if (view && rect) {
		BRect src = brect_from_paige(rect);
		BRect dst = src;
		dst.OffsetBy(distance_h, distance_v);
		view->CopyBits(src, dst);
	}
	if (affected_area && rect)
		*affected_area = *rect;
	pg_region result = new pg_modern_region;
	if (rect)
		result->rects.push_back(*rect);
	return result;
}

void pgNativePrepareOffscreen(paige_rec_ptr pg, rectangle_ptr target_area,
		rectangle_ptr real_bits_target, co_ordinate_ptr offset_adjust,
		long text_offset, point_start_ptr line_start, short draw_mode)
{
	(void)pg;
	(void)text_offset;
	(void)line_start;
	(void)draw_mode;
	if (real_bits_target && target_area)
		*real_bits_target = *target_area;
	if (offset_adjust)
		offset_adjust->h = offset_adjust->v = 0;
}

pg_boolean pgNativeFinishOffscreen(paige_rec_ptr pg, long text_offset,
		point_start_ptr line_start, co_ordinate_ptr new_offset,
		rectangle_ptr new_target, short draw_mode)
{
	(void)pg;
	(void)text_offset;
	(void)line_start;
	(void)new_offset;
	(void)new_target;
	(void)draw_mode;
	return FALSE;
}

pg_boolean pgNativeIsCaretTime(paige_rec_ptr pg)
{
	if (!pg)
		return FALSE;
	const bigtime_t now = system_time() / 500000;
	if (pg->port.caret_info == now)
		return FALSE;
	pg->port.caret_info = now;
	return TRUE;
}

short pgNativeGetCharWidth(paige_rec_ptr pg, style_info_ptr style, pg_char the_char)
{
	(void)pg;
	return static_cast<short>(std::lround(measure_char(cached_font(style, nullptr), the_char)));
}

void pgNativeDrawSpecialUnderline(paige_rec_ptr pg, co_ordinate from_pt,
		short distance, style_info_ptr style, short draw_bits)
{
	BView *view = view_from_pg(pg);
	if (!view || !style || distance <= 0)
		return;
	view->SetHighColor(rgb_from_paige(&style->fg_color));
	if (draw_bits & GRAY_UNDERLINE) {
		for (short x = 0; x < distance; x += 3)
			view->StrokeLine(BPoint(from_pt.h + x, from_pt.v), BPoint(from_pt.h + x + 1, from_pt.v));
	}
	else
		view->StrokeLine(BPoint(from_pt.h, from_pt.v), BPoint(from_pt.h + distance, from_pt.v));
	if (draw_bits & DOUBLE_UNDERLINE)
		view->StrokeLine(BPoint(from_pt.h, from_pt.v + 2), BPoint(from_pt.h + distance, from_pt.v + 2));
}

pg_boolean pgNativeClipboardWrite(pg_os_type native_format, int scrap_type,
		memory_ref dataref, pg_boolean clear_scrap)
{
	(void)native_format;
	if (!be_clipboard || !dataref || !be_clipboard->Lock())
		return FALSE;
	if (clear_scrap)
		be_clipboard->Clear();
	BMessage *message = be_clipboard->Data();
	const size_t byte_size = GetByteSize(dataref);
	void *bytes = UseMemory(dataref);
	bool ok = false;
	if (message) {
		if (scrap_type == pg_native_scrap)
			ok = message->AddData(kPaigeNativeMime, B_MIME_TYPE, bytes, byte_size) == B_OK;
		else if (scrap_type == pg_text_scrap)
			ok = message->AddData("text/plain", B_MIME_TYPE, bytes, byte_size) == B_OK;
		else if (scrap_type == pg_unicode_scrap) {
			std::string utf8 = utf8_from_paige(static_cast<pg_char_ptr>(bytes), GetMemorySize(dataref));
			ok = message->AddData(kPaigeUnicodeMime, B_MIME_TYPE, utf8.data(), utf8.size()) == B_OK
					&& message->AddData("text/plain", B_MIME_TYPE, utf8.data(), utf8.size()) == B_OK;
		}
	}
	UnuseMemory(dataref);
	if (ok)
		be_clipboard->Commit();
	be_clipboard->Unlock();
	return static_cast<pg_boolean>(ok);
}

memory_ref pgNativeClipboardRead(pgm_globals_ptr mem_globals, pg_os_type native_format,
		short PG_FAR *scrap_type, long PG_FAR *embed_type, size_t PG_FAR *raw_data_size,
		pg_boolean load_real_data)
{
	(void)native_format;
	if (embed_type)
		*embed_type = 0;
	if (!be_clipboard || !be_clipboard->Lock())
		return MEM_NULL;
	BMessage *message = be_clipboard->Data();
	const void *data = nullptr;
	ssize_t size = 0;
	short found = pg_void_scrap;
	memory_ref result = MEM_NULL;
	if (message && message->FindData(kPaigeNativeMime, B_MIME_TYPE, &data, &size) == B_OK)
		found = pg_native_scrap;
	else if (message && message->FindData(kPaigeUnicodeMime, B_MIME_TYPE, &data, &size) == B_OK)
		found = pg_text_scrap;
	else if (message && message->FindData("text/plain", B_MIME_TYPE, &data, &size) == B_OK)
		found = pg_text_scrap;
	if (scrap_type)
		*scrap_type = found;
	if (raw_data_size)
		*raw_data_size = size > 0 ? static_cast<size_t>(size) : 0;
	if (load_real_data && found != pg_void_scrap && data && size > 0)
		result = memory_ref_from_bytes(mem_globals, data, static_cast<size_t>(size), 1);
	be_clipboard->Unlock();
	return result;
}

pg_boolean pgNativeClipboardAvailable(pg_os_type native_format)
{
	short scrap_type = pg_void_scrap;
	pgNativeClipboardRead(nullptr, native_format, &scrap_type, nullptr, nullptr, FALSE);
	return static_cast<pg_boolean>(scrap_type != pg_void_scrap);
}

void pgNativeClipboardClear(void)
{
	if (be_clipboard && be_clipboard->Lock()) {
		be_clipboard->Clear();
		be_clipboard->Commit();
		be_clipboard->Unlock();
	}
}

#endif
