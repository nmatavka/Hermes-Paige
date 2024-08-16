## STYLE SHEETS

A style sheet in OpenPaige is a text format; and/or a paragraph format that is "shared" by various characters in document. Although a style sheet contains the same info as regular formats, affected text essentially "points" to these styles. A change to a single style sheet will change every place in the text that uses that style.

NOTE: The style_info record structure is described in "style_info" on page 30-21.

### 31.1 New sheets

$$
\begin{aligned}
& \text { (short) pgNewStyle (pg_ref pg, style_info_ptr new_style, font_info_ptr } \\
& \text { style_font); }
\end{aligned}
$$

Establishes a new style sheet and returns a unique ID code for that style.

FUNCTION RESULT: No text is changed from this function; all that occurs is new_style is added internally to $p g$, style_font is added (if it does not exist
already) and is assigned to the new style; the style is assigned a unique number which can be referenced in subsequent calls that affect such styles. The reference number for the style will never be zero.

new_style can contain anything a regular text style contains.

### 31.2 Remove style

(void) pgRemoveStyle (pg_ref pg, short style_id);

Removes the style sheet referenced as style_id. Immediately after this call is made, style_id will no longer be valid.

However, the text is not affected. The style_info that used to be a style sheet simply changes to a regular style run item; locations in the text that are set to style_id will retain their styles but each occurrence is no longer linked with the style_sheet reference.

### 31.3 Style count \& "Indexing"

(short) pgNumStyles (pg_ref pg);

(short) pgNumParStyles (pg_ref pg);

Returns the total number of style sheets in $p g$; pgNumParStyles() returns the total number of paragraph style sheets.
short pgGetIndStyleSheet (pg_ref pg, short index, style_info_ptr stylesheet);

short pgGetlndParStyleSheet (pg_ref pg, short index, style_info_ptr stylesheet);

Returns the nth style sheet found in $p g$. The style sheet to return is given in index (first style sheet is zero); pgGetIndStyleSheet() returns a regular (text) style while pgGetIndParStyleSheet() returns a paragraph style sheet.

Using these in conjunction with pgNumStyles() / pgNumParStyles() can provide "random access" to styles sheets existing in $p g$.

If the requested style sheet is found and stylesheet is nonnull, this style_info or par_info is initialized to the settings of the sheet.

FUNCTION RESULT: If index style sheet exists, its ID is returned. If no such style sheet exists, zero is returned.

### 31.4 Searching for a Style Sheet

short pgFindStyleSheet (pg_ref pg, par_info_ptr compare_style, par_info_ptr mask);

This function returns the style sheet ID, if any, whose style_info fields match compare_style exactly. If no match is found, zero is returned.

The mask parameter can be used to do partial or selective comparisons. If mask is nonnull, only the style_info fields that are nonzero in this structure are compared.

For example, to locate a style sheet that had a specific value in style_info.user_id, clear the mask to all zeros except user_id set to -1 .

short pgFindParStyleSheet (pg_ref pg, par_info_ptr compare_style, par_info_ptr mask);

This function is identical to pgFindStyleSheet except it is used for paragraph style sheets.

### 31.5 Get, set, change a style in a style sheet

(pg_boolean) pgGetStyle (pg_ref pg, short style_id, style_info_ptr style);

Returns the style_info record belonging to style sheet style_id. The style_id must be valid.

NOTE: If you want to get the associated font, use pgGetFontInfoRec.

Returns the style_info record belonging to style sheet style_id. The style_id must be valid.

NOTE: If you want to get the associated font, use $p g G e t F o n t I n f o R e c$.

If style_id is not a valid stylesheet ID, the function returns FALSE and the style_info record is not set to anything certain.

(void) pgChangeStyle (pg_ref pg, short style_id, style_info_ptr style, font_info_ptr style_font, short draw_mode);

Changes the style sheet style_id to the contents in *style. All text is affected that is set to this style sheet. Every character in the text that is set to this style — bor a subset of this
style — bwill change as follows: if the style_info attribute is the same as the original stylesheet, that same attribute changes to the new setting. If the attribute is different (i.e. has been changed by user), that attributes remains unchanged.

For example, suppose you created a style sheet for Helvetica-Bold-Italic and applied that sheet to the whole document. The user underlines a word (making it Helvetica-BoldItalic-Underline), then you change the style sheet to Times-Italic. The underlined word will change to Times-Italic but will retain the underline

If style_font is non-NULL, the font is also changed (otherwise the font already assigned to the style is retained).

If draw_mode is nonzero, the text is redrawn in the mode specified (see "Draw Modes" on page 2-30 for more about the display modes for pgDisplay). In most cases, draw_mode should be best_way.

(void) pgSetStyleSheet (pg_ref pg, select_pair_ptr selection, short style_id, short draw_mode);

(pg_boolean) pgGetStyleSheet (pg_ref pg, select_pair_ptr selection, short PG_FAR *style_id);

The pgSetStyleSheet function changes all the text in the specified selection to style sheet reference style_id.

NOTE: This differs from pgChangeStyle since, in this case, you are changing a selection range to assume the format of a specific style sheet - pyou are not changing the style sheet itself.

The selection parameter operates in the same way as all functions that accept a select_pair (see "Selection range" on page 8-2 for more information about the select_pair record).

If draw_mode is nonzero, the text is redrawn in the mode specified (see "Draw Modes" on page 2-30 for more information about display modes for pgDisplay). In most cases, draw_mode should be best_way.

To find out what style sheet, if any, is applied to an area of text, l use pgGetStyleSheet.

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-678.jpg?height=48&width=1265&top_left_y=93&top_left_x=420)
specified selection range. On return, if *style_id contains zero, no single style sheets affect the selected text, otherwise the stylesheet ID is placed in *style_id. Additionally, if the function result is TRUE, the style sheet is consistent throughout the selection range.

For more on styles and masks see “Changing Styles” on page 30-7.

NOTE: If the function returns TRUE, yet *style_id returns as zero, that means there are no style sheets anywhere within the selection. But if the function returns FALSE and *style_id is zero, there are some style sheets within the selection but they are not consistent.

(short) pgNewParStyle (pg_ref pg, par_info_ptr new_style);

(void) pgRemoveParStyle (pg_ref pg, short style_id);

(short) pgNumParStyles (pg_ref pg);

(short) pgGetParStyle (pg_ref pg, short style_id, par_info_ptr style);

(void) pgChangeParStyle (pg_ref pg, short style_id, par_info_ptr style, short draw_mode);

(void) pgSetParStyleSheet (pg_ref pg, select_pair_ptr selection, short style_id, short draw_mode);

(pg_boolean) pgGetParStyleSheet (pg_ref pg, select_pair_ptr selection, short PG_FAR *style_id);

All of the above functions are identical to their counterparts, but are used for paragraph format style sheets. OpenPaige maintains a separate list for paragraph formats.

Why is the style_sheet_id "negative"?

I was walking through the style_info records in the pg_ref and noticed that some of them have negative style_sheet_id values. How/why does this happen?

If your pg_ref has any stylesheets applied to text, when you change any of that text to additional style attributes, OpenPaige negates the style_sheet id. This is how it keeps track of "offspring" style sheets; upōn closer inspection you might notice that the "parent" style sheet ID is the compliment (negation) of this value, i.e. of parent sheet is 17, a
style_info.style_sheet id of -17 was originally the style \#17 before changes were made.

TECH NOTE

Can I build style sheets from scratch?

Would it be possible for me to set up the style_info records myself and put my own style_sheet id, then use pgAddStyleInfo, or would this cause problems for OpenPaige?

Actually, I do not think that method will work... and in fact after investigating the source code more carefully there are some problems with my suggestions to implement "global" stylesheets.

First, I'll outline what the problems are and then I'll suggest workarounds.

Problem \#1 is the fact that OpenPaige maintains style sheet ID's in ways that you might not expect. For example, to keep track of "clone" stylesheets (sheets that get altered slightly but still affected by global changes), OpenPaige negates the stylesheet ID so it knows who the "parent" style is. Do to this, I have a feeling your pg_ref would get messed up if you start assigning your own ID's.

Problem \#2 has to do with a field in style info called "used ctr". This field gets incremented for every occurrence of that style in the text stream and gets decrements every time that style is deleted from the text. Once it decrements to zero, openPaige will DELETE the style_info record. For "stylesheet" info records, however, it starts the used ctr at 1 so it doesn't get deleted, except at the moment of calling the delete-stylesheet function, in which case the used_ctr is decremented so it deletes once no text is using it.

The reason $\# 2$ is a problem is that OpenPaige forces this field to zero when you add new style info records, even if you only use the lower-level pgAddStyleIInfo.

Workarounds:

I think you will be better off by literally adding stylesheets the "normal" way (e.g. pgNewStyle, pgChangeStyle, etc.), also don't try to force your own "ID" into the target pg_ref.

When I say don't "force your own ID" I mean just let openPaige assign ID's to the stylesheets. That doesn't mean you can't have your own ID's (such as your resource ID's) and it also does not mean you even need to do anything with the stylesheet

ID's that OpenPaige returns. But, I wouldn't mess with stylesheet_id in the style_info records.

In light of this, I would slightly modify my suggestions in the last message as follows:

- To find out if a stylesheet already exists in a pg_ref, use pgFindStyleSheet to do an actual comparison against your style(s).
- To change a stylesheet "globally" (for example, opening a doc and applying a changed global stylesheet to the opened doc), also use pgFindStyleSheet to see if it exists, then change it by referring to it with the "local" ID OpenPaige returns from that function.
- To make it really solid, I would use one of the "refcon" fields in the style info record to store my own "ID" numbers to identify exact stylesheets. Specifically, the fields you can choose from in style info for this purpose are user_id, user_data and user_data2. Remember that openPaige plays no significance on any of these fields, but they can mean something to your app -- original resource ID's for example. Note that pgFindStyleSheet allows a "mask" to compare only certain fields. An interesting approach would be to slap in your own "ID" in one of the user_ $x x$ fields then simply compare that one field for locating stylesheets in question.

As for not using an "invisible" pg_ref, that's no problem if you do something along the line of my above suggestions.

## TECH NOTE

Building paragraph styles from scratch

When creating a Paragraph Style Sheet, does the par_info record need to be filled out completely?

Yes.

If so.... how does one fill in the fields in the par info record such as style_sheet_id, procs, maintenance and used_ctr?

This is actually ultra simple and takes only one line of code. You simply begin with a "default par_info" record that you get
from OpenPaige globals. If your potential style sheet is called "MyParStyleSheet", you do the following to initialize:

par_info MyParStyleSheet;

MyParStyleSheet = paige_globals.def_par;

The "paige globals" is of course your pg_globals struct given originally to pglnit. The above statement copies the default paragraph style, including all the default hooks, etc., into your paragraph style. Furthermore this method guarantees compatibility with any future versions (even if we add stuff to par_info such as new hooks, your style will get initialized correctly).

Does the paragraph style sheet mechanism ignore [some] fields?

I think it might ignore "style sheet id" in this case and I know it always ignores used ctr and maintenance. But that shouldn't matter if you do the above.

## 31.6 "Named" Styles

"Named" styles differ from OpenPaige's ordinary stylesheets by combining both style_info and par_info stylesheets into one, composite format that can be applied to the document. The composite stylesheet can be given a name, and can be applied by calling the appropriate OpenPaige function using the name only.
long pgNewNamedStyle (pg_ref pg, pg_char_ptr stylename, const style_info_ptr style, const font_info_ptr font, par_info_ptr par);

Creates a new, named stylesheet and keeps the resulting style in $p g$.

The stylename parameter is the name of the style; this is a cstring and can be from 1 to 64 bytes long (including the terminating null character). If the same, exact named style already exists it is replaced by this style.

The style, font, and par parameters define the text style, font and paragraph formats, respectively. However, either of these parameters can be NULL, in which case only the non-NULL attributes are applied.

For example, a NULL par parameter indicates that only text formatting (not paragraph formatting) will be change when this stylesheet is applied to text. If style is NULL, only font and/or par would be applied; if font is NULL, the style (if non-NULL) is applied using the current font of the targeted text.

Creating a new named stylesheet does not affect the document until you apply it to one or more characters using the functions listed below.

A value is returned from this function, which will be an "index" number, identifying this style, that you can use with some of the functions listed here that require an index value. This index number is optional — you can ignore it and still apply the stylesheet to the text by using its name.

long pgAddNamedStyle (pg_ref pg, pg_c_string_ptr stylename, const short style_id, const short par_id);

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-682.jpg?height=48&width=1287&top_left_y=1904&top_left_x=421)
number(s) are provided instead of style_info and par_info records.

If the named style already exists, style_id and par_id replace the style and paragraph styles.

A value is returned from this function, which will be an "index" number, identifying this style, that you can use with some of the functions listed here that require an index value. This index number is optional — you can ignore it and still apply the stylesheet to the text by using its name.

void pgApplyNamedStyle (pg_ref pg, select_pair_ptr selection, pg_char_ptr stylename, short draw_mode);

The stylesheet identified by stylename is applied to the text within the specified selection. If selection is NULL, the current selection is used. Text is redrawn using the draw_mode parameter (or not redrawn if draw_mode == draw_none).

If stylename does not exist, this function does nothing.

