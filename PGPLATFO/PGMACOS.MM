#include "PGMODERNNATIVE.H"

#if defined(POSIX_PLATFORM) && defined(MACOS_PLATFORM)

#include "MACHINE.H"
#include "PGDEFSTL.H"

#ifdef ExitToShell
#undef ExitToShell
#endif

#import <AppKit/AppKit.h>
#import <CoreGraphics/CoreGraphics.h>
#import <CoreText/CoreText.h>

#include <algorithm>
#include <cmath>
#include <cstring>
#include <cstdint>
#include <string>
#include <vector>

#ifndef DEF_POINT_SIZE
#define DEF_POINT_SIZE		(long)0x000C
#endif
#ifndef CR_CHAR
#define CR_CHAR				0x0D
#define SOFT_CR_CHAR		0x0A
#define TAB_CHAR			0x09
#define FF_CHAR				0x0C
#define CONTAINER_BRK_CHAR	0x0E
#endif

extern "C" PG_PASCAL (long) pgCStrLength(const pg_c_string_ptr str);

static NSString *const kPaigeNativePasteboardType = @"com.hermes.paige.native";

namespace {

short point_size_from_style(style_info_ptr style)
{
	if (!style || !style->point)
		return static_cast<short>(DEF_POINT_SIZE);
	return static_cast<short>(std::max<long>(1, style->point >> 16));
}

CGFloat cg_channel(unsigned short value)
{
	return static_cast<CGFloat>(value) / 65535.0;
}

void set_cg_color(CGContextRef context, color_value_ptr color, CGFloat alpha = 1.0)
{
	if (!context || !color)
		return;
	CGContextSetRGBFillColor(context, cg_channel(color->red), cg_channel(color->green),
			cg_channel(color->blue), alpha);
	CGContextSetRGBStrokeColor(context, cg_channel(color->red), cg_channel(color->green),
			cg_channel(color->blue), alpha);
}

CGRect cg_rect_from_paige(rectangle_ptr rect)
{
	if (!rect)
		return CGRectZero;
	const CGFloat left = static_cast<CGFloat>(rect->top_left.h);
	const CGFloat top = static_cast<CGFloat>(rect->top_left.v);
	const CGFloat right = static_cast<CGFloat>(rect->bot_right.h);
	const CGFloat bottom = static_cast<CGFloat>(rect->bot_right.v);
	return CGRectMake(left, top, std::max<CGFloat>(0, right - left), std::max<CGFloat>(0, bottom - top));
}

bool empty_paige_rect(rectangle_ptr rect)
{
	return !rect || rect->top_left.h >= rect->bot_right.h || rect->top_left.v >= rect->bot_right.v;
}

CGContextRef context_from_device(graf_device_ptr device)
{
	return device ? reinterpret_cast<CGContextRef>(device->machine_var) : nullptr;
}

CGContextRef context_from_pg(paige_rec_ptr pg)
{
	if (!pg)
		return nullptr;
	graf_device_ptr device = pg->globals && pg->globals->current_port ? pg->globals->current_port : &pg->port;
	return context_from_device(device);
}

CFStringRef create_string_from_paige(pg_char_ptr text, size_t length)
{
	if (!text || !length)
		return CFSTR("");
	return CFStringCreateWithCharacters(kCFAllocatorDefault,
			reinterpret_cast<const UniChar *>(text), static_cast<CFIndex>(length));
}

CFStringRef create_font_name(font_info_ptr font)
{
	if (!font || !font->name[0])
		return CFStringCreateWithCString(kCFAllocatorDefault, "Helvetica", kCFStringEncodingUTF8);

	if (font->environs & NAME_IS_CSTR) {
		const long length = pgCStrLength(font->name);
		return length ? CFStringCreateWithCharacters(kCFAllocatorDefault,
				reinterpret_cast<const UniChar *>(font->name), length)
				: CFStringCreateWithCString(kCFAllocatorDefault, "Helvetica", kCFStringEncodingUTF8);
	}

	const CFIndex length = static_cast<CFIndex>(std::min<int>(font->name[0], FONT_SIZE - 1));
	return length ? CFStringCreateWithCharacters(kCFAllocatorDefault,
			reinterpret_cast<const UniChar *>(&font->name[1]), length)
			: CFStringCreateWithCString(kCFAllocatorDefault, "Helvetica", kCFStringEncodingUTF8);
}

CTFontRef create_font(style_info_ptr style, font_info_ptr font)
{
	const CGFloat size = static_cast<CGFloat>(point_size_from_style(style));
	CFStringRef name = create_font_name(font);
	CTFontRef base = CTFontCreateWithName(name, size, nullptr);
	CFRelease(name);

	if (!base)
		base = CTFontCreateWithName(CFSTR("Helvetica"), size, nullptr);
	if (!base)
		return nullptr;

	CTFontSymbolicTraits traits = 0;
	if (style && style->styles[bold_var])
		traits |= kCTFontBoldTrait;
	if (style && style->styles[italic_var])
		traits |= kCTFontItalicTrait;

	if (!traits)
		return base;

	CTFontRef styled = CTFontCreateCopyWithSymbolicTraits(base, size, nullptr, traits, traits);
	if (styled) {
		CFRelease(base);
		return styled;
	}

	return base;
}

CTFontRef cached_font(style_info_ptr style, font_info_ptr font)
{
	if (!style)
		return create_font(style, font);
	if (!style->machine_var)
		style->machine_var = reinterpret_cast<size_t>(create_font(style, font));
	return reinterpret_cast<CTFontRef>(style->machine_var);
}

CGFloat measure_cfstring(CTFontRef font, CFStringRef string)
{
	if (!font || !string)
		return 0;
	CFTypeRef keys[] = {kCTFontAttributeName};
	CFTypeRef values[] = {font};
	CFDictionaryRef attrs = CFDictionaryCreate(kCFAllocatorDefault, keys, values, 1,
			&kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
	CFAttributedStringRef attr = CFAttributedStringCreate(kCFAllocatorDefault, string, attrs);
	CTLineRef line = CTLineCreateWithAttributedString(attr);
	const CGFloat width = static_cast<CGFloat>(CTLineGetTypographicBounds(line, nullptr, nullptr, nullptr));
	CFRelease(line);
	CFRelease(attr);
	CFRelease(attrs);
	return width;
}

CGFloat measure_char(CTFontRef font, pg_char ch)
{
	if (ch == TAB_CHAR)
		return measure_char(font, static_cast<pg_char>(' ')) * 4.0;
	if (ch == CR_CHAR || ch == SOFT_CR_CHAR || ch == FF_CHAR || ch == CONTAINER_BRK_CHAR)
		return 0;
	CFStringRef string = create_string_from_paige(&ch, 1);
	const CGFloat width = measure_cfstring(font, string);
	CFRelease(string);
	return width;
}

void fill_shape(CGContextRef context, shape_ref shape, color_value_ptr color, CGFloat alpha)
{
	if (!context || !shape)
		return;
	const size_t count = GetMemorySize(shape);
	if (!count)
		return;
	auto *rects = static_cast<rectangle_ptr>(UseMemory(shape));
	set_cg_color(context, color, alpha);
	const size_t first = count > 1 ? 1 : 0;
	for (size_t index = first; index < count; ++index) {
		CGRect rect = cg_rect_from_paige(&rects[index]);
		if (!CGRectIsEmpty(rect))
			CGContextFillRect(context, rect);
	}
	UnuseMemory(shape);
}

void copy_memory_ref_to_pasteboard(NSPasteboard *pasteboard, NSString *type, memory_ref dataref)
{
	if (!pasteboard || !dataref)
		return;
	const size_t byte_size = GetByteSize(dataref);
	void *bytes = UseMemory(dataref);
	NSData *data = [NSData dataWithBytes:bytes length:byte_size];
	UnuseMemory(dataref);
	[pasteboard setData:data forType:type];
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
	if (!font)
		return false;
	CFStringRef name = create_font_name(font);
	CTFontRef font_ref = CTFontCreateWithName(name, 12.0, nullptr);
	CFRelease(name);
	if (font_ref) {
		CFRelease(font_ref);
		font->environs |= FONT_GOOD;
		font->environs &= ~(FONT_NOT_AVAIL | FONT_BEST_GUESS);
		return true;
	}
	font->environs |= FONT_NOT_AVAIL | FONT_BEST_GUESS;
	return false;
}

void pgNativeStyleMetrics(paige_rec_ptr pg, style_info_ptr style, font_info_ptr font)
{
	(void)pg;
	CTFontRef font_ref = cached_font(style, font);
	if (!style || !font_ref)
		return;
	style->ascent = static_cast<short>(std::ceil(CTFontGetAscent(font_ref)));
	style->descent = static_cast<short>(std::ceil(CTFontGetDescent(font_ref)));
	style->leading = static_cast<short>(std::ceil(CTFontGetLeading(font_ref)));
	const CGFloat w_width = measure_char(font_ref, static_cast<pg_char>('W'));
	style->char_width = static_cast<pg_fixed>(std::max<CGFloat>(1, w_width)) << 16;
	style->left_overhang = 0;
	style->right_overhang = 0;
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
	CFRelease(reinterpret_cast<CTFontRef>(style->machine_var));
	style->machine_var = 0;
	style->machine_var2 = 0;
}

long pgNativeMeasureText(paige_rec_ptr pg, style_walk_ptr walker, style_info_ptr style,
		font_info_ptr font, pg_char_ptr data, size_t length, long slop, long num_spaces,
		long PG_FAR *positions)
{
	(void)pg;
	CTFontRef font_ref = cached_font(style, font ? font : (walker ? walker->cur_font : nullptr));
	const CGFloat space_extra = num_spaces ? static_cast<CGFloat>(slop) / static_cast<CGFloat>(num_spaces) : 0;
	CGFloat total = 0;

	for (size_t index = 0; index < length; ++index) {
		const pg_char ch = data ? data[index] : 0;
		CGFloat width = measure_char(font_ref, ch);
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
	if (!pg || !walker || !data || !draw_position || length <= 0) {
		if (draw_position)
			draw_position->from.h += length > 0 ? length : 0;
		return;
	}

	CGContextRef context = context_from_pg(pg);
	style_info_ptr style = walker->cur_style;
	font_info_ptr font = walker->cur_font;
	CTFontRef font_ref = cached_font(style, font);
	if (!context || !font_ref) {
		draw_position->from.h += pgNativeMeasureText(pg, walker, style, font, data + offset, length, extra, 0, nullptr);
		return;
	}

	const long spaces = std::count(data + offset, data + offset + length, static_cast<pg_char>(' '));
	const long text_width = pgNativeMeasureText(pg, walker, style, font, data + offset, length, extra, spaces, nullptr);
	const CGFloat x = static_cast<CGFloat>(draw_position->from.h);
	const CGFloat baseline = static_cast<CGFloat>(draw_position->from.v);
	const CGFloat ascent = std::max<CGFloat>(1, style ? style->ascent : CTFontGetAscent(font_ref));
	const CGFloat descent = std::max<CGFloat>(1, style ? style->descent : CTFontGetDescent(font_ref));

	CGContextSaveGState(context);
	if (draw_mode == direct_xor || draw_mode == bits_xor || draw_mode == bits_emulate_xor)
		CGContextSetBlendMode(context, kCGBlendModeDifference);
	else
		CGContextSetBlendMode(context, kCGBlendModeNormal);

	if (style && !pgTransColor(pg->globals, &style->bk_color)) {
		set_cg_color(context, &style->bk_color);
		CGContextFillRect(context, CGRectMake(x, baseline - ascent, text_width, ascent + descent + style->leading));
	}

	CFStringRef string = create_string_from_paige(data + offset, length);
	CFTypeRef keys[] = {kCTFontAttributeName, kCTForegroundColorFromContextAttributeName};
	CFTypeRef values[] = {font_ref, kCFBooleanTrue};
	CFDictionaryRef attrs = CFDictionaryCreate(kCFAllocatorDefault, keys, values, 2,
			&kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
	CFAttributedStringRef attr = CFAttributedStringCreate(kCFAllocatorDefault, string, attrs);
	CTLineRef line = CTLineCreateWithAttributedString(attr);

	color_value draw_color = style ? style->fg_color : pg->globals->def_bk_color;
	if (!style || (draw_color.red == 0 && draw_color.green == 0 && draw_color.blue == 0 && draw_color.alpha == 0))
		draw_color.red = draw_color.green = draw_color.blue = 0;
	set_cg_color(context, &draw_color, (draw_mode == grayish_or || draw_mode == grayish_bits_copy) ? 0.45 : 1.0);
	CGContextSetTextMatrix(context, CGAffineTransformIdentity);
	CGContextSetTextPosition(context, x, baseline);
	CTLineDraw(line, context);

	co_ordinate underline_pt;
	underline_pt.h = static_cast<short>(std::lround(x));
	underline_pt.v = static_cast<short>(std::lround(baseline + 1));
	const short underline_bits = style ? SpecialUnderline(style) : 0;
	if (style && (style->styles[underline_var] || underline_bits))
		pgNativeDrawSpecialUnderline(pg, underline_pt, static_cast<short>(text_width), style, underline_bits);
	if (style && style->styles[strikeout_var]) {
		underline_pt.v = static_cast<short>(std::lround(baseline - ascent / 3.0));
		pgNativeDrawSpecialUnderline(pg, underline_pt, static_cast<short>(text_width), style, 0);
	}
	if (style && style->styles[overline_var]) {
		underline_pt.v = static_cast<short>(std::lround(baseline - ascent));
		pgNativeDrawSpecialUnderline(pg, underline_pt, static_cast<short>(text_width), style, 0);
	}

	CFRelease(line);
	CFRelease(attr);
	CFRelease(attrs);
	CFRelease(string);
	CGContextRestoreGState(context);

	draw_position->from.h += text_width;
}

void pgNativeDrawHilite(paige_rec_ptr pg, shape_ref rgn)
{
	CGContextRef context = context_from_pg(pg);
	if (!context)
		return;
	color_value hilite = {0x3333, 0x7777, 0xFFFF, 0};
	CGContextSaveGState(context);
	fill_shape(context, rgn, &hilite, 0.35);
	CGContextRestoreGState(context);
}

void pgNativeDrawCursor(paige_rec_ptr pg, t_select_ptr select, short verb)
{
	if (!pg || !select)
		return;
	const bool showing = (verb == show_cursor || verb == toggle_cursor || verb == toggle_cursor_idle
			|| verb == restore_cursor || verb == update_cursor);
	if (!showing || (pg->flags2 & NO_CARET_BIT) || (pg->flags & NO_EDIT_BIT)) {
		pg->flags &= ~CARET_BIT;
		return;
	}

	CGContextRef context = context_from_pg(pg);
	if (context) {
		CGContextSaveGState(context);
		CGContextSetRGBFillColor(context, 0, 0, 0, 1);
		const CGFloat x = static_cast<CGFloat>(select->primary_caret ? select->primary_caret : select->original_pt.h);
		const CGFloat y = static_cast<CGFloat>(select->original_pt.v - 14);
		CGContextFillRect(context, CGRectMake(x, y, 1, 16));
		CGContextRestoreGState(context);
	}
	pg->flags |= CARET_BIT;
}

void pgNativeClipGrafDevice(paige_rec_ptr pg, pg_region clip_region)
{
	CGContextRef context = context_from_pg(pg);
	if (!context || !clip_region)
		return;
	if (clip_region->rects.empty()) {
		CGContextClipToRect(context, CGRectZero);
		return;
	}

	std::vector<CGRect> rects;
	rects.reserve(clip_region->rects.size());
	for (const rectangle &rect : clip_region->rects) {
		const CGFloat width = std::max<CGFloat>(0, rect.bot_right.h - rect.top_left.h);
		const CGFloat height = std::max<CGFloat>(0, rect.bot_right.v - rect.top_left.v);
		if (width > 0 && height > 0)
			rects.push_back(CGRectMake(rect.top_left.h, rect.top_left.v, width, height));
	}
	if (!rects.empty())
		CGContextClipToRects(context, rects.data(), rects.size());
}

void pgNativeEraseRect(pg_globals_ptr globals, rectangle_ptr rect,
		pg_scale_ptr scaling, co_ordinate_ptr offset_extra)
{
	if (!globals || empty_paige_rect(rect))
		return;
	rectangle converted;
	pgScaleRectToRect(scaling, rect, &converted, offset_extra);
	CGContextRef context = context_from_device(globals->current_port);
	if (!context)
		return;
	CGContextSaveGState(context);
	set_cg_color(context, &globals->current_port->bk_color);
	CGContextFillRect(context, cg_rect_from_paige(&converted));
	CGContextRestoreGState(context);
}

pg_region pgNativeScrollRect(paige_rec_ptr pg, rectangle_ptr rect, long distance_h,
		long distance_v, rectangle_ptr affected_area, short draw_mode)
{
	(void)pg;
	(void)draw_mode;
	if (affected_area && rect) {
		*affected_area = *rect;
		affected_area->bot_right.h += distance_h;
		affected_area->bot_right.v += distance_v;
	}
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
	const NSTimeInterval now = [NSDate timeIntervalSinceReferenceDate];
	const long tick = static_cast<long>(now * 2.0);
	if (pg->port.caret_info == tick)
		return FALSE;
	pg->port.caret_info = tick;
	return TRUE;
}

short pgNativeGetCharWidth(paige_rec_ptr pg, style_info_ptr style, pg_char the_char)
{
	(void)pg;
	CTFontRef font = cached_font(style, nullptr);
	return static_cast<short>(std::lround(measure_char(font, the_char)));
}

void pgNativeDrawSpecialUnderline(paige_rec_ptr pg, co_ordinate from_pt,
		short distance, style_info_ptr style, short draw_bits)
{
	CGContextRef context = context_from_pg(pg);
	if (!context || !style || distance <= 0)
		return;
	CGContextSaveGState(context);
	set_cg_color(context, &style->fg_color);
	CGContextSetLineWidth(context, (draw_bits & DOUBLE_UNDERLINE) ? 1.0 : 1.0);
	if (draw_bits & GRAY_UNDERLINE) {
		CGFloat dash[] = {1.0, 2.0};
		CGContextSetLineDash(context, 0, dash, 2);
	}
	const CGFloat x = static_cast<CGFloat>(from_pt.h);
	const CGFloat y = static_cast<CGFloat>(from_pt.v);
	CGContextMoveToPoint(context, x, y);
	CGContextAddLineToPoint(context, x + distance, y);
	CGContextStrokePath(context);
	if (draw_bits & DOUBLE_UNDERLINE) {
		CGContextMoveToPoint(context, x, y + 2.0);
		CGContextAddLineToPoint(context, x + distance, y + 2.0);
		CGContextStrokePath(context);
	}
	CGContextRestoreGState(context);
}

pg_boolean pgNativeClipboardWrite(pg_os_type native_format, int scrap_type,
		memory_ref dataref, pg_boolean clear_scrap)
{
	(void)native_format;
	if (!dataref)
		return FALSE;
	NSPasteboard *pasteboard = [NSPasteboard generalPasteboard];
	if (clear_scrap)
		[pasteboard clearContents];

	switch (scrap_type) {
		case pg_native_scrap:
			copy_memory_ref_to_pasteboard(pasteboard, kPaigeNativePasteboardType, dataref);
			return TRUE;
		case pg_text_scrap: {
			const size_t byte_size = GetByteSize(dataref);
			void *bytes = UseMemory(dataref);
			NSString *string = [[NSString alloc] initWithBytes:bytes length:byte_size encoding:NSUTF8StringEncoding];
			if (!string)
				string = [[NSString alloc] initWithBytes:bytes length:byte_size encoding:NSMacOSRomanStringEncoding];
			UnuseMemory(dataref);
			if (string) {
				[pasteboard setString:string forType:NSPasteboardTypeString];
				[string release];
				return TRUE;
			}
			break;
		}
		case pg_unicode_scrap: {
			const size_t chars = GetMemorySize(dataref);
			pg_char_ptr text = static_cast<pg_char_ptr>(UseMemory(dataref));
			NSString *string = [[NSString alloc] initWithCharacters:reinterpret_cast<const unichar *>(text) length:chars];
			UnuseMemory(dataref);
			if (string) {
				[pasteboard setString:string forType:NSPasteboardTypeString];
				[string release];
				return TRUE;
			}
			break;
		}
		default:
			break;
	}
	return FALSE;
}

memory_ref pgNativeClipboardRead(pgm_globals_ptr mem_globals, pg_os_type native_format,
		short PG_FAR *scrap_type, long PG_FAR *embed_type, size_t PG_FAR *raw_data_size,
		pg_boolean load_real_data)
{
	(void)native_format;
	if (embed_type)
		*embed_type = 0;
	NSPasteboard *pasteboard = [NSPasteboard generalPasteboard];
	NSData *native_data = [pasteboard dataForType:kPaigeNativePasteboardType];
	if (native_data) {
		if (scrap_type)
			*scrap_type = pg_native_scrap;
		if (raw_data_size)
			*raw_data_size = [native_data length];
		return load_real_data ? memory_ref_from_bytes(mem_globals, [native_data bytes], [native_data length], 1) : MEM_NULL;
	}

	NSString *string = [pasteboard stringForType:NSPasteboardTypeString];
	if (string) {
		if (scrap_type)
			*scrap_type = pg_unicode_scrap;
		const NSUInteger chars = [string length];
		if (raw_data_size)
			*raw_data_size = chars;
		if (!load_real_data)
			return MEM_NULL;
		memory_ref ref = MemoryAlloc(mem_globals, sizeof(pg_char), chars + 1, 0);
		pg_char_ptr target = static_cast<pg_char_ptr>(UseMemory(ref));
		[string getCharacters:reinterpret_cast<unichar *>(target) range:NSMakeRange(0, chars)];
		target[chars] = 0;
		UnuseMemory(ref);
		return ref;
	}

	if (scrap_type)
		*scrap_type = pg_void_scrap;
	if (raw_data_size)
		*raw_data_size = 0;
	return MEM_NULL;
}

pg_boolean pgNativeClipboardAvailable(pg_os_type native_format)
{
	short scrap_type = pg_void_scrap;
	pgNativeClipboardRead(nullptr, native_format, &scrap_type, nullptr, nullptr, FALSE);
	return static_cast<pg_boolean>(scrap_type != pg_void_scrap);
}

void pgNativeClipboardClear(void)
{
	[[NSPasteboard generalPasteboard] clearContents];
}

#endif
