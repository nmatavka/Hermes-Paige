# 33 WINDOWS CHARACTER WIDTHS

HERMES Paige contains a low-level function you can use to force specific character widths for any given text format.

For example, a cross-platform, HERMES Paige-based application might need to render exact, identical placement of characters drawn in the same font between Macintosh and Windows. As most developers realise, the subtle differences between fonts, even between fonts that are supposedly the same family and type, will not always render the same text widths between platforms, or between changing resolution or printers.

The following function has been created to help with a solution:

	void SetFontCharWidths (pg_ref pg, style_info_ptr style, int PG_FAR *charwidths);

This function causes the rendering of all text drawing in `style` to match pre-determined character widths defined in `charwidths`.

The `charwidths` table must be a pointer to 256 `int` values, each element must correspond to that same ordinal value of the style's character set. For example, `charwidths[0]` represents the width of a null (0) character; `charwidths[' ']` represents the width of a space character, `charwidths['A']` represents the width for an "A" character, etc.

**NOTE:** The character table applies only the precise, composite text format represented by the style parameter. This includes the associated `font_info` record (which is defined by the value in `style -> font_index`).

After this function is called, any text that is drawn in the precise format represented by `style` will be rendered using the widths in `charwidths`.

##### NOTES:

1. The function prototype for `SetFontCharWidths()` is defined in `pgtraps.h`.
2. `SetFontCharWidths()` makes a copy of the character widths; hence, you do not need to keep its array of `int` values around.
3. The `pg` parameter is required to have access to HERMES Paige globals as well as access to a font table (unique to the `pg_ref`). However, the character table you set becomes universal and global for all `pg_ref`s that use exactly the same style.