Since stylename represents a composite style (text and paragraph), each of them get applied differently. If a text style (style_info) is part of the style, only the characters within the selection are changed; if a paragraph format (par_info) is part of the style, the whole paragraph(s) within the selection are changed.

Hence, if you want to apply the style_info to entire paragraph(s) you must provide a selection range that covers the paragraph(s), otherwise you may not get the expected results.

void pgApplyNamedStyleIndex (pg_ref pg, select_pair_ptr selection, long index, short draw_mode);

This is identical to pgApplyNamedStyle() except the stylesheet is identified by its index number (the value returned from $\mathrm{pgNewNamedStyle).}$

pg_boolean pgGetAppliedNamedStyle (pg_ref pg, select_pair_ptr selection, pg_char_ptr stylename);

Returns the named style, if any, that is currently applied to the specified selection.

If there is indeed a named style applied, the name is returned in stylename.

NOTE: The selection range can have other style(s) applied, in which case pgGetAppliedNamedStyle() might still return TRUE if the text also contains the stylesheet.

long pgNumNamedStyles (pg_ref pg);

Returns the number of named stylesheets in $p g$. The number of named stylesheets is simply the number you have created; it does not necessarily mean any of them are applied to any text in the document.

pg_boolean pgGetNamedStyle (pg_ref pg, long named_style_index, named_stylesheet_ptr named_style);

Returns the named style record for named_style_index. The "index" is any value between 1 and pgNumNamedStyles().

If the stylesheet exists, the named_style record is initialized and the function returns TRUE. If named_style is NULL, the function merely returns TRUE if named_style_index is valid. The name_style structure is defined as follows:

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-684.jpg?height=298&width=831&top_left_y=1485&top_left_x=434)

The stylesheet_id and par_stylesheet_id are the style and paragraph stylesheets, respectively. If either are zero they are not a part of this composite style.

long pgGetNamedStyleIndex (pg_ref pg, pg_char_ptr stylename);

Returns the index value for stylesheet stylename. If one exists, a number between 1 and pgNumNamedStyles() will be returned, otherwise this function returns zero.

void pgDeleteNamedStyle (pg_ref pg, long named_style_index);

Deletes the named style indicated by named_style_index. The "index" value can be anything between 1 and $p g$ NumNamedStyles().

NOTE: The text is not affected by this function, even if a style is deleted that has been applied to one or more characters. (The characters will still retain that style until some other action changes their format).

void pgRenameStyle (pg_ref pg, long named_style_index, pg_char_ptr style_name);

Renames the style indicated by named_style_index; the new name is style_name. The "index" value can be anything between 1 and pgNumNamedStyles().

## STYLE WALKERS

## CHAPTER

### 32.1 Walker record structure

The following record structure is passed to certain low-level hooks (and can also be used for complex style and format manipulations):

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-687.jpg?height=553&width=1350&top_left_y=1329&top_left_x=224)

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-688.jpg?height=226&width=1322&top_left_y=142&top_left_x=218)

OpenPaige uses this structure to "walk" through a run of style (including paragraph styles). In other words, given a starting position in text, OpenPaige will initialize a style_walk to reflect that's positions style, font and paragraph format; then by calling special "style walker" functions, the style information can increment or decrement so the current formatting is always known. The purpose of the style_walk method is to avoid the necessity to constantly look up the style, font or paragraph info while walking through a series of text bytes.

From top to bottom, each field can be described as follows:

current_offset — Indicates the current, absolute offset (from beginning of text) in the pg_ref.

cur_style — A pointer to the current style_info record.

cur_par_style — A pointer to the current par_info record.

cur_font — A pointer to the current font_info record.

next_style_run — A pointer to the NEXT style_run record for styles. To determine the number of bytes from current position to next style, the formula is:

style_walk.next_style_run->offset - style_walk.current_offset;

next_par_run — A pointer to the NEXT style_run record for paragraph styles. To determine the number of bytes from current position to next paragraph style, the formula is:

style_walk.next_par_run->offset - style_walk.current_offset;
prev_style_run — A pointer to the previous (or "current") style_run record for styles. To determine the total number of bytes for this style (number of bytes this style applies to), the formula is:

style_walk.next_style_run->offset style_walk.prev_style_run->offset;

prev_par_run — A pointer to the previous (or "current") style_run record for paragraph styles. To determine the total number of bytes for this paragraph style (number of bytes this paragraph style applies to), the formula is:

style_walk.next_par_run->offset - style_walk.prev_par_run->offset;

style_base — A pointer to the first style_info record (element 0 of style_info array). This is used to quickly index the style_info records.

par_base — pointer to the first par_info record (element 0 of par_info array). This is used to quickly index the par_info records.

font_base _ A pointer to the first font_info record (element 0 of font_info array). This is used to quickly index the font_info records.

last_font — Contains the font index number for the pointer at cur_font. The purpose of this is to avoid re-initializing cur_font for every style change if the font remains the same.

t_length — The TOTAL length of text for the $p g \_$refassociated to this style_walk.

superimpose - Used for a temporary workspace when building a subset of style_info based on super_impose_var, all_caps_var, small_caps_var or all_lower_var.

The last element in a style_run array is a "dummy" entry whose offset field will be greater than the total text size of the $p g \_r e f$. For example, if the total text size of a $p g \_r e f$ is 100 bytes, the final element in the array of style_run records will contain a value in style_run.offset of $>100$.

Hence, if you are examining a walker to determine the amount of text that applies to a style, be sure to take this into account.

For example, if walker.next_style_run->offset is GREATER than walker.t_length, use walker.t_length in your calculations. The same is true for walker.next_par_run.

### 32.3 Walker Functions

OpenPaige provides the following functions to support a style_walk record:

Prepare style walk

(void) pgPrepareStyleWalk (paige_rec_ptr pg, long offset, style_walk_ptr walker, pg_boolean include_pars);

To initialize a style_walk record, call pgPrepareStyleWalk. The offset parameter should contain the starting text offset (relative to the start of all text). When this function returns, the style_walk pointed to by walker will be initialized to the styles of offset.

Once you are through using the style_walk, make one more call to pgPrepareStyleWalk but pass NULL for walker; this tells the OpenPaige code you are through using the fields. Every pgPrepareStyleWalk must eventually be balanced with a second call with NULL.

The purpose if the include_pars parameter is to enhance the speed when walking through style runs but the caller does not care about paragraph format runs: if include_pars is FALSE then pgPrepareStyleWalk will only initialize the walker for style runs (not paragraph formats) — bin which case all paragraph format-related fields in the walker will be null pointers. If include_pars is TRUE then all paragraph format runs will be included. Generally, if the intention is to examine only style_info runs, include_pars should be FALSE.

## Using pgPrepareStyleWalk

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-691.jpg?height=459&width=696&top_left_y=707&top_left_x=436)

This function advances the styles in walker by amount bytes. The amount parameter can be negative, in which case the styles are "decremented".

All this does is reset the fields in walker to reflect the styles that apply to the current text position (in walker) + amount. If the same style, font and paragraph format applies to all text, you would keep getting the same answers regardless of the value in amount. The function result from $p g S e t W a l k S t y l e$ returns "TRUE" if the style has changed from the previous setting. For example, if the style applied to the current text position (before

pgSetWalkStyle) is Plain, and calling pgSetWalkStyle now sits on text that is "Bold", the function would return TRUE.

Walk next/previous style

(pg_boolean) pgWalkNextStyle (style_walk_ptr walker);

(pg_boolean) pgWalkPreviousStyle (style_walk_ptr walker);

This function advances walker forward to the next or previous text style and, if appropriate, to the next paragraph style. The amount from the current position to the next text style is passed to $\mathrm{Pg}$ Wa IkStyle for "amount." It is your responsibility to determine that there really is another style before making this call. (Another style exists if walker.next_style_run->offset is less than walker.t_length). The function result from pgSetWalkStyle returns "TRUE" if the style has changed from the previous setting. For example, if the style applied to the current text position (before pgSetWalkStyle) is Plain, and calling pgSetWalkStyle now sits on text that is "Bold", the function would return TRUE.

## Set walk style

(pg_boolean) pgSetWalkStyle (style_walk_ptr walker, long position);

This function sets all fields in walker to reflect the styles that apply to position. The position parameter is absolute, i.e. it is the amount in bytes from the beginning of all text. The result of this function is identical to pgPrepareStyleWalk except walker must already be initialized. The function result from $p g S e t W a l k S t y l e$ returns "TRUE" if the style has changed from the previous setting. For example, if the style applied to the current text position (before pgSetWalkStyle) is Plain, and calling pgSetWalkStyle now sits on text that is "Bold", the function would return TRUE.

## WINDOWS CHARACTER WIDTHS

OpenPaige contains a low-level function you can use to force specific character widths for any given text format.

For example, a cross-platform, OpenPaige-based application might need to render exact, identical placement of characters drawn in the same font between Macintosh and Windows. As most developers realize, the subtle differences between fonts, even between fonts that are supposedly the same family and type will not always render the same text widths between platforms, or between changing resolution or printers.

The following function has been created to help with a solution:

void SetFontCharWidths (pg_ref pg, style_info_ptr style, int PG_FAR *charwidths);

This function causes the rendering of all text drawing in style to match predetermined character widths defined in charwidths.

The charwidths table must be a pointer to 256 int values, each element must correspond to that same ordinal value of the style's character set. For example, charwidths[0]
represents the width of a null (0) character; charwidths[' '] represents the width of a space character, charwidths ['A'] represents the width for an "A" character, etc.

NOTE: The character table applies only the precise, composite text format represented by the style parameter. This includes the associated font_info record (which is defined by the value in style->font_index).

After this function is called, any text that is drawn in the precise format represented by style will be rendered using the widths in charwidths.

## NOTES:

1. The function prototype for SetFontCharWidths() is defined in pgtraps.h.
2. SetFontCharWidths() makes a copy of the character widths, hence you do not need to keep its array of int values around.
3. The $p g$ parameter is required to have access to OpenPaige globals as well as access to a font table (unique to the $p g \_r e f$ ). However, the character table you set becomes universal and global for all $p g \_r e f(s)$ that use exactly the same style.

## 34 FiLE HANDLERS

CAUTION: Nearly every file input/output issue can be addressed by referring to "File Standards, Input \& Output" on page 22-1. Rarely will a developer need this chapter on File Handlers.

In fact, if you are looking to this chapter to help solve an input/output issue, you should probably contact DataPak Tech Support via email to see if it is absolutely necessary. In nearly every case, the "File Standards, Input \& Output" on page 22-1 will be enough to handle file saving and retrieving.

### 34.1 File Sub-system

The basic ingredients necessary to achieve the feature set listed above are:

1. Documents are saved exclusively as a series of components, where each component contains a standard "header" identifying the data type and length followed by the data, and
2. OpenPaige structures are saved and read as a series of component values, never as a single structure. Hence, upward compatibility and even backward compatibility becomes possible since every version reads only the field(s) it understands.
3. Numbers (or relative addresses) are stored as hexadecimal characters.
4. For specialized cases that require the application to bypass normal sequential $\mathrm{I} / \mathrm{O}$ within a data component, an alternate read and write function can be privately assigned to that data component.

## Data Components

An OpenPaige document is saved to a file as a series of data components, each component being independent of the other. It does not matter what order they are saved (or what order they are read when the file is open) and it does not matter if strange or unrecognized components are imbedded anywhere in the file stream.

Every component consists of:

-pA header defining the data type and its length

—pThe data, which immediately follows the header

When a file is "opened" and each component is scanned, if OpenPaige recognizes the data type (in the component header) it processes the information; if it does not recognize the data type it can simply skip over it. Thus, compatibility between versions, platforms and applications become possible since no single unknown component can throw OpenPaige for a spin or crash the application.

NOTE: Technical Note: The term "file" is being used here only to describe sequentially stored data. This does not always imply a physical disk file. An OpenPaige "file" can just as well be a block of memory such as the system scrap or "clipboard" or it could be a sequence of bytes sent over a modem, or any other type of medium that might support data transfer.

### 34.2 OpenPaige "Handler" Functions

How File Data is Recognized

The term "handler" is used here to describe a function which handles reading or writing a specific data component. Within OpenPaige, there are specific functions to handle each piece of data from an OpenPaige object; a set of pointers to these functions are maintained using the following record:

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-697.jpg?height=500&width=1213&top_left_y=741&top_left_x=435)

OpenPaige maintains an array of $p g \_h a n d l e r$, essentially one element for each data component that can be saved to a file. The key field in the $p g \_$handler record contains a unique code that is included in the data component for which the handler is responsible.

write_handler and read_handler — contain pointers to functions that will process the data component that is transferred to or from the file, respectively.

PG_PASCAL (pg_boolean) pg_handler_proc (paige_rec_ptr pg, pg_file_key key, memory_ref key_data, long PG_FAR *element_info, void* aux_data, long *unpacked_size);

When a $p g \_h a n d l e r \_p r o c$ is called, $p g$ contains the record structure of the OpenPaige object being written or (read into). The key parameter will contain the "key" code that will be written to the data component header (if writing) or the key code that has been found in the header (if reading). The key will be identical to the value found in the $p g \_h a n d l e r$ associated with this function.

key_data — is a memory_ref(memory allocation) that must be filled in with data to write (when writing) or contains the data that has been read (when reading).

