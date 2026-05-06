#include <Application.h>
#include <Bitmap.h>
#include <Clipboard.h>
#include <Message.h>
#include <Region.h>
#include <View.h>

#include "PAIGE.H"
#include "PGMEMMGR.H"
#include "PGSCRAP.H"

#include <cstdio>
#include <cstring>

namespace {

constexpr int32 kWidth = 640;
constexpr int32 kHeight = 220;
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

bool bitmap_has_ink(const BBitmap &bitmap)
{
	const uint8 *bits = static_cast<const uint8 *>(bitmap.Bits());
	const size_t length = static_cast<size_t>(bitmap.BitsLength());
	for (size_t index = 0; index + 3 < length; index += 4) {
		if (bits[index] < 245 || bits[index + 1] < 245 || bits[index + 2] < 245)
			return true;
	}
	return false;
}

int fail(const char *message)
{
	std::fprintf(stderr, "paige_haiku_smoke: %s\n", message);
	return 1;
}

BMessage snapshot_clipboard()
{
	BMessage snapshot;
	if (be_clipboard && be_clipboard->Lock()) {
		if (BMessage *data = be_clipboard->Data())
			snapshot = *data;
		be_clipboard->Unlock();
	}
	return snapshot;
}

void restore_clipboard(const BMessage &snapshot)
{
	if (be_clipboard && be_clipboard->Lock()) {
		be_clipboard->Clear();
		if (BMessage *data = be_clipboard->Data())
			*data = snapshot;
		be_clipboard->Commit();
		be_clipboard->Unlock();
	}
}

} // namespace

int main()
{
	BApplication app("application/x-vnd.hermes-paige-smoke");
	BRect bounds(0, 0, kWidth - 1, kHeight - 1);
	BBitmap bitmap(bounds, B_BITMAP_ACCEPTS_VIEWS, B_RGBA32);
	if (!bitmap.IsValid())
		return fail("could not create bitmap");

	BView *view = new BView(bounds, "paige-smoke-view", B_FOLLOW_NONE, B_WILL_DRAW);
	if (bitmap.AddChild(view) != B_OK)
		return fail("could not attach view");

	pgm_globals mem_globals;
	pg_globals globals;
	std::memset(&mem_globals, 0, sizeof(mem_globals));
	std::memset(&globals, 0, sizeof(globals));
	pgMemStartup(&mem_globals, 0);
	pgInit(&globals, &mem_globals);

	graf_device device;
	pgInitDevice(&globals, reinterpret_cast<generic_var>(view), 0, &device);

	rectangle vis_rect = make_rect(0, 0, kHeight, kWidth);
	rectangle page_rect = make_rect(24, 24, kHeight - 24, kWidth - 24);
	shape_ref vis_shape = pgRectToShape(&mem_globals, &vis_rect);
	shape_ref page_shape = pgRectToShape(&mem_globals, &page_rect);
	pg_ref doc = pgNew(&globals, reinterpret_cast<generic_var>(view), vis_shape, page_shape, MEM_NULL,
			NO_HIDDEN_TEXT_BIT);

	style_info style = globals.def_style;
	font_info font = globals.def_font;
	style.styles[bold_var] = 1;
	style.styles[underline_var] = 1;
	style.fg_color.red = 0x0000;
	style.fg_color.green = 0x3333;
	style.fg_color.blue = 0x6666;

	pg_char text[] = {
		'P', 'a', 'i', 'g', 'e', ' ', 'n', 'a', 't', 'i', 'v', 'e', ' ',
		'H', 'a', 'i', 'k', 'u', ' ', 's', 'm', 'o', 'k', 'e', ' ', 0x03C0,
		' ', 't', 'a', 'b', ':', '\t', 'o', 'k', 13,
		'B', 'V', 'i', 'e', 'w', ' ', 'r', 'e', 'n', 'd', 'e', 'r', 0
	};
	const size_t text_length = (sizeof(text) / sizeof(text[0])) - 1;
	pgBeginImport(doc, 0);
	pgInsertText(doc, text, static_cast<long>(text_length), &font, &style, NULL, draw_none);
	pgEndImport(doc, FALSE, draw_none);
	if (pgTextSize(doc) < static_cast<long>(text_length))
		return fail("text insert failed");

	if (view->LockLooper()) {
		view->SetHighColor(255, 255, 255, 255);
		view->FillRect(bounds);
		pgDisplay(doc, &device, vis_shape, page_shape, NULL, direct_or);
		view->Sync();
		view->UnlockLooper();
	}
	const bool rendered = bitmap_has_ink(bitmap);

	BMessage clipboard_snapshot = snapshot_clipboard();
	pgPutScrap(doc, kSmokeNativeFormat, pg_void_scrap);
	bool clipboard_ok = false;
	if (pgScrapAvail(kSmokeNativeFormat)) {
		pg_ref scrap = pgGetScrap(&globals, kSmokeNativeFormat, NULL);
		clipboard_ok = scrap != MEM_NULL;
		if (scrap)
			pgDispose(scrap);
	}
	restore_clipboard(clipboard_snapshot);

	pgDispose(doc);
	pgDisposeShape(vis_shape);
	pgDisposeShape(page_shape);
	pgCloseDevice(&globals, &device);
	pgShutdown(&globals);
	pgMemShutdown(&mem_globals);

	if (!rendered)
		return fail("rendered bitmap was blank");
	if (!clipboard_ok)
		return fail("clipboard round-trip failed");

	std::puts("paige_haiku_smoke: ok");
	return 0;
}
