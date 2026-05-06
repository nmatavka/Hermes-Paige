#import <AppKit/AppKit.h>
#import <CoreGraphics/CoreGraphics.h>

#include "PAIGE.H"
#include "PGMEMMGR.H"
#include "PGSCRAP.H"

#include <cstdio>
#include <cstring>
#include <vector>

namespace {

constexpr size_t kWidth = 640;
constexpr size_t kHeight = 220;
constexpr pg_os_type kSmokeNativeFormat = 0x50414745u; // PAGE

rectangle make_rect(long top, long left, long bottom, long right)
{
	rectangle rect;
	rect.top_left.v = top;
	rect.top_left.h = left;
	rect.bot_right.v = bottom;
	rect.bot_right.h = right;
	return rect;
}

bool bitmap_has_ink(const std::vector<unsigned char> &pixels)
{
	for (size_t index = 0; index + 3 < pixels.size(); index += 4) {
		if (pixels[index] < 245 || pixels[index + 1] < 245 || pixels[index + 2] < 245)
			return true;
	}
	return false;
}

NSMutableDictionary *snapshot_pasteboard(NSPasteboard *pasteboard)
{
	NSMutableDictionary *snapshot = [[NSMutableDictionary alloc] init];
	for (NSPasteboardType type in [pasteboard types]) {
		NSData *data = [pasteboard dataForType:type];
		if (data)
			[snapshot setObject:data forKey:type];
	}
	return snapshot;
}

void restore_pasteboard(NSPasteboard *pasteboard, NSDictionary *snapshot)
{
	[pasteboard clearContents];
	for (NSPasteboardType type in snapshot) {
		NSData *data = [snapshot objectForKey:type];
		if (data)
			[pasteboard setData:data forType:type];
	}
}

int fail(const char *message)
{
	std::fprintf(stderr, "paige_macos_smoke: %s\n", message);
	return 1;
}

} // namespace

int main()
{
	@autoreleasepool {
		std::vector<unsigned char> pixels(kWidth * kHeight * 4, 0xFF);
		CGColorSpaceRef color_space = CGColorSpaceCreateDeviceRGB();
		CGContextRef context = CGBitmapContextCreate(pixels.data(), kWidth, kHeight, 8, kWidth * 4,
				color_space, kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big);
		CGColorSpaceRelease(color_space);
		if (!context)
			return fail("could not create bitmap context");

		CGContextSetRGBFillColor(context, 1.0, 1.0, 1.0, 1.0);
		CGContextFillRect(context, CGRectMake(0, 0, kWidth, kHeight));

		pgm_globals mem_globals;
		pg_globals globals;
		std::memset(&mem_globals, 0, sizeof(mem_globals));
		std::memset(&globals, 0, sizeof(globals));
		pgMemStartup(&mem_globals, 0);
		pgInit(&globals, &mem_globals);

		graf_device device;
		pgInitDevice(&globals, reinterpret_cast<generic_var>(context), 0, &device);

		rectangle vis_rect = make_rect(0, 0, static_cast<long>(kHeight), static_cast<long>(kWidth));
		rectangle page_rect = make_rect(24, 24, static_cast<long>(kHeight - 24), static_cast<long>(kWidth - 24));
		shape_ref vis_shape = pgRectToShape(&mem_globals, &vis_rect);
		shape_ref page_shape = pgRectToShape(&mem_globals, &page_rect);
		pg_ref doc = pgNew(&globals, reinterpret_cast<generic_var>(context), vis_shape, page_shape, MEM_NULL,
				NO_HIDDEN_TEXT_BIT);

		style_info style = globals.def_style;
		font_info font = globals.def_font;
		style.styles[bold_var] = 1;
		style.styles[underline_var] = 1;
		style.fg_color.red = 0x0000;
		style.fg_color.green = 0x2222;
		style.fg_color.blue = 0x7777;

		pg_char text[] = {
			'P', 'a', 'i', 'g', 'e', ' ', 'n', 'a', 't', 'i', 'v', 'e', ' ',
			'm', 'a', 'c', 'O', 'S', ' ', 's', 'm', 'o', 'k', 'e', ' ', 0x03C0,
			' ', 't', 'a', 'b', ':', '\t', 'o', 'k', 13,
			'C', 'o', 'r', 'e', ' ', 'T', 'e', 'x', 't', ' ', 'r', 'e', 'n', 'd', 'e', 'r', 0
		};
		const size_t text_length = (sizeof(text) / sizeof(text[0])) - 1;
		pgBeginImport(doc, 0);
		pgInsertText(doc, text, static_cast<long>(text_length), &font, &style, NULL, draw_none);
		pgEndImport(doc, FALSE, draw_none);
		if (pgTextSize(doc) < static_cast<long>(text_length))
			return fail("text insert failed");

		pgDisplay(doc, &device, vis_shape, page_shape, NULL, direct_or);
		CGContextFlush(context);
		const bool rendered = bitmap_has_ink(pixels);

		NSPasteboard *pasteboard = [NSPasteboard generalPasteboard];
		NSMutableDictionary *snapshot = snapshot_pasteboard(pasteboard);
		bool clipboard_ok = false;
		pgPutScrap(doc, kSmokeNativeFormat, pg_void_scrap);
		if (pgScrapAvail(kSmokeNativeFormat)) {
			pg_ref scrap = pgGetScrap(&globals, kSmokeNativeFormat, NULL);
			clipboard_ok = scrap != MEM_NULL;
			if (scrap)
				pgDispose(scrap);
		}
		restore_pasteboard(pasteboard, snapshot);
		[snapshot release];

		pgDispose(doc);
		pgDisposeShape(vis_shape);
		pgDisposeShape(page_shape);
		pgCloseDevice(&globals, &device);
		pgShutdown(&globals);
		pgMemShutdown(&mem_globals);
		CGContextRelease(context);

		if (!rendered)
			return fail("rendered bitmap was blank");
		if (!clipboard_ok)
			return fail("clipboard round-trip failed");

		std::puts("paige_macos_smoke: ok");
		return 0;
	}
}