element_info _ parameter is an optional value that can be included in the header when writing and will be read and provided to this function when reading; aux_data is used by OpenPaige internally to provide information for some of the standard handlers (aux_data is ignored for all "custom" handlers added to the pg_handler list by the app) - bsee the Table on page 730,"Standard Handlers", which describes what each of these parameters will hold for standard handlers.

unpacked_size — parameter is a pointer to a long which the handler function must set to the actual (physical) size of the data being read or written, in bytes. This may differ from the byte size in key_data.

For example, suppose a special read handler is used for compressed text (ASCII text compressed in some way). The size of key_data might be much smaller than the uncompressed text size that is inserted into the $p g \_r e f$. In this case, *actual_size should be set to the uncompressed size since it is the "real" size of the data.

For writing, *actual_size should be set to the original size of the data that will be written to the file. In a similar example of compressed text, *actual_size in the case of a write handler should be the uncompressed size of text (text size before it is compressed into key_data).

FUNCTION RESULT: Both functions must return TRUE if it is through handling this key.

NOTE: A TRUE is the "normal" and usual response; the purpose of a possible TRUE or FALSE result is for special read/write cases where the same key is handled more than once. A FALSE result essentially tells OpenPaige to call the handler function again (see “Repetitive Handler Loops" below).

CAUTION: For simple read or write handlers, be sure to return TRUE or an endless loop can result! See “Repetitive Handler Loops” on page 34-698.

## Read and Write Data Functions

The read_data_proc and write_data_proc contain the function that will physically read the data to be processed by the handler function or to write the data processed by the handler function, respectively. For "normal" OpenPaige data components, these will get set to the standard I/O function, but either can be changed by the application for custom data transferring that is local and private to the respective component.

Writing

When writing to a file, each individual "handler" function is called to write its own data component. This is fairly straightforward because OpenPaige simply walks through the list of available pg_handler records and calls each write_handler function, one at a time.

## Reading

When reading a file, if the component is recognized (i.e., if OpenPaige can find a $p g \_h a n d l e r$ that contains the same key as found in the component header), the handler is called to process the data.

For example, when a file was saved, the write handlers typically saved blocks of text, style records, paragraph formats, font records, etc., all as individual components, each with its own code (from the "key" field in its pg_handler record) to identify the data type. When this file is "opened," the components are read one by one; if the data type is recognized, which is to say if a pg_handler record can be found that contains its code,
its "read handler" function is called to process the data; if the type is not recognized, i.e. if no handler can be found to match, it is skipped.

At once this guarantees future compatibility since no single data element involves hardcodes recognition; it also allows applications to save their own data structures by installing their own $p g \_h a n d l e r s$. If some other application or platform read the file, the unrecognized data components are simply skipped with no adverse effect!

## Repetitive Handler Loops

In certain situations it may be required for OpenPaige to call the same read or write handler more than once.

An example of this would be saving a huge data structure by breaking it into smaller components, writing each component as a separate "key."

One way to accomplish this is to return FALSE from a write handler which results in the same handler function to get called again; OpenPaige will keep calling the handlers until TRUE is returned.

Additionally, the value set (by you) in *element_info will remain unchanged between repetitive read-handler calls, so you can use that feature to know what to do (or where you are in the data, etc.) for each repetitive loop. The first time the handler function gets called, OpenPaige will set *element_info to zero.

## Repetitive Write Handlers

Writing more than one data component using the same write handler is accomplished in the same way as repeating read handlers (by returning FALSE and using *element_info).

However, when using a write handler in this fashion it may be important to observe the following:

- The value your write handler places in *element_info will be what gets written to the data component's header. Later when your read handler is called, the same value in *element_info will be given to you that was associated with the same data component.
- Remember that your data component is written after you return from the write handler (whereas data has already been read when a read handler is called). While this may seem obvious, it could prove to be an important point (see next item below).
- When you return from your write handler, OpenPaige will not write any additional data if the data component you just processed has a byte size of zero. This is an important "feature" since you can terminate the repetitive loop if there is no more data to write by setting key_data to zero size and returning TRUE.

For example, you could set up the first data component in a series of (potentially) many and return FALSE (indicating you want to get called again). On the subsequent call, however, you discover there are no more data components to be written, therefore you can simply call SetMemorySize(key_data, 0) and return TRUE indicating you are through.

### 34.3 Installing Handlers

NOTE: If you will simply be saving OpenPaige documents in the standard manner without any additional data, you may skip this section completely.

The most basic method of saving an OpenPaige document is to use only the standard, "built-in” handlers. If that is all your application needs to do (if you simply want to save OpenPaige objects with no special data types or custom handlers), you do not need to install any handlers as the defaults will be initialized automatically.

If you need to save or read additional data types, you can install your own handler(s) by calling the following function:

(void) pgSetHandler (pg_globals_ptr globals, pg_file_key key, pg_handler_proc read_handler, pg_handler_proc write_handler, file_io_proc read_data_proc, file_io_proc write_data_proc);

globals - must point to the same structure given to pglnit.

key — is the handler ID number you wish to install; this can be one of the predefined handler keys or it can be a custom ID specific to your application.
read_handler and write_handler — should contain a pointer to a valid $p g \_$handler_proc function, or NULL. These are the functions that will get called to handle data components that have been read or components that are to be written, respectively. If either parameter is NULL than the existing function for that key, if any, is left unchanged (or, if no handler yet exists for that key the standard (default) function is assumed). For example, to change only the read handler for a specific key, you would pass a pointer in read_handler and NULL for write_handler.

read_data_proc and write_data_proc — should be either a NULL pointer, or point to a valid file_io_proc (see "The file_io_proc" on page 34-705). If non-NULL, the respective function will get called to physically read or write the data to the file for that key; if NULL the existing I/O function for that key remains unchanged (or, if no handler yet exists for that key the standard (default) function is assumed).

NOTE: Setting a handler that already exists simply replaces the function pointers in that handler per the parameters given above; if the handler does not exist it is added.

If you want to get a copy of an existing handler, call the following:

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-702.jpg?height=108&width=1124&top_left_y=1047&top_left_x=433)

globals - must be a pointer to the same structure given to pgInit.

handler — parameter must point to a $p g \_$handler record (cannot be null); however, you only need to set the key field for the handler you wish to get a copy of. When the function returns, a copy of the read_handler and write_handler will be put into the handler record provided.

If the handler is not found (if no existing handler matches with the value you put in the $p g \_h a n d l e r$ 's key field), $N O \_H A N D L E R \_E R R$ is returned.

The following is a list of the standard handler "key" codes; if you want to read and write special data using your own unique code, you should always define it at least greater or equal to the \#define CUSTOM_HANDLER_KEY.

CONTROL_MOD_BIT is used mainly with "arrow" keys. This causes the selection to advance to the next word (right arrow) or to the previous word (left arrow).

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-703.jpg?height=933&width=1178&top_left_y=653&top_left_x=434)

"Key" codes can be any 16-bit value but must be positive numbers.

NOTE: Contact DataPak Software via electronic mail or fax regarding registering keys which you wish to make public with DataPak. DataPak will assist you in assigning numbers which will prevent duplication between applications. Those wanting to read custom data MUST use the author signature settings.

When used against keys, the author will let you know when you have your own document and not some other app's. See "Application Signature" on page 34726 (this chapter).

## Removing Handlers

To completely remove a handler, call the following:

$$
\text { (pg_error) pgRemoveHandler(pg_globals_ptr globals, pg_file_key key); }
$$

This function removes the handler indicated in key. If no such handler exists, the function returns NO_HANDLER_ERR.

## Setting / Resetting Standard Handlers

If you want to restore the list of $p g \_h a n d l e r s$ to the defaults, call the following:

$$
\text { (void) pglnitStandardHandlers (pg_globals_ptr globals); }
$$

globals - is a pointer to the same structure given to $\mathrm{pg}$ Init.

This function reinitializes the list of $p g \_$handlers to the defaults, and it will remove all custom handlers that have been installed.

The usual reason for calling pglnitStandardHandlers is to remove all custom handlers you have installed and/or to restore any you might have deleted.

You do not need to call pglnitStandardHandlers if you have not installed, changed or deleted any handlers, nor do you need to call pgInitStandardHandlers if you want to leave the handlers as-is throughout the application session.

### 34.4 Reading certain data only

This feature is for using OpenPaige to open only a few files keys in a document. For example one might want to open format and shapes of a document, but not the text, or perhaps display the text using a different format. This is used to implement stationary or templates.

OpenPaige handles such partial reads as follows:

Reading only certain data elements — pbut not all — pis possible by passing a list of file keys to $p g R e a d D o c$ that specify which elements to include for reading; OpenPaige will skip over all other keys that are not in this list.

However, reading only certain data components from an OpenPaige file might require some knowledge of dependencies among these components.

CAUTION: For example, if you read the OpenPaige text (by virtue of including text_key in the list of file keys to be read), you must ALSO include text_block_key or the file can crash; yet if you read no text at all then you must NOT include text_block_key.

On the other hand, if you elect to read only style_info records but no text, then you must NOT read the style run information (because the "run" info will contain offsets into text that will not exist).

The following guidelines should therefore be observed:

- You must always include paige_key regardless of how many (or how few) other keys are used. The paige_key must also be the first element in the key list given to pgReadDoc.
- To read "text only" without any styles, include ONLY the following keys, in this order:

$$
\begin{aligned}
& \text {-ppaige_key } \\
& \text {-ptext_block_key } \\
& \text {-ptext_key }
\end{aligned}
$$

- You can also read "text only" without styles and include certain other data items such as "shapes" by including:

$$
\begin{aligned}
& \text {-ppaige_key } \\
& \text {-ptext_block_key } \\
& \text {-ptext_key }
\end{aligned}
$$

-pvis_shape_key

-ppage_shape_key

- exclude_shape_key
- To read everything EXCEPT text, include all the keys you want to read EXCEPT for the following:

-btext_block_key

-ptext_key

-pline_key

-pstyle_run_key

-ppar_run_key

-pselections_key

- If you read style_info records (by including style_info_key in the read), you must ALSO include font_info_key or else using the styles will crash.

Using pgReadDoc for only the style info from an OpenPaige file saved with pgSaveDoc

The following is an example of reading only the styles from an OpenPaige file and omitting the text:

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-706.jpg?height=452&width=1073&top_left_y=1165&top_left_x=433)

CAUTION: IN ABOVE EXAMPLE: The usual reason for reading style_info records is to obtain a list of styles to apply to some other $p g \_r e f$. If you start "using" the $p g \_r e f$ above, i.e. if text is inserted and formatted, many of its style_info records will be removed! This is because OpenPaige will delete
style_info records that are not applied to any text (which will not occur until you attempt to apply new styles or change the text). The exception to this is the existence of stylesheet records: those will not be deleted.

### 34.5 The file_io_proc

If you want to provide your own function for reading or writing, the function pointer given to $p g$ SaveDoc or $p g R e a d D o c$ must be declared as follows:

PG_PASCAL (pg_error) file_io_proc (void* PG_FAR data, short verb, long PG_FAR *position, long * data_size, file_ref filemap);

This will get called whenever $p g S a v e D o c$ wants to write something, or when $p g R e a d D o c$ wants to read something.

The data parameter points to the data to be written (if this is a write function), or a pointer to the data to be read (if this is a read function); for read functions, ${ }^{\star}$ data will contain enough space to read the data requested.

CAUTION: The "data" parameter is not always a pointer, sometimes it is a memory_refindicated by the verb parameters - psee below.

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-708.jpg?height=359&width=1144&top_left_y=219&top_left_x=434)

If verb is io_data_direct, data is a pointer to the contents to be read to or written from.

If verb is io_data_indirect, the data parameter is a memory_ref(not a pointer to the data). Read functions must set the appropriate memory size of data and set its contents to the bytes read from the file; for write functions, the byte to be written are contained in data.

If verb is io_get_eof, this function should set ${ }^{*}$ data to the byte offset for end-of-file. (OpenPaige will call this function with verb $==$ io_get_eof to know how large the input file is; hence, if you require any kind of logical end of file, such as reading only a part of a file, you can set that value at this time).

position - is a pointer to the file offset to read or write. The file offset is always relative to the start of the file.

data_size — will point to a long word containing the number of bytes to transfer.

filemap — contains the machine-specific reference to use for file I/O. (The standard Macintosh file_io_proc assumes filemap contains a file reference).

For reading or writing (as opposed to getting end of file for verb $=$ io_get_eof), this function must do the following: (1) Transfer the data, (2) Update the *position by adding to it the number of bytes transferred, (3) Set *data_size to the actual bytes transferred (which will usually be the same as requested, barring file errors), and (4) return any errors, or 0 for no errors.

The function result must be 0 for no errors (successful) or some kind of error code (unsuccessful). The error code should be an OpenPaige-defined error —-psee "Error Codes" on page 39-765.

I want to be able to check the disk to see if my file will fit before I call pgSaveDoc. How do I check to see if my data will fit on the disk?

You can check for the actual size that will be created before a save simply by using a custom write_io proc. The proc will simply increment the offset for each of the kinds of data you want to save. It will count the times it is called and be multiplied by the size of the data being saved. You don't actually write during the proc, just advance the counter. It will then pass back the eventual position and will be very fast.

### 34.6 Reading \& Writing "Soft" Files (and transferring to the "scrap")

It may be desirable to transfer a file to something other than a disk file, such as to and from a block of memory, some communication line, etc.

To do so, you simply replace the file_io_proc with one of your own, or if you simply read and write to "memory" you can pass a built-in function for this purpose, pgScrapMemoryRead (for reading) and pgScrapMemoryWrite (for writing).

The following is an example of "writing" a document to the Macintosh "scrap" by simply replacing the file_io_proc with a custom version to fill in a Handle and calling pgSaveDoc:

// This function "writes" OpenPaige object pg to the scrap.

\#include "defprocs.h"/* must include this for prototype of pgScrapMemoryWrite */

void PutToScrap (pg_ref pg)

\{

file_ref data_ref;

Ptr scrap_data;

long file_position;

/* Our "filemap" will simply be a memory_ref that will get

filled with the data that is "writen" */

filemap = MemoryAlloc(\&paige_rsrv.mem_globals, sizeof(pg_char), $0,0)$;

file_position $=0$;

pgSaveDoc(pg, \&file_position, NULL, 0, memory_write_proc, filemap, $0)$;

scrap_data $=$ UseMemory $($ filemap);

PutScrap(file_position, PG_SCRAP_TYPE, scrap_data);

UnuseMemory(filemap);

DisposeMemory(filemap);

\}

## KEY NOTES:

1. For a thorough understanding of the memory functions in the above example, see "The Allocation Mgr" on page 25-441.
2. Both pgScrapMemoryRead and pgScrapMemoryWrite are defined in "defprocs. $h$ ". For both functions, the "filemap" is simply a memory_ref
created by your application; pgScrapMemoryRead will "read" from the contents of the memory_ref as if it were a file, and pgScrapMemoryWrite will set the memory_ref's contents as if it were a file being written to.

NOTE (Macintosh): We encourage developers to use the above method —-por a similar method -bfor transferring OpenPaige objects to the scrap, because the read/write handler scheme can be ultra compatible between diverse applications, and even between platforms, hence it could be an excellent standard.

### 34.7 Writing your Own Handlers

Almost without exception, applications will usually have one or more data elements that need to be saved along with an OpenPaige document. If nothing else, an app will typically want to save the window size or position and other similar items.

The best (and most compatible) way to save your own data elements is to save each data type (using the function provided below), and create your own "read" handlers that will recognize the data when the file is opened.

Writing / Saving

In actual practice, you don't really need to create a "write handler" function as such for saving custom data. In fact, in many situations the creation of a write handler function (given to $p g S a v e D o c$ to call) will reveal difficult situations for your application.

While this may appear inconsistent with the information in this section, it is not. To write your data components, you should first call $p g S a v e D o c$ and then save your data using the following low-level function that OpenPaige provides for this purpose:

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-712.jpg?height=242&width=1196&top_left_y=314&top_left_x=433)

NOTE: You need to \#include "pgFiles. $h$ " to use the above function.

This function takes a data component and a file key and writes them to the specified file offset in the standard OpenPaige format (so it can be processed later from pgReadDoc).

key - parameter must be your file key (the value to be recognized later, during pgReadDoc, that will match up with your installed read handler).

data — must point to the data you wish to save and data_length must contain the data size, in bytes; the data can be anything and length can be any size (assuming it will successfully fit on file).

element_info — can also be any value you want; whatever this is, it gets saved in the data component header and will be returned to you in the element_info parameter when your read handler function is called later on.

io_proc, file_position and filemap — are (and should be) identical to the same parameters you would gave to $\mathrm{pgSaveDoc}$.

data_io_proc — is an optional pointer to a different function that should write the physical data to the file. This function is effectively the same as the write_data_proc function that exists in a handler record. If this function is NULL then the same I/O function given in io_proc is used (or if io_proc is also NULL then the standard default write function is used).

ile_position — parameter, in particular, should point to the value of the file offset that was set when pgSaveDoc returned - bit is assumed that you first called pgSaveDoc, then called $p g$ WriteKeyData above, hence the ending file offset after $p g S a v e D o c$ should be the starting file offset of $p g W r i t e K e y D a t a$.

To read the document saved above, you must install your own read handlers to process all the custom data elements saved. Each read handler should contain the same code given to the key parameter when the data was written with pgWriteKeyData.

The read handler you install will contain a pointer to a function (which you create) declared as follows:

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-713.jpg?height=154&width=1221&top_left_y=591&top_left_x=434)

In the process of reading the document (by pgReadDoc), when a file key is found to match one of your handlers, your function, as defined above, will get called.

key — parameter will be the file key that matched your handler (which could be important if you installed, say, the same function for several different data components).

key_data — will contain the data — pthe same data you wrote when you called pgWriteKeyData. The data size will be:

size_of_data $=$ GetMemorySize(key_data);

element_info - will point to a long word containing the value you originally gave to element_info when calling pgWriteKeyData.

aux_data — is to be ignored (except in special cases noted elsewhere in this document).

The way you process the data, what you do with it, etc., is completely up to you; pgReadDoc does not care what happens with this data.

CAUTION: The key_data allocation will get disposed when you return from this function; therefore you need to copy its data if necessary because it will not be preserved.

## NOTES:

1. When you install your read handler, be sure to include a function pointer to the "write handler" even though it won't get called, otherwise OpenPaige will try to delete the handler. You can simply plug the same function pointer in both read and write handler fields.
2. By not installing one or more appropriate read handlers for your data, those data items in the file will simply be skipped; $p g R e a d D o c$ will not crash. (Your app, however, might crash if you completely depend on the items saved if it never sees them).

Using OpenPaige to save and read a picture

The following is an example of saving a Macintosh picture to an OpenPaige file, then reading that picture from the file when it is opened:

Saving

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-714.jpg?height=585&width=1215&top_left_y=1257&top_left_x=433)

## HUnlock((Handle) the_pict);

return error;

\}

Reading

// The read handler I need to install:

PG_PASCAL (void) ReadPictHandler (paige_rec_ptr pg, pg_file_key key, memory_ref key_data, long PG_FAR *element_info, void *aux_data);

/* This function gets called BEFORE pgReadDoc to install the readpicture handler.*/

void setup_pict_handler (void)

\{

pgSetHandler(\&paige_rsrv, mac_pict_key, ReadPictHandler, NULL, NULL, NULL);

\}

/*ThefunctionbelowwillgetcalledbyOpenPaigesometimeduringthepgReadDoc */

PG_PASCAL (void) ReadPictHandler (paige_rec_ptr pg, pg_file_key key, memory_ref key_data, long PG_FAR *element_info, void *aux_data)

\{

PicHandle read_pict;

Ptr data_ptr;

long data_size;

data_size $=$ GetMemorySize(key_data);
read_pict $=($ PicHandle) NewHandle(data_size);

data_ptr = UseMemory(key_data);

BlockMove(data_ptr, *read_pict, data_size);

UnuseMemory(key_data);

/* At this point, you would do << whatever >> with the PicHandle, such as place it in a global, insert it into the text stream, etc. */

NOTE: The sample does not install a "write handler" since the data was written with $p g$ WriteKeyData.

### 34.8 About pg_ref(s) in Handler Functions

It is often necessary to obtain the $p g \_$ref from within a handler function. However, you will notice that the handler function provides you with a paige_rec_ptr, not a $p g \_r e f$.

Getting a pg_ref from an OpenPaige record pointer

To get the pg_ref, assuming the paige_rec_ptr parameter is called "pg", simply do this:

$$
\begin{aligned}
& \text { pg_ref the_pg_ref; } \\
& \text { the_pg_ref }=\text { pg->myself; }
\end{aligned}
$$

### 34.9 Special "Initializing" Handlers

Not all of the handler key codes are used to transfer data to and from files.

format_init_key — is used to signal the application that a style, paragraph or font record has been loaded from a file. This gives an application a chance to initialize any of these records, setting custom function pointers, etc.

Also, the format_init_key is used to inform your application when the file begins and ends, i.e. "prepare-to-read / prepare-to-write" and "end-read / end-write".

The format_init_key has a verb which indicates which is being initialized; this verb value is given in key_data. Coercing key_data will indicate one of the following:

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-717.jpg?height=396&width=1156&top_left_y=856&top_left_x=436)

init_start_verb and init_end_verb — are given to flag "begin" and "end" of the read or write session for the file.

The other verbs work as follows: For every style_info, par_info or font_info record that is fully reconstructed after reading data from a file, the appropriate handler function is called (if one exists) for the respective key (style_init_key, par_init_key or font_init_key).

When this occurs, the aux_data parameter points to the appropriate structure to initialize; the element_info parameter points to the structure element number (which element in the array of the styles, paragraph styles or fonts).

For example, if the handler for format_init_key is called, *aux_data will be a style_info_ptr, which you would coerce as follows:

style_info_ptr style_to_init;

style_to_init = *aux_data;

Function pointers in style_info and par_info records will be set to the default functions before being passed to the initialization handler.

## The "Extra Struct" Handler

Since application-specific elements usually comprise the contents of "extra struct" (set with $p g S e t E x t r a S t r u c t$, etc. See "Storing Arbitrary References \& Structures" on page 3-90), when OpenPaige writes this data it makes consecutive calls to the write handler for each extra struct entry.

When doing so, the parameters are set as follows:

$$
\begin{array}{ll}
\text { element_info } & \text { points to the extra struct ID number. } \\
\text { aux_data } & \text { pointer to the long data set in extra struct. }
\end{array}
$$

When the write handler is called, you must fill key_data with the appropriate data to write.

When the extra struct data is read later on, OpenPaige will call the read handler, passing the data in key_data, and *element_info with the original element_info given to you (and possibly modified by your function). However, for read handlers, OpenPaige won't do anything with the data - byou must call pgSetExtraStruct, or whatever else is appropriate from within your extra struct read handler.

## NOTES:

1. OpenPaige does not call the write handler for extra structs that are zero.
2. When returning from a write handler for extra struct, OpenPaige will write whatever is contained in *element_info. You can therefore modify *element_info contents, if so desired, and you will be fed that information during a read handler when the document is opened.

### 34.10 The Exception Handler

There is one additional handler key — pthe exception_key — pthat does not transfer data, rather it is used to report an error.

If any errors occur during file transfer, OpenPaige will call the exception_key handler function, if any. When this occurs, it is the responsibility of the handler function to handle the error as follows: upon entry, the element_info parameter will point to the error code (which will be one of the values defined in pgErrors. $h$ ).

If the handler function decides to continue the file transfer, it must set *element_info to zero (i.e., *element_info $=0$ ); to abort the transfer, leave *element_info alone (or set some other appropriate nonzero error code).

NOTE: It is generally a good idea to continue file transfer, i.e. set *element_info to zero, if NO_HANDLER_ERROR is given. It is also a good idea to set *element_info to zero if GLOBALS_MISMATCH_ERROR is given (see next section). Otherwise, you will defeat the ability to "skip" over unrecognized data elements. The NO_HANDLER_ERROR is passed to the exception handler mostly as a debugging tool.

(See also “Error Codes” on page 39-765).

### 34.11 Document-specific pg globals

There might be certain cases when you want to change the behavior of an application if an OpenPaige-based document is opened which was originally saved with a different $p g$ globals than the defaults.

Considering localization issues, for example, might demand that you keep a set of pg_globals for each document in case different values were used for decimal tab, a different default script such as Kanji, etc.

A file saved (in version $1.0 \mathrm{~b} 1$ and greater) includes a copy of the critical fields of $p g \_g l o b a l s$ at the time it was saved; when that file is reopened and one or more critical field(s) of the original globals does not match the current fields in $p g \_g l o b a l s$, the exception_key handler is called indicating the mismatch.

By "critical fields" is meant the portions of $p g \_g l o b a l s$ that are typically changed by the application (as opposed to volatile static values such as function pointers) such as character values, default style and default font.

To recognize a "globals mismatch" between the current settings and the document currently being read, set a handler for the exception_key and observe the following:

- When and if document-specific globals do not match the current globals, the exception_key handler is called.
- The "error" given to the exception_key handler is GLOBALS_MISMATCH_ERROR.
- The document-specific globals (just read) will be contained in a memory_ref in the "last_message" field of the memory globals.


## Access globals record

To access the "new" globals record you would do something like the following (the " $p g$ " parameter is assumed to be the paige_rec_ptr passed to the exception_key handler function):

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-721.jpg?height=493&width=1176&top_left_y=244&top_left_x=434)

- OpenPaige does not change any existing globals, rather it is your responsibility and/or decision to handle the globals mismatch any way you see fit. OpenPaige merely reports that the globals are different and provides those settings in the last_message field.
- The usual response before returning from the exception_key handler is to set *element_info to NO_ERROR (i.e., claim the exception was handled and therefore no file errors are pending - bsee previous section). Otherwise pgReadDoc will raise an exception and abort the reading process (which is probably not what you want).


### 34.12 Saving \& Reading Multiple pg_ref(s)

Many applications have the need to save more than one $p g \_r e f$ to a file. For example, an application that employs "headers" (each one a $\left.p g \_r e f\right)$ may need to save these along with the main body.

Steps to saving multiple $p g \_r e f s$ to one file are as follows

1. Set a long-word variable to zero (or to the desired file position if you aren't saving the document to position 0). Let's call this variable "filePosition."
2. Call pgSaveDoc() for the first pg_ref, passing filePosition for the file_position parameter. You do not need to set any special file handlers (unless you are saving something else that requires it); just pass NULL for the keys parameter.
3. Call pgTerminateFile(), passing filePosition once again.
4. If you have another $p g \_$ref to save, simply repeat (2) and (3) above.

That is all there is to saving multiple $p g \_$refs. The only important thing to remember is to leave "filePosition" alone after step 1.

## Reading Multiple Refs

The method outlined below for reading multiple $p g \_$refs assumes you already know in advance how many $p g \_r e f s$ there are in the file (if this is not the case see the note below, “Unknown OpenPaige Object Quantities”).

1. As in saving, set a long-word variable to zero (or to whatever the first file position is for the first pg_ref that was saved). We will call this "filePosition."
2. Create a new $p g \_r e f$ if you have not already (you can use $\operatorname{pgNew}($ ) or pgNewShell() depending upon your requirements).
3. Call pgReadDoc() passing filePosition and the newly created $p g \_r e f$.
4. If there is another $p g \_r e f$ to read, repeat 2 and 3 .

## Unknown OpenPaige Ref Quantities

The steps to retrieve multiple $p g \_r e f s$ shown above assumes you know, in advance, how many $p g \_$refs are contained in the file. If that is not the case, the recommended method for determining the number of $p g \_$refs is to use $p g$ VerifyFile() after each $p g R e a d D o c()$ to verify whether or not there is another valid OpenPaige element.

The intended purpose for using pgVerifyFile() is to verify whether or not a file is truly an OpenPaige file versus something else (like a text file). However, this function can also be used as a test for multiple $p g \_r e f s:$ after each $p g R e a d D o c()$, if $p g V e r i f y F i l e()$ returns NO_ERROR then the current file position is, in fact, another OpenPaige file.

Example of Method 2, Unknown OpenPaige Ref Quantities

## \#include "pgErrors.h"

/* The following function reads an undetermined number of multiple pg_refs written to a file. For demonstration purposes we are assuming the first pg_ref was written to the physical beginning of the file. Upon entry, fileRef is the file ID (a file opened for read access, specific to your OS). The "refs" parameter is a pointer to an array of pg_refs, large enough to

hold the most possible pg_refs that will be in a file. The function result is the number ofpg_refs successfully read. */

int ReadMultiplePG (int fileRef, pg_ref * ${ }^{*}$ efs) \{

long filePosition, oldPosition;

memory_ref fileMap;

short PG_FAR *file;

int readQty;

pg_ref pg;

// Set up what OpenPaige expects for the "filemap" param:

fileMap = MemoryAlloc(\&mem_globals, sizeof(short), 1, 0);

file $=\left(\right.$ short $\left.{ }^{\star}\right)$ UseMemory(fileMap);

${ }^{*}$ file $=($ short)fileRef;

UnuseMemory(fileMap);

filePosition $=$ oldPosition $=0 ; / /$ Set first file pos

// (Note, "oldPosition" is only used for a work-around to a 1.2 bug)

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-724.jpg?height=1074&width=1216&top_left_y=190&top_left_x=434)

### 34.13 Bypassing Standard I/O

There are certain cases when you need to write your own data structure directly to the file.

For Macintosh, an example might be writing a QuickTime movie in which a built-in system function is required that will write its own data by passing a file reference. For such cases it is desirable to temporarily bypass OpenPaige's standard I/O function when the physical data for a specific key is read or written.

As mentioned earlier, and handler can have its own private io_proc for reading and/or writing. Hence, the way to bypass the standard function for a specific key is to set the read or write function to one of your own.

## How It Works

The private read or write function for a handler is called only to read or write the physical contents of the data element, not the key actual header. For example, if you set a private write function for a picture, OpenPaige will call your write function when it comes time to write the picture contents but after it has already written the key header (key ID, element info and data size), at which time the next file position will be passed to your write function.

The data size you write does not need to match the data size already written to the key header; OpenPaige will adjust the header's data size if you return a new file position that is different than it expected.

Additionally, the "data" processed by the write handler (the handler for the file key, not the io_proc) does not necessarily need to be the same data that gets written by the I/O function.

For example, suppose you wanted to write the contents of a picture directly to the file. This could done by a write handler placing a mere reference to the picture into the data buffer (for Macintosh, the 4-byte PicHandle itself could be returned from the write handler as the "data" to be saved); then the private I/O function associated with the picture handler could take this "data" and write the real picture to the file. Note that the real data - pthe picture contents - pmight be hundreds of $\mathrm{K}$ but the data returned from the write handler was only 4 bytes. This "trick" is therefore a good way to write large data structures without the need to make a copy of the data.

The following example shows how a write handler + an associated private I/O function would write pictures directly to a file. You can use this example as a starting "shell" to write any similar structure.

/* Prototype for the private write function for the picture data:*/

PG_PASCAL (pg_error) WritePictProc (void* data, short verb, long *position, long PG_FAR *data_size, file_ref filemap);

// The io_proc to write

1* The following function accepts a PicHandle to be written to the data file defined by the rest of the parameters. The "refcon_info" is <whatever> so you can identify what the picture is for later when the file is opened. In this example, the basic I/O proc is NULL (implying the standard) but the data-write function is WritePictProc -- which gets called to physically write the data. Note that I am passing off as "data" a pointer to the PicHandle itself. But what really happens eventually, by virtue of the WritePictProc is the contents the picture get written instead. */

static pg_error SavePicture (pg_ref pg, PicHandle the_pic, long *file_position, file_ref filemap, long refcon_info) \{ return pgWriteKeyData(pg, mac_pict_key, \&the_pic, sizeof(PicHandle),refcon_info, NULL, WritePictProc, file_position, filemap);

\}

/* WritePictProc gets called by OpenPaige to physically write some data to the file. In this < special> case I have passed a PicHandle as the "data" whose size is sizeof(PicHandle) but I will really end up writing the picture contents. OpenPaige will adjust the data element header to reflect the correct written size. */

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-727.jpg?height=1067&width=1222&top_left_y=190&top_left_x=446)

## Important Tips \& Cautions

When writing your own I/O remember the following:

- When doing custom writes, OpenPaige will not call your I/O write function if the write handler does not set key_data's memory size to at least 1 byte. This is because OpenPaige will think there is nothing to write (which is a correct assumption since "zero data" is one of the legitimate ways to terminate a write handler being called repetitively). It is therefore necessary to return some kind of "data" from your handler even if it is only dummy data (consult the example above where a PicHandle is being used as the "data" so OpenPaige is sure to call the write function).
- The data and its byte size that is physically written when a write function is called can be completely different than what OpenPaige thinks is being written. However, it is important to update the *position parameter to reflect correct, next sequential file positions - pthat is how OpenPaige knows you write a different number of bytes than was originally expected.
- You do not update the key header information - pOpenPaige does that for you if you wrote a different size than originally asked when the write function got called.
- For read functions, the data size given to your function should be considered the literal, physical size of the data component. Regardless what/how you read the data you should always return with * position updated to *position + data size or pgRead might crash. Unlike write functions you must not try to change the file position to anything other than its starting position upon entry + data size upon entry.
- When your io_proc is called, upon entry the file position will be the starting location after the key header. For write functions, that will be the next physical location following the header; for read functions, OpenPaige will have already read the header information, the data size given will be the physical data size of the data component and the file position will be the first byte to read.
- If you use pgScrapMemoryWrite or pgScrapMemoryRead — por some other special I/O function for general writing, make sure your private I/O functions for individual keys will handle this appropriately. For example, in the sample shown above for writing pictures, a call to pgStandardWriteProc will fail if pgSaveDoc gave pgScrapMemoryWrite as the general I/O function.


### 34.14 Application Signature

To avoid any possible conflict between your own custom handler ID's and other OpenPaige-based files, you can set a unique "author" ID that gets saved with the document and that ID can be examined at any time during or after $p g R e a d D o c$.

To set or access such an identifier, use the following functions:

(void) pgSetAuthor (pg_ref pg, long author);

(long) pgGetAuthor (pg_ref pg);

Calling $p g$ SetAuthor stores author into $p g$; this value can be anything and is always saved along with a document if pgSaveDoc is called.

To get the current author value, call $p g G e t A u t h o r$.

Both functions can be called at any time and can be called from within handler functions.

NOTE (Macintosh): It is recommended that you use the same "author" ID that you are using to identify your own application signature (i.e. the "creator" OSType).

Reading OpenPaige files from other developers

If you might be reading someone else's OpenPaige file (that might have identical custom key values that you used), you should check your signature in the "author" field of the paige_rec given to you in the read handler:

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-729.jpg?height=584&width=1185&top_left_y=1065&top_left_x=434)

NOTE: When your own file is saved, call pgSetAuthor to set a unique "ID" so you will recognize your own signature per the above example. The "author" field gets saved with the file.

For the purposes of reading a file ( $p g R e a d D o c)$, it might be desirable to create a completely empty $p g \_r e f$ without the requirement to pass many parameters to $\mathrm{pgNew}$. To do so, you can call the following:

(pg_ref) pgNewShell (pg_globals_ptr globals);

The globals parameter must be a pointer to the same structure given to $\mathrm{pglnit}$.

FUNCTION RESULT: This function will returns a new $p g$ _ref that has nothing in it, including all shapes that are completely empty.

The idea is to pass this $p g \_r e f$ to $p g R e a d D o c$, in which case every important data component, including wrap_area and vis_area will get initialized.

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-730.jpg?height=110&width=117&top_left_y=1031&top_left_x=243)

CAUTION: If for some reason you have suppressed the read handler for vis_shape_key and/or page_shape_key(which process the vis_area and shape_area), or if one of these shapes do not exist in the file, your $p g \_$ref will result in an empty shape for the vis_area and/or page_area. This is because pgNewShell simply creates empty shapes assuming they will get set in $p g R e-$ adDoc. An empty vis_shape will cause an OpenPaige object to be completely "invisible" and an empty page_area can cause an OpenPaige object to hang, crash or also be invisible.

### 34.16 Examining Incoming Data

At times it may be necessary or desirable to examine some of the incoming data during the pgReadDoc process.

The way you can do this is to set your own handler function for the data you wish to examine, but call OpenPaige's standard handler function to actually process it.

Although a unique function can be set for any handler key, OpenPaige only uses one function for handling all standard keys for reading and one for all writing. The function for handling all standard keys, which is made public in "defprocs. $h$ " is declared as follows:

\#include "defprocs.h"

(pg_boolean) pgReadHandlerProc (paige_rec_ptr pg, pg_file_key key, memory_ref key_data, long PG_FAR *element_info, void *aux_data, long PG_FAR *unpacked_size);

From your own handler function, you could first call $p g R e a d H a n d l e r P r o c$ to bring in the information then you could examine the resulting contents within $p g$.

NOTE: The read handler places the appropriate data into $p g$. (To learn exactly what is transferred for each file key, consult the table "STANDARD HANDLERS" on page 34-730 below).

### 34.17 Standard Handler Data

The following table shows what is transferred into a paige_rec for every call to the standard read handler. This information can be useful when implementing the "Examining Incoming Data" method as given above on page 728.

Generally, the table shows what each parameter contains when the read handler is called; this is assuming that the standard write handler originally saved the data. The associated data will exist within the $p g \_r e f$ after the read handler returns.

NOTE: Unless specified otherwise, the contents of key_data are always "packed" into a special compressed format. If necessary, you can "unpack" the data by calling the standard read handler (see "Examining Incoming Data" on page 34728).

| TABLE \#7 | STANDARD HANDLERS |  |  |
| :---: | :---: | :---: | :---: |
| Handler Key | key_data contents | *element_info | aux_data |
| paige_key | All non-memory_ref fields such <br> as version, platform attributes, <br> etc. | -not used- | -not used- |
| text_block_key | Array of text blocks (NO TEXT <br> or other mem structures will <br> exist yet within the blocks). | Number of records | -not used- |
| Handler Key | key_data contents | *element_info | aux_data |
| text_key | Text for ONE block (each block <br> of text is saved separately, one <br> belonging to each text block <br> record). | Absolute byte offset <br> for beginning of text | - not used - |
| line_key | Same as text_key except data is <br> array of point_start records <br> instead of text. | Absolute byte offset <br> for first record. | - not used - |
| style_run_key | Array of style_run records. | Number of records. | - not used - |
| par_run_key | (Same as styles). | Number of records. | - not used - |
| style_info_key | (Same as styles but data is <br> style_info's). | Number of records. | - not used - |
| par_info_key | (Same as styles but data is <br> par_info's). | Number of records. | - not used - |
| font_info_key | (Same as styles but data is <br> font_info's). | Number of records. | - not used - |
| vis_shape_key | Array of rectangles | Number of <br> rectangles. | - not used - |
| page_shape_key | (Same as vis_shape_key). | Number of <br> rectangles. | - not used - |
| exclude_shape_key | (Same as vis_shape_key). | Number of <br> rectangles. | - not used - |


| TABLE \#7 | STANDARD HANDLERS | (Continued) |  |
| :---: | :---: | :---: | :---: |
| selections_key | Array of t_select records. | Number of records. <br> (Note: this is number <br> of t_selects, not pairs. <br> Selection pairs will <br> be *element_info / 2 . | - |


| Handler Key | key_data contents | *element_info | aux_data |
| :--- | :--- | :--- | :--- |
| extra_struct_key | set by app only | extra struct ID | *long as set <br> in extra struct |
| applied_range_key | Array of longs | Number of longs. | - not used - |
| doc_info_keys | The doc_info record | - not used - | - not used - |
| exception_key | - not used - | Error code | - not used - |
| containers_key | array of longs (refCon) | - not used - | - not used - |
| exclusion_key | array of longs (refCon) | - not used - | - not used - |

### 34.18 Repetitive Write Handler "Trick"

Occasionally, if you write data from a write handler (as opposed to the "direct" approach of calling $p g$ WriteKeyData) but need to do repetitive writes for several different data elements, it becomes necessary to write some data, return FALSE from the write handler, then get called again until you finally return TRUE.

For example, suppose you create a write handler to save multiple items embedded in a style run. Sometimes it proves useful to perform the "repetitive write"loop by returning FALSE from the handler so OpenPaige calls your function repeatedly until all elements are written.

To help this situation, OpenPaige always set the aux_data parameter to a long* (pointer to a long), with the long set to zero the first time it calls your handler but left asis for the remaining calls.

What this provides is the ability to monitor your own reentrance.

For example, in the case of writing elements from each style_info record in the $p g \_r e f$, you might want to know which element was last written (so you know when to end the callbacks to the write handler). Basically, aux_data points to a refcon value that you can set to anything, and that value can be examined in each callback.

Using aux_data in write handlers to pass data to yourself

pg_boolean MyWriteHandler(paige_rec_ptr pg, pg_file_key key, memory_ref key_data, long PG_FAR *element_info, void PG_FAR *aux_data, long PG_FAR *unpacked_size)

\{

long PG_FAR ${ }^{*}$ counter;

counter = (long PG_FAR *)aux_data;

if ( ${ }^{*}$ counter $==0$ )// being called for first time // do whatever if called first time

${ }^{*}$ counter $+=1 ; \quad$ // This value will be in tact next time

// We might terminate when, say, the counter hits 10 :

return $\left({ }^{*}\right.$ counter $\left.==10\right)$;

CAUTION: The aux_data parameter only points to a long when it is not being used for something else, i.e. if the file key is one of the standard OpenPaige keys that uses aux_data the above example will not work. As a rule, all "custom" key values are guaranteed to give you aux_data as a long* to a "refcon" value ini-
tialized to zero when your handler is called for the first time, but all standard OpenPaige keys (non-custom) will not necessarily provide this feature.

## 35 SHARED STYLES

You can create putrefies that all "share" a common set of style, paragraph, font records and named stylesheets. The purpose of this feature is to minimize the extra overhead required to save a large quantity of individual OpenPaige documents and/or to provide a method to create a "master document".

### 35.1 Setting Up

This feature is enabled by programming the following steps:

1. Create an empty $p g$ _ref which you will keep in memory. This will be the "master" set of all text formats; subsequent $p g \_r e f$ creations will "share" all the formats from the master. You probably won't ever display or draw the master $p g \_r e f$ so you can create it with pgNewShell(\&paige_globals).
2. All subsequent $p g \_r e f(s)$ should be created for "shared" formatting (shared with the master $\left.p g_{-} r e f\right)$. If using the direct API, you call the following function instead of $\operatorname{pgNew}()$ :

pg_ref pgNewShared (pgNewShared (pg_ref shared_from, const generic_var def_device, shape_ref vis_area, shape_ref page_area, shape_ref exclude_area, long attributes);

This is identical to $p g N e w($ ) except the first parameter — shared_from — is a pg_ref instead of a pointer to OpenPaige globals. This should be the master pg_ref(the one created in step 1).

All other parameters are the same as $\mathrm{pgNew}($ ). However, for any parameter that is NULL, those structures are also "shared".

For example, if def_device is 0L, the samewindow device in the master $p g$ _ref is used; if vis_area is OL then the same physical vis_area shape is shared from the master $p g_{-} r e f$, and so on. If you don't want the new $p g_{-} r e f$ to share its vis_area or page_area or exclusion_area, do not pass 0L for these values.

NOTE: about exclusion area(s): Most often, you won't be creating a $p g \_r e f$ that begins with an exclusion shape. For shared $p g \_r e f(s)$, however, not providing an exclusion shape to pgNewShared() will result in the inability to create a non-shared exclusion later on. The work-around is to create an empty shape for the exclude_area parameter.

### 35.2 Custom Control

If you create a custom control (instead of using OpenPaige API), you can share the control with the master $p g \_$refby sending the following message:

SendMessage(hWnd, PG_SHAREREFS, flags, master_pg);

After this message is sent, the hWnd (control) will be sharing the structures from master_pg (the $p g \_r e f$ created in step 1).

The flags parameter indicates which structure(s) you wish to share, which can be any of the following bit settings:

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-739.jpg?height=221&width=1333&top_left_y=417&top_left_x=223)

0x0001

0x0002

0x0004

\#define PGSHARED_PAGE_AREA 0x0008

\#define PGSHARED_EXCLUDE_AREA

/* Style, font, par infos shared */
/* Common graphics context */
/* Shared vis area */
/* Shared page area */
/* Shared exclusion area */

Probably, you only want to set PGSHARED_FORMATS.

### 35.3 Disposing

You do not need to do anything special to dispose a "shared" pg_ref or control. Just dispose the $p g \_r e f($ or close the control) in the same way that you would if they were not shared.

However, you must never dispose the master pg_ref while any shared $p g \_r e f(s)$ or controls are still alive.

### 35.4 Saving \& Reading

Saving the individual shared $p g \_r e f(s)$ or control(s) works the same as before: when you call $p g \operatorname{SaveDoc()}$ or $p g \operatorname{CacheSaveDoc(),~OpenPaige~realizes~that~some~of~the~}$ structures are shared with a master $p g \_r e f$, and therefore those structures are not saved to the disk file. Hence, you eliminate excess file overhead. This is also true for saving a
control with PG_SAVEFILE or PG_CACHESAVEFILE, as well as saving with the OpenPaige Export extension to "native" format.

Reading a shared $p g \_r e f$ or control also works as before (pgReadDoc(), pgCacheReadDoc(), or PG_READFILE and PG_CACHEREADFILE). However, you must first create an empty "shared" $p g \_$ref or control before reading the file.

### 35.5 Saving the Master

The ability to read a shared document assumes that the master $p g \_r e f$ is in tact, i.e. that it contains all the appropriate styles and formatting that existed at the time you saved each document.

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-740.jpg?height=45&width=1284&top_left_y=915&top_left_x=420)
(probably when your application initializes), create an empty $p g$ _ref then read it in with $p g R e a d D o c()$. The file-read sequence for shared $p g \_r e f(s)$ is therefore:

1. Open the "master" $p g$ _ref, residing in memory.
2. For each $p g \_r e f$ that you read from a file, create the $p g \_r e f$ with $p g N e w S h a r e d()$ then read the file.

## ANATOMY OF TEXT BLOCKS

### 36.1 Access to the text block array

The information in this section has been provided for OpenPaige users who need to access a $p g \_r e f ' s$ text block array.

One of the more common reasons to access a text block is to examine an array of line records to determine specific locations of characters and/or to alter line positions.

For performance and portability reasons, OpenPaige splits large blocks of text into smaller portions rather than maintain one continuous text stream. The approximate size of a block is determined by the max_block_size in pg_globals: when any block of text exceeds pg_globals.max_block_size, OpenPaige will split it into two or more new blocks.

Text block record

Every block of text in a $p g \_r e f$ is represented by the following record:

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-742.jpg?height=830&width=1405&top_left_y=189&top_left_x=209)

Each field, from top to bottom, has the following meaning:

begin, end $\mathrm{p}-$ defines the absolute beginning and ending offsets for this block of text (relative to the beginning of all text). The text size:

$$
\text { text_block.end - text_block.begin. }
$$

boundsp — defines the outermost bounds, as a rectangle, for the calculated text (by "calculated" is meant how the text will appear once all word wrapping, etc. is computed for this block). This is not necessarily the actual shape of the drawn text, rather the rectangle's four sides represent the leftmost, topmost, rightmost and bottommost areas.

text $\mathrm{p}$ — the memory_ref containing the text. Passing this value to UseMemory would return a pointer to the first text byte.

linesp— the memory_ref containing an array of point_start records (see below). Passing this value to UseMemory would return a pointer to the first point_start.

| \#define NEEDS_CALC | $0 \times 0001$ | $1 *$ One or more lines need re-calc *\| |
| :---: | :---: | :---: |
| \#define NEEDS_PAGINATE | $0 \times 0002$ | $/ *$ Needs re-paginatation $* /$ |
| \#define SOME_LINES_GOOD | $0 \times 0004$ | $1 *$ One or more lines are probably OK */ |
| \#define SOME_LINES_BAD | $0 \times 0008$ | /* Some lines not calculated */ |
| \#define BROKĒ_BLOCK | $0 \times 0010$ | /* Terminator char deleted */ |
| \#define ALL_TEXT_HIDDEN | $0 \times 0020$ | $l^{*}$ All text in block is hidden! */ |
| \#define BOŪNDS_GUESSED | $0 \times 0040$ | $I^{*}$ Best guess only for bounds rect $* /$ |
| \#define LINES_PŪRGED | $0 \times 0080$ | /* Lines purged, but block OK */ |
| \#define BELOW_CONTAINERS | $0 \times 0100$ | /* Lines below last container */ |
| \#define NO_CR_BREAK | $0 \times 0400$ | $/ *$ Does not break on a CR */ |
| \#define SWITCHED_DIRECTIONS | $0 \times 0800$ | /* System text direction has switched! */ |
| \#define LINES_NOT_HORIZONTAL | $0 \times 1000 / 1$ | Point starts are not always horizontal */ |
| \#define JUMPED_4_EXCLUSIONS | $0 \times 2000 / *$ | One or more lines hop across exclusions */ |
| \#define NEEDS_PARNUMS | $0 \times 4000$ | Requires paragraph "line" computation */ |

num_lines through first_par_num — bIf COUNT_LINES_BIT is set in the pg_ref attributes, these fields are used to track line and paragraph numbering. The first_line_num and first_par_num values define the first line number and paragraph number in this block, respectively, while num_lines and num_pars indicate the number of lines and paragraphs found in this block only. If COUNT_LINES_BIT is not set, all these fields are zero.

end_start_pContains a copy of the ending point_start record (point_start for the ending line of text in this block).

NOTE: Most of the fields in a text_block are only accurate if the flags field has neither NEEDS_CALC, NEEDS_PAGINATE nor SOME_LINES_BAD set.

Text lines are represented by a series of point_start records; for every text block, an array of point_start's are maintained in the "lines" memory_ref.

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-744.jpg?height=460&width=1282&top_left_y=430&top_left_x=217)

Any line of text might have a number of point_start records to represent its character positions. Generally, a point_start will exist for every display change in a line. This includes style changes, tab positions and of course line-feed and line-wrap changes.

The meaning of each field, from top to bottom, is as follows:

offset — the text byte position for this point_start, relative to the start of text for this block. Hence, an offset of zero implies the first byte for the block.

r_num — the rectangle element in page_area where this point_start first intersects. If zero, it intersects the first rectangle in page_area (a shape, such as page_area, is a series of rectangles). If the $p g \_r e f$ is set for repeating shapes, the actual physical rectangle number can be computed as r_num / rect_qty (where rect_qty is the number of rectangles in the page_shape). To determine "page number", compute the modular value of $r_{-}$num and add one.

extra - either a tab record element OR a full justification value. If high bit is set (0x8000), the low-order bits define a tab record element index from the paragraph style applying to this text. If high-bit is not set, the value in extra defines the amount of "slop", in pixels, to compensate for full justification drawing.

baseline - amount of offset from line's bottom to draw the text, in pixels.
flags — contains bit setting(s) for various attributes for the text within this point_start (see "Line Flags" on page 36-743 below).

bounds — defines the bounding rectangle around the text for this point_start.

Text "Length" of a Line

The length of text for each point_start is determined by the NEXT point start in the array, i.e., text length of array[0] is array[1].offset - array[0].offset. The point_start array is always terminated with a "dummy" record for this purpose.

## Line Flags

If you examined any array of point_start records, a point_start's "flags" field will reveal much of the information you often want to know. The flags will be a combination of bit settings as follows:

| \#define LINE_BREAK_BIT | $0 \times 8000$ | $/ *$ Line ends here */ |
| :---: | :---: | :---: |
| \#define PAR_BREAK_BIT | $0 \times 4000$ | /* Paragraph ends here */ |
| \#define SOFT_PAR_BIT | $0 \times 2000$ | /* Soft CR ends line */ |
| \#define RIGHT_DIRECTION_BIT | $0 \times 1000$ | $/ *$ Text in this start is right to left $* /$ |
| \#define LINE_GOOD_BIT | $0 \times 0800$ | /* This line requires no recalc $* /$ |
| \#define NEW_LINE_BIT | $0 \times 0400$ | $1 *$ New line starts here $* /$ |
| \#define NEW_PAR_BIT | $0 \times 0200$ | /* New paragraph starts here $* /$ |
| \#define WORD_HYPHEN_BIT | $0 \times 0100$ | $1 *$ Draw a hyphen after this text */ |
| \#define TAB_BREAK_BIT | $0 \times 0080$ | /* Tab char terminates this line */ |
| \#define HAS_WORDS_BIT | $0 \times 0040$ | /* One or more word separaters exist * |
| \#define CUSTOM_CHARS_BIT | $0 \times 0020$ | /* Style(s) are custom, not OpenPaige * |
| \#define SOFT_BREAK_BIT | $0 \times 0010$ | /* Start breaks on soft hyphen $* /$ |
| \#define BREAK__CONTAINER_BIT | $0 \times 0008$ | /* Line breaks for next container $* /$ |
| \#define BREAK_PAGE_BIT | $0 \times 0004$ | /* Line broke for exclusion */ |
| \#define LINE_HIDDEN_BIT | $0 \times 0002$ | $1 *$ Line is invisible */ |
| \#\#define NO_LINEFEED_BIT | $0 \times 0001$ | /* Line does not advance vertically */ |
| \#define TERMINATOR_BITS | $0 x F F F F$ | $/ \star$ Flagged only as terminator record */ |
| \#define HARD_BREAK_BITS (PAR_BREAK_BIT \| SOFT_PAR_BIT | BREAK_CONTAINER_BIT | <br> BREAK PAGE BIT) |  |  |

As mentioned, every array of point_start records has at least one "dummy" record as a terminator. This record will always have the value TERMINATOR_BITS in the flags field.

For any point_start, if LINE_GOOD_BIT is NOT set, all remaining fields are not to be considered valid.

### 36.3 Text Block Support Functions

The following functions are available to find and otherwise access text blocks in a pg_ref:

(long) pgNumTextblocks (pg_ref pg);

(long) pgGetTextblock (pg_ref pg, long offset, text_block_ptr block, pg_boolean want_pagination);

$p g N u m T e x t B l o c k s$ returns the total number of text block records in $p g$. There will always be at least one, even if no text exists.

$p g G e t T e x t B l o c k$ will return a copy of the text_block record in *block that contains offset (which is an absolute position relative to the start of all text).

If want_pagination is TRUE, the block is calculated if necessary. Note that if want_pagination is FALSE, there it is possible to get a block whose line records are not in tact; paginating the block, however, can be time consuming particularly if it is are down the list of many blocks.

The function result of $p g G e t T e x t B l o c k$ is the record number (element number from the array of text blocks within $p g$ ).

I want to write a "Find" function so I need to walk through the text within a pg_ref. I do not want to "copy" the text to look at it, that would be too slow. Is there a way to do this?

When speed is a critical issue and you have the need to look at OpenPaige text, you are best off looking at these structures directly. The following code sample shows various "hacks" to do this:

// To look at the text_block records, we need to get // access to the paige_rec within the pg_ref:

paige_rec_ptrpg_rec;

text_block_ptrblocks;

long

char

num_blocks, num_bytes;

*text;

pg_rec $=$ UseMemory $(\mathrm{pg}) ;$

// Then get the pointer to the text_block array:

blocks = UseMemory(pg_rec->t_blocks);

/* To know how many text_block records exist, get memory sized of $t$ _blocks: */

num_blocks = GetMemorySize(pg_rec->t_blocks);

/* Also note that "blocks" is also an array, i.e.:

blocks[1] is next block, if any blocks[2] is the one after that, etc. - or -

blocks += 1 advances to next block. */

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-748.jpg?height=885&width=1141&top_left_y=173&top_left_x=429)

### 37.1 OpenPaige Custom Placement of Lines and Paragraphs

Occasionally, an OpenPaige users needs to enhance a word processing environment beyond the built-in feature set of OpenPaige. This particular chapter discusses the methods required to provide "widows and orphans," "keep paragraphs together", and other forms of paragraph and line manipulation.

For basic pagination techniques and how to build repeating shapes to contain your text see "Pagination Support" on page 13-1. For information about the line_adjust_proc hook which is the key hook used in this chapter, see "line_adjust_proc" on page 27-48.

For purpose of clarity, we will define the following technical terms used in this discussion:

Line -- a row of characters in a document. The reason we feel it necessary to define "line" is to avoid confusion with CR/LF-breaking text. In OpenPaige, a "line" is any row of characters that break due to either word wrapping or because of the presence of a CR character. Therefore, in such a wordwrapping environment, a line and paragraphs are not necessarily synonymous (in applications that do not word-wrap lines, a line and paragraph IS synonymous).

Page -- the area in a $p g \_r e f(u s u a l l y$ a rectangle) in which text will flow. For the purposes of this discussion, we assume that the $p g \_r e f$ contains multiple pages, i.e. "repeating shape" feature is enabled, providing the appearance of multiple page breaks.

Pagination -- the computation and vertical placement of lines. While the term "pagination" derives from the word "page" and often implies formatting of text across multiple page boundaries, we use the term "pagination" here to mean any vertical placement of lines, with or within multiple page breaks.

Paragraph -- a block of text that terminates with a CR character (or the last block of text in the document if no CR character). If OpenPaige is set for wordwrapping, a paragraph can consist of many lines (in which the ending line is terminated with a CR). If OpenPaige is not set for wrapping, a paragraph and line are synonymous.

### 37.3 How Pagination Occurs

OpenPaige formats the drawing positions for each line of text by building an array of records that define the text offset and bounding coordinates for groups of characters. If no changing styles or tabs exist in the text, a single line is usually represented by one of these records; for lines that change styles and/or contain tab characters, a line will consist of many of these records.

The record that composes a line (or part of a line) is called the point_start, which is defined as follows:

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-751.jpg?height=468&width=1126&top_left_y=283&top_left_x=435)

For a block of fully paginated text, OpenPaige will create a point_start record for all style and screen position changes. By "screen position changes" we mean either some extra horizontal "jump" (such as a tab character), or a new line (from wordwrapping or CR).

The bounds field in the point_start always represents the exact display location and dimensions of the text, i.e. bounds.top_left will contain the top-left pixel coordinate of the text, and bounds.bot_right will contain the bottom-right pixel coordinate of the text.

NOTE: The bounds dimensions always represent the display dimensions, not necessarily the character dimensions (for example, if extra line spacing or leading has been added to the text, bounds.bot_right.v might be larger than the actual characters' descent).

The display positions represented by the bounds rectangle are always unscaled and unscrolled. In other words, their coordinates always reflect the position of the text relative to the top-left origin of your window, whether or not the document is "scrolled" and whether or not the document is "scaled".

Implementing widows and orphans, keeping paragraphs together, etc., can be accomplished dynamically by intercepting the point_start array for each text_block record that is paginated, and making the necessary adjustments.

The recommended method for doing this is to set the paginate_proc within the $p g \_r e f$. OpenPaige will call this function after it is through paginating a text_block record.

NOTE: OpenPaige performs pagination on a text_block level, not a "page" or "line" level. For example, if a large document had to be paginated, OpenPaige would walk through the text_block array and paginate the text for one text_block record at a time; the paginate_proc hook gets called after the completion of pagination for each text_block.

The concept of using the paginate_proc is to make adjustments to the line array (point_start records) after OpenPaige is done calculating the lines within a block; several code examples are shown below for typical applications.

### 37.5 Changing the point_start Array

Although we can't tell you how to write your custom feature, we will attempt to provide enough information here to do almost any form of paragraph or line adjustments.

IMPORTANT — bMatching text_block Members

If you alter any of the point_start records within a text_block it is important to also make adjustments to the following members:

text_block.bounds — This defines the bounding rectangle for all text within the block. Essentially, text_block.bounds is the union of all point_start.bounds. Hence if you move some lines up or down you should also adjust the bounding area as recorded in the text_block.

text_block.end_start_pThis is a copy of the last point_start in the block. If you change the last point_start, copy its contents to this member.

Determining type of line

An obvious requirement for manipulating paragraphs or lines is to determine what kind of line you are looking at, i.e. is the line at the beginning of the paragraph, somewhere in the middle, or at the end.

Examining flag fields of a line

The easiest way is to examine the flags field of the first and last point_start of the line as follows:

If NEW_PAR_BIT is set in the first point_start, the line is the BEGINNING OF A PARAGRAPH.

## Example:

$$
\begin{gathered}
\text { if (starts->flags \& NEW_PAR_BIT) } \\
\text { // line begins a paragraph. }
\end{gathered}
$$

-- if PAR_BREAK_BIT is set in the last point_start, the line is the ENDING OF A PARAGRAPH.

## Example:

if (starts[num_starts - 1].flags \& PAR_BREAK_BIT)

// line is last one in paragraph, i.e. ends with CR.

NOTE: A line can, of course, have both NEW_PAR_BIT and PAR_BREAK_BIT set at the same time, which means the paragraph has only one line (or is just a CR character).

Adjusting vertical position

Another obvious requirement is the ability to move a line up or down (to adjust for a page break or to force the line to begin on the next page, etc.).

The easiest way to adjust the line's vertical position is to walk through num_starts records and move each bounds rectangle with $p g O$ OffsetRect. Suppose you wanted to move the line "down" 10 pixels; you would do so as:

$$
\begin{aligned}
& \text { pg_short_t counter; } \\
& \text { for (counter = } 0 ; \text { counter < num_starts; ++counter) } \\
& \text { pgOffsetRect }(\& s t a r t s[\text { counter].bounds, 0, 10); }
\end{aligned}
$$

However, you must ALSO ADJUST THE LINE_FIT RECTANGLE because OpenPaige uses that rectangle to place the next line it computes. Hence, in addition to the above, you must also do:

pgOffsetRect(line_fit, 0, 10);

Affecting subsequent lines -- All lines will follow suit (vertically) from the bottom position of line_fit when your function returns. In other words, OpenPaige starts the top of the next line at the precise position of line_fit->bot_right.v. Hence, if you want your line adjustment to affect future lines as well (i.e., if you move a line down you want all subsequent lines to move down by the same amount), you do nothing except adjust the current line and OpenPaige will handle the rest.

## Examining line(s) before the current

It may become necessary to examine one or more lines prior to the current line given in the adjust_proc.

One example might be a situation where the current line is the middle of a paragraph but you need to know the position of the first line in the paragraph.

Since the starts pointer actually points to a specific element in the entire array of point_starts that have been computed thus far, you can simply decrement it backwards to examine line(s) before the current position, if they exist.

However, the only point_start elements that are guaranteed to exist in the array are all the elements for the current paragraph; this is due to the fact that OpenPaige breaks apart large blocks of text into smaller sections -- but never in the middle of a paragraph.

Obtaining the point_starts of the current paragraph

To obtain the first point_start of the current paragraph, you can decrement the pointer until the flags field contains NEW_PAR_BIT. Here is an example:

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-756.jpg?height=403&width=660&top_left_y=194&top_left_x=434)

You can do the same thing to back up to start of the previous line, with a slight alteration:

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-756.jpg?height=356&width=663&top_left_y=800&top_left_x=432)

CAUTION: Be sure there are truly previous point_start elements before backing up the starts pointer. The simplest way to check this is to examine the "offset" field of the start; if it is zero, there are no elements before it.

## Example:

if (starts->offset $==0$ )

/* We must not "back up" because starts is the FIRST START.*/

NOTE: The "first start" does not necessarily mean the start of the whole document, rather the start of the current block of text. The first start however will always be the beginning of a paragraph.

For purposes of custom pagination of paragraphs, you probably need to compute the vertical location of page boundaries.

CAUTION: THE INFO GIVEN HERE IS ASSUMING "REPEATING SHAPES" ARE ENABLED to achieve a multiple-page effect. If you are using some other method for page breaks, this information might not apply (mainly because we do not know how you have implemented page sizes and breaks).

The following is a list of very useful low-level utility functions that you can use to find out about the current "page" that a line will display in:

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-757.jpg?height=156&width=1016&top_left_y=826&top_left_x=487)

This function returns sufficient information to construct the exact "page" rectangle for a given line of text. (Note, it is prototyped in "pgShapes. $h$ " and is intended to be called from low-level hooks such as adjust_proc).

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-757.jpg?height=45&width=1226&top_left_y=1274&top_left_x=425)
When this function returns, offset_extra gets set to the amount to adjust the original page rectangle to obtain the actual, physical page location (remember we are dealing with "repeating shapes," which means the pg_ref has only one page shape which repeats; this function computes the physical page position based on that information).

For purposes of obtaining only the vertical positions of the page, the function result can be ignored.

Here is an example of obtaining the page rect for a line of text in question (while in the adjust_proc):

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-758.jpg?height=497&width=1220&top_left_y=393&top_left_x=433)

## $37.7 \quad$ Page Break Characters

If your application implements page break characters, you can determine if the line has terminated with a forced page break by examining the ending point_start flags field:

$$
\begin{aligned}
& \text { if (starts[num_starts - 1].flags \& BREAK_PAGE_BIT) } \\
& \text { // line ends with forced page break char. }
\end{aligned}
$$

BREAK_PAGE_BIT only gets set if the line terminates with a physical page-break character (it does not get set just because more lines won't fit on the page.

## UNICODE SUPPORT

Using the appropriate OpenPaige library (or compiling OpenPaige with \#define UNICODE) will help you create a Unicode-aware application.

### 38.1 Compiler Settings

To compile an application using the OpenPaige Unicode library (or to build the OpenPaige Unicode library) you must provide the pre-definitions "UNICODE" and “_UNICODE". It is best to use the preprocessor settings in your compiler for these definitions (not CPUDEFS. $H$ ) because your Windows headers require these definitions to resolve various macros.

### 38.2 Absolute Unicode

OpenPaige Unicode expects absolute Unicode in every respect. This includes anything whatsoever that has previously been declared as a "char" or "unsigned char".

For example, $p g$ Insert() expects your character(s) insertions to be wide characters (16 bit). The font name(s) in font_info are expected to be 16-bit characters as well. If you are using the custom control, all strings are assumed to be Unicode (the "OpenPaige" window class, the default font name, etc.).

Text positions and offsets are also Unicode-aware, thus they must be considered character offsets and not byte offsets. For example, if the insertion point (caret) is sitting between characters 4 and 5, pgGetSelection() will return position 4 even though the physical byte position is 8. Similarly, pgTextSize() will return the total (Unicode) character size, not the physical byte size. Every structure within OpenPaige Unicode assumes Unicode-based text; this design has been implemented for transparency and ease of upgrading.

### 38.3 OpenPaige Character Types

To support both Unicode and non-Unicode in a portable fashion, a new generic type has been declared:

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-760.jpg?height=247&width=843&top_left_y=1135&top_left_x=432)

Most parameters in OpenPaige API have changed from $p g \_b y t e$ and $p g \_b y t e \_p t r$ to pg_charand pg_char_ptr.

For historical purposes, the older type "pg_byte" is still valid but it maps to $p g \_c h a r$.

If you need to declare a true byte (8-bit value), OpenPaige provides the following:

typedef unsigned char pg_bits8, *pg_bits8_ptr;

Most of the file I/O supported by OpenPaige Unicode will be transparent to your application. If an older OpenPaige file is opened and/or if an OpenPaige Unicode-aware program opens a non-Unicode OpenPaige file, the text will be translated appropriately with no required intervention from your application.

Even if you are running the non-Unicode version of OpenPaige, reading OpenPaige Unicode files will still be converted to 8-bit ASCII text.

### 38.5 Import/Export

The OpenPaige import/export extension will translate Unicode to ASCII or ASCII to Unicode, whichever is appropriate. For example, when importing a text file the importer checks for the existence of Unicode (or not) and will convert the characters as necessary during the import. This will work (more or less) even if you are running the non-Unicode OpenPaige library - if Unicode text is being imported it will be converted to nonUnicode, 8-bit ASCII.

## Exceptions

Exporting text and RTF, however, will export non-Unicode ASCII by default. If you need to export Unicode text, the following flag has been added to the export definitions:

## EXPORT_UNICODE_FLAG

After you have created the export object, set EXPORT_UNICODE_FLAG in the "export_bits" member.
filter $=($ PaigeExportObject) new PaigeRTFExportFilter(); filter->feature_bits |= EXPORT_UNICODE_FLAG;

### 38.6 Unicode Support Utilities

NOTE: Unless specified otherwise, these support utilities can be called even if the runtime OpenPaige library is non-Unicode (version 2.0 or above).

pg_boolean pglsPaigeUnicode (void);

Returns TRUE if the current runtime OpenPaige library supports Unicode. This function works for all 2.0b1+ versions, with or without Unicode support.

NOTE: A "TRUE" merely means that the library — not necessarily the OS

— supports Unicode.

pg_boolean pgInsertBytes (pg_ref pg, const pg_bits8_ptr data, long length, long position, short insert_mode, short modifiers, short draw_mode);

This function is identical to $p g I n s e r t($ ) except the data to be inserted is considered to be 8-bit characters. The purpose of this function is to provide a way for a Unicode application to (still) be able to insert 8-bit ASCII if necessary (calling pgInsert() assumes Unicode characters).

Calling this function in a non-Unicode OpenPaige library will do the same thing as pgInsert(). If called in a Unicode OpenPaige library, the bytes are converted internally to 16-bit Unicode characters.

You can force text to be saved as Unicode even if you are running in a non-Unicode environment. To do so, set the extended attribute "SAVE_AS_UNICODE" using pgSetAttributes2() before calling pgSaveDoc(). When this attribute is set, the text is converted to Unicode (16 bit characters).

NOTE: While converting Roman or "English" characters will generally convert to 16 bit characters properly, complex double byte languages such as Japanese may not convert correctly. To work around this problem you need to supply the necessary character conversion functions as described below.

### 38.8 Unicode Conversion Hooks

In certain cases, OpenPaige is required to convert Unicode to non-Unicode, or nonUnicode to Unicode. In every case, one of the two low-level "hook" functions are called as shown below.

Both of these functions are style_info hooks, i.e. they apply to individual text formats. Initially, an internal function is used as the default. For bytes_to_unicode_proc the standard (default) function merely converts 8 bit characters to 16 bit characters and unicode_to_bytes_proc performs the reverse. For special languages, scripts, etc. you would need to provide your own conversion functions to replace the defaults.

38.9 Non-Unicode to Unicode

long bytes_to_unicode_proc (pg_bits8_ptr input_bytes, pg_short_t PG_FAR *output_chars, font_info_ptr font, long input_byte_size);

Upon entry, input_bytes is a pointer to a buffer of bytes (8 bit characters); input_byte_size defines the number of bytes.

NOTE: The input is considered a byte stream even if they are logically "double byte characters” such as Japanese text.

If output_chars is NULL, no conversion is to occur; instead, this function should simply return the number of characters that would result from a conversion to Unicode.

If output_chars is not NULL, the converted characters are to be output to this buffer; note that the actual size of the output_chars buffer will be large enough to accommodate the conversion, assuming that each and every byte in input_bytes will be converted to a 16 bit value.

The font parameter will contain the current font of the text (which typically will contain language and script information).

NOTE: All the characters provided are guaranteed to be rendered in this font, i.e. the conversion function will never be called with "mixed" fonts.

FUNCTION RESULT: The function should return the total number of characters converted (that were placed into output_chars) or the number of characters that would be converted (if output_chars is NULL).

NOTE: This is a character count, not a byte count.

### 38.10 Unicode to Non-Unicode

long unicode_to_bytes_proc (pg_short_t PG_FAR *input_chars, pg_bits8_ptr output_bytes, font_info_ptr font, long input_char_size);

Upon entry, input_chars is a pointer to a buffer of 16 bit characters; the number of characters is given in input_char_size.

NOTE: input_char_size is a character count, not a byte count.

The converted characters are to be output to the output_bytes buffer.

NOTE: The actual size of the output_bytes buffer will be large enough to accommodate the conversion, assuming the possibility that all characters might result in double byte sizes (e.g., Japanese conversions, etc.).

This function only gets called if the characters in input_chars are, in fact, Unicode; a call will never occur otherwise.

The font parameter will contain the current font of the text (which typically will contain language and script information).

NOTE: All the characters provided are guaranteed to be rendered in this font, i.e. the conversion function will never be called with "mixed" fonts.

FUNCTION RESULT: The function should return the total number of bytes converted (that were placed into output_bytes.

NOTE: This is a byte count, not necessarily a character count.

### 38.11 Hook Names

The Unicode conversion hooks are members of style_info.procs; their respective names are:

style_info.procs
.bytes_to_unicode;// Non-Unicode to Unicode style_info.procs.unicode_to_bytes;// Unicode to Non-Unicode

## 39 <br> ERROR CODES

### 39.1 The \#define error codes

The following error codes are defined in "pgErrors. $h$ ".

NOTE: These defines are not brought in by Paige. $h$ : In addition, they vary slightly from platform to platform.

No error

|  | Mac | Windows |  |
| :--- | :--- | :--- | :--- |
| \#define NO_ERROR | Ox0000 | 0x0000 | /* No error *I |


| \#define NO_MEMORY_ERR <br> \#define NOT_ENOUGH_PURGED_ERR | Mac Windows |  |  |
| :---: | :---: | :---: | :---: |
|  | memFullErr | $0 \times 0100$ | $/ *$ Insufficient memory $* /$ |
|  | $0 \times 0101$ | $0 \times 0101$ | /* Cannot purge enough space */ |
| \#define NO_PURGE_FILE_ERR | $0 \times 0102$ | $0 \times 0102$ | $1 *$ Purge file not available $\star /$ |
| \#define LOCKKED_BLOCK_ERR | $0 \times 0103$ | $0 \times 0103$ | /* Cannot resize locked block */ |
| \#define NIL_ADDRESS_ERR | /* Address is NIL (not valid) */ |  |  |
| \#define BAD_ADDRESS_ERR | $0 \times 0104$ | $0 \times 0105$ | /* Address is bogus (not valid) * |
| \#define BAD_LINK_ERR | $0 \times 0105$ | $0 \times 0106 /$ | / Something wrong with internal ref |

OpenPaige memory_ref-specific errors

|  | Mac | Windows |
| :---: | :---: | :---: |
| \#define CHECKSUM ERR | $0 \times 0200$ | 0x0200 /* Memory ref checksum error */ |
| \#define ACCESS_ERR | $0 \times 0201$ | 0x0201 /* Access failed on memory_ref */ |
| \#define BAD_REF_ERR | $0 \times 0202$ | /* Bogus memory_ref */ |
| \#define REF_DISPOSED_ERR | $0 \times 0203$ | 0x0203/* Memory_ref has been disposed */ |
| \#define FILE_PURGE_ERR | $0 \times 0204$ | 0x0204 /* Error on file when purging */ |
| \#define FILE_UNPURGE_ERR | $0 \times 0205$ | $/ \star$ Error on reading purged file */ |
| \#define RANGĒE_ERR | $0 \times 0206$ | $1 *$ Access out of range */ |
| \#define PURGED_MEMORY_ERR | 0x0207 | 0x0207 /* Attempt to operate on a purged <br> block */ |
| \#define DEBUG_ZERO_ERR | $0 \times 0208$ | /* Access is zero debug check */ |
| \#define DEBUG_NZ_ERR | $0 \times 0209$ | 0x0209/* Access is non-zero debug check */ |
| \#define NO_ERR_HANDLER_ERR | $0 \times 020 A$ | /* No exception handler */ |
| \#define PG_PSTRING_TOO_BIG_ERR | 0x020B | 0x020B // conversion to Pascal string error |


|  | Mac | Windows |  |
| :---: | :---: | :---: | :---: |
| \#define NO_HANDLER_ERR | $0 \times 0300$ | $0 \times 0300$ | /* A key handler not found */ |
| \#define NO_SPACE_ERR | fnOpnErr | $0 \times 0301$ | $1 *$ File has insufficient space */ |
| \#define NOT_OPEN_ERR | fnOpnErr | $0 \times 0302$ | $/ *$ Requested file not open $* /$ |
| \#define FILE_LOCK_ERR | fLckdErr | $0 \times 0303$ | $/ *$ Disk write protected $\star /$ |
| \#define WRITE_EPRŌTECT_ERR | wPrErr | $0 \times 0304$ | /* Medium write protected $\star /$ |
| \#define ACCES̄S_DENIED_ERR | permErr | $0 \times 0305$ | $1 *$ Access permission denied */ |
| \#define EOF_ERR | eofErr | $0 \times 0305$ | $/ \star$ Attempt to go past end of file $\star /$ |
| \#define IO_ERR | ioErr | $0 \times 0306$ | $1 *$ Hard I/O Error */ |
| \#define BAD_TYPE_ERR | $0 \times 0301$ | $0 \times 0308$ | /* File not of appropriate type */ |
| \#define UNICODE_ERR | $0 \times 0309$ | 0x0309 <br> handle */ | /* File has unicode, platform cannot |
| \#define NO_FILE_ERR | 0x03FE | 0x03FF | $/ *$ File not found $* /$ |
| \#define SOFT_EOF_ERR | $0 \times 03 F F$ | $0 \times 03 \mathrm{FF} /$ | /* Logical end of file "error" abort */ |

Runtime debugging errors (not Allocation Manager related)

| \#define LOCKED_PG_ERROR <br> \#define ILLEGAL_RE_ENTER_ERROR <br> \#define BAD_PARAM_ERROR <br> \#define GLOBALS_MISMATCH_ERROR <br> \#define DUP_KEY_HANDLER_ERROR <br> \#define BAD_REFCON_ID_ERROR <br> \#define STRUCT_INTEGRITY_ERR <br> \#define USER_BREAK_ERR <br> \#define CARET_SYNC_ERR | All Platforms |  |
| :---: | :---: | :---: |
|  | $0 \times 0400$ | /* Attempt to change a locked pg_ref */ |
|  | $0 \times 0401$ | $/ *$ Illegal reentrance $* /$ |
|  | $0 \times 0402$ | $/ *$ Bad paramter in function $* /$ |
|  | $0 \times 0403$ | $/ *$ globals in doc don't match pg_globals */ |
|  | $0 \times 0404$ | $/ *$ pgWrite/Read key that already exists */ |
|  | $0 \times 0405$ | $/ *$ Bad refcon number of exclusion */ |
|  | $0 \times 0406$ | $l^{*}$ Style structures bad */ |
|  | $0 \times 0407$ | /* User-invoked debug break */ |
|  | $0 \times 0408$ | $1 *$ Caret and caret bit out of sync */ |
