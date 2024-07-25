## PAGINATION SUPPORT

Although OpenPaige does not provide full pagination features as such, several powerful support functions and features exist to help implement page breaks, columns, margins, etc.

For custom text placement not covered in this chapter and for custom pagination features such as widows and orphans, keep with next paragraph, etc. see "Advanced Text Placement" on page 37-1.

### 13.1 OpenPaige "Document Info"

In every $p g \_r e f$, the following structure is maintained:

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-238.jpg?height=1280&width=1192&top_left_y=191&top_left_x=434)

NOTE: Some of the fields in $p g \_d o c \_i n f o$ are currently unsupported, some of them are defined in Paige. $h$ but not included above (but exist for future enhancements and extensions).

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-239.jpg?height=217&width=385&top_left_y=322&top_left_x=764)

Text pushes

bottom for

infinite bottom

The fields that are currently supported are as follows:

attributes — defines special characteristics for the page_area shape. The attributes field applies only to the page_area shape (not vis_area or exclude_area), and it is a set of bits which can be any of the following:

| \#define V_REPEAT_BIT | 0x00000001 | /* Shape repeats vertically */ |
| :--- | :--- | ---: |
| \#define H_REPEAT_BIT | 0x00000002 | /* Shape repeats horizontally */ |
| \#define BOTTOM_FIXED_BIT | $0 \times 00000004$ | /* Shape's bottom does not grow */ |
| \#define NO_CONTAINER_JMP_BIT | $0 \times 00000010$ | /* Can't jump containers */ |
| \#define MAX_SCROLL_ON_SHAPE | $0 \times 00000020$ | /* Maximum scroll is on shape */ |
| \#define NO_CLIP_PAGE_AREA | 0x00000040 /* Page area does NOT clip text */ |  |
| \#define WINDOW_CURSOR_BIT | 0x00000400 /* Keep cursor in window view */ |  |
| \#define COLOR_VIS_BIT | 0x10000000 /* Page color covers whole vis area */ |  |

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-240.jpg?height=280&width=347&top_left_y=262&top_left_x=745)

Text creates a duplicate shape of itself when text overflows with repeat bits set

V_REPEAT_BIT or H_REPEAT_BIT causes the page_area to "repeat" itself when text overflows the bottom (see "Repeating Shapes" on page 13-243).

# FIGURE \#21 FIXED BOTTOM IS REQUIRED FOR CONTAINERS 

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-240.jpg?height=153&width=268&top_left_y=880&top_left_x=820)

Text does not<br>overflow<br>bottom of<br>shape

BOTTOM_FIXED_BIT—forces the page_area's bottom to remain the same (otherwise, the bottom is considered infinite or "variable" as text grows or shrinks).

NOTE: You want this bit to be set if you are implementing "containers." See "Containers Support” on page 14-1.

NO_CONTAINER_JMP_BIT— causes text to stay within one rectangle of the shape unless a container break character is encountered. (The usual purpose of setting this mode is for "form" documents and other matrix formats in which text can't leave a "cell" unless explicitly tabbed to do so).

MAX_SCROLL_ON_SHAPE— causes OpenPaige to compute the maximum vertical scrolling values by the bottom-most rectangle in the page area (as opposed to the bottom-most text position).

NOTE: This bit should be set for implementing "containers." See "Containers Support" on page 14-1.

NO CLIP_PAGE_AREA — causes text drawing to be clipped to the page area. Normally, text is only clipped to the vis area. If this bit is set, it is clipped to the intersection of vis area and page area.

NOTE: You generally want this bit set for "containers" and/or non-rectangular wrap shapes.

Each attribute is said to be "on" if the bit is set. The default, after $p g N e w$, is all zeros (all clear).

NOTE: If either "repeat" bit is set, BOTTOM_FIXED_BIT attribute is implied (and assumed) even if BOTTOM_FIXED_BIT is clear. This is because a shape cannot "repeat" unless the bottom is unchangeable.

WINDOW_CURSOR_BIT— causes the caret to stay within the vis_area regardless of where the document scrolls.

COLOR_VIS_BIT— informs OpenPaige that the page background color is one and the same as the window's background color. Setting this bit causes all "erased" areas to be painted with the page color. Usually you want to set this bit to avoid "flashing" during scrolling if your window's background is nonwhite and it is the same as the page background color.

## pg doc info fields (continued):

repeat_slop — b(used for repeating shapes only), defines the minimum amount of vertical space, in pixels, remaining at the end of a document before the page shape repeats (new page “appended”).

exclusion_inset_ bdefines the amount to inset each exclusion rectangle for clipping. For example, if exclusion_inset were -1, each exclusion rectangle would be expanded 1 pixel larger before being subtracted from the clipped text display.

caret_width_extra — defines the width of the caret, in pixels. The default is 1 .

scroll_inset—defines the amount to inset the vis_area when scrolling. For example, if scroll_inset were 1, a call to pgScroll() would visually scroll the vis_area minus 1 pixel on all four sides.
minimum_widow — defines the minimum number of lines that can exist at the end of a page, otherwise the paragraph breaks to the next page.

minimum_orphan — pdefines the minimum number of lines that can exist at the beginning of a new page, otherwise the whole paragraph breaks to the new page.

repeat_offset — defines the distance or "gap" to place between repeated shapes (see “Repeating Shapes” on page 13-243).

num_pages — Contains the current "number of pages," which is really the number of times the shape will repeat itself if the whole document was displayed.

NOTE: This is not necessarily the number of physical pages that should be printed!

Repeating shapes can have "blank" pages due to the slop value on a nearly-filled page causing a new repeat. For correct printing, see pgPrintToPage.

ref_con—can contain anything you want.

** Currently, page_origin, print_target and margins fields are not supported but are provided for future enhancements and extensions.

## TECH Continuous document

The example demos had spacing above the document that I couldn't get rid of. I'm interested in some of the "multimedia" features of openPaige, but I want a "streaming" document (no margins/headers/footers spaces). How do I get rid of the spacing so that all of the document is one long stream of data?

By "spacing" I assume you mean the white space between page breaks. We simply chose to implement the demo this way, with the "repeating shape" feature in OpenPaige. Using this implementation, the pages show exactly as they will appear when printed, i.e. with all the paper margins in each side including top/bottom.

Some developers like to implement documents that way, yet some prefer the method that you mention (as one continuous "page"). To do one continuous page, one simply does not implement the "repeating shape,"; non-repeating shapes is actually the default mode. You can examine non-repeating shapes in our "Simple Demo" for Macintosh

For information on page breaks in a continuous document see "Artificial page breaks" on page 13-245.

Relationship of page_area, vis_area and clipping regions clarified

I am having difficulty setting the appropriate attributes to make my document behave in a certain way...[etc.].

It is important to understand the relationship between the vis_area, page area and the various attribute bits in a pg_ref thăt might affect the behavior of both shapes (by "attributebits" is meant the value(s) originally given to the flags parameter for $p g N e w$ and/or new attribute settings given in pgSetAttributes) and/or the attributes set in pg_doc_info.

The most essential difference between a pg_ref's page area versus vis_area is the page_area is the "container" in which text will wrap, while its vis area simply becomes the document's clipping region.

Generally, the vis_area remains constant and unchanging, whereas the page ārea, particularly its bottom, can change dynamically as text is inserted or deleted depending on the attribute flags that are set in the $p g_{2} r e f$. The following is the expected behavior of the page_area when different attribute flags are set in the $p g_{-}^{-} r e f:$

| TABLE \#4 | EXPECTED BEHAVIOUR OF |  |
| :---: | :--- | :--- |
| ATIRIBUTE BITS | page_area ATTRIBUTES |  |
| no bits set (default) | Bottom grows/shrinks dynamically <br> as text is inserted or deleted (see <br> notes below). | All drawing is clipped to intersection of <br> vis_area and the window's current clip <br> region. |
| BOTTOM_FIXED_BIT | Bottom remains constant, never <br> changes regardless of the text con- <br> tent | All drawing is clipped to intersection of <br> vis_area, page_area and the win- <br> dow's current clip region. |
| NO_WINDOW_VIS_BIT | No effect | Same as above except window's clip <br> region is NOT included in the clip <br> region |


| ATIRIBUIE BITS | page_area BEHAVIOUR | CUPPING |
| :--- | :--- | :--- |
| EX_DIMENSION_BIT <br> ("pgNew()" flag) | No effect visually, but the total <br> height of the doc's contents include <br> the exclusion area shape (otherwise <br> the exclusion area is not considered <br> part of the document's dimensions). | No effect visually |
| COLOR_VIS_BIT | No additional effect (but vis_area <br> is erased with the background color) | No effect visually |
| V_REPEAT_BIT | Shape automatically "repeats when <br> text fills to its bottom, achieving <br> multiple page effect. | Each repeating shape intersects with <br> clip region |
| NO_CLIP_REGION | No effect | No clipping is set at all (the application <br> must set the clipping area) |

NOTES: on default behavior (when no attributes have been set):

1. In the default mode, the page area's bottom is said to grow dynamically to enclose the total height of text. In this case, the bottom of the page area originally given to pgNew is essentially ignored; the page area's TOP however is not ignored as that defines the precise top position of the first line of text.

2 When the page area's bottom is said to "grow" dynamically in the default mode, the shape itself does not actually change, rather OpenPaige temporarily pretends its bottom matches the text bottom when it paginates or displays. Although the page_area appears to "grow," any time you might examine the page area shape, its bottom would not be changed from the original dimensions (to get document's bottom, use pgTotalTextHeight instead).

3. NO_WINDOW_VIS_BIT works only in the Macintosh version and has no effect in the Windows version.
(void) pgGetDocInfo (pg_ref pg, pg_doc_ptr doc_info);

(void) pgSetDocInfo (pg_ref pg, pg_doc_ptr doc_info, pg_boolean inval_text, short draw_mode);

To obtain the current document info settings for $p g$, call $p g G e t D o c / n f o$ and a copy of the document info record will be placed in ${ }^{*} d o c$ _info.

To change the document info, call pgSetDocInfo and pass a pointer to the new information. OpenPaige will copy its contents in $p g$.

If inval_text is TRUE, OpenPaige marks all the text in $p g$ as "not paginated," forcing new word-wrap calculations the next time it paginates the document (which will normally be the next time the contents of $p g$ are drawn).

draw_mode can be the values as described in "Draw Modes" on page 2-30:

draw_none, $\quad 1 *$ Do not draw at all */ best_way, /* Use most effecient method(s) */det_apy

/* Directly to screen, overwrite */

direct_or, /* Directly to screen, "OR" */

direct_xor, /* Directly to screen, "XOR" */

bits_copy, $\quad$ /* Copy offscreen */

bits_or, $\quad /$ Copy offscreen in "OR" mode $* /$

bits_xor, $\quad / *$ Copy offscreen in "XOR" mode */

### 13.3 Repeating Shapes

If $\mathrm{V} \_R E P E A T \_B I T$ or $\mathrm{H}_{-} \mathrm{REPEAT}$ _ BIT is set in the attributes field of $p g \_d o c \_i n f o$, the page_area shape will "repeat" itself each time text overflows the bottom. For

V_REPEAT_BIT, the shape repeats itself vertically; for H_REPEAT_BIT the shape repeats itself horizontally (see Figure 15 on page 12-13).

NOTE: Only one of these bits should ever be set at a time (the shape will not repeat both ways).

However, the shape itself does not physically grow. Instead, OpenPaige displays the shape repeatedly down the screen, one "page" at a time. Hence, if you changed page_area to some other shape while one of the "repeat" bits were on, then all repeating shapes will change to the new shape.

The simplest application of the "repeat" bits is to provide a page rectangle (in original page_area), then as the document grows multiple "pages" are added.

Note that the term "page" is used here to describe a logical section of a document: the original shape does not really need to be a "page" rectangle, rather it could be a set of columns or any non-rectangular shape. In any event, the entire shape "repeats" itself each time text fills it up.

For such a feature, if you require a "gap" (page break area), you can do so by setting repeat_offset in $p g \_d o c \_i n f o$ to a nonzero value. This is the amount, in pixels, to add between repeated shapes. Note that repeat_offset is a co_ordinate. This means you can specify both a vertical and horizontal displacement for repeated shapes (a horizontal displacement + vertical displacement would cause a "staircase" effect).

If the shape is to repeat vertically, each occurrence of the shape falls below the last one; for horizontal repeating, each occurrence falls to the right of the last one. The "gap" (repeat_offset), however, is added to the appropriate corresponding sides:

repeat_offset. $v$ always displaces the repeating shape vertically and repeat_offset. $h$ always displaces horizontally, regardless of whether V_REPEAT_BIT or H_REPEAT_BIT is set.

The purpose of the repeat_slop field is to append a repeated shape before text actually fills the entire shape.

For example, many applications prefer a new "page" to become available when text is almost filled to the bottom of the last page. The value you place in repeat_slop is used for this purpose, and is used by OpenPaige as follows: once the bottom of text PLUS repeat_slop is equal to or greater than the shape's bottom, the shape is repeated.

NOTE: Only the text bottom is measured against the shape, not the right or left sides. Even if you set H_REPEAT_BIT, a shape only repeats when text BOTTOM + repeat_slop hits or surpasses the shape's bottom.

For more details on how OpenPaige paginates, see "Advanced Text Placement" on page 37-1.

## Artificial page breaks

I am doing my word processor similar to MS Word. I put in a page break and draw my line dividing the pages. But when I go to print, openPaige draws a single page, the text does not break to the next page.

The only real problem is if a pg_ref has no repeating shapes (or containers), a page break chār has no place to "jump." Well, then during printing - - just before pgPrintToPage -- all one needs to do is set repeating shapes, then print. When printing is done, restore non-repeating shapes. That will cause openPaige to work "correctly."

But when you want to go to page view mode, you should then switch to repeating shapes, just as the above note does for printing. That will help solve your problem.

Note that in a long document, anything exceeding 10 pages, finding a page will require pagination which the user wili probably notice. This is why word includes a Paginate Now menu item in the menu! You will probably want to paginate once and then I believe pgFindPage should work more quickly. If you don't allow OpenPaige to paginate, it can't know which character is the top of the page! OpenPaige (and all other word processors that display WYSIWYG pages) will have to repaginate to find the positions of the first character on the page. OpenPaige currently performs many tricks and second guessing on when and how to calculate those positions.

Please note that OpenPaige cannot make the same assumptions as Word on WHEN to perform those calculations. We must sometimes rely on the developer to know when to perform the pagination.

In Figure 13 on page 12-10, the initial page_area shape contains text which is within the bounds of the shape. Once text overflows the bottom, the shape is repeated and placed at repeat_offset. $v$ pixels down and repeat_offset. $h$ pixels across.
![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-248.jpg?height=834&width=1478&top_left_y=682&top_left_x=224)

The next illustration shows what happens when repeat_stop is nonzero. In this example, repeat_stop value is added to the bottom of the text and, if the result overflows the shape's bottom, the shape is repeated. This provides an 'extra page' to get added before the text completely fills the page shape.

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-249.jpg?height=729&width=1303&top_left_y=114&top_left_x=420)

A repeating shape can actually be any shape and does not need to be a "page" rectangle. The Figure 17 on page 12-15 shows an example of "columns" repeating for each "page."

Initial shape with text

| ![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-249.jpg?height=25&width=109&top_left_y=1448&top_left_x=608) | ![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-249.jpg?height=25&width=109&top_left_y=1448&top_left_x=749) | ![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-249.jpg?height=25&width=113&top_left_y=1448&top_left_x=893) |
| :---: | :---: | :---: |
| ![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-249.jpg?height=25&width=109&top_left_y=1474&top_left_x=608) | $x<x<x<x<x<x<x<x<2$ | ![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-249.jpg?height=25&width=113&top_left_y=1474&top_left_x=893) |
| $x \times x \times x \times x \times x \times x \times x \times x \times x \times x$ | $x \times x \times x \times x \times x \times x \times x \times x \times x \times x$ | ![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-249.jpg?height=25&width=113&top_left_y=1498&top_left_x=893) |
| ![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-249.jpg?height=25&width=109&top_left_y=1518&top_left_x=608) | $x x x<x<x<x<x<x$ | $x<x<x<x<x<x<x$ |
| ![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-249.jpg?height=25&width=109&top_left_y=1542&top_left_x=608) | ![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-249.jpg?height=25&width=109&top_left_y=1542&top_left_x=749) | $8 x x x x x x x>3$ |
| ![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-249.jpg?height=24&width=109&top_left_y=1566&top_left_x=608) | ![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-249.jpg?height=24&width=109&top_left_y=1566&top_left_x=749) |  |
| $x \times x \times x \times x \times x \times x \times x \times x>0$ | ![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-249.jpg?height=25&width=109&top_left_y=1589&top_left_x=749) |  |
| $x x<x<x<x<x<x<x$ | $x<x<x<x<x<x<x<x$ |  |
| ![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-249.jpg?height=25&width=109&top_left_y=1637&top_left_x=608) | ![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-249.jpg?height=25&width=109&top_left_y=1637&top_left_x=749) |  |

Sha pe once text fills

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-249.jpg?height=232&width=127&top_left_y=1434&top_left_x=1142)

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-249.jpg?height=227&width=134&top_left_y=1439&top_left_x=1279)

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-249.jpg?height=225&width=115&top_left_y=1440&top_left_x=1435)

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-249.jpg?height=229&width=412&top_left_y=1679&top_left_x=1143)

### 13.5 Shape Repeat Deletions

Repeated shapes will "delete" themselves if text shrinks above the repeated shape.

For example, if text filled up "page 1" causing a "page 2" to be created, deleting all the text will effectively delete "page 2."

### 13.6 Widows and Orphans

OpenPaige supports "widow and orphan" control, to a certain extent. When lines are computed to flow within repeating shapes (multiple "pages"), whenever pg_doc_info.minimum_widow is nonzero, OpenPaige will force a whole paragraph to the next page if its number of lines on the bottom of the page are equal to or less than minimum_widow.

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-250.jpg?height=48&width=1207&top_left_y=1031&top_left_x=420)
forced to the next page if its number of lines already breaking to the next page are less than or equal to minimum_orphan.

### 13.7 Header \& Footer Support

While OpenPaige does not directly support "headers and footers," a some functions are provided to implement them more easily.

Page modify hook

This "hook" allows an application to temporarily modify the top, left, right and bottom margins of a "page" before pagination occurs. This is useful for header/footer/footnote
support since temporary "exclusion" areas can be tailored for any specific page - pand without actually modifying the exclusion shape itself.

$$
\begin{aligned}
& \text { PG_PASCAL (void) page_modify_proc) (paige_rec_ptr pg, } \\
& \text { long page_num, rectangle_ptr margins); }
\end{aligned}
$$

The above is the prototype for page_modify. This is a general OpenPaige hook (not a style hook). When this gets called, page_num will indicate a page number (the first page is zero) and margins will point to a "rectangle" that represents four margins.

OpenPaige calls this hook for every page that it "paginates." Note that the margins rectangle is not actually rectangle, rather it represents four margin values to add to the top or left, and/or subtract from bottom or right. Normally, these values will be zero (no extra margins); but if you wanted to remove, say, 16 pixels of space from the bottom of the page, you would set margins>bot_right.v to 16 .

Each time this function is called, all four "margins" are cleared to zero (the default). Hence if your function does nothing, the page remains the original size.

This hook is also very useful for alternating "gutters," i.e. extra space on the right side for odd pages and the same extra space on the left side for even pages, etc.

CAUTION: The top and bottom of the page can be modified randomly, i.e. each page can be different. Modifying left and/or right sides, however, must result in the same width for all pages. For example, you should not modify the left or right sides of page 1 but leave page 2's left or right side alone; it is OK to alternate sides as they are modified as long as the distance between left and right edges remain the same.

For additional information, see also the chapter "Customizing OpenPaige" on page 271 .

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-252.jpg?height=107&width=1239&top_left_y=261&top_left_x=434)

The above function is useful for drawing a $p g \_$_ref to any arbitrary location; the text will move (and optionally wrap) to a specified target location regardless of where its "normal" coordinates exist.

PURPOSE: Since most applications that implement headers and footers use $p g \_$refs for a "header" or "footer" this new function exists for header/footer utilities.

If target_pg is not null, the drawing occurs to the graphics device attached to that OpenPaige record; otherwise the drawing occurs to the device attached to pg. Note there are two usual ways to obtain a paige_rec_ptr: the first is from a low-level hook, in which case the paige_rec_ptr is usually one of its parameters. The second way is to do UseMemory(pg_ref) and then UnuseMemory(pg_ref) when you are done using the paige_rec_ptr.

The target_box parameter is a pointer to a rectangle which defines bounding "box" in which to draw the text. This rectangle defines the top-left position of the drawing as well as the clipping region. Text will not rewrap into this shape, rather it repositions its text to align with the box's top-left coordinate, and target_box also becomes the clipping region.

The wrap_rect parameter is a pointer to an optional, temporary "page rect" for the text to wrap. If this is a null pointer, the page_area is used within $p g$ (the source $p g \_r e f$ ).

The draw_mode is identical to all other functions that accept a drawing mode.

OpenPaige supports several other page finding and setting commands. These are closely aligned with printing. These are shown in "Computing Pages" on page 16-13, and “Skipping pages" on page 16-14. Custom page display techniques are described in "Display Proc" on page 16-16

## CONTAINERS SUPPORT

OpenPaige has some built-in support for this purpose by providing several functions to insert, delete and change a list of rectangles that constitute page_area, as well as the ability to attach an application-defined reference to each "container" of the shape.

The term "container" is used to describe a rectangular portion of the page_area. For an application to support page-layout text containers, the typical method is to build the page_area (the shape in a pg_refin which text will flow) with the desired series of rectangles.

### 14.1 Setting Up for "Containers"

By default, a $p g \_r e f$ will not necessarily handle containers the way you might expect without first setting the appropriate values in $p g \_d o c \_i n f o$.

Before using any of the functions below, you should set at least the following bits using pgSetDocInfo:

$$
\begin{aligned}
& \text { BOTTOM_FIXED_BIT } \\
& \text { MAX_SCROLL_ON_SHAPE (optional) }
\end{aligned}
$$

These bits are not set by default, so you should set them soon after $p g N e w$ and before inserting or displaying a $p g \_$ref with "containers". For more information about pgSetDocInfo see “Getting/Setting Document Info” on page 13-9.

Setting up for containers

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-256.jpg?height=396&width=1240&top_left_y=916&top_left_x=433)

The purpose of BOTTOM_FIXED_BIT is to keep the last rectangle from "growing" along with text.

MAX_SCROLL_ON_SHAPE is optional, but will usually be what you want. Normally, OpenPaige will assume the maximum vertical scrolling position is the same as the bottom-most text position. In a "containers" application, however, that is often undesirable since a document can contain many "empty" containers. By setting MAX_SCROLL_ON_SHAPE, OpenPaige will find the bottom-most page area rectangle for computing maximum vertical scrolling.

Number of containers

$$
\text { (pg_short_t) pgNumContainers (pg_ref pg); }
$$

Returns the number of "containers" currently in $p g$. This function actually returns the number of rectangles in the page area. Initially, after $p g N e w$, the answer will be however many rectangles were contained in the initial page_area shape, which will be at least one rectangle.

Inserting containers

(void) pgInsertContainer (pg_ref pg, rectangle_ptr container, pg_short_t position, long ref_con, short draw_mode);

This makes a copy of the rectangle pointed to by container and inserts it into $p g$ 's page_area. Consequently, text will flow within the new shape now including the container rectangle, hence a new "container" is inserted.

Assuming that the current page area shape is a series of rectangles, from 1 to $\mathrm{n}$, the new rectangle is inserted after the rectangle number in the position parameter. However, if position is zero, the new container is inserted at the beginning (becomes first rectangle in the shape). If position is $p g$ NumContainers $(p g)$, it is inserted as the very last rectangle.

You can also "attach" any long-word value (such as a pointer or some other reference) to the new "container"by passing that value in ref_con. Consequently, you can access this value at any time using pgGetContainerRefCon (see "Container refCon” on page 14257).
draw_none,

best_way,

direct_copy,

direct_or,

direct_xor,

bits_copy,

bits_or,

bits_xor
/* Do not draw at all */

/* Use most effecient method(s) */

/* Directly to screen, overwrite */

/* Directly to screen, "OR" */

/* Directly to screen, "XOR" */

$1 *$ Copy offscreen */

/* Copy offscreen in "OR" mode */

/* Copy offscreen in "XOR" mode */

NOTE: The position parameter is not checked for validity! Make sure it is within the boundaries between 0 and $p g$ NumContainers $(p g)$ or a crash can result. (Also note that the other container functions given here require that the range be between 1 and $p g$ NumContainers; only in pgInsertContainer can position be zero).

Getting particular container

void pgGetContainer (pg_ref pg, pg_short_t position, pg_boolean include_scroll, pg_boolean include_scale, rectangle_ptr container);

Returns the "container" rectangle defined by the position parameter. This can be any of the rectangles contained in pg's page area, from 1 to $p g N u m C o n t a i n e r s(p g)$. The rectangle is copied to the structure pointed to by the container parameter.

If include_scroll is TRUE, the container returned will be in its "scrolled" position (as it would appear on the screen). If include_scale is TRUE, the container returned will be scaled to the appropriate dimensions (based on the scaling factor in pg).

Range checking on position is not performed. Make sure it is a valid rectangle number.
(long) pgGetContainerRefCon (pg_ref pg, pg_short_t position);

(void) pgSetContainerRefCon (pg_ref pg, pg_short_t position, long ref_con);

The application-defined reference that is "attached" to container position is returned from pgGetContainerRefCon; you can also set this value using pgSetContainerRefCon.

Range checking on position is not performed. Make sure it is a valid rectangle number.

NOTE: OpenPaige does not know what you have set in ref_con, hence if you have set some kind of memory structure it is your responsibility to dispose of it before $p g$ Dispose.

### 14.3 Changing Containers

(void) pgRemoveContainer (pg_ref pg, pg_short_t position, short draw_mode);

Deletes the rectangle of the page_area given in position. This value must be between 1 (first rectangle) and $p g$ NumContainers( $p g$ ). Range checking on position is not performed.
draw_none,

best_way,

direct_copy,

direct_or,

direct_xor,

bits_copy,

bits_or,

bits_xor
/* Do not draw at all */

$1 *$ Use most effecient method(s) */

/* Directly to screen, overwrite */

/* Directly to screen, "OR" */

/* Directly to screen, "XOR" */

/* Copy offscreen */

$1 *$ Copy offscreen in "OR" mode */

/* Copy offscreen in "XOR" mode *

CAUTION: Warning: Never delete the last (and only) "container." OpenPaige does not check for this situation, and by deleting the only container you will essentially have no area for the text to flow!

CAUTION: If you have set a ref_con value attached to the container to be deleted, it is gone forever after calling this function. It is your responsibility to do whatever is appropriate prior to deleting the container, such as disposing any memory structures involved with the refCon value, etc.

(void) pgReplaceContainer (pg_ref pg, rectangle_ptr container, pg_short_t position, short draw_mode);

Replaces container defined in position with the rectangle given in container. Note that only the rectangle in the page area is replaced, the "refCon" value will remain in tact unless you change it with pgSetContainerRefCon.

This is the function to use to change a container's dimensions, be it dragging, resizing, etc.
draw none, best_way, direct_copy, direct_or, direct_xor, bits_copy, bits_or, bits_xor $1 *$ Do not draw at all * /* Use most effecient method(s) $* /$ /* Directly to screen, overwrite */ /* Directly to screen, "OR" */ /* Directly to screen, "XOR" */ /* Copy offscreen */ $1 *$ Copy offscreen in "OR" mode */ $1 *$ Copy offscreen in "XOR" mode */

(void) pgSwapContainers (pg_ref pg, pg_short_t container1, pg_short_t container2, short draw_mode);

The two containers defined by container1 and container2 "trade places." This function is therefore useful for "bring to front" and "send to back" features.

The associated refCon values for container 1 and container 2 are also reversed, i.e. both rectangles and attached refCons are swapped.

draw_mode can be the values as described in "Draw Modes" on page 2-30:

draw_none,

best way,

direct_copy,

direct or,

direct_xor,

bits_copy,

bits_or,

bits_xor
/* Do not draw at all */

/* Use most effecient method(s) */

/* Directly to screen, overwrite */

/* Directly to screen, "OR" */ /* Directly to screen, "XOR" */

/* Copy offscreen */

/* Copy offscreen in "OR" mode */ /* Copy offscreen in "XOR" mode */

CAUTION: Range checking is not performed. Make sure container1 and container2 area valid rectangle numbers, between 1 and $p g$ NumContainers $(p g)$.

## 14.4 <br> "Clicking" and Character Support

Point within container

(pg_short_t) pgPtInContainer (pg_ref pg, co_ordinate_ptr point, co_ordinate_ptr inset_extra);

Returns the container rectangle containing point, if any.

If inset_extra is non-NULL, every rectangle in the page area is first inset by inset_extra-> $h$ and inset_extra-> $v$ values before it is checked for containing point. Negative inset values expand the rectangle outwards, and positive number shrink the rectangle.

The usual purpose of inset_extra is to detect a certain amount of "slop" when looking for a mouse-click within a container. For example, if you want a container-click detection within four pixels of each container's edges, pass inset_extra as a pointer to a co_ordinate of -4 , -4 .

FUNCTION RESULT: If no container contains point, zero is returned. Otherwise, the container number is returned (which will always be between 1 and pgNumContainers(pg)).

NOTE: Both scrolled position and scaling are taken into consideration by this function. In other words, container rectangles will be checked as they appear on the screen.

$$
\left(p g \_s h o r t \_t\right) \text { pgCharToContainer (pg_ref pg, long offset); }
$$

Returns the container number, from 1 to $p g$ NumContainers $(p g)$, containing the specified text offset. The offset parameter is relative to the start of all text and is a byte offset; it must be between 0 and $p g$ TextSize(pg).

However, offset can also be CURRENT_POSITION (\#defines as -1) which will return the container number for the current insertion point (or the starting selection point if there is a highlight range).

(long) pgContainerToChar(pg_ref pg, pg_short_t position);

Returns the text offset of the first character that exists in container number position. This function is useful to locate the first character within a container.

However, it is possible that the container has no text at all (text is not large enough to fill all containers), in which case the function result will be -1 .

The position parameter must be between 1 and $p g$ NumContainers $(p g)$.

CAUTION: Range checking is not performed!

How expensive is containers support in general, compared to repeating shapes?

Repeating shapes are light-years faster, because they don't really "repeat," at least not physically. All a repeating shape does is repeat its display. If you have, say, a singlerect shape, if it is repeating that shape remains a single rect even if the doc repeats a million times.

Containers, on the other hand, consist of a physical array of rectangles. So that's one big difference -- if a single rect repeats 100 times, the "containers" method will have 100 rectangles; a repeating shape of course has just one.

"Repeating" is fastest because OpenPaige only has to consider one rectangle -- and relative positions thereof. On the other hand, when computing wordwrapping within containers, OpenPaige must continuously walk through ALL rects to see which ones intersect the text line, etc. So the processing is much more extensive in this case.

The general rule is: If your shape, regardless of its complexity, must literally "repeat" in its exact form, then use REPEATING SHAPES. If your shape does not necessarily repeat as-is -- or if the reoccurrence of the shape can be slightly different than the previous occurrence, then you are forced to use containers.

## EXCLUSION AREAS

An OpenPaige "exclusion" area is typically used for page layout features in which text will wrap around one or more rectangles, including complex shapes (which are also a series of small rectangles).

### 15.1 Setting \& Maintaining Exclusions

As in OpenPaige's "container" support in the previous chapter, several functions are provided to insert, delete and change the series of rectangles in the exclude shape of an OpenPaige object.
(pg_short_t) pgNumExclusions (pg_ref pg);

Returns the number of exclusion rectangles currently in $p g$. This function actually returns the number of rectangles in the exclude area. Initially, after $\mathrm{pgNew}$, the answer will be however many rectangles were in your exclude_area shape, if any.

Unlike pgNumContainers, it is possible (and often likely) to have zero exclusion rectangles, so this function can legitimately report zero.

Inserting exclusion

(void) pgInsertExclusion (pg_ref pg, rectangle_ptr exclusion, pg_short_t position, long ref_con, short draw_mode);

This makes a copy of the rectangle pointed to by exclusion and inserts it into $p g$ 's exclude_area. Consequently, text will flow around (will avoid) the new shape now including the exclusion rectangle.

If position is zero, the new exclusion is inserted at the beginning (becomes first rectangle in the shape). If position is $p g$ NumExclusions $(p g)$, it is inserted as the very last rectangle.

It is possible that the current exclusion area in pg is empty or does not exist; for example, you might have passed a null pointer for exclude_area in $p g N e w$. This function will recognize that situation and will work correctly, building an initial exclude area if necessary. However, in this situation the only valid position for insertion is zero.

You can also "attach" any long-word value (such as a pointer or some other reference) to the new exclusion rectangle by passing that value in ref_con. Consequently, you can access this value at any time using pgGetExclusionRefCon (see "Exclusion refCon" on page 15-267).
draw none,

best_way,

direct_copy,

direct_or,

direct_xor,

bits_copy,

bits_or,

bits_xor $1 *$ Do not draw at all */

$1 *$ Use most effecient method(s) */

/* Directly to screen, overwrite */ I* Directly to screen, "OR" */

/* Directly to screen, "XOR" */

/* Copy offscreen *

$1 *$ Copy offscreen in "OR" mode */

$1 *$ Copy offscreen in "XOR" mode */

CAUTION: The position parameter is not checked for validity! Make sure it is within the boundaries between 0 and pgNumExclusions $(p g)$ or a crash can result.

(void) pglnsertExclusionShape (pg_ref pg, pg_short_t position, shape_ref exclude_shape, short draw_mode);

Inserts an entire shape into the exclusion area of $p g$. The list of rectangles within exclude_shape is inserted after rectangle number position; if position is zero, the shape is inserted at the beginning. If position is $p g N u m E x c l u s i o n s(p g)$ the shape is inserted at the end.

If no exclusion area exists in $p g$ prior to this function, the result is essentially the same as

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-267.jpg?height=45&width=717&top_left_y=1637&top_left_x=433)

The contents of exclude_shape are copied, therefore you can dispose exclude_shape any time after calling this function.
draw none,

best_way,

direct_copy,

direct_or,

direct_xor,

bits_copy,

bits_or,

bits_xor
/* Do not draw at all */

I* Use most effecient method(s) */

/* Directly to screen, overwrite */ /* Directly to screen, "OR" */

/* Directly to screen, "XOR" */ /* Copy offscreen *

/* Copy offscreen in "OR" mode */

$/{ }^{*}$ Copy offscreen in "XOR" mode */

NOTE: Associated ref_con values for all new rectangles will be initialized to zero.

### 15.2 Get exclusion information

(void) pgGetExclusion (pg_ref pg, pg_short_t position, pg_boolean include_scroll, pg_boolean include_scale, rectangle_ptr exclusion);

Returns the exclusion rectangle defined by the position parameter. This can be any of the rectangles contained in $p g$ 's exclusion area, from 1 to $p g N u m E x c l u s i o n s(p g)$. The rectangle is copied to the structure pointed to by the exclusion parameter.

If include_scroll is TRUE, the rectangle returned will be in its "scrolled" position (as it would appear on the screen). If include_scale is TRUE, the rectangle returned will be scaled to the appropriate dimensions (based on the scaling factor in $p g$ ).

CAUTION: Warning 1: Range checking on position is not performed. Make sure it is a valid rectangle number.

CAUTION: Warning 2: Unlike "containers," it is possible to have zero exclusion rectangles. You must not call this function if pgNumExclusions $=0$.

### 15.3 Exclusion refCon

(long) pgGetExclusionRefCon (pg_ref pg, pg_short_t position);

(void) pgSetExclusionRefCon (pg_ref pg, pg_short_t position, long ref_con);

The application-defined reference that is "attached" to exclusion rectangle position is returned from pgGetExclusionRefCon; you can also set this value using pgSetExclusionRefCon.

NOTE: Range checking on position is not performed. Make sure it is a valid rectangle number.

CAUTION: Warning: OpenPaige does not know what you have set in ref_con; hence if you have set some kind of memory structure it is your responsibility to dispose of it before pgDispose.

### 15.4 Changing Exclusion Rectangles

Remove exclusion

(void) pgRemoveExclusion (pg_ref pg, pg_short_t position, short draw_mode);

Deletes the rectangle of the exclusion area given in position. This value must be between 1 (first rectangle) and pgNumExclusions(pg). Range checking on position is not performed.

draw_mode can be the values as described in "Draw Modes" on page 2-30:

draw_none, best_way, direct_copy, direct_or, direct_xor, bits_copy, bits_or, bits_xor
/* Do not draw at all */ I* Use most effecient method(s) */ /* Directly to screen, overwrite */ $1 *$ Directly to screen, "OR" */ $/ *$ Directly to screen, "XOR" */ $1 *$ Copy offscreen */

/* Copy offscreen in "OR" mode */ /* Copy offscreen in "XOR" mode */

NOTE: Unlike containers, it is acceptable to delete the last and only exclusion rectangle.

CAUTION: If you have set a ref_con value attached to the exclusion rectangle to be deleted, it is gone forever after calling this function. It is your responsibility to do whatever is appropriate prior to deleting the exclusion, such as disposing any memory structures involved with the refCon value, etc.
(void) pgReplaceExclusion (pg_ref pg, rectangle_ptr exclusion, pg_short_t position, short draw_mode);

Replaces exclusion rectangle defined in position with the rectangle given in exclusion.

NOTE: Only the rectangle in the exclusion area is replaced, the "refCon" value will remain in tact unless you change it with pgSetExclusionRefCon.

This is the function used to change an exclusion rectangle's dimensions, be it dragging, resizing, etc.

draw_mode can be the values as described in "Draw Modes" on page 2-30:

| draw_none, | /* Do not draw at all */ |
| :--- | ---: |
| best_way, | /* Use most effecient method(s) */ |
| direct_copy, | /* Directly to screen, overwrite */ |
| direct_or, | /* Directly to screen, "OR" */ |
| direct_xor, | /* Directly to screen, "XOR" */ |
| bits_copy, | /* Copy offscreen */ |
| bits_or, | /* Copy offscreen in "OR" mode */ |
| bits_xor | /* Copy offscreen in "XOR" mode */ |

## Swap exclusions

(void) pgSwapExclusions (pg_ref pg, pg_short_t exclusion1, pg_short_t exclusion2, short draw_mode);

The two exclusion rectangles defined by exclusion1 and exclusion2 "trade places". This function is therefor useful for "bring to front" and "send to back" features.

The associated refCon values for exclusion1 and exclusion2 are also reversed, i.e. both rectangles and attached refCons are swapped.

draw_mode can be the values as described in "Draw Modes" on page 2-30:

draw_none,

best_way,

direct_copy, direct_or, direct_xor, bits_copy, bits_or, bits xor
/* Do not draw at all */

I* Use most effecient method(s) */ /* Directly to screen, overwrite */ /* Directly to screen, "OR" */

/* Directly to screen, "XOR" */ /* Copy offscreen */

/* Copy offscreen in "OR" mode */

/* Copy offscreen in "XOR" mode */

CAUTION: Range checking is not performed. Make sure exclusion1 and exclusion2 are valid rectangle numbers, between 1 and $p g N u m E x c l u-$ sions(pg).

## 15.5 "Clicking" Exclusion Rectangles

(pg_short_t) pgPtInExclusion (pg_ref pg, co_ordinate_ptr point, co_ordinate_ptr inset_extra);

Returns the exclusion rectangle containing point, if any. It is safe to call this function even if there are no exclusion rectangles (in which case pgPtInExclusion will always return zero).

If inset_extra is non-NULL, every rectangle in the exclusion area is first inset by inset_extra- $>h$ and inset_extra- $>v$ values before it is checked for containing point.

Negative inset values expand the rectangle outwards, and positive numbers shrink the rectangle.

The usual purpose of inset_extra is to detect a certain amount of "slop" when looking for a mouse-click within an exclusion area. For example, if you want a click detection within four pixels of each exclusion's edges, pass inset_extra as a pointer to a co_ordinate of -4, -4.

FUNCTION RESULT: If no exclusion contains point, or if no exclusion area exists, zero is returned. Otherwise, the exclusion rectangle number is returned (which will always be between 1 and $p g$ NumExclusions( $p g)$ ).

NOTE: Both scrolled position and scaling are taken into consideration by this function. In other words, rectangles will be checked as they appear on the screen.

### 15.6 Drawing Exclusion Contents

If your exclusion rectangle(s) contain some type of graphic image you need to draw, the recommended method for doing this is to use the page_proc hook. OpenPaige calls this "hook" function after drawing each page of text; this is explained in the chapter "Customizing OpenPaige".

### 15.7 Attaching Exclusions to Paragraphs

Any exclusion rectangle can be "attached" to the top of a paragraph. First, create the exclusion rectangle, then call the following function:

void pgAttachParExclusion (pg_ref pg, long position, pg_short_t index, short draw_mode)

The exclusion rectangle is represented by index; this is a value from 1 to pgNumExclusions(pg).

The paragraph is represented by the position parameter; this is a text position into the document, and the paragraph to which the exclusion rectangle attaches is the paragraph which contains the position.

NOTE: The text position does not need to be the exact position of a paragraph beginning, rather it can be anywhere within the paragraph (before the carriage return).

The exclusion rectangle, however, "attaches" to the top line of the paragraph regardless of the text position given.

The draw_mode can be any of the standard OpenPaige drawing modes, in which case the document will redraw. Or, draw_mode can be draw_none, in which case no part of it will redraw.

After this function is called, the exclusion rectangle will constantly and dynamically align to the top line of the paragraph even as the text is changed or deleted. If the paragraph is deleted, the exclusion rectangle will still exist but will remain stationary and attached to no paragraph.

Otherwise, the exclusion rectangle is no different than any other exclusion rectangle - text will wrap around the rectangle appropriately, even if that text is part of the paragraph to which the exclusion is attached.

NOTE: Only the vertical position of the exclusion rectangle is aligned to the paragraph; its horizontal position will remain unchanged.

CAUTION: WARNING: Do not attach more than one exclusion rectangle to the same paragraph or unexpected erroneous results will occur.

To determine if an exclusion rectangle is currently attached to a paragraph, call the following function:

long pgGetAttachedPar(pg_ref pg, pg_short_t exclusion);

The exclusion rectangle in question is represented by exclusion; this can be any value from 1 through $p g$ NumExclusions(pg).

This function returns the text position of the paragraph to which the exclusion is attached, if any.

If the exclusion is attached to no paragraph, the function returns -1.

## SCALING, PRINTING \& DisPlAYING

### 16.1 Scaling

An OpenPaige object can be scaled, which is to say enlarged or reduced by a specified amount.

Scaling, however, must be equal for both vertical and horizontal dimensions.

The scaling factor is maintained by OpenPaige using the following record:

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-277.jpg?height=308&width=1166&top_left_y=1504&top_left_x=433)

In the above, the origin field contains the origin point to compute scaling. Generally, this should be the top-left point of your overall page_area (text flow area). The purpose
for the origin value is for OpenPaige to know what scaling is relative to, or stated more simply, what is the top-left point of the entire area that is being scaled.

The scale field is a long whose high-order and low-order words define a numerator and denominator. Stated as a formula, the scale value is computed as:

high word of (scale) / low word of (scale)

Hence, if scale is $0 x 00020001$, scaling is 2 to 1 (2 / 1); if scale is $0 x 00010002$ then scaling is $1 / 2(1 / 2)$, etc.

If the scale value is zero, that is interpreted as no scaling (same as 1/1).

### 16.2 Scaling an OpenPaige Object

(void) pgSetScaling (pg_ref pg, pg_scale_ptr scale_factor, short draw_mode);

This sets the scaling for $p g$. The scale_factor parameter must be a pointer to a $p g \_s c a l e \_f a c t o r$ (above); it cannot be a null pointer. From that moment on, $p g$ will display and edit in the specified scaled amount.

If draw_mode is nonzero, the text is redrawn in the new scale. draw_mode can be the values as described in "Draw Modes" on page 2-30:

draw_none, best_way, direct_copy,

direct_or,

direct_xor,

bits_copy,

bits_or,

bits_xor

$$
\begin{aligned}
& / * \text { Do not draw at all */ } \\
& / * \text { Use most effecient method(s) */ } \\
& \text { /* Directly to screen, overwrite */ } \\
& \text { /* Directly to screen, "OR" */ } \\
& \text { /* Directly to screen, "XOR" */ } \\
& \text { /* Copy offscreen */ } \\
& \text { /* Copy offscreen in "OR" mode */ } \\
& \text { /* Copy offscreen in "XOR" mode */ }
\end{aligned}
$$

NOTE: OpenPaige makes a copy of your scale_factor, so it does not need to remain static.

NOTE (Macintosh): Scaling text may be inaccurate for environments that do not support Color QuickDraw.

To obtain the current scaling factor, call:

(void) pgGetScaling (pg_ref pg, pg_scale_ptr scale_factor);

The scaling factor of $p g$ is returned in the $p g \_s c a l e \_f a c t o r$ pointed to by scale_factor (which cannot be null).

### 16.3 Scaling and Vis Areas

A scaled $p g \_r e f$ normally does not scale its vis_area. If the attribute flag scale_vis_bit has been set in the pg_ref, the vis_area is scaled, otherwise the vis_area remains unscaled.

## FIGURE \#25 WHAT HAPPENS WHEN SCALE_VIS IS SET (OR NOT)

SCALE_VIS_BIT not set (vis area is unchanged):

This is the text

within the

vis_area.
This is the text

This is the tex
within the

No scaling

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-280.jpg?height=160&width=213&top_left_y=752&top_left_x=1008)

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-280.jpg?height=86&width=352&top_left_y=928&top_left_x=759)

vis area.

This is the text

within the

No scaling

$50 \%$ scaling

SCALE_VIS_BIT is usually set when one or more $p g \_$refs are components of a larger document, not the whole document itself. For example, an object-oriented drawing program using OpenPaige to show text objects would probably want to set SCALE_VIS_BIT to achieve the rendering as shown in the bottom part of the above example.

A general word-processor window, however, would probably not want SCALE_VIS_BIT; instead, it may be more desirable to leave the vis_area alone, as shown in the top part of the above example.

### 16.4 Additional Scaling Utilities

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-281.jpg?height=294&width=1105&top_left_y=611&top_left_x=447)

The three functions above will scale a long, a co_ordinate and a rectangle, respectively.

For pgScaleLong, the scale_factor is only the "scale" part of a complete $p g \_s c a l e \_f a c t o r$ and the origin is the appropriate origin position. The value to scale must be pointed to by value and the value will be scaled when the function returns.

For pgScalePt, the $p t$ parameter gets scaled by the scaling factor in scale_factor; for pgScaleRect, the rect parameter is scaled by scale_factor. For both $p g \bar{S} c a l e P t$ and pgScaleRect, amount_offset should be a pointer to the amount the document is "scrolled", or a null pointer if this does not matter.

Usually, amount_offset should be a negative compliment of pgScrollPosition(\&offset), as:

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-282.jpg?height=498&width=1162&top_left_y=267&top_left_x=433)

This function is similar to pgScaleRect except the scaled result of src_rect is placed in target_rect. In addition, src_rect is temporarily offset before scaled by offset_extra amounts (unless offset_extra is a null pointer).

## TECH Scaling a point inside a shape

$I$ was looking through the manual and can't seem to find any function that scales a whole shape. Am I missing something?

You are right, Openpaige does not have a "scale shape" function, probably because it does not need one internally. That is because nothing inside an OpenPaige document is ever really "scaled," it's all an illusion (only the drawing itself is scaled; all shapes and text positions remain the same at all times).

OpenPaige locates a mouse point in a scaled document by "reverse scaling" the mouse point. By that I mean it scales the co_ordinate in question the opposite amount that the document is (apparently) scaled.

It's fairly easy to reverse-scale something. All you do is negate the current scale factor.
scale_factor.scale $=$-scale_factor.scale;

pgScalePt(\&scale_factor, \&point);

This brings up an interesting point. Although I can see that your function to find a point in a scaled shape will work, perhaps a much faster method is to reverse-scale the point instead of scaling the whole shape, then just find the point without the non-scaled shape.

### 16.5 Printing Support

NOTE: IMPORTANT: Do not confuse "pages" in the context of printing with "paging" for repeating shapes. These are two different concepts entirely. Printed pages are simply sections of text; "pages" of repeating shapes are simply repeating display of the same shape, and in fact, might not contain text at all. A printed page and repeating shape page are not necessarily the same dimensions. See "Repeating Shapes" on page 13-9.

"Printing" for OpenPaige is simply an alternate method of displaying its text offset and pinned neatly to a specified "page" rectangle. In itself it knows nothing about printing or printer devices.

The following function is intended to handle most printing requirements:

$$
\begin{aligned}
& \text { (long) pgPrintToPage (pg_ref pg, graf_device_ptr target, long } \\
& \text { starting_position, rectangle_ptr page_rect, short draw_mode); }
\end{aligned}
$$

The target parameter is an optional pointer to a graphics device other than $p g$ 's default device (see “Graphic Devices” on page 3-8).

The starting_position parameter is the text offset, in bytes of the first text that should be printed (the first character of the document is position zero).

The function will draw as much text that can completely fit inside page_rect, starting with starting_position in $p g$, and drawing the first line relative to page_rect's topleft. No text will be drawn that does not completely fit vertically inside page_rect, hence no "half lines" will exist at page_rect's bottom. Horizontal fitting is not checked —bsee note below.

The effective result of this function is that a "page" of text is drawn to some specified device.

The draw_mode parameter should generally be set to best_way, direct_or or bits_emulate_or.

Highlighting and the "caret" is suppressed when the text is drawn by this function and the current vis_area is ignored (it is temporarily replaced with the dimensions of page_rect).

FUNCTION RESULT: The function returns the next text position following the last line printed. If no more text is available (all text fit from starting_offset to end of document), zero is returned.

To print consecutive pages, you would print the first page with starting_offset $=0$, then call pgPrintToPage again with starting_offset = function result, and continue doing so until the function result is zero.

During the time $p g$ PrintToPage is being executed, $p g$ 's attribute flags will have PRINT_MODE_BIT set.

## NOTES:

(1) Text is not automatically rewrapped to page_rect even if page_rect is a different width than $p g$ 's page_area. Whether or not the lines will spill off to the right is also not checked. It is therefore your responsibility to make sure page_rect is wide enough and, if necessary, force $p g$ to rewrap by changing the page_area first.

(2) If you want to print in a "scaled" state, simply set $p g$ to the desired scaling then print the pages.

OpenPaige printing for Windows is similar to Macintosh in the sense that drawing is temporarily redirected to some device other than the application document window.

When calling pgPrintToPage, you need to set up a graf_device (a multipurpose output device for platform-independent drawing) and pass a pointer to that record in the target parameter.

To create the graf_device, use:

(void) pglnitDevice(pg_globals_ptr globals, generic_var the_port, long machine_ref, graf_device_ptr device);

The globals parameter must be a pointer to OpenPaige globals (same structure given to pgInit() and $p g N e w())$. Pass MEM_NULL to the_port. For Windows, this would normally be type HWND but in this case there isn't any window associated with the device.

Pass the Device Context handle in machine_ref. This should be the HDC that you will be "printing" to.

After calling pgInitDevice() you then pass the graf_device structure to pgPrintToPage() as the target parameter.

Once you are completely through using the graf_device, you must call the following:

(void) pgCloseDevice (pg_globals_ptr globals, graf_device_ptr device);

The globals parameter must point to the same OpenPaige globals as before; the device parameters must be a pointer to the graf_device previously initialized with pgInitDevice.

NOTE: $p g$ CloseDevice only disposes memory structures created by OpenPaige the Device Context is not affected.

CAUTION: When creating a graf_device in this fashion, the Device Context given to $p g$ InitDevice must remain valid until you are completely through drawing to the device (and consequently call pgCloseDevice).

## Printer Resolution

If you create a graf_device for printing per the instructions above, you do not need to do anything special with regard to the resolution of the target print device. The pgInitDevice() function will resolve all resolution issues and $p g$ PrintToPage() should render the image correctly.

## Sample to print on the Macintosh

Here is an example of printing a $p g \_r e f$ to a standard Macintosh print driver. Note that you must first create a graf_device for OpenPaige to accept the print driver as the current "port". This example shows how to do that as well:

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-286.jpg?height=642&width=834&top_left_y=1141&top_left_x=434)

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-287.jpg?height=1462&width=1132&top_left_y=216&top_left_x=485)

I noticed that the coordinate system is in pixels. How do we handle coordinates in for high resolution printing? Are we limited to a $1 / 60$ " granularity?

In Windows, OpenPaige handles printing resolution by checking the capabilities of the device context you provide. Assuming you have set up a graf device (per examples shown in the Programmer's Guide), the device resolution is determined and stored within the grafdevice structure. Then when you call pgPrintToPage(), OpenPaige will scale the image to match the printer's resolution.

Usually, you don't need to do anything special for printing the maximum resolution since openPaige handles the difference(s) automatically between the screen and printer.

## TECH

 NOTE
## Scaled Printing

[Using Windows version]. I am trying to print an OpenPaige document scaled to something other than $100 \%$. I do this by setting openpaige scaling but it has no effect, the document always prints $100 \%$

The reason scaling doesn't get reflected when you print is that OpenPaige overrides the scaling you have set to the screen intentionally.

This is because it has to scale everything to match the printer's resolution, hence it temporarily changes the scaling factor.

The work-around is to trick OpenPaige into thinking the printer's resolution is something else. The scaling will reflect the printer's resolution + whatever you want. This is possible to do as long as you change the resolution in the graf_device AFTER you initialize it (see below).

For example, suppose you are printing to a 300 DPI device. Let's say you want to reduce the printed image by $50 \%$. All you do is set the resolution to the grafdevice to $300 / 2,=150$ DPI. In this case, OpenPaige will scale only half the size it should for 300 DPI, which would render your output $50 \%$ reduced.

The printer resolution is in the graf device.resolution field, and this value is a long word whose high/low words are the horizontal and vertical resolutions (dots per inch).

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-289.jpg?height=988&width=1083&top_left_y=331&top_left_x=434)

### 16.6 Computing Pages

(short) pgNumPages (pg_ref pg, rectangle_ptr page_rect);

FUNCTION RESULT: This function returns the number of "pages" that would print with page_rect.

In other words, if page_rect were passed to pgPrintToPage and the whole document were printed, $p g$ NumPages will return how many passes would be made (which implies how many pages would print).

NOTE: This function only works if you have the exact same settings in $p g$ that will exist when pgPrintToPage is called, i.e., scaling factor, page_area size, etc.

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-290.jpg?height=134&width=134&top_left_y=658&top_left_x=230)

CAUTION: Using this function for large documents will consume a lot of time. This is because OpenPaige has to determine the exact number of pages by paginating every line of text according to the page rectangle you have specified.

### 16.7 Skipping pages

(long) pgFindPage (pg_ref pg, short page_num, rectangle_ptr page_rect;

FUNCTION RESULT: This function will return a text offset that you could pass to pgPrintToPage to print page number page_num, assuming a page rectangle page_rect.

In other words, the following question is answered by this function: if you called pgPrintToPage page_num times using page_rect, what text offset would be returned?

In essence, you could use this function to "skip" pages by computing the offset in advance without printing.

When printing in the Windows environment, the printer font(s) do not always match the screen font(s) in terms of character placement and width. Per Microsoft's technical notes, if the application prefers that print quality take precedence over screen quality, yet WYSIWYG character placement is equally important, the recommended method is to render the screen image to match the (eventual) printed page. OpenPaige provides the following function to accomplish this:

pg_boolean pgSetPrintDevice ( $p g \_$ref pg, generic_var device);

Calling this function causes all subsequent wordwrapping and character placement in $p g$ to match the printed result. The device parameter should be the printer HDC you will be using to print the document. Please note that in this case, device is an HDC and not a graf_device.

The appropriate time to call $p g$ SetPrintDevice() is immediately after $p g N e w($ ), and any time after the user has changed the print device or printing attributes.

FUNCTION RESULT:If the new print device is not the same as the previous device within the $p g$, TRUE is returned.

NOTE: It is your responsibility to delete the printer DC. OpenPaige will not delete the DC even if you set it to something else or destroy the $p g \_r e f$.

CAUTION: If you use this feature, the printer DC must remain valid until the $p g \_r e f$ is destroyed, or until another device is set. Or, you can clear the existing DC from the $p g \_r e f$ by passing MEM_NULL for the "device".

## NOTES:

(1) This function changes the way the text appears on the screen, not the printer. Its one and only purpose is to force the screen rendering to match the printing as close as possible.

(2) You should not use this function if the quality of the screen rendering is more important than the printed quality.

generic_var pgGetPrintDevice (pg_ref pg);

This function returns the existing printer DC stored in $p g$, previously set by pgSetPrintDevice(). If no DC exists, MEM_NULL is returned.

### 16.9 Display Proc

(void) pgDrawPageProc (paige_rec_ptr pg, shape_ptr page_shape, pg_short_tr_qty, pg_short_t page_num, co_ordinate_ptr vis_offset, short draw_mode_used, short call_order);

The purpose of $p g$ DrawPageProc is to give the app a chance to draw "page stuff" such as gray outlines around page margins, and/or in the demo's case, it draws outlines around containers. Other ornaments can be drawn such as floating pictures -- the demo uses this function to draw picts that are anchored to document (as opposed to picts embedded into text).

pgDrawPageProc is actually the default procedure for the page_draw_proc, see "draw_page_proc" on page 27-65.

This function only gets called from $p g$ Display and/or from internal display following a ScrollRect. It is not called when typing/display of keyboard inserts. The function is called after all text is drawn and just before returning to the app.

If called from $p g$ Display, the clip region is set to the vis_area of $p g$. If called from pgScroll the clip is set to the scrolled "white" space.

This function can appear more complex than it is when you have irregular page_area shapes and/or repeating shapes. But all that is happening is that pgDrawPageProc gets
called once for each "page" regardless of how many rects are inside your page_area shape.

Assume the simplest case: a $p g \_r e f$ with a single rectangle for page_area and nonrepeating (i.e. one long rectangular document). In this case, $p g D r a w P a g e P r o c$ is called ONCE after drawing text in pgDisplay (see param description below).

Assume the next most simple case, which is a single rect page area but V_REPEAT_BIT set. In this case, $p g$ DrawPageProc gets called for n number of times, where $\mathrm{n}$ is the number of "repeats" that appear in the vis_area.

But here's what might be confusing: $p g$ DrawPageProc always gets called once only if non-repeating shape, and n-times if repeating, regardless of how many "containers" you have (how many rects comprise the page_area shape). If you have multiple rects such as columns or containers, it is up to <your> pgDrawPageProc to do whatever it needs to, say, draw an outline around each rect. The fact that $p g D r a w P r o c$ might get called more than once depends purely on V_REPEAT_BIT being set or not.

If you imagine page_area as being one thing, i.e. a "page," then $p g D r a w P r o c$ makes the most sense.

Stated simply, if you had one huge monitor that showed 3 "pages" (3 occurrences of shape repeat), pgDrawPageProc gets called 3 times. It doesn't matter how complex the shape or how many columns/containers, etc.

The meaning of each parameters in $p g D r a w P r o c($ is as follows:

page_shape_- is a pointer to the FIRST rectangle in the page_area shape. This will literally be a <used> pointer to the page_area's first rect (a shape is a series of rectangles). The rect(s) are UNALTERED (they are unchanged as you set them in the $p g \_r e f$, i.e. the are neither "scrolled" nor scaled).

r_qty—contains \# of rects that page_shape points to.

NOTE: This is NOT how many "repeating occurrences" exist for repeating shape mode, rather how many physical rects page_shape points to. This will always be at least 1. For simple docs, it will be 1 (rect); for a three-column doc it will <probably> be 3, and so on. For example, if your page shape had three rects representing columns, the "column rects" could be accessed as page_shape[0],
page_shape[1] and page_shape[2]. Simply stated, page_shape for r_qty rects represents the unscaled/unscrolled/original page_area of $p g$.

page_num — contains the logical "page" number for which the function is intended, the first page being " 1 ". This is a LOGICAL number, not a physical rect element (r_qty might only be "1" but page_num could be 900). Note that this param makes more sense if you have repeating shapes, otherwise it is always " 1 ". As mentioned above, if the doc has repeating shapes, OpenPaige makes repetitive calls to $p g D r a w P a g e P r o c$ for as many shape-repeats as will fit in the vis_area. Also note that page_num can be literally interpreted as "page number" as it represents the nth repeat of the shape. That also means that if the doc is scrolled to, say, page 100, the first call to pgDrawProc will probably be 100 -- NOT " 1 ". Lastly, note that page_shape points to the SAME rects for every repetitive call to $\mathrm{pgDrawPageProc}$.

vis_offset— the amount you would need to "offset" each rect in page_shape to achieve the correct visual screen position for page number page num. As stated above, page_shape points to unscrolled rect(s). Suppose you wanted to use pgDrawPageProc to draw page margins. Because the doc might be scrolled and/or because the "page" might be the nth repeating of the shape, you can't just do FrameRects -- you need to offset each rect by vis_offset amount. This amount also includes the REPEAT element, i.e. it is supplied to you with extra amounts based on page_num. Hence, all rects in page_shape offset by vis_offset= physical screen locations for page_num.

draw_mode_used is the draw_mode that was used by OpenPaige just before it called pgDrawPageProc. The intended purpose of this is to let an app know if it did a bitmap draw of text. There are < future> cases where an extension will need to know that for optimization. EXAMPLE: Background picts that get drawn directly into the offscreen bitmap along with text -- have already been drawn before $p g D r a w P a g e P r o c$ gets called. Hence, it would be useful for the app to know this so that it would not draw picts unless draw_mode_used was non-bitmap.

call_order — tells you how many times pgDrawPageProc has been called so far in this display loop. For example, if you call $p g$ Display for a doc that has repeating shape, pgDrawProc might get called 2 or 3 times (one for each "page"). The call_order param gives you info regarding this. If zero, it is the first call of several; if nonzero positive, it is the nth call but there will be at least one more; if negative, it is being called for the last time. One thing I use this for is drawing floating picts -- I don't want to draw pictures until $p g$ DrawProc is being called for the LAST time.

## OPENPAIGE IMPORT EXTENSION

## (for "RTF" and other types)

The OpenPaige import extension provides high-level functionality for importing ASCII files, other OpenPaige files and Rich Text Format (RTF) files. Although it is designed as a C++ framework, files can be imported from straight $C$ if necessary.

17.1 Installation

NOTE: IMPORTANT The installation procedure mentions the directory, "pgtxr".

If you are installing the importer for the Macintosh platform and/or for Win16 or Win32 and are not using MFC, simply add the following files from the "pgtxr" directory to your project:

Minimum configuration (import ASCII text only)

pgimport.cpp

pgdeftbl.c

Native OpenPaige file Import (in addition to above)

pgnative.cpp

RTF File Import (in addition to Minimum Configuration)

pgrtfdef.c

pgrtfimp.cpp

If you will be importing files using C++:

\#include "pgtxrcpp.h"

If you will be importing only from straight $\mathrm{C}$ :

\#include "pgtxr.h"

### 17.2 Importing Files (from C++)

CUSTOM CONTROL USERS: There are (intentionally) no control messages that support the OpenPaige Import extension. Use the method shown below, also see"Importing to the OpenPaige Custom Control" on page 17-311.

Loading a file with this extension can be accomplished in a few easy steps:

1. Start with an existing $p g \_r e f$ or OpenPaige control as the target document to receive the import. This may be a, empty document or a document which already has text (in which case the file contents will be inserted at the current insertion point).
2. To import from a disk file, open the file you wish to import. To import from memory, allocate the memory and fill its contents with the appropriate file data. If you are starting with a Macintosh Handle or Windows HGLOBAL you can convert it to a memory_ref by calling HandleToMemory().
3. Create a new object (with "new" keyword) of the appropriate type for the file. (If you aren't sure about what type of file you just opened, see"Determining File Type" on page 17-301 in this document. Currently we support raw text files, RTF and OpenPaige files. The following is an example of creating an appropriate import object:

## \#include "pgTxrCPP.h"

PaigelmportObject filter;

// To import a plain ASCII text file:

filter $=$ new PaigelmportFilter();

// To import an RTF file:

filter $=$ (PaigelmportObject) new PaigeRTFImportFilter();

// To import an OpenPaige file:

filter $=($ PaigelmportObject) new PaigeNativeImportFilter();

4. Call the initialization member function, "pgInit/mportFile()". This function is defined as follows:

pg_error pglnitlmportFile (pg_globals_ptr globals, pg_file_unit fileref, memory_ref memory_unit, file_io_proc read_proc, long first_position, long last_position);

This function prepares for importing a file, setting up whatever is necessary for the file's native format. A file can be imported from a physical file, or from memory. This is differentiated by the value(s) you pass in the function parameters, as follows:
globals - A pointer to your OpenPaige globals. Custom control users: You can get a pointer to the OpenPaige globals as follows: (a) Get the $p g \_r e f$ from the control by sending a PG_GETPGREF message, and (b) Calling the OpenPaige API, "pgGetGlobals()".

fileref - If importing from a disk file, this parameter must be a reference to the opened file (the "refNum" for Macintosh, or a file handle for Windows). If importing from memory, fileref should be zero. Windows MFC users: The fileref parameter must be a "real" HFILE (or NULL if importing from memory), not some other MFC-generated class member that you may assume is a file handle.

memory_unit — If importing from a disk file, this parameter must be MEM_NULL. If importing from memory, this must be a memory_ref(see"The Allocation Mgr" on page 25-1). Importing from memory requires that memory_unit contains the same information, in the same format as it would if it were a disk file.

read_proc-This is an optional I/O function to be used instead of the default low-level reading function. Refer to the OpenPaige Programmer's Guide for information about custom I/O functions. For reading a standard file from disk or memory, pass NULL for this parameter.

first_position, last_position — These two values indicate the beginning and ending file positions to import, respectively. The first_position can be zero or some other byte offset into the file to begin reading. If last_position is unknown (or if you want to read the file whole), pass UNKNOWN_POSITION in for last_position. Otherwise, the file will be imported from byte offset first_position to, but not including the byte at last_position.

FUNCTION RESULT: If this function is successful, zero is returned, otherwise an error code is returned.

5. Call the member function, "pglmportFile()":

pg_error pgImportFile (pg_ref pg, long pg_position, long import_flags, pg_boolean keep_selection, short draw_mode)

This reads the file and inserts its contents into an OpenPaige document.
$p g$ — The target document. Custom control users: obtain the $p g \_r e f$ by sending a PG_GETPGREF message.

$p g \_p o s i t i o n$ - The text position (in the OpenPaige document) to receive the insertion. If this value is CURRENT_POSITION the file will be imported to the current insertion.

import_flags — A set of bits defining which item(s) to import, which can be any or all of the data types shown below. (Note, setting these bits causes that data item to import only if supported by the importer).

| \#define IMPORT_EVERYTHING_FLAG | 0x00FFFFFF | // Import everything |
| :--- | :--- | :---: |
| \#define IMPORT_TEXT_FLAG | 0x00000001 | // Import raw text |
| \#define IMPORT_TEXT_FORMATS_FLAG | $0 \times 00000002$ | // Import text formats |
| \#define IMPORT_PAR_FORMATS_FLAG | 0x00000004 | // Import paragraph formats |
| \#define IMPORT_PAGE_INFO_FLAG | 0x0000000 | // Import page info |
| \#define IMPORT_CONTAINERS_FLAG | $0 \times 00000010$ | // Import container boxes |
| \#define IMPORT_HEADERS_FLAG | $0 \times 00000020$ | // Import headers |
| \#define IMPORT_FOOTERS_FLAG | $0 \times 00000040$ | // Import footers |
| \#define IMPORT_FOOTNOTES_FLAG | $0 \times 00000080$ | // Import footnotes |
| \#define IMPORT_EMBEDDED_OBJECTS_FLAG | 0x00000100 | // Import embedded graphics |
| \#define IMPORT_PAGE_GRAPHICS_FLAG | $0 \times 00000200$ | // Import page pictures |
| \#define IMPORT_STYLESHEETS_FLAG | $0 \times 00000400$ | // Import stylesheets |
|  |  |  |

In addition to the above, setting the following bit causes page dimensions (paper size, margins) to get applied:

\#define APPLY_PAGE_DIMENSIONS \#define IMPORT_CACHE_FLAG 0x02000000

0x04000000
// Apply page size(s) // Page-read the file

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-300.jpg?height=52&width=1280&top_left_y=1605&top_left_x=422)
import information (if such information is supported). For example, when importing an RTF file, setting APPLY_PAGE_DIMENSIONS will apply the page size(s) found in the RTF information. If this bit is not set, the page area remains unchanged.

If IMPORT_CACHE_FLAG is set, the file is opened in "paging" mode, i.e. its text is not read all at once; rather, its text sections are read as needed. This is particularly useful for opening very large files.

NOTE: IMPORT_CACHE_FLAG is only supported for OpenPaige and ASCII text files

(See 2.0b release notes, "Huge File Paging”)

keep_selection — If TRUE, the selection point in the text does not advance, otherwise the selection point in the document advances by the number of bytes that were imported.

draw_mode_-If nonzero, the document is redrawn showing the new data contents. Otherwise nothing is redrawn.

FUNCTION RESULT: If this function is successful, zero is returned, otherwise an error code is returned.

6. Delete the object, or if you want to import another file, repeat steps 4 through 5.

## Import File Example

## \#include "pgTxrCPP.h"

/* This function imports a file into a pg_ref, first creating an object for the appropriate file type. If all is well, the document is re-drawn and NO_ERROR is returned. */

pg_error ImportFile (pg_ref pg, pg_filetype filetype, long feature_flags, long file_begin, pg_file_unit f_ref)

\{

PaigelmportObject filter;

pg_globals_ptr globals;

long flags;

pg_error $\quad$ result $=$ NO_ERROR;

if (!(flags $=$ feature_flags))

flags = IMPORT_EVERYTHING_FLAG;

globals $=$ pgGetGlobals $(\mathrm{pg})$;

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-302.jpg?height=1217&width=1068&top_left_y=202&top_left_x=475)

There might be cases where the file type is unknown and/or you want to verify that a file is truly the type that you expect. There is a function you can call to determine the type:

pg_filetype pgDetermineFileType (pg_file_unit fileref, file_io_proc io_proc, long starting_position)

NOTE: Calling this function examines the appropriate contents of a file looking for a signature recognized by one of the support file import classes. The actual file contents are examined to determine the type.

fileref — An opened file reference (the "refNum" for Macintosh or file handle for Windows).

io_proc — The low-level function to perform I/O. This is described in the OpenPaige Programmer's Guide. Except for implementing very special features, usually you should pass NULL for this parameter.

starting_position — Indicates the file position you will (eventually) begin importing.

This function will always return one of the following types:

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-303.jpg?height=458&width=1029&top_left_y=1531&top_left_x=432)

NOTE: IMPORTANT: An unrecognized file will usually return as "pg_text_type" because a "text" file is considered to be practically anything. For this reason, pgDetermineFileType() will first check for $p g \_r t f \_t y p e$ and $p g \_p a g e \_t y p e$ before deciding it is simply a text file

### 17.4 Determining the Feature Set

You can determine what data type(s) are supported by the importer if you examine object->feature_bits immediately after creating the import object. This member will be initialized to some combination in list shown on the following page:

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-304.jpg?height=544&width=1157&top_left_y=991&top_left_x=433)

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-305.jpg?height=345&width=1171&top_left_y=219&top_left_x=447)

### 17.5 Cross-Mapping Font Tables

The OpenPaige importer extension provides a default mapping table for font names when you import a file generated from another platform. For any font name that is imported, if a match is found in the table then the suggested substitute name is used; if no match is found, the default font name (defined in OpenPaige globals) is used instead. The assumption is that the font name won't exist in the target platform.

You can override the defaults in one of two ways:

1. Substitute your own pointer to a font mapping table (see below). You can substitute your own table after the PaigelmportFilter object is created.

## PaigelmportObject filter;

filter $=$ (PaigelmportObject) new PaigeRTFImportFilter();

filter->font_cross_table $=\left(\right.$ pg_char_ptr)MyOwnFontTable; $^{2}$

2. Override the font mapping member function. The function that maps font substitution can be overridden if you subclass the desired import structure. The font mapping function is declared as:

virtual void pgMapFont (font_info_ptr font, long importing_os, long current_os);

Upon entry, "font" is the font_info pointer in question. The importing_os and current_os define the platform of the importing file and the current (runtime) platform, respectively. These platform definitions will be one of the following:

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-306.jpg?height=158&width=641&top_left_y=1104&top_left_x=435)

To substitute a font, simply change font->name before returning from the function.

CAUTION: The font name, by default, is a pascal string (first byte is its length). If you replace it with a cstring you must set font->environs to NAME_IS_CSTR.

If you want no font mapping at all, set the object's member "font_cross_table" to NULL after creating it.

## Font Table Format

The font mapping table is a table of null-terminated text strings. Each entry (delineated with a null character) is ordered in ascending alphabetical order, the last entry is terminated with \ff (see default tables below). Each entry contains a font name (with possible “*” wildcard) followed by a [bracketed] substitute name.

## EXAMPLE 1:

"WingDings[Zapf Dingbats] $10 "$

If the imported font name is "WingDings" then substitute "Zapf Dingbats".

EXAMPLE 2 (USING "WILDCARD"):

"Times*[Times] $] 0$ "

If imported font's first five characters is "Times" then substitute "Times". (Hence, both “Times New Roman” and “Times Roman” would be subtitled with “Times”).

Importing to Macintosh (and file is from Windows):

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-308.jpg?height=941&width=813&top_left_y=495&top_left_x=434)
static pg_char cross_font_table[] =

\{

"Avant Garde[Arial]10"

"Bookman[Times New Roman]10"

"Chicago[FixedSys] $\backslash 0 "$

"Courier[Courier New]10"

"Geneva[MS Sans Serif] $10 "$

"Helvetica[Arial]10"

"Monaco[Courier New]10"

" $N$ Helvetic*[Arial] $] 0 "$

"New York[MS Serif]10"

"Palatino[Arial] $10 "$

"Symbol[WingDings]10"

"Times[Times New Roman]10"

"Zapf Chancery[Script]10"

"Zapf Dingbats[WingDings]10"

"|ff"

\};

### 17.7 Character Mapping

The importing mechanism will also map ASCII characters $>0 x 7 F$. If you wish to override the defaults you should subclass the import class and override the following function:

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-310.jpg?height=157&width=1158&top_left_y=472&top_left_x=435)

This function gets called after each block of text is imported. Upon entry, chars points to the block of text and num_chars defines the number of bytes. The file_os and current_os define the platform of the importing file and the current (runtime) platform. The possible values for these will be one of the following:

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-310.jpg?height=153&width=644&top_left_y=1003&top_left_x=432)

You can also override the character mapping by substituting your own character mapping table. The character mapping table is a series of unsigned characters, each entry representing consecutive characters from $0 \times 80$ to $0 x F F$.

For example, if the first three bytes being imported were $0 \mathrm{x} 80,0 \mathrm{x} 81$ and $0 \mathrm{x} 82$, the following character mapping table would cause $0 \mathrm{xAA}, 0 \mathrm{xBB}$ and $0 \mathrm{xCC}$ to be inserted into the OpenPaige document:

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-310.jpg?height=203&width=582&top_left_y=1673&top_left_x=434)

An entry in the mapping table of null (zero value character) denotes that the character is not available in the current platform. If so, the unknown_char member in paige_globals is used.

To substitute your own mapping table, first create the import object then change object>character_table.

## EXAMPLE:

PaigelmportObject filter;

filter $=($ PaigelmportObject) new PaigeRTFImportFilter();

filter->character_table $=($ pg_char_ptr)MyOwnCharTable;

### 17.8 Importing from C

CUSTOM CONTROL USERS: There are (intentionally) no control messages that support the OpenPaige Import extension. Use the method shown below, also see"Importing to the OpenPaige Custom Control" on page 17-311. If you need to import a file from a non-C++ environment - or if you want to import a file from a single line of code - you can do so by calling the following function:

pg_error pgImportFileFromC (pg_ref pg, pg_filetype filetype, long feature_flags, long file_begin, pg_file_unit f_ref)

This function imports a file of type filetype into $p g$. The filetype parameter must be one of the following:

pg_text_type, pg_rtf_type, pg_paige_type
// Standard ASCII text // RTF format // Standard OpenPaige file type

The feature_flags parameter indicates which data type(s) you want to import, which can be any of the following bit settings:

|  |  |  |
| :--- | :---: | :---: |
| \#define IMPORT_EVERYTHING_FLAG | 0x00FFFFFF | // Import everything |
| \#define IMPORT_TEXT_FLAG | 0x00000001 | // Import raw text |
| \#define IMPORT_TEXT_FORMATS_FLAG | 0x00000002 | // Import text formats |
| \#define IMPORT_PAR_FORMATS_FLAG | 0x00000004 | // Import paragarph formats |
| \#define IMPORT_PAGE_INFO_FLAG | 0x00000008 | // Import page info |
| \#define IMPORT_CONTAINERS_FLAG | 0x00000010 | // Import container boxes |
| \#define IMPORT_HEADERS_FLAG | 0x00000020 | // Import headers |
| \#define IMPORT_FOOTERS_FLAG | 0x00000040 | // Import footers |
| \#define IMPORT_FOOTNOTES_FLAG | 0x00000080 | // Import footnotes |
| \#define IMPORT_EMBEDDED_OBJECTS_FLAG 0x00000100 | // Import embedded graphics |  |
| \#define IMPORT_PAGE_GRAPHICS_FLAG | 0x00000200 | // Import page pictures |
| \#define IMPORT_PAGE_GRAPHICS_FLAG | Can import page-layout graphics |  |
|  |  |  |

In addition to the above, setting the following bit causes page dimensions (paper size, margins) to get applied:

\#define APPLY_PAGE_DIMENSIONS

\#define IMPORT_CACHE_FLAG 0x02000000

$0 \times 04000000$
// Apply page size(s)

// Can disk-page the file

The file_begin parameter indicates the first file position to begin reading.

The $f$ _ref parameter must be a reference to an opened file ("refNum" for Mac, file handle for Windows).

If this function is successful, the contents of the file are inserted into the current position of $p g$ and the document is redrawn and $N O_{-} E R R O R(0)$ is returned. Otherwise the appropriate error code will be returned.

### 17.9 Importing to the OpenPaige Custom Control

There is no message-based support in the custom control to import a file using the methods shown in this document - the lack of message-based importing is an intentional omission to allow optional compiling of the import classes independent of the control. To import a file into the custom control you simply obtain the $p g \_r e f$ using the PG_GETPGREF message.

However, importing a file into a control can cause an out-of-sync situation with scrollbar positions, pagination, etc., so you should always send the following message immediately after importing a file:

SendMessage(hwnd, PG_REALIZEIMPORT, wParam, 0);

The PG_REALIZEIMPORT message informs the control that you have imported a file and that it should make any adjustments necessary to reflect those changes.

If wParam is TRUE the control repaints itself.

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-314.jpg?height=1029&width=1122&top_left_y=292&top_left_x=434)

NOTE: All of the class definitions are not shown. Only the members of potential interest and usefulness are given. For a complete description of this class, see pgtxrcpp.h.

Member-by-Member Description

font_cross_table — A pointer to the font mapping table. See "Cross-Mapping Font Tables" on page 17-303 in this document.
character_table - A pointer to the character mapping table (for characters $>0$ x7F). See "Character Mapping" on page 17-308 in this document.

pg_error pgVerifySignature()

Called to verify if the file about to be imported contains valid contents for the supported type. For example, pgVerifySignature() for the RTF class checks for the existence of the keyword "Irtf" at the start of the file to verify if it is truly an RTF file or some other format.

If the file is valid, $N O \_E R R O R$ should be returned, otherwise return BAD_TYPE_ERR.

pgPreparelmport()

Called to make any preparations for importing the file. No actual file transfer is performed, but this function should be used to initialize private members to perform the first "read". There are no parameters to this function. The values taken from the application's call to $\mathrm{pgInitImportFile( \text {)willhavebeenplacedintotheappropriate }}$ member values before $p g$ Preparelmport() is called.

## pg_boolean pgReadNextBlock()

Called to import (read) the next block of text. A "block of text" means a block of one or more characters that are rendered in the same consistent format.

For example, if the incoming text contained "Bold_Plain_Italic", the import class must consider "Bold_", "Plain_" and "Italic_" as three separate blocks. The first time pgReadNextBlock() gets called, the text "Bold_"would be returned; the next time "Plain_"is returned, and so forth.

Most of the text and format information must be placed in the "translator" member of the class; this member is a record defined as follows:

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-316.jpg?height=1028&width=1178&top_left_y=224&top_left_x=424)

Imported text bytes are inserted into the translator.data memory_ref(using the appropriate OpenPaige Allocation Manager calls). The byte size returned is assumed to be GetMemorySize(translator.data). Note, to implement special features it is acceptable to return zero bytes for each call. Your function will be called repeatedly until you return FALSE.

For the best examples of pgReadNextBlock() consult the source code files for each import class.

FUNCTION RESULT: If there are no more bytes to transfer, return FALSE. Note that you can return FALSE even if the current function called transferred one or
more bytes, yet end-of-file comes after that position. A result of FALSE indicates that pgReadNextBlock() should not be called again.

$$
\text { pglmportDone() }
$$

Called when importing has completed. This function essentially balances "pgPreparelmport()". Anything you allocated previously in pgPeparelmport() should be disposed.

$$
\begin{gathered}
\text { void PG_FAR * pgProcessEmbedData (memory_ref ref, } \\
\text { long embed_type); }
\end{gathered}
$$

Called when the import class has read data that is intended for an embed_ref. (** For version 1.02b of the import extension, this function only gets called by the RTF importer).

Upon entry, ref contains the data read from the file and embed_type is the type of embed_ref that will be inserted. Note that the data in ref is not an embed_ref, rather it is raw, binary data read from the file. The purpose of pgProcessEmbedData() is to convert that binary data into whatever form necessary to be successfully inserted as an embed_ref.

FUNCTION RESULT: This function must return the appropriate data type for a subsequent creation and insertion of an embed_ref. Note, however, that the class that calls this function assumes that the memory_ref "ref" is either no longer valid, or the same memory_ref is returned as the function result (with its contents altered, for instance).

In other words, the assumption is made that the "ref" parameter has been converted into something else, appropriate for the embed type, and that new data element is returned as the function result.

For example, if the embed_type were embed_meta_file, the appropriate function result might be to create a new HMETAFILE, set the bitstream data from ref into the new metafile HANDLE, dispose the embed_ref and return the new HMETAFILE.

## Default Function

The default function (when using the RTF import class) processes embed_mac_pict and embed_meta_file; if the type is embed_mac_pict, the memory_ref is converted to a Handle and returned as the function result. If the type is embed_meta_file, the contents of the memory_ref are converted to a new HMETAFILE and the memory_ref is disposed.

See source code for the default function in pglmport.cpp.

$$
\text { pgMapFont(), pgMapChars() }
$$

These are called to cross-map fonts and characters between platforms. See"CrossMapping Font Tables" on page 17-303 and "Character Mapping" on page 17-308 (this document) for a detailed description.

There are some lower-level member functions in PaigeRTFImportFilter class that you can override to process unsupported key words:

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-319.jpg?height=500&width=1080&top_left_y=442&top_left_x=434)

ProcessInfoCommand() gets called by the RTF class when a "document information" key word is recognized but not processed. Upon entry, the command parameter will be equivalent to one of the values shown in the table below.

The value in parameter will be the numerical appendage to the keyword, if any. For example, the key word "dy23" would result in a command value of 5 (for "dy" and a parameter value of 23).

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-320.jpg?height=1172&width=263&top_left_y=262&top_left_x=433)

UnsupportedCommand() gets called by the RTF class when a key word is encountered that is not understood. The purpose of this overridable member function is to get access and process RTF tokens that are not normally supported.

Upon entry, command is a null-terminated string that contains the literal command (minus the "" prefix); the value in parameter will be the numerical appendage to the keyword, if any. For example, the key word "bonus99" would result in a command string of "bonus 10 " and a parameter value of 99).

### 17.12 Processing Tables

Since OpenPaige does not support the concept of "tables" directly, importing a table from an RTF file results in a tab-delineated text stream which represents each cell of the table. If your application requires more extensive implementation of tables, there are specific functions in the RTF importing class which you may override to implement them differently.

Table Processing Member Functions

void BeginTablelmport ();

This function is called when a table is recognized in the RTF input stream, but no data has been processed. The purpose of BeginTablelmport() is to prepare whatever structure(s) are necessary to process the table.

NOTE: The RTF class contains a private variable, "doing_table" which must be set to TRUE at this time. Otherwise, the remaining table functions will never be called.

Only table_cell, cell_setright and table_row_end are processed; all other key words are ignored. For table_cell, a tab character is imported; for cell_setright, a paragraph tab position is set; for table_row_end, a carriage return is imported.

The class member "doing_table" is set to TRUE.

pg_boolean ProcessTableCommand (short command, short parameter);

This function is called for all table-type RTF key words. Upon entry, command contains the table key word (below) while parameter contains the appended parameter to the keyword, if any.

For example, the RTF key word "cellx900" indicates a cell's right position, in this case 900 (measured in TWIPS). The command value given to this function would be cell_setright, and parameter would be 900 .

FUNCTION RESULT: A result of "TRUE" implies that the current text and formatting should be inserted into the main document, otherwise the current text and formatting is buffered and the next text and/or key words are read.

The following values are defined in PGRTFDEF.H:

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-323.jpg?height=1181&width=1238&top_left_y=363&top_left_x=436)
pg_boolean InsertTableText ();

This function is called if text (cell contents) is processed while in "table mode." This function will never get called unless doing_table is TRUE and one or more characters other than key words are read.

This function will also never overlap text formats, i.e. InsertTableText() gets called every time the character or paragraph style changes.

Upon entry, all information regarding the text and its format can be found in the "translator" member of the class:

translator.data - A memory_ref contain the text. translator.bytes_transferred — Number of characters in translator.data. translator.format — Current text format (style_info). translator.par_format — Current paragraph format (par_info). translator.font — Current font (font_info).

FUNCTION RESULT: A result of "TRUE" implies that the current text and formatting should be inserted into the main document, otherwise the current text is discarded (and never inserted into the main document).

NOTE: A result of FALSE would be necessary if you are processing the text into a target that is not the main document (such as a graphic picture).

The doing_table member is cleared to FALSE, then TRUE is returned.

pg_boolean EndTableImport();

This function is called when the end of the table is reached. The purpose of EndTablelmport() is to terminate the table.

FUNCTION RESULT: If TRUE if returned, any pending text and formatting will be inserted into the main document, otherwise existing text and formatting will be discarded.

NOTE: This function must clear "doing_table" to FALSE.

## OPENPAIGE EXPORT EXTENSION

(FOR "RTF" AND OTHER TYPES)

The OpenPaige export extension provides high-level functionality for saving files to nonOpenPaige formats. Version 1.03b supports OpenPaige format, ASCII text format and Rich Text Format (RTF). Although the export extension is a C++ framework, it can be called from straight $\mathrm{C}$ programs if necessary.

### 18.1 Installation

NOTE: IMPORTANT: The installation procedure mentions the directory, "pgtxr".

### 18.2 Macintosh and Windows Users

Simply add the following files from the "pgtxr" directory to your project:

Minimum configuration (export ASCII text only):

pgexport.cpp

pgdeftbl.c

Native OpenPaige File Export (in addition to above)

pgnative.cpp

RTF File Export (in addition to Minimum Configuration)

pgrtfdef.c

pgrtfexp.cpp

If you will be exporting files using C++:

\#include "pgtxrcpp.h"

If you will be exporting only from straight $C$ :

\#include "pgtxr.h"

### 18.3 Exporting Files (from C++)

NOTE: "Exporting", in many cases, is synonymous to "Save". We refer to the term "export" only to distinguish it from earlier methods of saving OpenPaige files (such as $p g$ SaveDoc), but from an implementation viewpoint your application can respond to Save and Save As by "exporting" a file.

Exporting a file with this extension can be accomplished in a few easy steps:

1. To export to a disk file, create and open the file you wish to export. To export to memory, allocate an empty memory_ref(using MemoryAlloc).

NOTE: You can discover the recommended file type (Macintosh) or file extension (Windows) by examining the "file_kind" member of the export class — "File Type and Extension" on page 18-334).

2. Create a new object (with "new" keyword) of the appropriate type for the file. Currently we support raw text files, RTF and OpenPaige files. The following is an example of creating an appropriate export object:

## \#include "pgTxrCPP.h"

PaigeExportObject filter;

// To export a plain ASCII text file:

filter $=$ new PaigeExportFilter();

// To export an RTF file:

filter $=($ PaigeExportObject) new PaigeRTFExportFilter();

// To export an OpenPaige file:

filter $=$ (PaigeExportObject) new PaigeNativeExportFilter();

3. Call the initialization member function, "pgInitExportFile()". This function is defined as follows:

pg_error pgInitExportFile (pg_globals_ptr globals, pg_file_unit fileref, memory_ref memory_unit, file_io_proc write_proc, long first_position);

This function prepares for exporting a file, setting up whatever is necessary to write file's native format. A file can be exported to a physical file, or to memory. This is differentiated by the value(s) you pass in the function parameters, as follows:

globals - A pointer to your OpenPaige globals. Custom control users: You can get a pointer to the OpenPaige globals as follows: (a) Get the $p g \_r e f$ from the control by
sending a PG_GETPGREF message, and (b) Calling the OpenPaige API, "pgGetGlobals()".

fileref - If exporting to a disk file, this parameter must be a reference to the opened file (the "refNum" for Macintosh, or a file handle for Windows). If exporting to memory, fileref should be zero. Windows MFC users: The fileref parameter must be a "real" HFILE (or NULL if exporting to memory), not some other MFC-generated class member that you may assume is a file handle.

memory_unit — If exporting to a disk file, this parameter must be MEM_NULL. If exporting to memory, this must be a memory_ref of zero byte size (see "The Allocation Mgr” on page 25-1).

write proc — This is an optional I/O function to be used instead of the default lowlevel writing function. Refer to the OpenPaige Programmer's Guide for information about custom I/O functions. For writing to standard file from disk or memory, pass NULL for this parameter.

first_position — This value indicates the beginning file position to write. The first_position can be zero or some other byte offset into the file to begin writing.

FUNCTION RESULT: If this function is successful, zero is returned, otherwise an error code is returned.

4. Call the member function, "pgExportFile()":

pg_error pgExportFile (pg_ref pg, select_pair_ptr range, long export_flags, pg_boolean selection_only);

This exports the data from a $p g \_r e f$ to the file (or memory_ref) specified in pgInitExportFile(). The parameters follow:

$p g$ - The source document. Custom control users: obtain the $p g \_r e f$ by sending a $P G \_G E T P G R E F$ message.
range - The selection range (in the OpenPaige document) to export. This parameter is ignored, however, if selection_only is FALSE (in which case the whole document is exported). If range is NULL and selection_only is TRUE, only the current selection range is exported. If range is NULL and selection_only is FALSE, the whole document is exported.

export_flags — A set of bits defining which item(s) to export, which can be any or all of the data types shown below.

NOTE: Setting these bits causes that data item to export only if supported by the exporter.

|  |  |  |
| :--- | :---: | :---: |
| \#define EXPORT_EVERYTHING_FLAG | 0x00FFFFFF | // Export everything |
| \#define EXPORT_TEXT_FLAG | 0x00000001 | // Export raw text |
| \#define EXPORT_TEXT_FORMATS_FLAG | 0x00000002 | // Export text formats |
| \#define EXPORT_PAR_FORMATS_FLAG | 0x00000004 | // Export paragraph formats |
| \#define EXPORT_PAGE_INFO_FLAG | $0 x 00000008$ | // Export page info |
| \#define EXPORT_CONTAINERS_FLAG | 0x00000010 | // Export container boxes |
| \#define EXPORT_HEADERS_FLAG | 0x00000020 | // Export headers |
| \#define EXPORT_FOOTERS_FLAG | $0 x 00000040$ | // Export footers |
| \#define EXPORT_FOOTNOTES_FLAG | 0x00000080 | // Export footnotes |
| \#define EXPORT_EMBEDDED_OBJECTS_FLAG | 0x00000100 | // Export embedded graphics |
| \#define EXPORT_PAGE_GRAPHICS_FLAG | 0x00000200 | // Export page pictures |
| \#define EXPORT_STYLESHEETS_FLAG | 0x00000400 | // Export stylesheets |
| \#define EXPORT_UNICODE_FLAG | 0x08000000 | // Write Unicode text |
| \#define INCLUDE_LF_WITH_CR | 0x02000000 | // Add LF with CR |
|  |  |  |

selection_only — If TRUE, the only current selection (or the selection specified in the range parameter) is exported. If range is NULL and selection_only is TRUE, only the current selection range is exported. If range is NULL and selection_only is FALSE, the whole document is exported.

FUNCTION RESULT: If this function is successful, zero is returned, otherwise an error code is returned.

5. Delete the object, or if you want to export another file, repeat steps 3 through 4.

## \#include "pgTxrCPP.h"

/* This function exports a file from a pg_ref, first creating an object for the appropriate file type. If all is well, NO_ERROR is returned. */

pg_error ExportFile (pg_ref pg, pg_filetype filetype, long feature_flags, select_pair_ptr output_range, pg_boolean use_selection, pg_file_unit f_ref)

\{

PaigeExportObject filter;

pg_globals_ptr globals;

long flags, file_begin;

pg_error $\quad$ result $=$ NO_ERROR;

if (!(flags $=$ feature_flags))

flags $=$ EXPORT_EVERYTHING_FLAG;

globals $=$ pgGetGlobals $(\mathrm{pg})$;

switch (filetype)

\{

case pg_text_type:

filter $=$ new PaigeExportFilter();

break;

case pg_rtf_type:

filter $=($ PaigeExportObject $)$ new

PaigeRTFExportFilter();

break;

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-334.jpg?height=1129&width=1099&top_left_y=152&top_left_x=434)

### 18.4 Determining the Feature Set

You can determine what data type(s) are supported by the exporter if you examine object->feature_bits immediately after creating the export object. This member will be initialized to some combination of the following:

| EXPORT_TEXT_FEATURE | Can export raw text |
| :--- | :---: |
| EXPORT_TEXT_FORMATS_FEATURE | Can export stylized text |
| EXPORT_PAR_FORMATS_FEATURE | Can export paragraph formats |
| EXPORT_PAGE_INFO_FEATURE | Can export page dimensions |
| EXPORT_CONTAINERS_FEATURE | Can export containers |
| EXPORT_HEADERS_FEATURE | Can export headers |
| EXPORT_FOOTERS_FEATURE | Can export footers |
| EXPORT_FOOTNOTES_FEATURE | Can export footnotes |
| EXPORT_EMBEDDED_OBJECTS_FEATURE Can export supported embed_refs |  |
| EXPORT_PAGE_GRAPHICS_FEATURE | Can export page-layout graphics |
| EXPORT_UNICODE_FEATURE | Can export UNICODE |
|  |  |

## EXAMPLE:

PaigeExportObject filter;

filter $=($ PaigeExportObject) new PaigeRTFExportFilter();

if (!(filter->feature_bits \&

EXPORT_EMBEDDED_OBJECTS_FEATURE))

AlertUser("Any pictures in document will be lost. Save anyway?");

Resulting File Size

If exporting is successful, the physical end-of-file is set to the first position beyond the last byte written (if writing to a disk file). If exporting to memory, the memory_ref is set to the exact size that was saved.

### 18.5 File Type and Extension

For Windows development, it may be convenient to determine what type of file extension to create (e.g., ".txt”, “.rtf”, etc.); for Macintosh it may also be convenient to determine the default type ("TEXT", "RTF_", etc.). This might become increasingly important in the future if many export classes are developed.

Every export class will place the recommended file type or extension into the following member by its constructor function:

pg_by tefile_kind[KIND_STR_SIZE]; // Recommended filetype

If running in a Windows environment, file_kind will be initialized to the recommended 3-character extension ("TXT", "RTF", etc.). If running in a Macintosh environment, file_kind will get set to the recommended 4-character file type.

### 18.6 Exporting from $\mathbf{C}$

If you need to export a file from a non-C++ environment — or if you want to import a file from a single line of code - you can do so by calling the following function:

pg_error pgExportFileFromC (pg_ref pg, pg_filetype filetype, long feature_flags, long file_begin, select_pair_ptr output_range, pg_boolean use_selection, pg_file_unit f_ref);

This function exports a file of type filetype into $p g$. The filetype parameter must be one of the following:

| pg_text_type, | // Standard ASCII text |
| :--- | :---: |
| pg_rtf_type, | // RTF format |
| pg_paige_type | // Standard OpenPaige file type |
|  |  |

The feature_flags parameter indicates which data type(s) you want to export, which can be any of the following bit settings:

|  |  |  |
| :--- | :--- | :---: |
| \#define EXPORT_EVERYTHING_FLAG | 0x00FFFFFF | // Export everything |
| \#define EXPORT_TEXT_FLAG | 0x00000001 | // Export raw text |
| \#define EXPORT_TEXT_FORMATS_FLAG | $0 x 00000002$ | // Export text formats |
| \#define EXPORT_PAR_FORMATS_FLAG | $0 x 00000004$ | // Export paragraph formats |
| \#define EXPORT_PAGE_INFO_FLAG | $0 x 00000008$ | // Export page info |
| \#define EXPORT_CONTAINERS_FLAG | $0 x 00000010$ | // Export container boxes |
| \#define EXPORT_HEADERS_FLAG | $0 \times 00000020$ | // Export headers |
| \#define EXPORT_FOOTERS_FLAG | $0 x 00000040$ | // Export footers |
| \#define EXPORT_FOOTNOTES_FLAG | $0 x 00000080$ | // Export footnotes |
| \#define EXPORT_EMBEDDED_OBJECTS_FLAG | $0 \times 00000100$ | // Export embedded graphics |
| \#define EXPORT_PAGE_GRAPHICS_FLAG | $0 x 00000200$ | // Export page pictures |
| \#define EXPORT_STYLESHEETS_FLAG | $0 x 00000400$ | // Export stylesheets |
| \#define INCLUDE_LF_WITH_CR | 0x02000000 | // Add LF with CR |

The file_begin parameter indicates the first file position to begin writing.

The output_range and use_selection parameters indicate the range of text to export: if use_selection is FALSE, output_range is ignored and the entire document is exported. If use_selection is TRUE, the selection specified in output_range is specified (or if NULL the current selection in pg is used).

The $f$ _ref parameter must be a reference to an opened file ( "refNum" for Mac, file handle for Windows).

If this function is successful, the contents of the $p g_{\_} r e f$ are written to the file, the endof-file mark is set and NO_ERROR ( 0 ) is returned.

### 18.7 The OpenPaige Export Filter: Overridables

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-338.jpg?height=840&width=1085&top_left_y=292&top_left_x=429)

NOTE: All of the class definitions are not shown. Only the members of potential interest and usefulness are given. For a complete description of this class, see pgtxrcpp.h.

## Member-by-Member Description

file_kind — Contains the recommended file type (Mac) or file extension (Windows). This is initialized by the class constructor.

pgPrepareExport() — Called to make any preparations for exporting the file. No actual file transfer is performed, but this function should be used to initialize private members to perform the first "write". There are no parameters to this function. The

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-338.jpg?height=50&width=1287&top_left_y=1912&top_left_x=421)
the appropriate member values before $p g$ PrepareExport() is called.
pg_boolean pgWriteNextBlock() — Called to export (write) the next block of text. A "block of text" means a block of one or more characters that are rendered in the same consistent format.

For example, if the outgoing text contained "Bold_Plain_Italic", the export class must consider "Bold_", "Plain_" and "Italic_" as three separate blocks. The first time pgWriteNextBlock() gets called, the text "Bold_" would be provided; the next time "Plain_" is provided, and so forth.

The text and format information is placed in the "translator" member of the class; this member is a record as defined in the following example:

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-340.jpg?height=1177&width=1147&top_left_y=221&top_left_x=434)

Text byte(s) are available in translator.data; the byte size can be determined with GetMemorySize(translator.data).

For each consecutive call to pgWriteNextBlock(), if format_changed, par_format_changed or font_changed are TRUE then the text format, paragraph format or font is different than the last $\mathrm{p} g$ WriteNextBlock() call, respectively.

For the best examples of pgReadWriteBlock() consult the source code files for each import class.

FUNCTION RESULT: If TRUE is returned, $p g$ WriteNextBlock() will get called again if there is any more text to export; if FALSE is returned, exporting is aborted.

$$
\text { pgExportDone() }
$$

Called after exporting has completed. This function essentially balances "pgPrepareExport()". Anything you allocated previously in pgPepareExport() should be disposed.

$$
\text { pg_error pgPrepareEmbedData() }
$$

Called to prepare embed_ref data to be exported. The purpose of this function is to make any data conversions necessary to provide a serialized, binary stream of data to be exported.

Upon entry, the refparameter is the embed_ref that is about to be exported. This function needs to return a pointer to byte stream to transfer and the byte count of the byte stream should be stored in *byte_count.

The local_storage parameter is a pointer to a long word; whatever is placed in *local_storage will be returned in pgReleaseEmbedData(), below. The purpose of this parameter is to provide a way for $p g$ PrepareEmbedData() to "remember" certain variables required to un-initialize the embed_refdata (for example, *local_storage might be used to save a HANDLE that gets locked, hence it can be unlocked when pgReleaseEmbedData() is called).

Default Function

The default pgPrepareEmbedData() function processes a Mac picture by locking the PicHandle and returning a de-referenced pointer to the PicHandle contents; if the runtime platform is Windows, a metafile is processed by returning the metafile bits.

$$
\text { pgReleaseEmbedData () }
$$

Called to balance a previous call to pgPrepareEmbedData(). The purpose of this function is to un-initialize anything that was done in pgPrepareEmbedData(), and it gets called after the embed_ref data has been exported.

Upon entry, the ref parameter is the embed_ref, the local_storage parameter will contain whatever value was set in *local_storage during pgPrepareEmbedData().

### 18.8 RTF Export Overridables

The RTF export class is derived from PaigeExportFilter and has some RTF-specific functions that can be overridden as well as data members that may prove useful:

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-342.jpg?height=643&width=1007&top_left_y=1134&top_left_x=432)

This member function is called to export document headers; the default function does nothing (since OpenPaige does not directly support headers). To implement this feature (in terms of export), (see “Custom RTF Output" on page 18-343 this document).

$$
\text { pg_error OutputFooters (); }
$$

This member function is called to export document footers; the default function does nothing (since OpenPaige does not directly support footers). To implement this feature (in terms of export), (see "Custom RTF Output" on page 18-343 this document).

$$
\text { pg_error OutputEmbed(); }
$$

This member function gets called to export an embed_ref. Upon entry, the embed_ref to be exported is available as:

$$
\text { this->translator.format.embed_object; }
$$

The default function handles the "supported" embed_reftypes — embed_mac_pict and embed_meta_file. To implement exporting of other types, you need to override this function and handle the data transfer in some way that is appropriate.

## OutputCustomParams();

This function gets called after all text and paragraph formatting attributes have been exported, but before any text has been exported for each call to pgWriteNextBlock(). The purpose of this function is to output additional formatting information.

For example, OpenPaige 2.0 does not support paragraph borders, but if they were implemented by your application you might want to output border information when appropriate.

The default function does nothing; to write your own RTF data, (see"Custom RTF Output" on page 18-343 this document).

18.9 Lower-level Export Member Functions

To create new or custom export classes, typically you would override PaigeExportFilter (or a subclass of PaigeExportFilter). When you do so, the following lower-level member functions are available to assist you in exporting data to the target file:

void pgWriteByte (pg_char the_byte);

This sends a single byte to the output file.

pgWriteNBytes (pg_char_ptr bytes, long num_bytes);

This sends to the output file; the bytes are taken from the bytes pointer.

void pgWriteDecimal (short decimal_value);

Sends an ASCII representation of decimal_value to the target file. For example, a binary value of -2 would be sent out as (ASCII) "-2". All leading zeros are suppressed (i.e., a value of 1 is sent as " 1 ", not " 000001 ").

void pgWriteHexByte (pg_char the_byte);

Sends a hex representation of the_byte to the target file. For example, a binary value of $0 \mathrm{x} 0 \mathrm{~A}$ would be sent out as (ASCII) " $\mathrm{OA}$ ".

void pgWriteString (pg_char_ptr the_str, pg_char prefix, pg_char suffix);

Sends the contents of the_str (a null-terminated string) to the output file. If prefix is nonzero, that byte is sent first before the contents of the string are sent; if suffix is nonzero, that byte is sent after the contents of the_str is sent.

### 18.10 Custom RTF Output

If you have derived a new class from PaigeRTFExportFilter, the following member functions are available to assist you with exporting custom RTF data:

void WriteCommand (pg_char_ptr rtf_table, short table_entry, short PG_FAR *parameter, pg_boolean delimeter);

WriteCommand will write an RTF token word, followed by an optional parameter value and character delimiter to the output file.

The table parameter should be a null-terminated string containing one or more token word entries, each entry separated by a single space character. The table_entry parameter must indicate which of these elements to write.

## NOTES:

(1) The first element is 1 , not zero.

(2) The "token" entries in this string have no significance to this function; rather, the nth element (table_entry) of the space-delineated table is merely written to the output file.

The token word must not contain any special command character — only ASCII characters less than $0 \mathrm{x} 7 \mathrm{~B}$ should be contained in this string, and the token word must terminate with a space character (the space character is not sent to the output). This function will automatically prefix the token word output with the RTF command character (")").

If parameter is non-NULL, then the value in *parameter is appended to the output as an ASCII numeral. For example, if the token were is "bonus" and *parameter contained a value of 3 , the resulting output would be:

"lbonus3"

If delimiter is TRUE, a single space character is output following the token word; otherwise no extra characters are output.

## EXAMPLE 1

$$
\text { pgWriteCommand((pg_char_ptr) "border I0", 1, NULL, FALSE); }
$$

OUTPUT:

$$
\text { "lborder" }
$$

$$
\begin{aligned}
& \text { short param; } \\
& \text { param = 24; } \\
& \text { pgWriteCommand((pg_char_ptr) "border } 10 ", 1, \text { \&param, TRUE); }
\end{aligned}
$$

## OUTPUT:

"lborder24 "

## EXAMPLE 3

pg_char custom_table[] = $\{$ "comment footer footerl footerf footerr footnote "\};

pgWriteCommand(custom_table, 6, NULL, TRUE);

OUTPUT:

"Ifootnote "

OutputCR (pg_boolean unconditional);

OutputCR outputs a hard carriage return. If unconditional is FALSE, the carriage return is not output unless no carriage returns have been output during the last 128 or
more characters; if unconditional is TRUE the carriage return is output regardless of the previously output characters.

short PointConversion (short value, pg_boolean convert_resolution, pg_boolean $\times 10)$;

PointConversion converts value to points and/or decipoints (a decipoint is a point $\mathrm{X}$ 10). If convert_resolution is TRUE, the value given to this function is converted to points (1/72 of an inch) based on the current screen resolution setting. If x10 is TRUE, the resulting output is multiplied times 10 before being returned as the function result.

Hence, if value is a screen size value (for example, the pixel width of a graphic), passing TRUE for both convert_resolution and x10 would result in a true decipoint conversion.

## PARAGRAPH BORDERS AND SHADING

A "paragraph border" is a frame drawn around one or more paragraphs and is part of the paragraph format (par_info) definition.

Paragraph borders are defined as four potential sides to a frame. Any one side may be drawn or not. Hence, a paragraph border can be defined to show only part of the frame (such as the bottom side), or two sides, or all four sides, etc.

## Setting a Border

Borders are set by changing the "table" structure within the par_info structure, as shown below. Applying the par_info to the desired portion of the text will render the affected paragraphs with that border definition:

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-350.jpg?height=393&width=1082&top_left_y=225&top_left_x=433)

The table member contains information for both tables and paragraph borders:

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-350.jpg?height=445&width=1082&top_left_y=996&top_left_x=434)

NOTE: The pg_table record, generally used for defining table formats, also contains the definition for the paragraph borders, if any. If table.table_columns is zero, border_info is applied to the whole paragraph; if table.table_columns is nonzero then border_info applies to frame of the table.

If par_info.table.border_info is zero, the paragraph has no borders. Otherwise, borders are defined by one or more of the following bit combinations:

| \#define PG_BORDER_LEFT | 0x000000FF | /* Left border */ |
| :--- | :--- | ---: |
| \#define PG_BORDER_RIGHT | 0x0000FF00 | /* Right border */ |
| \#define PG_BORDER_TOP | 0x00FF0000 | /* Top border */ |
| \#define PG_BORDER_BOTTOM | 0xFF000000 | /* Bottom border */ |

Each of the above definitions define 8-bit fields within a long word for each side of a border; which bits you should set in each 8-bit field depends upon the desired border effect.

In other words, the lowest-ordered byte defines the properties of the left border line; the second lowest byte defines the properties of the right border line; the next higher bytes define the properties of the top and bottom lines.

For each of these four 8-bit fields, the following properties can be set:

Lower three bits: define the width of the border line, in pixels. This may be any value between 0 and $0 \times 07$, inclusively. If the value is zero no line is drawn, otherwise a line is drawn 1 to 7 pixels wide.

Upper five bits: define additional characteristics for the line, as follows:

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-351.jpg?height=256&width=1312&top_left_y=1716&top_left_x=242)

The following border definitions are also provided that represent commonly applied borders:

\#define PG_BORDER_ALLGRAY 0x08080808 /* All sides gray */

\#define PG_BORDER_ALLDOTTED 0x10101010 /* All sides dotted */

\#define PG_BORDER_ALLDOUBLE 0x40404040 /* All sides double */

\#define PG_BORDER_ALLSIDES 0x01010101/*All sides 1 pixel */

\#define PG_BORDER_SHADOWBOX 0x21012101 /* Shadowbox all sides */

Some of these definitions need to be combined. For example, to obtain a four-sided double border you would set par_info.table.border_info to:

## PG_BORDER_ALLDOUBLE | PG_BORDER_ALLSIDES

To set a four-sided gray border you would use:

## PG_BORDER_ALLGRAY | PG_BORDER_ALLSIDES

### 19.2 Paragraph Shading

Paragraph shading is an optional color that will fill the background of a paragraph. Usually this shading applies to table formats, yet paragraph shading can be drawn for non-table paragraphs as well.

Shading set by changing the "table" structure within the par_info structure, as shown below. Applying the par_info to the desired portion of the text will render the affected paragraphs with that shading (color) definition:

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-353.jpg?height=398&width=1045&top_left_y=548&top_left_x=434)

The table member contains information for both tables and paragraph borders:

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-353.jpg?height=451&width=1099&top_left_y=1368&top_left_x=434)

NOTE: The $p g$ g table record, generally used for defining table formats, also contains the definition for paragraph shading, if any. If table.table_columns is zero, border_shading is applied to the whole paragraph; if table.table_columns is nonzero then border_shading applies to default background shading of the table.

If border_shading is zero no shading is applied; otherwise, border_shading represents a "red-green-blue" component using the following bitwise fields:

## 0x00BBGGRR

The "BB" bits represent the blue component of the color, the "GG" bits represent the green component and "RR" represents the red component.

NOTE (Windows): These bits are identical to the bits in a COLORREF.

## 20 <br> OPENPAIGE HYPERTEXT LINKS

version 3.0 (FOR INTERNAL USE AND SOURCE CODE USERS ONLY)

### 20.1 General Concept

A "hypertext link" is similar to a character style and can be applied to groups of characters anywhere in the document. However, its attributes are independent to the text and paragraph formats.

OpenPaige maintains two hypertext link runs - a source run and target run.

The hypertext link source run generally contains all the visual links (e.g. displayed in a different color, underlined, and expected to provide some type of response when the user clicks).

The target run generally contains "markers" for the source run links to locate. In actuality, the source and target runs are independent of each other and each run knows nothing about the other. It is therefore the responsibility of the application to provide logical "linking" between them.

Most of the functions and definitions are in pghtext.h; you should therefore add the following to your code:

\#include "pghtext.h"

### 20.3 Contents of a Hypertext Link

Every hypertext link is stored as a record structure containing the following information:

## TEXT RANGE.

The beginning and ending position of the link. This text range is maintained by OpenPaige as the document is changed.

## URL STRING.

Link-specific information represented by a cstring. The OpenPaige API refers to this mostly as the "URL" parameter, but in reality this is simply a string. Both source and target hypertext links each contain their own URL string; it is the application's responsibility to understand and/or parse its contents.

## DISPLAY STYLE(S).

Style(s) that define how the hypertext link should be drawn in various states. These styles are represented by OpenPaige stylesheet ID(s). For each link created there are default stylesheets created; the application can override these defaults, hence displaying the links in any text style that OpenPaige supports.

CLARIFICATION: This document makes reference to a "URL" member of the hypertext link record. The "URL" in this sense is merely a data string and is not
to be confused with a genuine network locator address (although it can be used as such by an application).

### 20.4 Setting New Links

Setting Source Links

long pgSetHyperlinkSource (pg_ref pg, select_pair_ptr selection, pg_char_ptr URL, pg_char_ptr keyword_display, ht_callback callback, long type, long id_num, short state1_style, short state2_style, short state3_style, short draw_mode);

## PURPOSE

Sets a new source hypertext link. A link is "set" by applying the attributes (defined by the other parameters in this function) to one or more characters in the document.

## PARAMETERS

The $p g \_$ref to receive the link.

selection

URL
An optional range of text to apply the link. If selection is NULL, then current selection (highlighting) is used.

An optional string that will get stored with the link. If NULL, no string is stored; otherwise, the URL parameters is considered a cstring of any length. The URL string can be accessed and/or changed later by your application if necessary.

keyword_display An optional string to insert that displays as the key word for the
link. If this is NULL, the characters contained in the selection
parameter become the "key word"; otherwise, the
keyword_display parameter is inserted into the beginning of
the specified selection and the character range of the link
becomes the beginning of that insertion + the length of
keyword_display.
Pointer to a callback function (which you provide) that is called
when the hypertext link is clicked. If callback is NULL the
default callback function is used.
An optional type variable. This value can be used by the
application to distinguish between different types of links.
type An optional unique ID value. This can be used for searching
and connecting links. The typical use for id_num is to set this
value to a number that exists in the same field for a target link.
You can then call pgFindHyperlinkTargetByID().
state1_style through state3_style_- Optional stylesheet IDs that define the
display attributes for three different hypertext links states. If
the parameter is zero the default style is used (see below). The
three "states" are actually arbitrary as the application generally
should control the "state" of a link; at the lowest level, a
"state" is simply the choice of display style to use any given
moment.
The drawing mode to use. If draw_none, nothing redraws.

FUNCTION RESULT

The function returns the id_num value (which will be whatever was passed in id_num).

## COMMENTS

All hypertext links must include at least one character in its selection in order to be valid. In other words you must not apply a hypertext link to an empty selection (where selection begin == selection end). The single exception, however, is when the keyword_display parameter is a valid non-empty cstring. In this case, the "selection" range becomes the current selection's beginning + the length of keyword_display.

State 1

(the initial state when the link is set) — Blue with underline.

State 2 Red with underline.

State 3 Dark gray (no underline).

Setting Target Links

long pgSetHyperlinkTarget (pg_ref pg, select_pair_ptr selection, pg_char_ptr URL, ht_callback callback, long type, long id_num, short display_style, short draw_mode);

## PURPOSE

Sets a new target hypertext link. A link is "set" by applying the attributes (defined by the other parameters in this function) to one or more characters in the document. A target link differs from a source link mainly in the implementation from the application; essentially, both types of links contain the same kind of information.

## PARAMETERS

The $p g \_r e f$ to receive the link.

selection

An optional range of text to apply the link. If selection is NULL, then current selection (highlighting) is used.

URL

An optional string that will get stored with the link. If NULL, no string is stored, otherwise the URL parameters is considered a cstring of any length. The URL string can be accessed and/or changed later by your application if necessary.

Pointer to a callback function (which you provide) that is called when the hypertext link is clicked. If callback is NULL the default callback function is used.

NOTE: for target links you will usually want a NULL callback since clicking on a target link probably requires no special action).

type

An optional type variable. This value can be used by the application to distinguish between different types of links. For example, an index entry (to generate an index listing) would be different than a link to somewhere else in a document. For convenience there are some predefined types:

| \#define HYPERLINK_NORMA | $-0 \times 00000000$ | // Hyperlink normal |
| :---: | :---: | :---: |
| \#define HYPERLINK_INDEX | 0x00000001//Hyperlink is an index |  |
| \#define HYPERLINK TOC | $0 \times 00000002$ | // Hyperlink is TOC |

id_num

display_style

draw_mode

NOTE: : The RTF importer will set HYPERLINK_INDEX and HYPERLINKS_TOC for index and table-of-contents entries wherever appropriate.

An optional unique ID value. This can be used for searching and connecting links. The typical use for id_num is to initialize this value to a unique id_num that can be searched for. If you have created a source link to connect to this target, that same id_num can be placed in the target. Using the function pgFindHyperlinkTargetByID() allows you to find a link by the value in id_num. If the id_num parameter is zero, pgSetHyperlinkTarget initializes the target link id_num to a unique value (which does not exist in any other target link). Optional stylesheet ID that defines the display attributes of the link. If the parameter is zero the default style is used (see below).

The drawing mode to use. If draw_none, nothing redraws.

The function returns the id_num value (which will be the unique number chosen for the target link if the id_num parameter was zero, or the value in id_num if it was nonzero).

NOTE: unlike setting a source link, setting a target link automatically assigns a unique "ID" value if id_num is zero. You can find this link in the document using pgFindHyperlinkTargetByID().

## COMMENTS

All hypertext links must include at least one character in its selection in order to be valid. In other words, you must NOT apply a hypertext link to an empty selection (where selection begin == selection end).

## DEFAULT DISPLAY

Target links display with a yellow background color. You can turn this display off by setting the following attribute with pgSetAttributes2():

\#define HIDE_HT_TARGETS

This value must be set with pgSetAttributes2() (note the "2").

## EXAMPLE:

To turn off the default display so target links display in their own native style(s) you would do the following:

long flags;

flags $=$ pgGetAttributes2(pgRef);

flags |= HIDE_HT_TARGETS;

pgSetAttributes2(pgRef);

PG_PASCAL (void) ht_callback (paige_rec_ptr pg, pg_hyperlink_ptr hypertext, short command, short modifiers, long position, pg_char_ptr URL);

This is the function that gets called for various events (usually when a link is clicked). You need to provide a pointer to your own function that handles these events.

## PARAMETERS

$p g$

hypertext The internal hypertext link record (see structure below).

command The value defining the event (see table below).

modifiers The state of the mouse (where applicable). These will be set to the appropriate bits. For example if modifiers contained EXTEND_MOD_BIT then the application has performed a shift-click. This can be important to determine the nature of a mouse click within a link; typically you may not want to "jump" to a link if the user is performing a shift-click or control-click, etc.

position

URL
The text position of the link (relative to the beginning of the document).

The "URL" string contained in the link. This will contain the character string given to the URL parameter when the link was created (or the string that was set using other function calls).

NOTE: the URL parameter will never be NULL; if you created the link with a NULL pointer for URL, the parameter at this time will be an empty cstring.

COMMENTS

Do not try to use the URL data from the "hyperlink" parameter; use the URL parameter instead.

The hyperlink parameter points to a copy of the original record; it is therefore safe to alter (and even delete) the original (via the proper function calls) even from within this hook.

When responding to a hypertext link event you should call the default source callback function if you want the link display to change states. This function is called pgStandardSourceCallback(); when you do so, the link that has been clicked will change its display to state 2 and all other links in the document will change to state 1.

## EXAMPLE:

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-363.jpg?height=889&width=1143&top_left_y=683&top_left_x=434)
hyperlink_doubleclick_verb Called if link is double-clicked

hyperlink_mouseup_verb Called when mouse is up

hyperlink_delete_verb_Called when link gets deleted

### 20.6 Hyperlink Record Struct

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-364.jpg?height=737&width=1177&top_left_y=776&top_left_x=434)

The applied_range member contains the current text positions for the beginning and ending of the link. The URL member contains the URL string if it is < FONT_SIZE - 1. Otherwise, the string is inside alt_URL. The unique_id, type and refcon members are optional values that can be used by the application for locating specific links.

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-365.jpg?height=356&width=1233&top_left_y=511&top_left_x=435)

long pgFindHyperlinkSource (pg_ref pg, long starting_position, long PG_FAR *end_position, pg_char_ptr URL, pg_boolean partial_find_ok, pg_boolean case_insensitive, pg_boolean scroll_to)

These functions can be used to perform a "search" that locates a specific link based on its URL string value. The $p g$ FindHyperlinkTarget function searches for a target link while pgFindHyperlinkSource searches for a source link.

## PARAMETERS

starting_position The text position to begin the search, the first character in the document is position zero.

end_position Optional pointer to a long word. If this is nonnull, the long word gets initialized to the text position following the link if found (the *end_position value remains unchanged if a match is not found).

URL The string to search for (cstring).

partial_find_ok If TRUE, a match is considered valid if URL matches only the first part of the link's URL. For example, if searching for "Book" a match will be made on "Book1" and "Book2", etc. case_insensitive If TRUE then the comparison is not case-sensitive. scroll_to If TRUE, the document is scrolled to the found location.

If the link is found, the function returns the text position where the link begins (and if *end_offset is nonnull it gets set to the link's ending position). If no match is found, the function returns -1 .

Finding by "ID" Number

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-366.jpg?height=248&width=1233&top_left_y=624&top_left_x=433)

These functions can be used to perform a "search" that locates a specific link based on its value in id_num. The link's id_num is usually set when you set the original source or target link. The pgFindHyperlinkTargetByID function searches for a target link while pgFindHyperlinkSourceByID searches for a source link.

## PARAMETERS

starting_position The text position to begin the search, the first character in the document is position zero.

end_position Optional pointer to a long word. If this is nonnull, the long word gets initialized to the text position following the link if found (the *end_position value remains unchanged if a match is not found).

id_num The value being searched for. The id_num member in the link is compared to the id_num parameter passed to this function. The link's id_num is usually set when you set the original source or target link.

scroll_to If TRUE, the document is scrolled to the found location.

If the link is found, the function returns the text position where the link begins (and if *end_offset is nonnull it gets set to the link's ending position). If no match is found, the function returns -1 .

### 20.8 Changing Existing Links

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-367.jpg?height=544&width=1093&top_left_y=720&top_left_x=446)

void pgChangeHyperlinkTarget ( $\mathrm{pg \_}$ ref pg, long position, select_pair_ptr selection, pg_char_ptr URL, ht_callback callback, short display_style, short draw_mode);

These two functions are used to change the attributes of an existing hypertext link; pgChangeHyperlinkSource() changes a source link and pgChangeHyperlinkTarget() changes a target link.

All parameters are completely identical to $\mathrm{pgSetHyperlinkTarget()} \mathrm{and}$ pgSetHyperlinkSource() except for the additional "position" parameter — this specifies where the link is located, i.e. its character position in the text. (There are several ways to find the character position, not the least of which is simply getting the selection range from the $p g \_r e f$, assuming it is within a link). See also the various utility functions that return a text position of a link.

For each parameter that is nonzero, that value is changed to the value specified; otherwise, the current corresponding value remains unchanged.

For example, a nonnull URL parameter changes the URL string, while a null pointer leaves the existing string unchanged.

### 20.9 Detecting Mouse Points

long pgPtInHyperlinkSource(pg_ref pg, co_ordinate_ptr point);

long pgPtInHyperlinkTarget(pg_ref pg, co_ordinate_ptr point);

These two functions are used to detect which link, if any, contain a point. Use pgPtInHyperlinkSource for detecting a point in a source link and pgPtInHyperlinkTarget for detecting one in a target link.

The point parameter is a point in screen coordinates (NOT scrolled and NOT scaled).

## FUNCTION RESULT

If a link contains the point, its beginning text position is returned. If no link contains a point, point -1 is returned.

### 20.10 Changing Display State

void pgSetHyperlinkTargetState (pg_ref pg, long position, short state, pg_boolean redraw);

void pgSetHyperlinkSourceState (pg_ref pg, long position, short state, pg_boolean redraw);

These functions can be used to change the display state of a link; pgSetHyperlinkTargetState changes the display state of target links and pgSetHyperlinkSourceState changes the display state of source links.

## PARAMETERS

position

The text position of the link. Or, if position is -1 the state is applied to all the links of this type (i.e. all target links or all source links). For example, to force all source links to state 0 you could call pgSetHyperlinkSourceState(pg, -1, 0, TRUE).

state

One of three states ( 0,1 or 2 ). The "state" simply defines which of the three possible styles to display the link.

redraw If TRUE the link(s) redraw their new state.

### 20.11 File I/O

There is no special function you need to call to read or write OpenPaige hypertext links. However, after reading or importing a file with possible links you must reinitialize your callback function pointers, if any:
void pgSetHyperlinkCallback (pg_ref pg, ht_callback source_callback, ht_callback target_callback);

This function walks through all existing links, sets the callback function in the source links to source_callback and the callback function in target links to target_callback. Either function can be null, in which case the default callback is used.

### 20.12 Removing Links

void pgDeleteHyperlinkSource (pg_ref pg, long position, pg_boolean redraw);

void pgDeleteHyperlinkTarget (pg_ref pg, long position, pg_boolean redraw);

These functions remove a source link or target link, respectively.

## PARAMETERS

position indicates which link to remove; this must be a text position that exists somewhere within the link.

redraw If TRUE the document is redrawn showing the change.

NOTE: Only the applied link and its displayed styles, etc. are removed; the text itself as it exists in the document is not changed. For example, if the word "Book" existed in the document and had a target hypertext link applied to it, removing the link simply means there is no longer any associated link to this word yet the word "Book" remains in the text, drawn in its normal (non-link) style.
pg_boolean pgGetSourceURL (pg_ref pg, long position, pg_char_ptr URL, short max_size);

pg_boolean pgGetTargetURL (pg_ref pg, long position, pg_char_ptr URL, short max_size);

These functions return the contents of the URL string from a specific source or target link, respectively.

## PARAMETERS

position

URL

max_size
The text position of the link.

Pointer to a character buffer (to receive the string).

The maximum number of characters that can be received in the buffer, including the null terminator of the cstring.

## FUNCTION RESULT

If there is no link found at the specified text position, FALSE is returned (and no characters are copied into URL). Otherwise the string is set at URL (and truncated, if necessary, if the string size > max_size).

long pgGetSourcelD (pg_ref pg, long position);

long pgGetTargetID (pg_ref pg, long position);

These functions return the unique "ID" value in a specific source or target link, respectively.
position The text position of the link.

## FUNCTION RESULT

The unique ID value, if any, belonging to the specified link is returned.

NOTE: a value of zero is returned if the link's id_num member is zero or if there is not a link associated to the specified position.

pg_boolean pgGetHyperlinkSourcelnfo (pg_ref pg, long position, pg_boolean closest_one, pg_hyperlink_ptr hyperlink);

pg_boolean pgGetHyperlinkTargetlnfo (pg_ref pg, long position, pg_boolean closest_one, pg_hyperlink_ptr hyperlink);

These two functions return the actual hyperlink record for a specific source or target link, respectively.

## PARAMETERS

position

closest_one

hyperlink
The text position of the link.

If FALSE, the link must be found at the specified position, otherwise the link is found nearest to, or to the right of the specified position.

Pointer to a pg_hypertext record. If the link is found the record is copied to this structure.

FALSE is returned if no link is found at the specified position (or no link is found between the position and end of document when closest_one is TRUE).

void pglnitDefaultSource (pg_ref pg, pg_hyperlink_ptr link);

void pglnitDefaultTarget (pg_ref pg, pg_hyperlink_ptr link);

These functions initialize a hypertext record to the defaults. Usually you won't need to call this function. It is mainly used for building hypertext links while importing files.

short pgNewHyperlinkStyle (pg_ref pg, pg_short_t red, pg_short_t green, pg_short_t blue, long stylebits, pg_boolean background);

This function creates a stylesheet that can be subsequently passed to a function that sets a new hypertext link.

## PARAMETERS

red, green, blue define the R-G-B components of a color ("black" is the result of red, green and blue all zeros). This color is applied to the text if the background parameter is FALSE; otherwise, the color is applied to the text background.

stylebits

Defines optional style(s) to apply to the text. This is a set of bits which can be a combination of the following:

|  |  |  |
| :--- | :--- | :--- |
| \#define | X_PLAIN_TEXT | 0x00000000 |
| \#define | X_BOLD_BIT | $0 \times 00000001$ |
| \#define | X_ITALIC_BIT | $0 \times 00000002$ |
| \#define | X_UNDERLINE_BIT | $0 \times 00000004$ |
| \#define | X_OUTLINE_BIT | $0 \times 00000008$ |
| \#define | X_SHADOW_BIT | $0 \times 00000010$ |
| \#define | X_CONDENSE_BIT | $0 \times 00000020$ |
| \#define | X_EXTEND_BIT | $0 \times 00000040$ |
| \#define | X_DBL_UNDERLINE_BIT | $0 \times 00000080$ |
| \#define | X_WORD_UNDERLINE_BIT | $0 \times 00000100$ |
| \#define | X_DOTTED_UNDERLINE_BIT $0 \times 00000200$ |  |
| \#define | X_HIDDEN_TEXT_BIT | $0 \times 00000400$ |
| \#define | X_STRIKEOUT_BIT | 0x00000800 |
| \#define | X_ALL_CAPS_BIT | 0x00008000 |
| \#define | X_ALL_LOWER_BIT | 0x00010000 |
| \#define | X_SMALL_CAPS_BIT | 0x00020000 |
|  |  |  |

background

If TRUE then the color is applied to the text background, otherwise the color is applied to the text.

## FUNCTION RESULT

A new stylesheet ID is returned. If the exact stylesheet already exists its ID is returned instead (hence, you will not create duplicate styles). This stylesheet ID can be given to the function(s) that set new hypertext links.

void pgScrollToLink (pg_ref pg, long text_position);

This function causes the document to scroll to the specified text position.

NOTE: the text position does not necessarily contain a link; rather, this is a convenience function that forces the document to scroll to the location specified.

## TABLES AND BORDERS

A table is tab-delineated text formatted as rows and columns of "cells." The formatting information itself is paragraph-based, while the text itself is internally maintains each cell as tab or CR-delineated text and each row is delineated by a CR.

At a very low level, table attributes are applied with pgSetParInfo(). Higher level functions, described in this document, provide methods to insert new tables and format existing ones.

Table attributes are part of par_info.table represented by the following record:

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-376.jpg?height=649&width=1175&top_left_y=381&top_left_x=420)

## PARAMETERS

table_columns Number of columns in the table. If this is zero then the paragraph is not a table.

table_column_width The default width for each cell. If this is zero, cell widths are determined dynamically according to the width of the paragraph. For example, if the width of the paragraph after subtracting paragraph indents is 6 inches, a 6-column table will render 1 " cells.

NOTE: individual column widths can be altered after a table is inserted.

table_cell_height The default height for a row. This is the minimum height for all rows. If zero, the height is determined by the height(s) of the text within the row.

border_info _ Paragraph border information. If table_columns is zero, border_info defines the surrounding paragraph border lines (see section on Paragraph Borders).
border_spacing

border_shading The background color for the paragraph or table. If this is zero the normal window color is used. Otherwise, this is a 24-bit representation of an RGB value (see RGB Values below).

cell_borders The default border line(s) around each cell. This differs from border_info since it applies only to cells within a table.

grid_borders

unique_id

cell_h_extra

The amount of extra spacing between border line(s) and the text, in pixels. This value is applied to paragraph borders. The default border line(s) to display around cells if no other borders are present. These cell borders are not drawn when the document is printed; they apply only to table cells.

USED INTERNALLY. The table_id is used to maintain unique paragraph records; DO NOT alter this value.

Extra space, in pixels, between cells.

### 21.3 RGB Values

Border and cell shading is represented by the following bitwise settings in a long word:

## 0x00BBGGRR

The "BB" bits represent the blue component of the color, the "GG" bits represent the green component and " $\mathrm{RR}$ " represents the red component.

NOTE (Windows): These bits are identical to the bits in a COLORREF.

### 21.4 Table Functions

NOTE: : These functions are defined in pgTable. $h$.

Inserting New

void pgInsertTable (pg_ref pg, long position, pg_table_ptr table, long row_qty, short draw_mode);

Inserts a new table beginning at the text position specified. The position parameter can be CURRENT_POSITION.

## PARAMETERS

table is a pointer to a $p g \_t a b l e$ record defining all the table attributes.

row_qty indicates the desired number of rows. If this is zero at least one row is inserted.

draw_mode causes the text to redraw if nonzero.

NOTE: Since tables are a paragraph format, this function may insert carriage return(s) before and after the specified position to clearly delineate the format run.
void pgSetColumnWidth (pg_ref pg, long position, short column_num, short width, short draw_mode);

Changes the width of a specific column in a table.

## PARAMETERS

position indicates the text position of the table, which can also be CURRENT_POSITION. This value can be the position of any character within the table (i.e. it does not need to be the very beginning of the table or a cell). If the specified position is no part of a table, this function does nothing.

column_num The column defined by column_num gets set to the value in width; the first column is zero.

void pgSetColumnBorders (pg_ref pg, long position, short column_num, long border_info, short draw_mode);

Changes the cell border line(s) of a specific column in a table.

## PARAMETER

position

indicates the text position of the table, which can also be CURRENT_POSITION. This value can be the position of any character within the table (i.e. it does not need to be the very beginning of the table or a cell). If the specified position is no part of a table, this function does nothing.

column_num The column defined by column_num changes its cell borders to border_info; the first column is zero.
void pgSetColumnShading (pg_ref pg, long position, short column_num, long shading, short draw_mode);

Changes the cell shading (background color) of a specific column in a table.

## PARAMETERS

position indicates the text position of the table, which can also be CURRENT_POSITION. This value can be the position of any character within the table (i.e. it does not need to be the very beginning of the table or a cell). If the specified position is no part of a table, this function does nothing.

column_num The column defined by column_num changes its background color to shading; the first column is zero.

void pgSetColumnAlignment (pg_ref pg, long position, short column_num, short alignment, short draw_mode);

Changes the cell text alignment ("justification") of a specific column in a table.

## PARAMETERS

position

indicates the text position of the table, which can also be CURRENT_POSITION. This value can be the position of any character within the table (i.e. it does not need to be the very beginning of the table or a cell). If the specified position is no part of a table, this function does nothing.

column_num

The column defined by column_num changes its alignment to the value specified; the first column is zero. Alignment values are the same as paragraph justification values (justify_left, justify_center, justify_right, justify_full).
pg_boolean pglsTable ( $p g \_$ref $\mathrm{pg}$, long position);

Returns TRUE if the specified position is within any part of a table.

## PARAMETERS

position can be CURRENT_POSITION.

pg_boolean pgPtlnTable (pg_ref pg, co_ordinate_ptr point, pg_boolean non_focus_only, select_pair_ptr offsets);

Returns TRUE if the specified point is anywhere within a table.

## PARAMETERS

non_focus_only

value is ignored; pass FALSE for compatibility.

offsets if non-NULL and the point is within a table, offsets->begin and offsets->end gets set to the beginning and ending text position for the whole table. (If the point was not within any table, offsets is unchanged).
memory_ref pgTableColumnWidths (pg_ref pg, long position);

Returns a memory_ref containing the width(s) for each column. The memory size of the reference will be equal to the number of columns in the table.

## PARAMETERS

position indicates the text position of the table, which can also be CURRENT_POSITION. This value can be the position of any character within the table (i.e. it does not need to be the very beginning of the table or a cell). If the specified position is no part of a table, this function returns MEM_NULL.

void pgCellOffsets (pg_ref pg, long position, select_pair_ptr offsets);

Returns the text positions of the text contents of a specific cell.

## PARAMETERS

position indicates the text position of the table, which can also be CURRENT_POSITION. This value can be the position of any character within the table. If the specified position is no part of a table, this function does nothing; otherwise, offsets->begin and offsets->end will return with the beginning and ending of the contents of the cell containing the original position.

NOTE: If the cell contents are empty, offsets->begin will be the same as offsets->end.
void pgTableOffsets (pg_ref pg, long position, select_pair_ptr offsets);

Returns the text positions for the beginning and ending of the whole table.

## PARAMETERS

position indicates the text position of the table, which can also be CURRENT_POSITION. This value can be the position of any character within the table. If the specified position is no part of a table, this function does nothing; otherwise, offsets->begin and offsets->end will return with the beginning and ending of the whole table.

Inserting / Deleting Rows and Columns

void pgInsertColumn (pg_ref pg, long position, short column_num, tab_stop_ptr info, short draw_mode);

Inserts a new, empty column into the table that contains the text position specified.

## PARAMETERS

position

can be CURRENT_POSITION; if it is contained in no table this function does nothing.

column_num The new column is inserted before column_num (first column is zero). To append a column to the far right side of the table, column_num should be equal to the current number of columns (see pgNumColumns).
tab_stop.position

tab_stop.type
Column width, in pixels. (Zero = automatic widths). Justification. Set to left_tab, center_tab, etc. Default cell borders.

Default background shading (zero for none).

void pgInsertRow (pg_ref pg, long position, long row_num, short draw_mode);

Inserts a new, empty row into the table that contains the text position specified.

## PARAMETERS

| position | can be CURRENT_POSITION; if it is contained in no table |
| :--- | :--- |
| this function does nothing. |  |
| row_num | The new row is inserted before row_num (first row is zero). To |
| append a row to the end of the table, row_num should be equal |  |
| to the current number of rows (see pgNumRows). |  |

void pgDeleteColumn (pg_ref pg, long position, short column_num, short draw_mode);

Removes a column (including its text contents).

## position

column_num specifies a text position anywhere within a table and can be CURRENT_POSITION. If the position is contained in no table, or if there is only one column this function does nothing.

specifies the column to delete and must be between zero and pgNumColumns() - 1 .

void pgDeleteRow (pg_ref pg, long position, long row_num, short draw_mode);

Removes a row (including its text contents).

## PARAMETERS

position

specifies a text position anywhere within a table and can be CURRENT_POSITION. If the position is contained in no table, or if there is only one row this function does nothing.

row_num specifies the row to delete and must be between zero and pgNumRows() - 1 .

## Miscellaneous

short pgNumColumns (pg_ref pg, long position);

Returns the number of columns in the table containing position. If position is contained in no table this function returns zero.
long pgNumRows (pg_ref pg, long position);

Returns the number of rows in the table containing position. If position is contained in no table this function returns zero.

NOTE: the total number of rows is returned for the whole table regardless of the position parameter. For instance, a 10-row column would cause pgNumRows () to return 10 whether the position is in the first row, middle row or last row, etc.

## Changing "Row" Information

There are no row-specific functions for tables since a table "row" is really a paragraph. Hence, to change attributes to a row you should use pgSetParInfo() and make the desired changes.

For example, setting the justification value for a table row (paragraph) will cause each of the cells in that row to assume the justification. Setting paragraph borders or shading for the row will affect all the cells in that row, etc.

CAUTION: DO NOT ALTER the tab settings or tab quantity in a paragraph format applied to tables; the tab array is used to record column attributes. Also, DO NOT ALTER the number of columns in the table record.

## Getting Other Table Info

Table information is simply a member of par_info. To get information about a table that is not covered in one of the functions above, use pgGetParInfo().

## FILE STANDARDS, INPUT \& OUTPUT

NOTE: If you will only be saving files as OpenPaige native format or RTF and will be including no customized file formatting, see "OpenPaige Import Extension" on page 17-293 and "OpenPaige Export Extension" on page 18-325. Importing and exporting may be a simpler approach

The OpenPaige technology includes a file handling system to help implement the following:

- Platform-independent file transfers — ba proposed standard and function set that enables OpenPaige software to read files saved by other C.P.O.S. as well as save or re-save files to be understood in reverse.
- Upgrade / update independent file transfers - pthe proposed standard guarantees upward and even backwards compatibility for future enhancements to OpenPaige with regards to file transfer. For example, every internal record structure, including style records, can theoretically be altered and enhanced, yet older files will still be loaded correctly and older software will even be able to read the newer files (eliminating, of course, any new feature set that was inherently saved).
- Application-independent file transfers - pdiverse applications, even on the same platforms, are able to read file saved by other applications even if unknown elements have been saved. Using OpenPaige's file transfer methods, application-specific data embedded in the file is simply "skipped" without any adverse consequences.
- One thing to consider is OpenPaige structures that have longs would get flipped around (backwards) if you roll your own I/O. For example if you just slam an OpenPaige struct to a file as a byte stream it won't work on the other end. Fortunately OpenPaige's built-in I/O handler takes care of this problem. I strongly recommend you utilize the file "key" system provided. If special/custom I/O is required anywhere, take a look at the latest release notes regarding files -- there are now ways to "roll your own" while still using OpenPaige's system.
- A subset of functions for app-specific saves - pOpenPaige makes it fairly easy to save and read your own data structures along with the OpenPaige object data, all the while maintaining compatibility with each concept listed above.


### 22.1 Up \& Running

Since the information in this chapter can be somewhat complex in its entirety, the following example is provided for you to be "up and running" with file I/O by simply using the defaults.

/* DoSave saves the current pg_ref to a file. If file_stuff is not NULL a new file is to be saved (first-time saves and Save As) ) $^{2}$

void DoSave (HWND hWnd, OPENFILENAME far *file_stuff)

I* In this file saving example, the ref_num parameter is a file reference created and opened using File OS functions. If there is a problem, an error result is returned. */

static OSErr save_file (pg_ref pg, short ref_num)

\{

OSErr error;

file_ref filemap;

long file_position;

short *filemap_ptr;

filemap = MemoryAlloc(\&mem_globals, sizeof(short), 1, 0);

filemap_ptr = UseMemory(filemap);

${ }^{*}$ filemap_ptr $=$ ref_num;

UnuseMemory(filemap);

error;

\}

file_position $=0$;

error $=$ pgSaveDoc(pg, \&file_position, NULL, 0, NULL, filemap, 0);

DisposeMemory(filemap);

return

NOTE: The "save" code is quite small. If you aren't saving anything special, writing a document is fairly straightforward.

I* In this file reading example, the ref_num parameter is a file reference opened using File OS functions. The app assumes it is an OpenPaige file (saved with the example above). If there is a problem, NULL is returned, otherwise a new pg_ref is returned. ${ }^{*}$

pg_ref read_file (short ref_num)

\{

pg_ref pg;

OSErr error;

file_ref filemap;

long file_position;

short *filemap_ptr;

pg $=$ pgNewShell(\&paige_rsrv); $\quad / *$ Creates empty OpenPaige object *I

filemap = MemoryAlloc(\&paige_rsrv.mem_globals, sizeof(short), 1, 0);

filemap_ptr= UseMemory(filemap);

*filemap_ptr= ref_num;

UnuseMemory(filemap);

file_position $=0$;

// NOTE: You can also use OpenPaige's PG_TRY, PG_CATCH here for exception handling

error = pgReadDoc(pg, \&file_position, NULL, 0, NULL, filemap); if (error != noErr)

\{

show_error(error);

pgDispose(pg);

$\mathrm{pg}=\mathrm{NULL}$;

\}

DisposeMemory(filemap);

return pg;

\}

## /* DoFileOpen opens an OpenPaige file which has already been specified by user. */

void DoFileOpen (HWND hWnd, OPENFILENAME far *file_stuff) \{

int file_ref, far ${ }^{* f} \_$ptr;

long position;

RECT view_area;

memory_ref file_map;

if $(($ file_ref $=$ _lopen(file_stuff->|pstrFile, OF_READ $))!=-1)\{$

file_map = MemoryAlloc(\&mem_globals, sizeof(int), 1, 0);

f_ptr = UseMemory(file_map);

*f_ptr = file_ref;

UnuseMemory(file_map);

position $=0$;

pgReadDoc(test_pg, \&position, NULL, 0, NULL, file_map);

DisposeMemory(file_map);

_Iclose(file_ref);

GetClientRect(hWnd, \&view_area);

InvalidateRect(hWnd, \&view_area, FALSE);

\}

\}

### 22.2 Saving a Document

If you want to "save" a pg_ref in the native, default format, call the following:

$$
\begin{aligned}
& \text { (pg_error) pgSaveDoc (pg_ref pg, long PG_FAR *file_position, } \\
& \text { pg_file_key_ptr keys, pg_short_t num_keys, file_io_proc write_proc, } \\
& \text { file_ref filemap, long doc_element_info); }
\end{aligned}
$$

This function writes all the information within $p g$ to a specified file; the first byte is written to * file_position. When the function returns, ${ }^{*}$ file_position will be updated to the next file location (hence, ${ }^{*}$ file_position minus the position before the function is called $=$ total byte size written to the file).

If the ending file position of all OpenPaige data will not necessarily be the physical endof-file, you must terminate the file properly. ( Please see Section on page 398. )

The keys parameter is an optional pointer to a list of file keys known as file handlers. If this is a null pointer, all components of $p g$ are written.

If the keys parameter is nonnull, then num_keys must indicate how many items are in the list pointed to by keys, and pgSaveDoc only writes the components in the list of keys.

The write $\_$proc is a pointer to a function that should do the physical I/O. However, this parameter can be a null pointer, in which case the standard file transfer function is used.

If write_proc is nonnull, it must point to valid file_io_proc function — bsee "The file_io_proc" on page 34-705 if you want to write your own io function.

The filemap parameter is a memory_ref allocation that contains machine-specific information referencing the physical file that is to be written to.

NOTE: The filemap must be a file reference to an opened file with write permission. The way to accomplish this is shown in the following function example (the $f \_$ref parameter is a file reference obtained from FSOpen, or PBOpen, etc. for Macintosh or_lopen, etc. for Windows).

The doc_element_info parameter is used for identifying multiple $p g \_r e f$ "documents" written to the same file. For a single document (or first in a series of pg_ref writes) doc_element_info should be zero.

CAUTION: If you intend to write any data following the OpenPaige data, or if the physical end-of-file will not exactly match the ending file position after pgSaveDoc(), it is essential that you terminate the OpenPaige file by calling pgTerminateFile(). Not doing so will result in unexplained crashes when the file is reopened.

CAUTION: OpenPaige does not set the physical end-of-file. In other words, if you created a file 1 megabyte in size and OpenPaige wrote only $10 \mathrm{~K}$ of data, your physical file size will still be 1 megabyte. If appropriate you must truncate your file once all the data is saved.

## How to create an OpenPaige filemap

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-394.jpg?height=829&width=1235&top_left_y=973&top_left_x=433)

NOTE: For a complete understanding of memory allocations, as shown in the above examples, see "The Allocation Mgr" on page 25-441.

If $p g$ SaveDoc is successful, zero is returned (implying no I/O errors). If unsuccessful, the appropriate error code will be returned (see "Error Codes" on page 39-765).

Saving text only

The best/fastest way to save text only is to walk through each block of text and write the text to a file. (OpenPaige maintains text as separate records, each record containing a piece of the whole document). The following is a brief example of how you can do this:

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-395.jpg?height=1085&width=1219&top_left_y=668&top_left_x=448)

If the ending file position of all OpenPaige data will not be the physical end-of-file, you must terminate the file properly using $p g$ TerminateFile as shown below.

For example, if you called $p g S a v e D o c$ then set the physical end-of-file to the ending file position, your file save is complete (you do not need to terminate the file in any other way). However, if you called pgSaveDoc but you then want to write additional data of your own beyond that point, you must first call the following function:

(pg_error) pgTerminateFile (pg_ref pg, long PG_FAR *file_position, file_io_proc write_proc, file_ref filemap);

This function writes a file key that specifies the logical end-of-file for the OpenPaige document. Later, when the file is read with $p g R e a d D o c$, OpenPaige will recognize this key as the logical end of file and discontinue reading any data beyond that position.

Upon entry, pg, file_position and write_proc should all be the same parameters that were given to $p g$ SaveDoc.

## NOTES:

- You do NOT need to call this function if the end of the OpenPaige document and physical end-of-file is identical (but it does not hurt to do so).
- The term "logical end of file" implies the end of the very last piece of data that can be read (later) by $p g R e a d D o c$. That includes all data written by $p g S a v e D o c$ and/or $p g$ WriteKeyData.
- When and if $p g R e a d D o c$ encounters the logical end-of-file, the file offset returned by $p g R e a d D o c$ will be positioned at the first byte after the end-of-file (which would have been the first byte written by your application if you wrote nonOpenPaige data after this position).


### 22.3 Reading a Document

To read a document previously saved with $p g S a v e D o c$, call the following:

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-397.jpg?height=154&width=1199&top_left_y=397&top_left_x=446)

In this function, $p g$ must be a valid OpenPaige object reference for which all data that is read can be placed; $p g$, however, can be completely empty. If it does contain data (text, styles, etc.), those items get replaced with data that is read from the file; items that are not processed from the file (i.e., data components not recognized or components that don't even exist in the file) will leave that component in $p g$ unchanged.

NOTE: Helpful hint: A function exists to create a completely "empty" pg_ref for purposes of pgReadDoc —psee "A Quick \& Easy Empty OpenPaige Object" on page 34-728.

When pgReadDoc is called, what occurs is as follows: a data component is read beginning at the specified file offset; the data component always includes a "header" that includes the data key as well as the data size. The file handler ( $p g \_$handler) is searched for that contains the data key and, if found, the file handler function is called to process the data. If a pg_handler is not found, the data is skipped (actually, before it is skipped a special "exception handler" is called which will be described later in this section).

Each data component is handled in this way, one element at a time until the end of file is reached.

NOTE: The "end of file" is not necessarily the physical end of file, rather it is determined by the file position given to pgTerminateFile().

The file_position parameter must point to the first file offset to read, in bytes. The offset is relative to the beginning of the file (beginning is zero) and it must be the same position given to $p g \operatorname{SaveDoc}($ ) when the file was written.

If keys is a null pointer, $p g R e a d D o c$ will try to process every data element it reads. If keys is nonnull, it must be a pointer to a list of num_key keys; if such a list is given, pgReadDoc will only consider processing the keys that are in the list.

The read_proc is a pointer to a function that should do the physical I/O. However, this parameter can be a null pointer, in which case the standard file transfer function is used.

If read_proc is nonnull, it must point to valid file_io_proc function —bsee "The file_io_proc" on page 34-705 if you want to write your own io function.

The filemap parameter is a memory_ref allocation that contains a file reference, native to the runtime platform.

NOTE: The filemap must be a file reference to an opened file with at least read permission. The way to accomplish this is shown in the example above for pgSaveDoc.

If $p g R e a d D o c$ is successful, zero is returned (implying no I/O errors). If unsuccessful, the appropriate error code will be returned (see "Error Codes" on page 39-765 in the Appendix).

### 22.4 Verifying an OpenPaige-formatted File

You can verify if a file is a true OpenPaige file or not by calling the following:

(pg_error) pgVerifyFile (file_ref filemap, file_io_proc io_proc, long position);

Use this function to "test" any file to find out if it is truly an OpenPaige file. Upon entry, filemap and io_proc must be the same parameters you would pass to $\operatorname{pgReadDoc().}$

The position parameter specifies the file position, in bytes.

FUNCTION RESULT: If the function returns zero, the file is a valid OpenPaige file.

Otherwise the appropriate error code is returned (usually “BAD_TYPE_ERR").

### 22.5 Do you need the remaining info?

The rest of this chapter explains the details of extending the OpenPaige saving mechanisms to your own applications. If you are saving only the contents of a pg_ref, you do not need to read anything else in this chapter. If you are saving "custom" information, read on.

### 22.6 Saving Your Own Data Format

NOTE: There is nothing preventing you from writing and reading whatever you want before and after OpenPaige transfers the contents of a pg_ref. Calling pgSaveDoc() simply serializes a stream of objects beginning at the file position you have specified, and there is nothing to prevent you from writing other data after that location. If you consider the composite stream of OpenPaige data as one single "record" in a file, the concept of integrating your own data may be simplified.

On more than one occassion, an OpenPaige user has asked how some particular data format can be forced using the OpenPaige file mechanism, or how text can be saved as one continuous block of text, etc.

The answer is possibly non-intuitive, yet fairly simple once it is grasped:

1. If you have no reason to make OpenPaige automatically read your custom data, just write the data before or after the OpenPaige data and read it back the same way (see note above).
2. If you want OpenPaige to save the data for you and notify you when it reads it back, use pgWriteKeyData to save the structure and then use a custom handler to read the structure (see example below).

If you want OpenPaige to write your data and notify you when it is read, the easiest way to go is to write the data with pgWriteKeyData and retrieve it with a read handler. If this method seems appropriate to your situation, the following sample code illustrates how this can be done:

void SaveMyData (some_arbitrary_struct *myData, long myDataSize, pg_file_key myFileKey)

\{

$1 *$ First, save pg_refusing all the defaults. The "myFileRef" is a memory_ref containing the file reference specific to the machine. For Macintosh, the memory_ref contains the file refNum. For Windows, the memory_ref contains the integer result from OpenFile (or_lopen, etc.). */

long position;

position $=0$; // We save file starting at first byte (but don't have to) pgSaveDoc(pg, \&position, NULL, 0, NULL, myFileRef, 0);

pgWriteKeyData(pg, myFileKey, (void *)myData, myDataSize, myRefCon, NULL, \&position, myFileRef);

\}

Notes: (1) "myRefCon" represents any value you want to save as a reference. You will get this value handed back to you in the read handler (below). "MyFileKey" can be any number >= CUSTOM_HANDLER_KEY. This value is used to identify the data item when the file is read later. (3) You can call a similar function as above multiple times. When the file is read, your read handler will get called for each occurrence of the data as it is read. */

To read the data structure(s) back, you first install a read handler:

pgSetHandler(\&paige_globals, myFileKey, myReadHandler, NULL, NULL, NULL, NULL);

The value for myFileKey should be the same as the value used in the above example for writing the data.

Your read handler should look like this:

PG_PASCAL (pg_boolean) myReadHandler (paige_rec_ptr pg, pg_file_key the_key, memory_ref key_data, long PG_FAR *element_info, void PG_FAR *aux_data, long PG_FAR *unpacked_size)

\{ I* OpenPaige calls this function each time it reads data from the file that was saved as the_key (previously written as "myFileKey" in the example above). You can ignore almost all of the parameters, the only two you probably care about are "key_data" (which holds the data that has been read from the file), and "element_info" (which points to "myRefCon" saved earlier). */

\}

Retrieving the data in the read handler: the data, as originally written to the file, will be contained in key_data. To get a pointer to the data, simply do:

$$
\text { ptr = UseMemory(key_data); }
$$

NOTE: be sure to do UnuseMemory after accessing the data in $p t r$.

$$
\text { data_size = GetMemorySize(key_data); }
$$

## The Hybrid

Sometimes you need to save file data in some specific format, bypassing the OpenPaige file I/O system altogether, yet you want OpenPaige to save most of the $p g \_$ref data items.

To do so, perform the following logic:

## To save:

1. Call $p g S a v e D o c$ in the normal way (if you need to save regular $\left.p g \_r e f i t e m s\right)$.
2. Call $p g$ TerminateFile (which tells OpenPaige there will be no more OpenPaigebased data).
3. At this point the file position will be known (i.e. the next byte offset to write some additional data). Write this data in any way you choose.

## To retrieve the data:

1. Call $p g R e a d D o c$ (if you used $p g S a v e D o c$ to save).
2. The file offset will return to you positioned on the first byte you originally wrote. Read the data in whatever method is appropriate.

NOTE: You can perform the "reverse," if necessary, by calling pgSaveDoc AFTER you write your own data. This will still work so long as you provide the correct file position to $p g S a v e D o c$ to begin saving that same file position for $p g R e a d-$ $D o c$ to begin reading.

OpenPaige file mechanism provides some OPTIONAL utilities to "compress" a series of numbers so your data transfer is smaller. You can certainly use your own instead of these.

These functions make data portable between platforms. This is because they resolve the saving and retrieving certain numbers between the Mac and DOS which save those numbers backwards from each other. These function make those numbers portable.

By "series of numbers" is meant an array of longs or shorts, or consecutive fields in a record structure, etc.

For example, suppose you need to save a large record structure that consists mostly of zeros. Using the "pack" and "unpack" methods described below you can conserve a great deal of space.

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-403.jpg?height=154&width=1146&top_left_y=941&top_left_x=434)

pgSetupPacker sets up a special record to begin packing or unpacking numbers. For "packing" numbers, you begin with a zero-size $p g \_r e f$ and the packing functions append data to it; for unpacking, you begin with a memory_ref that already has packed data (or contains packed data read from a file) and retrieve the data with the unpacking functions (see below).

The walker parameter must be a pointer to a pack_walk record (defined in pgFiles.h). If you are packing/unpacking from a read or write handler, the ref parameter should be the key_data memory_refgiven to you when a read or write handler is called. Or, if you are using the pack/unpack functions outside of a read or write handler, ref must be a valid memory_ref with a record size of 1 byte and a memory size of zero for packing, or a valid memory_ref containing previously packed data for unpacking.

The first_offset parameter should be zero.

Once the pack_walk record is "set up" you can use the functions given below.

NOTE: If packing numbers, once you are through, call pgFinishPack.
\#include "pgFiles.h"

(void) pgPackNum (pack_walk_ptr out_data, short code, long value);

Adds a long or short numeric value to the packed data. The out_data parameter must point an initialized pack_walk record previously set up with pgSetupPacker.

The code parameter should be short_data if packing an integer or long_data for packing a long. The value parameter is the numeric value to pack.

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-404.jpg?height=151&width=1160&top_left_y=778&top_left_x=434)

Identical to $p g P a c k N u m$ except an array of numbers are packed. The ptr parameter must point to the first number in the array, the qty parameter indicates the number of elements in the array and data_code must be short_data if the elements are integers or long_data if the elements are longs. All elements must be the same type (all must be either shorts or longs, not a mixture).

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-404.jpg?height=109&width=897&top_left_y=1351&top_left_x=434)

Completes the packing within walker (by optimizing the compression and terminating the internal packed data structure).

If you have packed anything at all, you must call this function.

CAUTION: Do NOT call pgFinishPack if you have not actually packed any data (i.e., the original memory_ref is still zero size).

The function returns the same memory_ref you originally gave pgSetupPacker.

## \#include "pgFiles.h"

(long) pgUnpackNum (pack_walk_ptr in_data);

Returns a number that was previously packed. The in_data parameter must be a pointer to an initialized pack_walk record (using pgSetupPacker).

NOTE: Numbers must be unpacked in the same order as they were packed. However, $p g$ UnapckNum will simply return zero(s) if you ask for more numbers than were packed.

\#include "pgFiles.h"

(void) pgUnpackNumbers (pack_walk_ptr out_data, void PG_FAR *ptr, short qty, short data_code);

Identical to $p g$ UnpackNum except an array of numbers are unpacked. The $p t r$ parameter must point to the first number in the array to receive the numbers, the qty parameter indicates the number of elements in the array and data_code must be short_data if the elements are integers or long_data if the elements are longs. All elements must be the same type (all must be either shorts or longs, not a mixture) and they must be the same type(s) that were originally packed. If more numbers are asked for than were packed, this function fills the extra array elements with zeros.

NOTE: You do NOT call pgFinishPack for unpacking —-pthat function is only used to terminate data after using the "pack" functions.
\#include "pgFiles.h"

(void) pgPackBytes (pack_walk_ptr out_data, pg_char_ptr the_bytes, long length);

Appends the_bytes data of length size to the packed data in out_data.

NOTE: The data is not actually "compressed" but is simply included in the data stream and can be retrieved with pgPackBytes or pgUnpackPtrBytes below.

It is OK to mix pgPackBytes with $p g$ PackNum or $p g P a c k N u m b e r s$ as long as you retrieve the data in the same order you packed it.

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-406.jpg?height=107&width=1222&top_left_y=930&top_left_x=434)

Unpacks data previously packed with $p g$ PackBytes. The bytes are written to *out_ptr in the same order they were originally packed. It is your responsibility to make sure out_ptr can contain the number of bytes about to be unpacked. (If you aren't sure about the size of the unpacked data, or if the data might be arbitrarily huge, it might be better to use pgUnpackBytes below).

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-406.jpg?height=112&width=1214&top_left_y=1453&top_left_x=433)

Identical to pgUnpackPtrBytes except the unpack data is placed in out_data memory_ref. The memory_ref will be sized to hold the total number of unpacked
bytes. The advantage of using this method versus pgUnpackPtrBytes is you do not need to know how large the data is.

\#include "pgFiles.h"

(long) pgGetUnpackedSize (pack_walk_ptr walker);

Returns the size, in bytes, of the next data in walker. This function works for all types of data that have been packed, both numbers and bytes.

## Rectangle

\#include "pgFiles.h"

(void) pgPackRect (pack_walk_ptr walker, rectangle_ptr r);

(void) pgUnpackRect (pack_walk_ptr walker, rectangle_ptr r);

Packs/unpacks a rectangle $r$.

## Co_ordinate

## \#include "pgFiles.h"

(void) pgPackCoOrdinate (pack_walk_ptr walker, co_ordinate_ptr point);

(void) pgUnpackCoOrdinate (pack_walk_ptr walker, co_ordinate_ptr point);

Packs / unpacks a co_ordinate.

## \#include "pgFiles.h"

(void) pgPackColor (pack_walk_ptr walker, color_value PG_FAR *color);

(void) pgUnpackColor (pack_walk_ptr walker, color_value PG_FAR ${ }^{*}$ color);

Packs / unpacks a color_value color into the packed data.

## Shape

\#include "pgFiles.h"

(long) pgPackShape (pack_walk_ptr walker, shape_ref the_shape); (void) pgUnpackShape (pack_walk_ptr walker, shape_ref the_shape);

Packs / unpacks a shape the_shape.

## Select pair

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-408.jpg?height=160&width=1222&top_left_y=1175&top_left_x=434)

Packs / unpacks a select_pair pair.

## 23 <br> HUGE FILE PAGING

"File paging" is a method in which large files are not read into memory all at once; rather, only the portion(s) that are needed to display are read dynamically as the user scrolls or "pages" through the document.

### 23.1 Paging OpenPaige Files

Any file that has been saved with $p g S a v e D o c($ (or with the custom control message $P G \_S A V E D O C$ ) can be opened in "paging" mode by calling a different function instead of $p g R e a d D o c()$ :

pg_error pgCacheReadDoc (pg_ref pg, long PG_FAR *file_position, const pg_file_key_ptr keys, pg_short_t num_keys, file_io_proc read_proc, file_ref filemap);

This function is $100 \%$ identical to $\mathrm{pgReadDoc()} \mathrm{except} \mathrm{the} \mathrm{document} \mathrm{is} \mathrm{set} \mathrm{to} \mathrm{disk-}$ paging mode. This means that the text portion of the document is not loaded into memory all at once; rather, only the portions that are needed are loaded dynamically.

The parameter values should be completely identical to what you would pass to pgReadDoc(). However, the physical disk file must remain open for disk paging to be successful, and closed only after the $p g \_$ref is finally disposed.

Not only should the file remain open, the filemap parameter must remain valid. For example, if filemap is a memory_ref (which it will be for the standard "open" function), that memory_ref and its content must remain in tact until the $p g \_r e f$ has been disposed.

If pgCacheReadDoc() returns without error, all portions of the document except for its text will have been loaded into memory; the text portions will be loaded as needed during the course of the user's session with this document.

Similarly, for tight memory situations, OpenPaige will unload text portions as required (if they have not been altered) to make room for other allocations. This unloading process occurs transparently even if you have not enabled virtual memory.

NOTE: Calling pgDispose() does not close the file; your application must close the file after the $p g \_r e f$ has been destroyed. If you need to obtain the original file reference, see"pgGetCacheFileRef()" on page 23-409 this document. If you are using your own file_io_proc, that file_io_proc must be available at all times until the document is disposed.

### 23.2 OpenPaige Import Extension

If you are opening file(s) with the OpenPaige import extension, instead of calling pgReadDoc(), file paging is enabled by setting the IMPORT_CACHE_FLAG bit in the "import_flags" parameter — see Section page 17-298

You can set a raw ASCII text file for file paging by using the OpenPaige import extension for ASCII text files. To enable file paging, set the IMPORT_CACHE_FLAG bit in the "import_flags" parameter — see Section page 17-302.

Paging text files causes only the text that is required for displaying to be loaded into memory.

NOTE: You must keep the text file open until the document is disposed.

### 23.3 Custom Control

Use the message $P G \_C A C H E R E A D D O C$ instead of $P G \_R E A D D O C$ to enable disk paging.

NOTE: wParam and IParam are identical with both messages. The file must remain open, however, until the control window is closed.

### 23.4 Getting the File Reference

file_ref pgGetCacheFileRef (pg_ref pg);

This function returns the file, if any, that was given to $p g C a c h e R e a d D o c($ ). Or, if you enabled file paging with the OpenPaige import extension, the value returned from this function will be the original file reference given to the import class.

The usual reason for calling this function is to obtain the file reference before disposing the $p g \_r e f$ so the file can be closed.

### 23.5 File Paging Save

pg_error pgCacheSaveDoc (pg_ref pg, long PG_FAR *file_position, const pg_file_key_ptr keys, pg_short_t num_keys, file_io_proc write_proc, file_ref filemap, long doc_element_info);

This function is identical to $p g S a v e D o c($ ) except it should be called to save a file that is currently enabled for file paging (i.e., the document was previously opened with pgCacheReadDoc() or imported with the IMPORT_CACHE_FLAG bit).

Calling $p g$ CacheSaveDoc() creates an identical file to $p g S a v e D o c()$ and accepts the same parameters to its function; the difference, however, is how it handles certain situations that might otherwise fail (for example, saving to the same file that is currently open for file paging).

It is safe to use $p g$ CacheSaveDoc() even if the document is not enabled for file paging.

If this function is successful, the filemap parameter becomes the new file paging reference (the same as if you had reopened the file with pgCacheReadDoc()). It is therefore important that you close the previous file (if it was different) and that you do not close the new file just saved.

NOTE: Opening a file with $p g C a c h e R e a d D o c($ ) then re-saving to that same file with $p g$ CacheSaveDoc() will only work correctly if the filemap parameter is exactly the same file_ref for both opening and saving.

### 23.6 Writing Additional Data

If you need to write additional data to an OpenPaige file it is safe to do so after pgCacheSaveDoc() returns; or, if you are certain that the file being written is a different file than the original file given to $p g C a c h e R e a d D o c()$ it is safe to write data before and/or after $p g$ CacheSaveDoc().

You may also call extra functions such as $p g$ SaveAlIEmbedRefs() and pgTerminateFile() in the same way they are used with pgSaveDoc().

### 23.7 OpenPaige Export Extension

If you are saving file(s) with the OpenPaige export extension, you can cause the same effect as pgCacheSaveDoc() y setting the EXPORT_CACHE_FLAG bit in the “export_flags" parameter - see See “OpenPaige Export Extension”, Chapter 18.

## 24

## CHAPTER

## MISCELLANEOUS UTILITIES

### 24.1 Require recalc

(void) pgInvalSelect (pg_ref pg, long select_from, long select_to);

The text from select_from to select_to in $p g$ is "invalidated," which is to say it is marked to require recalculation, new word wrap, etc.

Both parameters are byte offsets, relative to zero. No actual calculation is performed until the text (or highlighting) is drawn, or pgPaginateNow is called (see "Paginate Now" on page 24-414).
pg_boolean pgGetHiliteRgn (pg_ref pg, select_pair_ptr range, memory_ref select_list, shape_ref rgn);

This function sets rgn to the "highlight" shape for the specified range of text.

The rgn parameter must be a valid shape_ref (which you create); when the function returns, that shape will contain the appropriate highlight region.

The text offsets that are used to compute the region are determined as follows: if range is not a NULL pointer, that selection pair is used; if range is NULL but select list is not MEM_NULL, then select_list is used as a list of selection pairs (see below). If both are NULL, the current selection range in $p g$ is used.

The select_list parameter, if not MEM_NULL, must be a valid memory_ref containing a list of select_pair records. Usually, a selection list of this type is used for discontinuous selections (see "Discontinuous Selections" on page 10-161 for information about pgGetSelectionList and pgSetSelectionList).

FUNCTION RESULT: TRUE is returned if the resulting highlight region is not empty.

### 24.3 Paginate Now

(void) pgPaginateNow (pg_ref pg, long paginate_to, short use_best_guess);

The OpenPaige object is forcefully "paginated" (lines computed) from the start of the document up to the text offset paginate_to.

If use_best_guess is TRUE, OpenPaige does not calculate every single line, rather it makes a guess as to the document's height.

If the document is already calculated, this function does nothing.

## NOTES:

(1) Full pagination, from top to bottom on a large document can take several seconds. However, it is the only guaranteed method to produce $100 \%$ accuracy on text height or line positions.

(2) OpenPaige automatically calls this function for you in most cases that require it.

### 24.4 Style Info

(pg_boolean) pgFindStyleInfo (pg_ref pg, long PG_FAR *begin_position,

long PG_FAR *end_position, style_info_ptr match_style, style_info_ptr mask, style_info_ptr AND_mask);

FUNCTION RESULT: This function returns "TRUE" if a specific style — por portions of a style — pcan be found in $p g$.

Upon entry, begin_position must point to a text offset (which is a byte offset, beginning at zero); when this function returns and a style found, *begin_position will get set to the offset where the found style begins and *end_position to the offset where that style ends in the text.

Styles are searched for by comparing the fields in match_style to all the style_info records in $p g$ as follows: Only the fields corresponding to the nonzero fields in mask are compared; before the comparison, the corresponding value in AND_mask is AND'd temporarily with the value in the style_info record in question. If all fields match in this way, the function returns "TRUE" and sets begin_position and end_position accordingly.

If match_style is a null pointer, the function will always return "TRUE" (it will simply advance to the next style). If mask is null, then all fields are compared (so the whole
style must match to be TRUE). If AND_mask is null, no AND'ing is performed (and the whole field is compared).

### 24.5 Examine Text

pg_char_ptr pgExamineText (pg_ref pg, long offset, text_ref *text, long PG_FAR *length);

This function provides a way for you to examine text directly in an OpenPaige object.

The offset parameter should be set to the absolute byte offset you wish to return (beginning at zero). The text parameter is a pointer to a text_ref variable which will get set to a memory_ref by OpenPaige before the function returns. The length parameter must point to a long, which also gets set by OpenPaige.

FUNCTION RESULT: A pointer is returned that points to the first character of offset; *text is set to the memory_ref for that text, which you must "unuse" after you are through looking at the text (see below); *length will get set to the text length of the pointer, which will be the number of characters to the end of the text block from which the text was taken (it won't necessarily be the remaining length of all text in $p g$ ).

// This shows getting the text at offset 123:

text_ref ref_for_text;

long $\quad t$ length;

pg_char_ptr the_text;

the_text = pgExamineText(pg, 123, \&ref_for_text, \&t_length);

// ... do whatever with the text, then:

UnuseMemory(ref_for_text);/* .. otherwise it stays locked! */

## TECH Examining some text

I'd like to know how to fetch the text from an openpaige document. I've read the manual and still don't get it. I've created an OpenPaige document in a dialog so $I$ can allow the user to enter more than 255 characters. Inserting text is no problem. How do I get it back out. A hint would be fine, a snippet of code would be marvelous.

Although some of the solutions below will work, the method above described is more for high-speed direct text access used for find/replace features, or spell checking, etc.

In your case, however, I think walking through the text blocks using pgExamineText might be unnecessarily complex. Just use the following function:[^0]

Given a specific selection of text in "selection," this returns a memory ref that has the text you want. Very simple. The "data_type"parameter should be one of the following:

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-420.jpg?height=447&width=1195&top_left_y=291&top_left_x=434)

Probably the one you want is "all_data" of "all_text_chars."

If "selection" is NULL the text returned will be the currently selected (highlighted) text, otherwise it returns the text within the specified selection (which is probably what you want). This parameter should therefore point to a select_pair record which is defined as:

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-420.jpg?height=256&width=1159&top_left_y=1190&top_left_x=434)

To copy all text, "begin" should be zero and "end" should be pgTextSize(pg).

The function returns a "text_ref" which is a memory_ref. To get the text inside, do this:

Ptr text;

text $=$ UseMemory(ref); /* .. where "ref" is function result $* /$

Then when you're finished looking at the text, do:

UnuseMemory(ref);

Finally, to dispose the text_ref call DisposeMemory(ref).

This should be the way to go.

## Examining text across the text blocks

I am using pgExamineText to access the text in the openPaige object I am searching. This creates some problems because of the openPaige text blocks.

It depends on what you are searching for. Under normal conditions, OpenPaige always splits a block on a CR (carriage return) character (including the CR as its last character, which means it can't ever break in the middle of a line or word. And by "normal" conditions I mean a document composed of reasonably sized paragraphs where CR's exist at, say, every few hundred characters. If you have some mongo paragraph that goes for pages, the block gets split somewhere else with no other choice. Even then, however, it tries to break it at a word boundary and not in the middle.

Hence you might improve your searching by checking for CR at the end of the block -- it would only be when you're searching on something that must cross a CR boundary would it be necessary to cross the block.

In any event, it also depends on how you are actually doing the search/compare. If you're using some black-box code that requires a continuous text pointer, then I see why you have a problem. But if you rolled your own, why can't you just increment to the next buffer with a new pgExamineText?

A second related issue has to do with non-case sensitive searches. To handle this I convert the find string \& all the text in the openPaige target to upper case (c toupper function) and search in the regular way.

Again, I am wondering if you're doing your own comparecharacter function versus calling someone's "compare" black box. I've written character compare searches many times, and to do case insensitive compares I simply convert the character
from each pointer to upper case (in a separate variable) before comparing. Of course you can do all that at once by copying all of it to a buffer. I'm not sure which way is fastest.

it appears that I need to allocate memory, move the text into it and work with the copy. It looks like MemoryDuplicate may be the way to go.

Maybe, but what would be a lot faster is to allocate a worstcase memory_ref, then use MemoryCopy. The reason this would be a lot faster is you wouldn't need to keep creating a memory ref, rather you would just slam the text straight into your allocation for each block. And, the way OpenPaige Allocation Mgr works is that almost no "SetHandleSize" would ever occur.

If you want to concatenate two text blocks together in your memory ref, you should first do MemoryCopy for the first one, then for the second you do:

ptr = AppendMemory(memory_ref, size_of_2nd_block, FALSE);

BlockMove(text_ptr_of_send_block, ptr, size_of_2nd_block);

UnuseMemory(memory_ref);

For additional speed, if you elect to do the MemoryCopy method, you might consider bypassing pgExamineText and going directly to the block. You can do this using the same functions OpenPaige uses (the "offset" param is the desired text offset):

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-422.jpg?height=304&width=1064&top_left_y=1443&top_left_x=433)

UnuseMemory(block->text);

UnuseMemory(pg_rec->t_blocks);

UnuseMemory(pg);

You can also get total number of blocks as:

num_blocks = GetMemorySize(pg_rec->t_blocks);

NOTE: you need to include "pgText.h" which contains the lowlevel function prototype for pgFindTextBlock.

Things to know about text blocks

Text blocks are an important part of openPaige. We have found that only by using text blocks can you get acceptable performance. In fact, these text blocks are around $2 \mathrm{~K}$ in size by default. If text was not put into blocks you would get performance like TextEdit when text exceeds small blocks. As you know, it comes to a crawl when the text is larger than about $5 K$.

There are some important things you may want to know about text blocks however.

First of all, you should not look at the text using HandleToMemory, etc. OpenPaige provides functions for getting a locked pointer to a chunk of text.

Second, you should not change the text by inserting directly into a block. You can exchange a character while in pgExamineText. But if you try and do any direct insertions, the block will be messed up, and all the subsequent styles will be wrong.

Third, OpenPaige does its very best to break text blocks at a carriage return. If there is a carriage return within the block, Openpaige breaks there. If not, it simply breaks it at a convenient place. Therefore, you cannot be assured what the
last character is. You must use the length given you by pgExamineText. There is no character that you can check to know where the end of the text block is. OpenPaige cannot assume you will want to use any particular character. No matter what character we might pick, someone will be using it in their data.

To accessall the text, you simply walk through the blocks using pgExamineText.

### 24.6 Information about a particular character

(long) pgCharType (pg_ref pg, long offset, long mask_bits); (pg_short_t) pgCharByte (pg_ref pg, long offset, pg_char_ptr char_bytes);

The two functions above will return information about a character.

FUNCTION RESULT: The function result of pgCharType will be a set of bits describing specific attributes of the character in $p g$ at byte location offset.

The mask_bits parameter defines which characteristics you wish to know; this parameter should contain the bit(s) set, according to the values listed below, that you wish to be "tested."

For example, if all you want to know about a character is whether or not it is "blank," you would call pgCharType and pass BLANK_BIT in mask_bits; if you wanted to know if the character was blank or if the character is a control character, you would pass BLANK_BIT | CTL_BIT, etc. Selecting specific character info bits greatly enhances the performance of this function.

The result (and mask) can contain one or more of the following bits:
\#define BLANK_BIT

\#define WORD_BREAK_BIT

\#define WORD_SEL_BIT

\#define SOFT_HYPHEN_BIT

\#define INCLUDE_BREAK_BIT

\#define INCLUDE_SEL_BIT

\#define CTL_BIT

\#define INVIS_ACTION_BIT

\#define PAR_SEL_BIT

\#define LINE_SEL_BIT

\#define TAB_BIT

\#define FIRST_HALF_BIT

\#define LAST_HALF_BIT

\#define MIDDLE_CHAR_BIT

\#define CONTAINER_BRK_BIT

\#define PAGE_BRK_BIT

\#define NON_BREAKAFTER_BIT

\#define NON_BREAKBEFORE_BIT

\#define NUMBER_BIT

\#define DECIMAL_CHAR_BIT

\#define UPPER_CASE_BIT

\#define LOWER_CASE_BIT

\#define SYMBOL_BIT

\#define EUROPEAN_BIT

\#define NON_ROMAN_BIT

\#define NON_TEXT_BIT

\#define FLAT_QUOTE_BIT

\#define SINGLE_QUOTE_BIT

\#define LEFT_QUOTE_BIT

\#define RIGHT_QUOTE_BIT

\#define PUNCT_NORMAL_BIT

\#define OTHER_PUNCT_BIT 0x00000001

0x00000002

0x00000004

0x00000008

0x00000010 /* Word break but include with word */

0x00000020 /*Select break but include with word*/

0x00000040 /* Char is a control code */

0x00000080 /* Char is not a display char, but arrow, bs, etc. */

0x00000100 /* Char breaks a paragraph *

0x00000200 /* Char breaks a line (soft CR) */

0x00000400 /* Char performs a TAB *

0x00000800 / 1 st half of a multi-byte char */

0x00001000 /* Last half of a multi-byte char *

0x00002000 /* Middle of a multi-byte char run */

0x00004000 /* Break-container bit */

0x00008000 /* Break-repeating-shape bit *

0x00010000 /*Char must stay with char(s) after it *

0x00020000/*Char must stay with $\operatorname{char}(\mathrm{s})$ before it ${ }^{\star /}$

0x00040000 /* Char is numeric */

0x00080000 $1 *$ Char is decimal (for decimal tab) */

0x00100000 /* Char is UPPER CASE */

0x00200000 /* Char is lower case */

0x00400000 /* Char is a symbol *I

0x00800000 /* Char is ASCII-European */

0x01000000 /* Char is not Roman script */

0x02000000 /* Char is not really text */

0x04000000 /* Char is a "flat" quote */

0x08000000 /* Quote char is single quote */

0x10000000 $\quad /$ Char is a left quote */

0x20000000 /* Char is a right quote */

0x40000000 $1 *$ Char is normal punctuation */

0x80000000/* Char is other punctuation in multibyte ${ }^{\star /}$

/* Convenient char_info macro for any quote char in globals: */

\#define QUOTE_BITS (FLAT_QUOTE_BIT | SINGLE_QUOTE_BIT | LEFT_QUOTE_BIT |

RIGHT_QUOTE_BIT)

/* CharInfo / pgCharType convenient mask_bits* /

\#define NON_MULTIBYTE_BITS ( (FIRST_HALF_BIT | LAST_HALF_BIT))

\#define WORDBREAK_PROC_BITS (WORD_BREAK_BIT | WORD_SEL_BIT |

NON_BREAKAFTER_BIT | NON_BREAKBEFORE_BIT)

NOTE: When pgCharType is called, OpenPaige calls the char_info function for the style assigned to the character at the specified offset.

If you need additional information about a character - or to obtain the character itself - buse pgCharByte. This function will return the length of the character at byte location offset (remember that a character can be more than one byte). In addition to returning the length, the character itself will be copied to the buffer pointed to by char_bytes; make sure that this buffer contains enough space to hold a potential multibyte character.

When calling $p g$ CharByte, if the specified offset calls for a byte in the middle of a character, the appropriate adjustment will be made by OpenPaige so the whole character is returned in char_bytes; the function result (length of character) will also reflect that adjustment. Hence, it will always return the whole character size even if offset indicates the last byte of a multibyte character.

You can also use $p g C h a r B y t e$ just to determine the length of a character: by passing a NULL pointer to char_bytes, pgCharByte simply returns the character size.

## Control characters don't draw

OpenPaige makes the assumption that all control characters (less than ASCII space) should be "invisible." Rightly or wrongly, this is the default behavior we chose to avoid drawing unwanted, garbage characters. Hence, if you insert a "command" char (ASCII 17) it will be drawn as a blank.

The correct workaround is to override OpenPaige's default character handling in this one special case. This is not as difficult or complex as it may first seem and I will illustrate the exact code you need to implement:

Right after pglnit, you need to place a function pointer in OpenPaige globals default style "hook" for getting character info. This function pointer will point to some small code that you will write (which I will show you). Let's suppose your openPaige globals is called "paigeGlobals" and this (new) function you will write is called "CommandCharlnfo." Right after pglnit, you do this:

paigeGlobals.def_style.procs.char_info = CommandCharInfo;

This sets "CommandCharInfo" as the "default" function for all future styles, and OpenPaige calls that function to find out about a character of text. The CommandCharlnfo function definition must look like the function example below. This function's main duty in life is to tell openPaige that the command character is NOT blank, otherwise it just falls through and calls the standard charlnfo function:

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-427.jpg?height=866&width=1176&top_left_y=448&top_left_x=434)

For more information on char info proc see "char_info_proc" on page 27-498.

Many applications that want to display the Command Character may want to display other special characters, so I thought you
might prefer including something like the attached code in your examples as an alternative to the above.

## PG_PASCAL (long) CommandCharInfo(paige_rec_ptr pg,

style_walk_ptr style_walker, pg_char_ptr data, long global_offset, long local_offset, long bits)

\{

switch(data[local_offset])

\{/include <Fonts.h>

case commandMark:

case checkMark:

case diamondMark:

case appleMark:

return (bits \&

(WORD_BREAK_BIT|WORD_SEL_BIT));

\}

## return

\}

pgCharInfoProc(pg,style_walker,data,global_offset,local_offset,bits);

### 24.7 Finding The Boundaries (word, line or paragraph)

(void) pgFindWord (pg_ref pg, long offset, long PG_FAR *first_byte, long

PG_FAR *last_byte, pg_boolean left_side, pg_boolean smart_select);

(void) pgFindCtIWord (pg_ref pg, long offset, long PG_FAR *first_byte, long PG_FAR *last_byte, short left_side);

(void) pgFindPar (pg_ref pg, long offset, long PG_FAR *first_byte, long PG_FAR *last_byte);

(void) pgFindLine (pg_ref pg, long offset, long PG_FAR *first_byte, long PG_FAR *last_byte);

NOTE: The term "find" in these functions does not imply a context search; rather, it refers to locating the bounding text positions at the beginning and ending of a section of text.

These function can be used to locate words, paragraphs, or lines.

For all functions, the offset parameter should indicate where to begin the search. This is a byte offset, beginning at zero.

For pgFindWord, ${ }^{*}$ first_byte and *last_byte will get set to the nearest word boundary beginning from offset.

NOTE: *first_byte can be less than offset, but *last_byte will always be equal to or greater than offset. If left_side is TRUE, the word to the immediate left is located if offset is not currently in the middle of a word.

For example, suppose the specified offset sat right after the word "the" and before a ".". If left_side is FALSE, the "word" that is found would be "." but if left_side is TRUE, the word found would be "the".

The smart_select parameter tells pgFindWord whether or not to include trailing blank characters for the word that has been found. If smart_select is TRUE, then trailing blanks ("spaces") that follow the word are included. Example: If the text contained "This is a test for find word," if smart_select is TRUE then finding the word "test" will return the offsets for "test " (where "-" represent spaces).

The pgFindCt/Word function works exactly the same as pgFindWord except "words," in this case, are sections of text separated by control codes such as tab and CR. ("Control codes" is used here to explain this function, but in actuality a character is considered only a "control" char by virtue of what is returned from the char_info_proc —- bsee “Customizing OpenPaige” on page 27-485.

The pgFindPar and pgFindLine return the nearest paragraph boundaries or the nearest line boundaries to offset, respectively.

### 24.8 Line and Paragraph Numbering

NOTE: IMPORTANT NOTE: The attribute bit COUNT_LINES_BIT must be set in the $p g \_r e f$ for any of the following functions to work. This attribute can be set either by including it with other bits in the flags parameter for $p g N e w$, or can be set with $p g$ SetAttributes.

CAUTION: Constantly counting lines and paragraphs, particularly within a large document with wordwrapping enabled and complex style changes can consume considerable processing time. Hence, the COUNT_LINES_BIT has been provided to enable line counting only for applications that truly need this feature.

Line Numbering

(long) pgNumLines (pg_ref pg);

Returns the total number of lines in $p g$. This function will return zero if COUNT_LINES_BIT has not been set in $p g$ (see IMPORTANT NOTE above).

NOTE: A "line" in an OpenPaige object is simply a horizontal line of text which may or may not end with a CR or LF character. If wordwrapping has been enabled, a line can terminate either because it wordwrapped or because it ended with CR.

CAUTION: WARNING: This function may consume a lot of time if the document is relatively large and has not been paginated to the end of the document. This is because OpenPaige cannot possibly know how many wordwrapping lines exist unless it computes every line in the document from beginning to end; even if wordwrapping is disabled, OpenPaige must still count all the line breaks (CR characters) if text has recently been inserted. NOTE that OpenPaige will always take the fastest approach wherever possible, e.g. if the document has already been fully paginated this function will return a relatively instant response.

$$
\begin{aligned}
& \text { (long) pgOffsetToLineNum (pg_ref pg, long offset, pg_boolean } \\
& \text { line_end_has_precedence); }
\end{aligned}
$$

Returns the line number that contains offset text position. The line number is one-based (i.e., the first line in $p g$ is " 1 "). The offset parameter can be any position from 0 to $p g$ TextSize( $p g)$, or CURRENT_POSITION for the current insertion point.

This function will always return at least one line even if the document has no text (since an empty document still has one line albeit blank).

If line_end_has_precedence is TRUE, then the line number to the immediate left of offset is returned in situations where that offset is on the boundary between two lines.

NOTE: The only time this happens is when the specified offset is precisely at the end of wordwrapping line and there is another line below that.

For example, consider the insertion point within the following two lines:

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-431.jpg?height=145&width=1158&top_left_y=1491&top_left_x=550)

NOTE: In the example above, the "|" point text offset could be interpreted to be the end of the first line OR the beginning of the next line. Since OpenPaige can't possibly know which one is desired, the line_end_has_precedence parameter has been provided. From the above example, if line_end_has_precedence is TRUE then the first line would be returned, otherwise the second line would be returned.
(void) pgLineNumToOffset (pg_ref pg, long line_num, long *begin_offset, long *end_offset);

Returns the text offset(s) of line_num line. The line_num parameter is assumed to be 1 -based (i.e., the first line of text is " 1 " and not " 0 ").

The beginning text position of the line is returned in *begin_offset end the ending position is returned in *end_offset; both values will be zero-based (first position of text is zero). Either begin_offset or end_offset can be a null pointer, in which case it is ignored.

Paragraph numbering

(long) pgNumPars (pg_ref pg);

Returns the total number of paragraphs in $p g$. This function will return zero if COUNT_LINES_BIT has not been set in $p g$ (see IMPORTANT NOTE at the top of this section).

NOTE: A "paragraph" in an OpenPaige object is simply a block of text that terminates with a CR character (or CR/LF), or the last (or only) block of text in the document. This has nothing to do with wordwrapping, and in fact if wordwrapping has been disabled then a line and paragraph are one and the same (since lines would only break on a CR character).

(long) pgOffsetToParNum (pg_ref pg, long offset);

Returns the paragraph number that contains offset text position. The paragraph number is one-based (i.e., the first paragraph in pg is " 1 "). The offset parameter can be any position from 0 to $p g$ TextSize(pg), or CURRENT_POSITION for the current insertion point.

This function will always return at least one paragraph even if the document has no text (since an empty document still has one "paragraph" albeit empty).

(void) pgParNumToOffset (pg_ref pg, long par_num, long *begin_offset, long *end_offset);

Returns the text offset(s) of par_num paragraph. The par_num parameter is assumed to be 1 -based (i.e., the first paragraph of text is " 1 " and not " 0 ").

The beginning text position of the paragraph is returned in *begin_offset end the ending position is returned in *end_offset; both values will be zero-based (first position of text is zero). Either begin_offset or end_offset can be a null pointer, in which case it is ignored.

NOTE: The ending offset of a "paragraph" will be the position after its CR character (or the end of text if last or only paragraph in the document).

## Line and paragraph bounds

(void) pgLineNumToBounds (pg_ref pg, long line_num, pg_boolean want_scrolled, pg_boolean want_scaled, line_end_has_precedence, rectangle_ptr bounds);

Returns the bounding rectangular area that encloses line_num line. The line number is assumed to be 1 -based (first line in $p g$ is " 1 " and not " 0 ").

The bounding rectangle is returned in * bounds (which must not be a null pointer).

If want_scrolled is TRUE, the bounding rectangle will be offset to reflect the current scrolled position of $p g$, if any; if want_scaled is TRUE the bounding rectangle will be scaled to $p g$ 's current scaling factor, if any.

NOTE: The width of the rectangle that is returned will be the width of the text in the line, which is not necessarily the width of the visible area nor is it necessarily the same as the document's page width; the line width can also be zero if the line is completely empty.

(void) pgParNumToBounds (pg_ref pg, long par_num, pg_boolean want_scrolled, pg_boolean want_scaled, rectangle_ptr bounds);

Returns the bounding rectangular area that encloses par_num paragraph. The paragraph number is assumed to be 1-based (first paragraph in $p g$ is " 1 " and not " 0 ").

The bounding rectangle is returned in ${ }^{*}$ bounds (which must not be a null pointer).

If want_scrolled is TRUE, the bounding rectangle will be offset to reflect the current scrolled position of $p g$, if any; if want_scaled is TRUE the bounding rectangle will be scaled to $p g$ 's current scaling factor, if any.

NOTE: The width of the rectangle that is returned will be the width of all composite lines within the paragraph, which is not necessarily the width of the visible area nor is it necessarily the same as the document's page width; the paragraph width can also be zero if the paragraph is completely empty.

Getting pixel height between lines

What's the best way to calculate the pixel height of the text between given startline and endline? (replacing TEGetHeight (endLine, startLine, macTE)).

The easiest approach depends on how you are currently determining the text location of these two "lines." In your question you mention copying the lines to another pg ref. But how did you figure out where the boundaries are of these two lines?

I will assume that you already know the text offset position for the start of each line. In this case, you can simply use "pgCharacterRect" for each text position and subtract the first rectangle's top from the second rectangle's bottom, which would be the line height difference between them.

Another method which is not as fast (but is certainly faster than your chosen method of copying the text into a temp pg_ref) is to make a temporary highlight region for the text range, then get the enclosing bounds rect for the highlight. To get a highlight region, use "pgGetHiliteRgn" (you also have to know the text positions for each line). The way this function works is that you first create a shape (using pgRectToShape(\&pgm_globals, NULL)) and passing that shape to pgGetHiliteRgn. Then to get the "bounds" area of the shape, you use pgShapeBounds(shape, \&rectangle).

### 24.9 Character type

(long) pgFindCharType (pg_ref pg, long char_info, long PG_FAR *offset, pg_char_ptr the_byte);

This function locates the first character in $p g$ that matches char_info, beginning at byte offset *offset.

The char_info parameter should be set to one or more of the character info bits as explained for char_info_proc. See "Information about a particular character" on page 24-422.

For example, to search for a "return" character, you would pass PAR_SEL_BIT for char_info (which will locate a character that can break a paragraph).

If the_byte pointer is nonnull, the character located, if any, gets placed into the buffer it points to.

CAUTION: You must be sure that the character found will fit into the buffer, since characters in OpenPaige can be more than one byte. If you aren't sure, then pass a null pointer for the_byte until you get the information about the character, then make another call to get the data.

FUNCTION RESULT: The complete character type is returned (all the appropriate char_info_proc bits will be set). The offset parameter will be updated to the byte offset for the character found.

However, if this function returns with ${ }^{*}$ offset the same value as the text size in $p g$, the requested character was not found.

### 24.10 Change counter

(long) pgGetChangeCtr (pg_ref pg);

(long) pgSetChangeCtr (pg_ref pg, long ctr);

OpenPaige maintains a "changes made" counter which you can use to detect changes made to the object; for every change made (insertions, deletions, style changes, etc.), the change counter is incremented. Additionally, a pgUndo will decrement the counter.

This counter begins at zero when a new $p g \_r e f$ is created; to get the counter, call $p g G e t C h a n g e C t r$. To set it, call pgSetChangeCtr with $c t r$ as the new value.

When does change counter change?

I'm using this counter to tell myself whether I need to resave the document. Why is the count different from what I expect?

This counter is changed by OpenPaige anytime IT thinks it needs to be changed. It changes for EVERYTHING. We use our own change counter in the demo to keep track of when we need to resave the document.

I suggest that you may want to keep your own change counter.
(void) pgTextRect (pg_ref pg, select_pair_ptr range, pg_boolean want_scroll, pg_boolean want_scaled, rectangle_ptr rect);

(void) pgCharacterRect (pg_ref pg, long position, short want_scrolled, short want_scaled, rectangle_ptr rect);

These functions can be used to compute outline(s) around one or more characters.

For $p g T e x t R e c t$, a rectangle is returned in rect that exactly encloses the text range in range. If want_scroll is TRUE, the rectangle is "scrolled" to the location where it would appear on the screen, otherwise it remains relative to pg's top-left of page_area. If want_scaled is TRUE, the rectangle is scaled to the scale factor set in $p g$.

To get the rectangle surrounding a single character, call pgCharacterRect which does exactly the same thing as $p g$ TextRect except you give it a single byte offset.

(long) pgPtToChar (pg_ref pg, co_ordinate_ptr point, co_ordinate_ptr offset_extra);

FUNCTION RESULT: This function returns the (byte) offset of the first character that contains point. If offset_extra is non-null, the point is first offset by that much before the character is located.

OpenPaige computes the smallest rectangle that will fit around all text when you call:

(void) pgMaxTextBounds (pg_ref pg, rectangle_ptr bounds, pg_boolean paginate);

Returns the smallest bounding rectangle pointed to in bounds that encloses all the text in $p g$. The bounds parameter must point to a rectangle and can't be a null pointer.

The dimensions of bounds essentially gets set to the top of the first line for the rectangle's top, the line furthest to the left and right for the rectangle's left and right sides, and the furthest line to the bottom for the rectangle's bottom.

If paginate is TRUE then OpenPaige will repaginate the document if necessary to render the most accurate possible dimensions.

NOTE: When paginate is TRUE the pagination can be slower, but if you pass "FALSE" you won't always get an accurate measurement.

CAUTION: Paginating a large document can consume a lot of time. However, the only way OpenPaige can possibly return exact dimensions is if every line has been calculated from top to bottom.

How to call pgMaxTextBounds

rectangle bounds; long doc_width;

pgMaxTextBounds(pg, \&bounds, TRUE);

doc_width = bounds.bot_right.h - bounds.top_left.h;

The doc_width in the above example would be the width of the WIDEST text line (from the left margin to the right side of the last character).

I want to set an ever expanding page_area that grows as the user types, but only if I need to. How and when should I do that with pgMaxTextBounds?

As for changing the page area of the pg_ref, yes you should use pgSetAreas and/or pgSetAreaBounds -- but only when it really changes and/or only when you physically want to expand it.

To answer your question as to WHEN you figure out the doc width, I would not do it every key insertion (you are right, that would be VERY slow particularly when text gets fairly large). The best way to detect the document's height has grown is to examine a field inside the pg_ref called "overflow_size". This field gets set by OpenPaige when/if one or more characters have flowed below the bottom of your page area.

However, for this feature to work you need to set CHECK_PAGE_OVERFLOW with pgSetAttributes2(). By setting this attribute, openPaige will check the "character overflow" situation after every operation that can cause text to change.

So after anything that might cause an overflow (which would notify the need to change the page rectangle), check overflow_size as follows:

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-439.jpg?height=510&width=686&top_left_y=1198&top_left_x=434)

In the above example, the function result is the number of character(s) that overflow the bottom of the page rectangle. If overflow_size is -1 , the text overflows the bottom only by a single CR character (i.e. blank line).

### 24.13 Unique value

This function obtains a unique ID value unique within a $p g \_r e f$.

(long) pgUniqueID(pg_ref pg);

This simply returns a number guaranteed to be unique ("unique" compared to the previous response from $p g$ UniqueID).

This function simply increments an internal counter within pg and returns that number, hence each response from $p g$ UniqueID is "unique" from the last response. The very first time this function gets called after $p g N e w$, the result will be "1."

The intended purpose of this function is to place something in a style_info or par_info record to make it "unique" so it will be distinguished from all other style runs in $p g$. Other than that, this function is rarely used by an application.

For example, if an application applied a customized style to a group of characters, as far as OpenPaige is concerned that style might look exactly like the style(s) surrounding those characters; since OpenPaige will automatically delete redundant style runs, customized styles generally need to place something in one of the style_info fields to make it "unique."
\#include "MemMgr.h"

(void) pgFillBlock (void PG_FAR *block, long block_size, pg_char value);

pgFillBlock fills a memory block of block_size byte size with byte value in pg_char parameter)

### 24.15 Splitting a long byte

\#include "pgUtils.h"

(short) pgLoWord(long value);

(short) pgHiWord(long value);

It is often necessary to split a long into two shorts. This is a cross platform way of doing exactly that. The low word returns the least significant short, the high word returns the most significant.

## High word Low word

## $0 \times \longdiv { 1 2 3 4 5 6 7 8 }$

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-442.jpg?height=304&width=967&top_left_y=293&top_left_x=432)

pgAbsoluteValue — returns an absolute value.

pgRoundFixed — rounds the fixed number to the nearest whole (but is still a pg_fixed). For example, 0x00018000 will return as 0x00020000.

pgMultiplyFixed - multiplies two fixed decimal numbers (a fixed decimal is a long whose high-order word is the integer and low-order word the fraction. Hence, $0 x 00018000=1.5)$.

pgDivideFixed — divides fixed number fix1 into fix2 (a fixed decimal is a long whose high-order word is the integer and low-order word the fraction. Hence, $0 x 00018000=$ 1.5).

pgFixedRatio - returns a fixed number which is the ratio of $\mathrm{n} / \mathrm{d}$ (a fixed decimal is a long whose high-order word is the integer and low-order word the fraction. Hence, 0x00018000 = 1.5).

## THE Allocation MANAgER

This section deals exclusively with the Allocation Manager within OpenPaige (the portion of software that creates, manages and disposes memory allocations).

### 25.1 Up \& Running

The Allocation Manager used by OpenPaige is full of features that have been requested by developers and used by OpenPaige itself. All these features are available to you as a developer.

Like most of OpenPaige, there are just some basics to know about the Allocation Manager to initially use it effectively.

These are:

1. To allocate a block of memory, you call a function that returns an "ID" code (not a pointer or an address).
2. Then to access that memory, you pass the "ID" code to a function which returns an address to that memory.
3. Once you are through accessing that memory, you report its "non-use" by calling another function.
4. "Reporting" to the allocation manager when you are accessing a memory block and when you are through makes virtual memory possible (blocks can be purged that are not in use).
5. Memory allocations do not have to be byte-oriented, rather they can be groups of logical records. For example, a memory allocation can be defined as a group of 100-byte records.

Simple example to allocate some memory and use it.

## / *Allocation: *

memory_ref allocation; allocation = MemoryAlloc(\&mem_globals, sizeof(char), 100, 0);

/* Note: "mem_globals" is the pgm_globals field in the OpenPaige globals, same struct given to pglnit and pgNew.

The "allocation" result is not an address, but an "ID" code. To get the address, call: */

char *memory_address;

memory_address = UseMemory(allocation);

I* In the above, not only is the memory addressed returned but the memory is now locked and unpurgable. Thus it is important to "report" when you are through accessing it: */

UnuseMemory(allocation);/* Tell allocation mgr we are done. */

// Once you are completely through, dispose the allocation:

DisposeMemory(allocation);

Since OpenPaige is intended to operate on multiple platforms, it became necessary to remove the majority of its code as far away from a specific operating system as possible.

An integral part of any computer OS is its memory management system. However, no two memory management designs are alike, and for this reason OpenPaige's Allocation Manager works as follows:

1. OpenPaige only creates memory allocations through high-level functions, far removed from the operating system. Among these functions are MemoryAllocate, MemoryDuplicate and MemoryCopy.
2. Regardless of platform, functions to allocate memory remain constant (the same function names and parameters are the same regardless of the OS).
3. To allow for virtual memory and debugging features, OpenPaige must inform the Allocations Manager, as a rule, when it is about to access a block of memory and when it is through accessing that block. The purpose of this is threefold:

(C) If no part of OpenPaige is accessing a memory block, the Allocation Manager can "unlock" the block and allow it to relocate for maximum memory efficiency,

(D) Blocks of memory can be temporarily purged if they are not being accessed.

(E) Debugging features can be implemented: since the main software must "ask" for access to a block of memory, the Allocation Manager can check the validity of the block at that time (when running in "debug mode").

4. Since memory is never allocated directly, the Allocation Manager can provide additional features to a block of memory. Among the features that exist in OpenPaige's Allocation Manager are logical record sizes (a block of memory can be an array of records, as opposed to bytes), nested "lock memory" capability (more than one function can "lock" a block from relocating or purging, in which case the block can not be free for relocation or purging until each "lock" has been "unlocked").

### 25.3 Memory Block References

As far as OpenPaige (and your application) is concerned, when memory is allocated the Allocation Manager does not return a memory address; rather, it returns an "I.D." number called a memory_ref. You can consider a memory_ref as simply a long word
whose value, when given later to the Allocation Manager, will identify a block of memory.

### 25.4 Access Counter

Frequent reference is made in this chapter to a memory reference's access counter.

Every block of memory created through the Allocation Manager has an associated access counter. This counter increments every time your program requests the block to become locked (non-relocatable and non-purgeable), and decrements for every request to unlock the block (making it re-locatable and purgeable). The purpose of this is to allow nested "lock/unlock" logic as opposed to a simple locked or unlocked state: using the access counter method, Allocation Manager will make a block relocatable or purgeable only when its access counter is zero. This provides protection against memory blocks moving "out from under" nested situations.

### 25.5 Logical vs. Physical Sizes

Every allocation made through the Allocation Manager is considered to have two sizes: a logical size and a physical size. (For how this is implemented, see "The extend_size parameter" on page 25-447).

The physical size of a block is the actual amount of reserved memory that has been allocated, in bytes; the logical size, however, may or may not be the same amount and in fact is often smaller.

The physical size of an allocation might be, for example, $10 \mathrm{~K}$ but its logical size might be as small as zero. The purpose of the two-size distinction is speed and performance. Depending on the OS, physically resizing a block of memory can consume large amounts of time, particularly in tight situations where thousands of blocks require relocation or purging just to append additional memory to one block. For this reason, the Allocation Manager may elect to allocate a block larger (physical size) than what you have asked for but "tell" you it is a smaller size (logical size); then if you asked for that block to be extended to a large size, the extra space might already exist, in which case the

Allocation Manager merely changes its logical size without any need to expand the block physically.

Generally, it is a block's logical size — pnot physical size —-byour program should always work with.

### 25.6 Purged Blocks

All references in the Chapter to "purging" and "purged blocks" imply virtual memory, in which a block's contents are saved to a scratch file so that the allocation can be temporarily disposed. Such allocations are not lost, rather they recover on demand by reloading from the scratch file. At no time does the Allocation Manager permanently dispose an allocation unless you explicitly tell it to do so.

### 25.7 Starting Up

The Allocation Manager must have already been started before $Y$ was called. You need to make any function calls to initialize this portion of the software. To start OpenPaige with the Allocation Manager and for details on pgMemStartup see "Software Startup" on page 2-10.

CAUTION: You must not, however, use any functions listed below unless you have called pgMemStartup.

NOTE: You can theoretically use the Allocation Manager, by itself, without ever initializing OpenPaige.

To allocate memory through the Allocation Manager, call one of the following:

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-448.jpg?height=201&width=1217&top_left_y=586&top_left_x=434)

MemoryAlloc allocates a block of memory and returns a memory_ref that identifies that block; MemoryAllocClear is identical except it clears the block (sets all bytes to zero).

By allocation is meant a block of memory of some specified byte size that becomes reserved exclusively for your use, guaranteed to remain available until you de-allocate that block (using DisposeMemory, below).

Both functions return a memory_ref, which is a reference "I.D." to the allocation. You should not consider a memory_ref as an address, nor a pointer. Rather, give this reference to the various functions listed below to get a pointer to the memory block, change its allocation size, make it purgeable or nonpurgeable, etc.

The memory_refreturned is always nonzero if it succeeds or MEM_NULL (zero) if it fails. The easiest way to check for failures is by using OpenPaige's try/catch exception handling. See "The TRY/CATCH Mechanism" on page 26-1 and the example, "Creating a memory_ref" on page 26-6.

The globals parameter must point to the mem_globals you gave to pgMemStartup. Or, if you have initialized OpenPaige with pgInit() you can also access mem_globals through the OpenPaige globals:

paige_globals.mem_globals;

The size of the allocation is determined rec_size * num_recs, where rec_size is a record size, in bytes, and num_recs is the number of such records in the block. Hence, you can create allocations that are considered arrays of records, if necessary.

For example, allocating a block of rec_size $=16$ and num_recs $=100$, the total byte size of the allocation would be 1600. The intended purpose of allowing a record size, as opposed to always creating blocks consisting of single bytes is to provide high-level features of accessing record elements.

If you only want a block of bytes, without regard to any "record" size, simply create an allocation with a "record" size of 1.

A rec_size of zero are not allowed; a num_recs value of zero, however, is allowed.

The extend_size parameter

The purpose of the extend_size parameter is to provide the Allocation Manager with some insight, for performance purposes, as to how large the allocation might grow from subsequent SetMemorySize calls.

To understand this fully, a distinction between a memory_ref's "logical size" versus "physical size" must be clarified: when a memory_ref is initially created, its logical size is simply the size that was asked for (which is rec_size * num_recs). However, the actual size allocation can be greater than the logical size which essentially provides an extra "buffer" that can be utilized to change the logical size later without the necessity to physically resize the allocation through OS calls.

A good example of this would be the allocation a large string whose initial byte size begins at zero, yet it is expected to grow larger in size as time goes by, perhaps as large as 500 bytes in length. If such a memory allocation started at a physical byte size of zero, then it would become necessary (and very slow) to ask the OS to physically resize the allocation each and every time new byte(s) were appended.

However, if such an allocation were initially created with a 500-byte extend_size (but a logical size of zero), it would never need to physically resize unless or until the string grew larger than 500 bytes.

Hence, MemoryAlloc could create such an allocation whereby the physical size and logical size are different:

MemoryAlloc(\&mem_globals, sizeof(byte), 0, 500);

The extend_size is therefore an enhancement tool, and should be set to a reasonable amount according to what the resizing forecast holds for that allocation. If the allocation will never be resized, extend_size should be zero.

NOTE: The value of extend_size does not necessarily imply future memory resizing will occur or will not occur, rather it is a performance variable only: the Allocation Manager will still resize a memory allocation even if extend_size is zero (although possibly slower than if extend_size were larger).

See also “Logical vs. Physical Sizes” on page 25-444.

NOTE: The extend_size indicates a number of records (each of size rec_size bytes), not bytes.

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-451.jpg?height=859&width=1538&top_left_y=398&top_left_x=183)

De-Allocation

Once you no longer need a memory allocation, pass its memory_ref to the following:

void DisposeMemory (memory_ref ref);

DisposeMemory physically disposes the block assigned to ref, and ref is no longer a valid reference thereafter.

$$
\begin{aligned}
& \text { (memory_ref) MemoryDuplicate (memory_ref src_ref); } \\
& \text { (void) MemoryCopy (memory_ref src_ref, memory_ref target_ref); }
\end{aligned}
$$

MemoryDuplicate returns a new memory_ref whose data content and record size is exactly the same as src_ref. In effect, this function returns a "clone" of src_ref, but it is a new, independent memory_ref.

MemoryCopy copies the contents of src_ref into target_ref.

MemCopy differs from MemoryDuplicate in that for MemCopy both src_ref and target_ref are allocations that already exist. MemoryDuplicate actually creates a new memory_ref for you, so it cannot already exist.

The logical size of target_ref can be any size, even zero, as MemoryDuplicate will change its size as necessary. Record sizes of each memory_ref, however, must match.

The access counters are not set, since the memory is allocated but not in use.

## TECH NOTE

What is the difference really between "MemoryCopy" and MemoryDuplicate"?

Please do comment on the appropriate situation for using "Memorycopy" and "MemoryDuplicate".

I can clarify the difference in usage between MemoryCopy and MemoryDuplicate, it is very simple:

MemoryDuplicate is to obtain a "clone" of a memory_ref.

MemoryCopy is to FILL IN an already existing memory_ref with the contents of another.

In my own code, I am constantly wanting to copy contents of a memory_ref into one that I created earlier. One example of this is some routine that wants to keep copying a bunch of different memory refs -- to copy an array of "tabs" for instance. It would be a lot slower to create a memory ref with MemoryDuplicate, then dispose it, then create it again, then
then keep copying different memory_refs into it.

### 25.10 Accessing Memory

## Using memory

To obtain a pointer to a block of memory allocated from MemoryAlloc or MemoryAllocClear, call one of the following:

(void PG_FAR*) UseMemory (memory_ref ref); (void PG_FAR*) UseForLongTime (memory_ref ref);

UseMemory and UseForLongTime takes a memory_ref in ref and returns a pointer to the memory block assigned to that reference. The ref's access counter is incremented, which means that the memory block is now guaranteed to neither relocate nor purge (see “Access Counter" on page 25-444).

The primary purpose of UseMemory is to tell the Allocation Manager that a particular block of memory is now "in use," in which case it is marked as unpurgeable and nonrelocatable.

UseForLongTime does exactly the same thing as Use M e mo ry except the memory block is relocated in the most optimum way, before locking, to avoid memory fragmentation. The purpose of using this function as opposed to UseMemory is for situations where you know the block will stay locked for quite a while and you don't want to cause unreasonable fragments.

NOTE: Don't use UseForLongTime too liberally because it is substantially slower than UseMemory (since the machine often needs to relocate the memory).

UseMemory and UseForLongTime calls can be nested, but each must be eventually balanced with UnuseMemory or else the block will remain in a locked state, which in
turn can cause memory difficulties such as fragmentation and the inability to change the allocation size.

memory_ref my_record_ref

## UseMemory

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-454.jpg?height=129&width=294&top_left_y=615&top_left_x=661)

## UnuseMemory

can be purged to hard disk when not in use and drawn back in on next UseMemory a fter UnuseMemory my_record_ptr no longer guāranteed to be valid!

Unuse memory

Once you are finished using the pointer returned from UseMemory or UseForLongTime, call the following:

(void) UnuseMemory (memory_ref ref);

Essentially, UnuseMemory decrements ref's access counter. If its access counter goes to zero, the allocation is then free to relocate or purge.

It is therefore important that all UseMemory and UseForLongTime calls get eventually balanced with UnuseMemory, otherwise unwanted locked memory fragments will result.

## The main program

my_record_struct *my_record_ptr; memory_ref my_record_ref $=$ MemoryAlloc ( $\&$ globals, size of(my_rec ord_struct), 3, 1);

## A procedure

my_record_ptr=UseMemory(my_record_ref);

// my_record_ptr is valid, locked pointer to the data UnuseMemory(my_record_ref);

DisposeMemory(my_record_ref);

You can obtain a pointer to a specific "record" within a block of memory by calling the following:

(void PG_FAR*) UseMemoryRecord (memory_ref ref, long wanted_rec, long seq_recs_used, short first_use);

This function is similar to UseMemory except a pointer to a specified record of an allocation is returned.

The wanted_rec is the record number you need a pointer to (first record is zero). The record size (originally defined in rec_size for MemoryAlloc) determines which physical byte the resulting pointer will reference. For instance, if the record size were, 128 bytes, a UseMemoryRecord for wanted_rec of 10 would return a pointer to the 128*10th byte.

The seq_recs_used parameter should indicate how many additional sequential records beyond wanted_rec record you want access to. The purpose of this parameter is for future Allocation Manager enhancements in which partial block(s) can be loaded into memory from a purged state. In such as case, UseMemoryRecord needs to know how many additional sequential records, besides wanted_rec, you would like to have loaded into memory if the allocation has been purged.

For example, suppose a block of memory consisting of 1000 records is temporarily purged (which really means its contents have been saved to a "scratch" file and the block currently does not physically exist in memory). A for full access to all records would require the Allocation Manager to load the entire allocation (all 1000 records). UseMemoryRecord, however, could get away with loading only a few records within that allocation, but it needs to know how many sequential records you intend to access beyond wanted_rec.

If you want to use all records following wanted_rec, whatever that quantity might be, you can also pass USE_ALL_RECS (value of -1) for seq_recs_used.

If first_use is TRUE, the block's access counter is incremented (same thing that occurs from a UseMemory call); if first_use is FALSE, the access counter remains unchanged. The purpose of this parameter is for situations where you intend to randomly access many records from the same memory_ref within the same routine, but you
essential need only one UseMemory to lock the allocation; otherwise, you would need to balance every random access with UnuseMemory.

Thus, setting first_use to TRUE is essentially sending the Allocation Manager the message, "Please lock the allocation," then subsequent UseMemoryRecord calls with first_use as FALSE is saying "I know the allocation is already locked, so just give me another pointer."

An UnuseMemory call must eventually balance each UseMemoryRecord call that gave TRUE for first_use.

NOTE: OpenPaige loads the whole allocation specified in UseMemoryRecord (does not do partial loads). However, to guarantee future compatibility, you should assume that all records in the allocation lower than wanted_rec and all records greater than wanted_rec + seq_recs_used are purged, not loaded, and therefore not valid should you attempt to access them with the same pointer.

### 25.12 "Quick Record"

If you simply want a copy of a single record from an allocation, call the following:

void GetMemoryRecord (memory_ref ref, long wanted_rec, void PG_FAR *record);

Record number wanted_rec (the first record is zero) in ref is copied to the structure pointed to by record. The access counter in ref is unchanged. Hence, GetMemoryRecord provides a way to get a single record without the need to balance UseMemory / UnuseMemory.

It is your responsibility to make sure record is sufficient size to hold a record from ref.

NOTE: GetMemoryRecord will work correctly regardless of the memory_ref's access counter state and regardless of whether or not the allocation has been purged.

### 25.13 Changing Allocation Sizes

Memory sizes

(void) SetMemorySize (memory_ref ref, long wanted_size);

(long) GetMemorySize (memory_ref ref);

SetMemorySize changes the logical size of ref to wanted_size. GetMemorySize returns the logical size of ref.

For both functions, the "size" is not a byte size, but rather a record quantity. A SetMemorySize $(r e f, 10)$ for an allocation whose record size is 500 bytes, the allocation is set to 5000 bytes, i.e. $10 * 500$; SetMemorySize(ref, 10) for a 1-byte record size allocation would result in a logical byte size of 10 , and so on.

If SetMemorySize fails for any reason, an OpenPaige exception is raised (see “Exception Handling" on page 26-1).

GetMemorySize returns the current size of ref (number of records within ref).

Changing the size of an allocation whose access counter is nonzero might fail! (A nonzero access counter means sufficient UnuseMemory calls have not been made to balance UseMemory calls, resulting in a locked allocation).

NOTE: GetMemorySize will work correctly regardless of the memory_ref's access counter state and regardless of whether or not the allocation has been purged.
(short) GetMemoryRecSize (memory_ref ref);

(long) GetByteSize (memory_ref ref);

GetMemoryRecSize returns the record size in ref (which will be whatever size you gave MemoryAlloc or MemoryAllocClear when the allocation was made). This function is useful for generic functions that need to know a memory_ref's record size.

GetByteSize returns the byte size of ref (as opposed to the number of records as in GetMemorySize). Essentially a memory_ref's byte size is its record size times number of logical records.

NOTE: Both functions above will work correctly regardless of the memory_ref's access counter state and regardless of whether or not the allocation has been purged.

Can I make my record size bigger after it is allocated?

No. You can only change the number of records. You can create a new memory_ref and copy the old data into the new one using pgBlockMove.
(void PG_FAR*) InsertMemory (memory_ref ref, long offset, long insert_size);

(void PG_FAR*) AppendMemory (memory_ref ref, long append_size, pg_boolean zero_fill);

InsertMemory — inserts insert_size records into ref's allocation at record position offset, then returns a pointer to the first record inserted. The new record(s) are not initialized to anything — the allocation size is simply increased by insert_size and record(s) are moved to make room for the insertion.

AppendMemory - does the same thing except the "insertion" is added to the end of the memory block: the allocation is increased by append_size and a pointer to the first record of the appendage is returned. If zero_fill is TRUE, the appended memory is cleared to zeros.

Both InsertMemory and A p p e nd M e mo ry - assume record quantities, not byte sizes (i.e., InsertMemory for a ref whose record size is 100 will insert 200 bytes if insert_size $=2$ ).

For both functions, the access counter in ref is incremented (or not) according to the following rules:

- if ref's access counter is zero upon entry, the requested memory is inserted, the access counter is incremented by 1 and the allocation is set to its "used" state (locked, unpurgeable);
- if the access counter is 1 upon entry, it is decremented to zero and unlocked, the requested memory is inserted, then the access counter is incremented and the allocation is set to its "used" state;
- if the access counter is greater than 1 , nothing occurs and the situation is considered illegal, generating an error if debugging has been enabled (see "Debug Mode" on page 25-465).

The reasoning behind these rules for the access counter when inserting memory is the common situation where multiple insertions need to occur within a loop. Since InsertMemory and AppendMemory allow an access counter of " 1 ," each repetitive insertion can avoid the requirement of calling UnuseMemory.

CAUTION: For insertions with an access counter of " 1 ", the pointer you had prior to InsertMemory or AppendMemory might be invalid after memory has been inserted (the block might relocate). Therefore, always update your pointer with whatever is obtained from the function result.

UnuseMemory after InsertMemory or AppendMemory

So do I need to do a UnuseMemory after these?

InsertMemory and AppendMemory really are the same as SetMemorysize to a larger number of records, then a (single) UseMemory. So you need to do a single UnuseMemory() after a series of repetitive of Inserts. In other words, if you called InsertMemory() or AppendMemory() 100 times, you only need to do ONE UnuseMemory().

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-461.jpg?height=544&width=815&top_left_y=1138&top_left_x=433)
(void) DeleteMemory (memory_ref ref, long offset, long delete_size);

DeleteMemory deletes delete_size records in refbeginning at offset. Both delete_size and offset are record quantities, not bytes.

The access counter is not changed by this function. However, the access counter must be zero when this function is called.

### 25.15 Purging Utilities

NOTE: All references in the Chapter to "purging" and "purged blocks" imply virtual memory, in which a block's contents are saved to a scratch file so that the allocation can be temporarily disposed. If the scratch file was not set up when the Allocation Manager was initialized, there will be no purging.

NOTE: $t$ is not to be confused with "purging" resources on the Mac. Allocation Manager knows nothing about the Mac other than basic things about the file system. It handles it own "purging" without the Resource Manager.

An allocation is said to be "purged" when additional memory space needs to be freed, thus an allocation is saved to a "scratch" file and it is temporarily disposed.

The priority for purging, i.e., what memory_refs should get purged first can be controlled by calling the following:

(void) SetMemoryPurge (memory_ref ref, short purge_priority, pg_boolean no_data_save);

The refallocation's purging priority is set to purge_priority. The purge_priority can be any number between 0 and 255, with 0 as the lowest priority (will get purged first above all others). The purge_priority parameter can also be NO_PURGING_STATUS (OXFF), in which case it will never be purged.

If no_data_save is TRUE, the contents of ref do not need to be saved to a scratch file when purged. Another way to state this is a ref with TRUE for no_data_save is known to have nothing in its contents of any value or consequence, thus Allocation Manager can simply purge it without saving any of its contents.

An example of a memory_ref that could be set for no_data_save would be an "offscreen bitmap" buffer. After it is used to transfer an image, an application might not care if all its contents get temporarily disposed, because on the next usage whole new contents (new bits) will be created all over again anyway.

NOTE 1: This function will still work regardless of ref's access counter state and regardless of whether or not it is purged.

NOTE 2. A memory_ref whose access counter is nonzero will not be purged, even if its purge priority is zero.

NOTE 3. Setting "NO_PURGING_STATUS "on a memory_ref that has already been purged will not take effect until it is unpurged. In other words, changing purge status does not automatically reload purged allocations —-byou still need to access its pointer (such as UseMemory) if you want its contents loaded into memory.
(pg_error) MemoryPurge (pgm_globals_ptr globals, long minimum_amount, memory_ref mask_ref);

MemoryPurge will purge memory_ref(s) until at least minimum_amount of memory (in bytes) has become available.

The globals parameter must be a pointer to the same structure given to MemoryAlloc (which is also the mem_globals field within the structure given to $p g l n i t$ ).

If mask_ref is non-null, that memory_ref is considered "masked" (protected) and will not be purged during this process.

All purgeable, unlocked allocations will be purged, one at a time and in the purge priority they are set for (lowest purge priorities are taken first) until minimum_amount of available space has been achieved.

If minimum_amount fails to become available, even after purging every eligible allocation, MemoryPurge will return an error (see "Error Codes" on page 39-1 in the Appendix); if successful, $N O \_E R R O R(0)$ will be the function result.

The minimum_amount specified is for the total memory available, which means if there is already enough or nearly as much available as minimum_amount, very little will get purged.

NOTE 1: The amount of "available memory" is based on what was given to pgInit for max_memory minus the total physical sizes of all existing memory_refs -psee pgInit.

NOTE 2. You normally do not need to call this function since MemoryPurge gets called for you as required for allocations and resizing blocks. The function has been provided mainly for freeing memory for objects that you are not allocating with the OpenPaige Allocation Manager.

NOTE 3. Even though this function might return no error (success), that still does not necessarily guarantee a block of minimum_amount can be allocated since the available memory might not be contiguous.

### 25.16 Allocation Manager Shutdown

(void) pgMemShutdown (pgm_globals_ptr mem_globals);

Call this function once you are through using the Allocation Manager. Be sure it is called after $p g$ Shutdown.

NOTE (Macintosh): This function is not necessary if you will be doing ExitToShell().

See “OpenPaige Shutdown” on page 2-13.

Unuse \& dispose

(void) UnuseAndDispose (memory_ref ref);

UnuseAndDispose decrements the access counter in ref, then disposes the allocation. This function does exactly the same thing as:

UnuseMemory(ref);

DisposeMemory(ref);

Memory globals

(pgm_globals_ptr) GetGlobalsFromRef (memory_ref ref);

GetGlobalsFromRef returns a pointer to pgm_globals located from an existing memory_ref. This function is useful for situations where you do not have access to the "globals" structure. Any valid, non-disposed memory_ref, locked or unlocked, purged or not, is can be used for ref. For more information on getting pgm_globals_ptr see “Get globals from pg_ref, paige_rec_ptr, etc." on page 26-11.

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-466.jpg?height=207&width=1232&top_left_y=1765&top_left_x=434)

HandleToMemory accepts handle h and returns a memory_reffor that Handle.

FUNCTION RESULT: After this function is called, the Handle is now "owned" by the Allocation Manager, which is to say you should no longer access that Handle nor should you dispose it.

Access to the Handle's contents must be made from that point on using the functions given above (UseMemory, UseMemoryRecord, etc.).

The mem_globals parameter must point to the same structure as given to MemoryAlloc.

The rec_size must contain the record size for the new memory_ref, which must be an even multiple of the original Handle. If unknown, then make rec_size $=1$.

It does not matter if Handle h is locked or unlocked, but it should at least temporarily be unpurgeable.

MemoryToHandle performs the reverse: it returns a Handle built from memory_ref ref. Again, once this call is made, ref is no longer valid and must not be given to any Allocation Manager functions.

NOTE (Windows): The term "Handle" is typedef'd from HANDLE, so the two terms are synonymous.

NOTE: These functions do not perform huge copies. Rather, they convert Handles to memory_refs and visa versa by appending some special information before and after the data contents, or removing this appendage. So it is generally safe to do HandleToMemory, MemoryToHandle under fairly tight situations that could not withstand the doubling of a Handle's size.

### 25.18 Debug Mode

There are two compiled versions of OpenPaige software, one for "debug mode" and one or "non-debug" or runtime mode.

In debug mode, memory_refs are checked for validity, including the verification of appropriate access counters, each time they are given to one of the functions listed above. While this significantly reduces the speed of execution, it does aid substantially in locating bugs that would otherwise crash your system.

For example, calling SetMemorySize for an allocation that is currently "in use" (access counter nonzero) could fail and/or crash your program. Under debug mode, however, you would be warned immediately if an attempt to change the size of an allocation was made on a locked block.

Object Code Users (Macintosh)

There are two sets of Macintosh (or Power Mac) object code libraries: one for "debug" and the other for "non-debug." As a general rule, you should use the debug versions to develop your application, then switch to non-debug before release (non-debug runs much faster).

NOTE (Windows): Object code for "debug" mode is not supported. This is because Windows General Protection Mode can be used instead and is generally superior.

Source Code Users

Debug/non-debug is controlled by the following \#ifdef in CPUDefs.h:

## \#define PG_DEBUG

If that \#define exists, the source files are compiled in "debug" mode.

To run OpenPaige debug libraries, you must include pgDebug.c in your project. When doing so, you can place a source-level debugger break at the location shown below; when the Allocation Manager detects a problem, the code will break at this spot.

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-469.jpg?height=734&width=1197&top_left_y=453&top_left_x=434)

NOTE: The error message string is a pascal string.

Debug Assert Messages

The debugger "assert" is simply a debugger break with one of the following messages:

Out of memory - Block of requested size cannot be allocated (or block cannot be resized). If virtual memory has been enabled, this will only happen if the block is so huge there is insufficient, contiguous memory available.

Purge file not open- Memory needs to be purged but "scratch" file doesn't exist or is closed.

Attempt to resize —locked memory Allocation is locked, yet a SetMemorySize has been attempted.

NIL memory_ref_memory_ref is a null pointer and/or an address inside of it is null.

Bogus memory_ref — address An address in a memory_ref is bad (would result in a bus error for Mac).

Internal damage in memory_ref —pmemory_ref's address OK but certain characteristics are missing (so it is assumed “damaged" or overwritten).

Overwrite error — blast 1 of 4 bytes beyond the logical size of a memory_ref has been overwritten.

Access counter invalid for operation - paccess counter is illegal for given function. Examples: SetMemorySize and access! 0 (illegal); UnuseMemory and access counter $==0$ (illegal); DisposeMemory and access counter! $=0$.

Bogus memory_ref —-pMemory_ref given is not a memory_ref but some other address.

Operation on disposed memory_ref —-bMemory_ref given has been disposed.

Error in purging - bAn allocation was writing to a scratch file an I/O error resulted (such as out of space).

Error in un-purging - bRead error occurred while recovering a purged allocation from scratch file.

Attempt to access record out of range — $\mathrm{p}$ UseMemoryRecord asking for a record beyond the size of the allocation.

Structure integrity failed - pthis message occurs when structural damage has occurred to the style_info or par_info run. For example, a style run might (incorrectly) reference a style_info that does not exist.

The standard purge function is a built-in part of the Allocation Manager which purges (disposes) memory that is not being used to make room for new allocations. The to-bepurged blocks are saved to a "temp" file so they can be resurrected later when asked to be used by OpenPaige or by the application.

If necessary, you can replace the standard purge function with one of your own. To do so, first declare a function as follows:

$$
\begin{aligned}
& \text { PG_FN_PASCAL pg_error my_purge_proc (memory_ref ref_to_purge, } \\
& \text { pgm_globals_ptr mem_globals, short verb); }
\end{aligned}
$$

In the pgm_globals structure (same one passed to MemoryAlloc), the purge field contains a pointer to the purge function. What you need to do is place a pointer to your purge function, as defined above, into that field:

paige_rsrv.mem_globals.purge = my_purge_proc;

The paige_rsrv variable is the same structure given to $p g I n i t$, and mem_globals is the Allocation Manager subset (same one given to MemoryAlloc).

When the Allocation Manager purges memory, it locates memory refs that are purgeable and passes each of them, one at a time, to the purge function; additionally, when a memory_ref needs to be reloaded (unpurged), the purge function is called again to unpurge the data. The standard purge function handles this by saving the contents of the memory_ref to a temp file, then setting the ref's byte size to sizeof(mem_rec); then when unpurging, the allocation is resized to the original size and data is read from the temp file.

The temp file reference used by the standard purge function is stored in the purge_ref_con field in pgm_globals (see "Memory globals" on page 25-464).

In addition to purging and unpurging, the purge function is also called to initialize "virtual memory" and to completely dispose an allocation that is currently purged.

Whether the purge function is getting called to purge, unpurge, initialize or dispose an allocation depends on the verb parameter, which will be one of the following:

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-472.jpg?height=354&width=1178&top_left_y=267&top_left_x=434)

For each verb, the purge function must perform the following:

purge_init — "Virtual Memory" must be set up. When purge_init is the reason for the function call, "ref_to_purge" will be NULL. The standard function initializes the "temp" file (whose file reference will already be contained in mem_globals>purge_ref_con).

purge_memory — The ref_to_purge memory must be purged. The Allocation Manager will only call with purge_memory if the reference is not yet purged, i.e. it won't try to purge the same reference twice. The standard purge function saves ref_to_purge's data to the temp file and sets the physical allocation size to sizeof(mem_rec).

unpurge_memory — The ref_to_purge memory must be unpurged. The Allocation Manager will only call with unpurge_memory if the reference has been purged, i.e. it won't try to unpurge the same reference twice. The standard purge function resets the physical size of ref_to_purge and loads the data from the temp file.

dispose_purge — The ref_to_purge is already purged but is about to be disposed. THE PURGE FUNCTION DOES NOT DISPOSE THE MEMORY, rather it does whatever is necessary knowing that the purged allocation will be disposed forever. The standard purge function "deletes" the saved data on the temp file, and does nothing else.

FUNCTION RESULT: The purge function should return NO_ERROR (zero) if all was successful, otherwise it should return the appropriate error code per pgErrors.h.

The following structure is used by the Allocation Manager (and is also a subset of pg_globals):

struct pgm_globals

\{

short signature; /* Used for checking/debugging */

short debug_flags; /* Debug mode, if any */

memory_ref master_list; /* List of all active memory_refs */

long master_list_size; /* Size of ref_list (even if zeros) */

long master_handle; /* HANDLE for master list (Windows only) */

long next_master; /* Next master list element */

long total_unpurged; $/ *$ Total bytes not purged *

long max_memory; /* Max memory (set by app) */

long purge_threshold; $/ *$ Amount extra to purge */

void *spare_tire; $\quad / *$ Extra memory for tight situations */

void PG_FAR *machine_var; /* Machine-specific generic ptr */ mem_debug_proc debug_proc; $\quad / *$ Called when a bug is detected */

purge_proc purge; $/ *$ Called to purge/unpurge */

long purge_ref_con; /* Reference for purge proc */ memory_refpurge_info;/* Machine-based purge information $\star /$

long next_mem_id; /* Used for unique ID's for refs */long «ud /* ID to use for MemoryAlloc's */

long active_id; /* Which ID to ignore for purging */

long last_message; /* Last message in exception handling */

pg_fail_info_ptr top_fail_info; $/$ * Current exception in linked list *I

pg_error_handlerlast_handler; $\quad$ /* Last app handler before Paige */

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-474.jpg?height=501&width=1159&top_left_y=172&top_left_x=434)

For more information on pg_globals, see "Changing Globals" on page 3-21. For more information on error codes see "Error Codes" on page 39-1.

## EXCEPTION HANDLING

### 26.1 The TRY/CATCH Mechanism

OpenPaige provides a fairly straightforward method of detecting runtime errors (such as disk I/O errors, memory errors, etc.) without the requirement of checking every function result or excessive code.

This is accomplished by using a set of predefined macros: $P G_{-} T R Y, P G \_C A T C H$, and PG_ENDTRY.

Although this mechanism is patterned after exception handling in $\mathrm{C}++$, you do not need to be using C++ to utilize OpenPaige error detection features (nor are any C++ "header" files or libraries required).

Anywhere in your application that calls an OpenPaige function that can fail for "legitimate" reasons, such as allocating memory or reading/writing files, you simply bracket your code as follows:

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-476.jpg?height=108&width=416&top_left_y=259&top_left_x=470)

.. if any error causes OpenPaige to abort a function, this part of your code is executed; otherwise this part is not executed. Hence you would do whatever is appropriate here such as an error alert to user.

... code is executed here if no error; also it is executed here if the code under PG_CATCH does nothing to abort the program any further.

The above example shows the simplest form of error detection: none of the code under $P G_{-} T R Y$ is necessarily required to check for errors at all since anything fatal within OpenPaige (such as out of memory or a disk error) will throw CPU execution into the first line of code under $P G_{-} C A T C H$. This is done automatically.

The parameter, $m e m_{-} r s r v$ after $P G_{-} T R Y$ must be a pointer to the global structure given to pgMemStartup earlier (see "Software Startup" on page 2-10 for information about pgMemStartup).

NOTE: PG_TRY, PG_CATCH and PG_ENDTRY macros are automatically available by including Paige.h (the actual definitions for these exist in pgExceps. $h$ which is \#included in Paige.h).

If your code executes under PG_CATCH that means OpenPaige aborted something due to a fatal error. You can learn what the error code was by examining the memory globals (same structure given to $P G_{-} T R Y$ ) as follows:

mem_rsrv.last_error;

### 26.3 Nested TRY/CATCH

PG_TRY / PG_CATCH / PG_ENDTRY can be "nested" throughout your application, in as many places as required. What literally occurs is that the CPU gets forced to the $P G_{-} C A T C H$ that corresponds to the most recent $P G_{-} T R Y$; then if the code under $P G \_C A T C H$ decides to abort that section of code, it can force an additional exception using $p g F a i l u r e$ (given below), in which case the next most recent $P G \_C A T C H$ (from some other place in your program, if any) gets executed. In short, TRY/CATCH can be effectively "daisy chained" in this fashion so any fatal error can cycle up through any level of nested subroutines - pall without the need to even check for errors!

### 26.4 Refinements

There are many situations where your code might need to "force" an exception after detecting additional errors while executing code between $P G_{-} T R Y$ and $P G_{-} C A T C H$. There are also many situations where your $P G_{-} C A T C H$ code needs to abort the entire
subroutine, returning control to some other part of the program. The following functions are available for this purpose:

(void) pgFailure (pgm_globals_ptr globals, pg_error error, long message);

This function forces unconditional execution to the code under $P G_{-} C A T C H$ that belongs to the most recent $P G_{-} T R Y$. For example, if you use $p g F a i l u r e$ while executing the code under $P G_{-} T R Y$, then the first line under $P G_{-} C A T C H$ in that section will get executed; if you use $p g F a i l u r e$ under $P G \_C A T C H$, then the first line under $P G \_C A T C H$ belonging to the previous $P G \_T R Y$ (somewhere higher up in your program) gets executed. You would most often use pgFailure when executing $P G_{-}$CATCH to completely abort an operation.

The globals parameter must be a pointer to pgm_globals (same structure given in pgMemStartup). The error and message parameters are stored in globals->last_error and globals->last_message, respectively, and can be any value(s) appropriate.

(void) pgFailNIL (pgm_globals_ptr globals, void PG_FAR *allocation);

This function can be used to force an exception if allocation parameter is a null pointer. The globals parameter must be a pointer to pgm_globals (same structure given in pgMemStartup).

What actually occurs when $\mathrm{pgFailNIL}$ is called is the following:

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-478.jpg?height=311&width=1028&top_left_y=1588&top_left_x=433)

This function can be used to force an exception if error parameter is nonzero. The g lo b a ls parameter must be a pointer to pgm_globals (same structure given in pgMemStartup).

What actually occurs when $\mathrm{pgFailError}$ is called is the following:

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-479.jpg?height=350&width=1002&top_left_y=419&top_left_x=434)

This function can be used to force an exception if actual_error parameter is nonzero and it does not equal acceptable_error. The globals parameter must be a pointer to pgm_globals (same structure given in pgMemStartup).

A typical use of this function is to force an exception for file $\mathrm{I} / \mathrm{O}$ errors unless the error is nonfatal. For example, there might be some code that keeps reading a data file until "endof-file-error" occurs. In such a case, you would want to abort if an error was detected other than end-of-file error.

What actually occurs when pgFailNotError is called is the following:

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-479.jpg?height=359&width=1232&top_left_y=1411&top_left_x=434)

This function can be used to force an exception if $\mathrm{b}$ is "TRUE." The globals parameter must be a pointer to pgm_globals (same structure given in pgMemStartup).

## if (b)

$$
\text { pgFailure(globals, BOOLEAN_EXCEPTION, 0); }
$$

### 26.5 Bridging to C++ Exceptions

If you are using $\mathrm{C}++$ and its TRY/CATCH mechanism, you can "bridge" an OpenPaige failure to the standard C++ exception handling by calling "Failure" (defined in C++ headers). Here's an example:

Code that follows PG_CATCH

Failure(mem_globals.last_error, mem_globals.last_message);

Creating a memory_ref

Let's take a simple but common example of using this method to detect insufficient memory when attempting to create a memory_ref. Here's how it would look (the
mem_globals variable is the same structure that you gave to pgMemStartup when you initialize OpenPaige):

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-481.jpg?height=1074&width=1234&top_left_y=283&top_left_x=434)

When you execute the above code, by virtue of making the $P G_{-} T R Y$ statement, OpenPaige now knows that any failure to create memory should invoke the exception handler and jump to your PG_CATCH statement. Hence, if MemoryAlloc fails, the CPU is immediately forced to the line that contains "PG_CATCH." At that place in your code you can do whatever to recover or alert the user or raise your own exception, etc.

The above example is the simplest of all cases since it only creates one memory_ref, hence, there really is nothing to "recover." It gets slightly more involved when you create, say, multiple memory_refs and you need to dispose the allocations that succeeded. I usually handle this by setting them all to "null" so I know which ones succeeded in PG_CATCH:

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-482.jpg?height=1131&width=1238&top_left_y=231&top_left_x=434)

In the above example, $P G \_C A T C H$ gets automatically executed upon the first failure of the three MemoryAlloc's. At that time, we dispose only the memory_refs that are non-NULL (which means they were successfully created).

NOTE: We call DisposeFailedMemory instead of DisposeMemory. This is a special "dispose" that OpenPaige provides for this case. It disposes the memory_ref regardless of its locked or "used" state so it doesn't jump into the low-level debugger.

The above examples illustrate where OpenPaige itself, by virtue of MemoryAlloc, automatically invokes the exception handler. There will be other cases, however, when you want to cause a similar exception for your own error checking (but you haven't called an OpenPaige function). One example of this would be calling "NewHandle" and having it return NULL (this indicating it failed). Here's how you do that:

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-483.jpg?height=804&width=1049&top_left_y=532&top_left_x=434)

In the above, we use "pgFailNIL", which checks for $\mathrm{h}$ being a null pointer and if so, throws an exception (causing $P G \_C A T C H$ ) to immediately execute).

There are other "pgFailxxx" functions to raise an exception in other ways. Using "pgFailure", for example forces an exception unconditionally (see "pgSetJmp.h" to see the various functions and/or the docs on this).

There is the possibility you might need to recover from a failed $\mathrm{pgNew}$ (also $\mathrm{pgCopy}$ would be same thing). You do this the same way as in my second example of creating allocations -- except for a pg_ref you call a special error-recovery-dispose:

\{

pg_ref MyNewPG;

pg_ref = MEM_NULL;

PG_TRY(\&mem_globals)

\{

MyNewPG $=\operatorname{pgNew}(. ., . .$,

\}

I* If ALL above code succeeded, "PG_CATCH" does NOT get executed. */

PG_CATCH

\{

I* If it gets here, then OpenPaige did not succeed and rasied an exception $* /$

\}

pgFailureDispose(MyNewPG);

## PG_ENDTRY;

\}

The function "pgFailureDispose" is called for situations like the above when none or only part of the $p g \_r e f$ may have been created.

NOTE: pgFailureDispose can accept a "null" pg_ref, so if you initially set the $p g \_r e f$ to MEM_NULL you can pass it to pgFailureDispose safely.

Get globals from pg_ref, paige_rec_ptr, etc.

So I am buried deep within a bunch of functions and I need to do a PG_TRY/PG_CATCH.

All I have is a pg_ref.

How do I get the globals I need for PG_TRY?

The availability of "memory globals" will generally depend on the kind of program you are developing.

In a regular application, you generally keep memory globals around as a static record that is accessible by any module of the program. Hence, the "availability" of globals is merely a matter of design, usually by including the necessary application header file. Example:

//Inside one of your application headers:

extern pg_globals pgm_globals mem_globals;

Hence, "mem_globals" is available anywhere you include the above header.

For certain circumstances where only a memory_ref (or a pg_ref) is available, however, you can also get the memory globals by calling "GetGlobalsFromRef."

Suppose, for instance, all you had available is "ref," where "ref" is a memory ref (or a pg_ref). You can get a copy of memory globals as follows:

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-486.jpg?height=599&width=1148&top_left_y=285&top_left_x=427)

## CUSTOMIZING OPENPAIGE

This section discusses the low-level "hook" structure of OpenPaige and general guidelines of customizing the look and feel of text display.

### 27.1 Standard Procs

(void) pgSetStandardProcs (pg_globals_ptr globals);

This function initializes all the standard low-level function pointers for styles, paragraph formats and general OpenPaige hooks.

The globals parameter should be a pointer to the same globals given to pglnit and pgNew. (Actually, globals can be a pointer to any pg_global record. For example, an application may want to set up different "versions" of globals or copies of globals; but the normal use of this function would be to reset the standard functions in the globals record currently in use).

The intended purpose of this function is to reinitialize all the standard functions after temporarily changing them to something else.

## OpenPaige "Add-on" Extensions

For future OpenPaige extensions, pgSetStandardProcs will still work correctly even if some of the low-level functions require special function pointers from the extension(s) as the "standard."

This is accomplished by the extend_proc which OpenPaige will now call every time pgSetStandardProcs is invoked (and after the standard functions are set). For this purpose the following values are possible for the verb parameter when extend_proc is called:

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-488.jpg?height=304&width=1166&top_left_y=879&top_left_x=433)

### 27.2 Style Functions

If you want to do anything at all with text styles or paragraph formatting that is beyond the scope (or different) than the way OpenPaige will normally handle formatting, you must first understand the following basic design concept on which OpenPaige is founded:

1. Every style_info record contains an extensive set of function pointers that, when called, perform all the associated tasks for that style, including text measurement (determining width of characters), display, height computation (ascent, descent, etc.), and information about characters (which char is a control code, which one is a line breaking char, etc.).
2. All new style_info records, by default, are initialized with OpenPaige standard functions: every function pointer gets set with the appropriate default procedure for that aspect of the style, none are null.
3. To alter the way a style behaves or draws, you simply replace the appropriate function pointer(s) within that style_info record with your own.

The end result of this method is that any section of text "knows how to measure and draw" itself.

### 27.3 Standard Low-Level Function Access

A subset of the above design concept is the public availability of OpenPaige standard low-level style functions.

An OpenPaige user has access to all the standard low-level functions that are normally placed into a style_info record by default. The reason this can be important is because a custom style function, which you create, has the ability to make a call to the "standard" function any time when it becomes necessary to facilitate your feature.

For example, suppose you had a requirement to create a "boxed" style which draws an outline around the text. To draw the text itself, you simply call the standard OpenPaige low-level function that draws text for any given style; then you would draw the outlining box. A great deal of coding time can be saved with this feature. See "Creating a simple custom style" on page 30-37 for a sample implementation.

The typical use for calling one of these functions is for situations where you only want to ALTER something only slightly from the standard and you want to avoid the necessity of writing huge code to replace the standard handling.

Another obvious example is using the line_measure function to build a line of text. As we have frankly stated in our own documentation, using this function to completely replace OpenPaige's standard handling is a mistake due to its enormous complexity. However, this function can become useful if you replace it with your own function that FIRST CALLS THE STANDARD FUNCTION to build the line; from that point you could alter the results. See "Anatomy of Text Blocks" on page 36-1 for additional information on line structures.

To call a standard low-level style function, simply include "defprocs. $h$ ". The standard functions are listed with their corresponding hook below.

### 27.4 Setting Style Functions

Every style_info record contains the following structure as one of its components:

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-490.jpg?height=991&width=1164&top_left_y=763&top_left_x=429)

In each field shown above, a pointer to an appropriate function exists. Every style must contain these function pointers, but any (or all) styles can be different functions.

Hence, to customize a style, you need to decide which of these functions you need to "override," then change the pointer(s) in the style record so OpenPaige will call your function for that task.

NOTE (Windows 3.1 Users): Function pointers you use to override the OpenPaige functions must be created with MakeProcInstance().

### 27.5 Setting / Changing Function Pointers

There are three general methods of changing function pointer(s) for styles.

## 1. Use pgSetStyleInfo

The first way is to simply use $p g S e t S t y l e I n f o:$ set your mask parameter to all zeros except for the style functions you wish to change, and place pointers to your functions into the new style_info record (see "style_info" on page 30-21 for details about pgSetStyleInfo). Doing it this way you will literally apply your customization to whatever range(s) of text you prefer.

## 2. Set the style procs

The second method is to use the following function(s):

void pgSetStyleProcs (pg_ref pg, pg_style_hooks PG_FAR *procs, style_info_ptr match_style, style_info_ptr mask_style, style_info_ptr AND_style, long user_data, long user_id, pg_boolean inval_text, short draw_mode);

(void) pgSetParProcs (pg_ref pg, pg_par_hooks PG_FAR *procs, par_info_ptr match_style, par_info_ptr mask_style, par_info_ptr AND_style, long user_data, long user_id, pg_boolean inval_text, short draw_mode)

This function applies procs to all styles that match a specified criteria, as follows: each field of a style_info record in $p g$ is compared to the same field in match_style if the corresponding field in mask_style is nonzero; in other words, only nonzero fields in mask_style are compared. Before each comparison is performed, the field in pg's style_info record is temporarily AND'd with the corresponding field in AND_style before the comparison.

Either match_style, mask_style or AND_style can be null, in which case the following occurs: if match_style is null, then all styles in $p g$ are changed (no comparisons are made). If mask_style is null, every field is compared and must match; if $A N D \_$style is null, no AND'ing is performed.

Only styles that completely match the comparison criteria based on match_style, mask_style and AND_style are changed and, if so, all functions in procs are placed into that style.

Additionally, for every style that is changed, user_data and user_id are placed in the style_info record (see "style_info" on page 30-21 for more information about the style_info record).

If inval_text is TRUE, the text for which the changed styles apply is "invalidated" (tagged to require re-word-wrapping and line calculations).

If draw_mode is nonzero the text is redrawn (see"Draw Modes" on page 2-30).

This function is mainly used to restore all custom function pointers for "opened" files.

pgSetParProcs is $100 \%$ identical to pgSetStyleProcs except paragraph style records are changed.

3. Initialize function pointers

The third method is to initialize function pointers while a file is being "read". This is done using the file handler functions as described in "File Handlers" on page 34-1 (see also “Special “Initializing" Handlers” on page 34-23).

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-492.jpg?height=114&width=115&top_left_y=1842&top_left_x=244)

CAUTION: Warning: Style functions can not be null, ever. They must point to a valid function even if that function does nothing. As a general rule, if you don't need to change a style function, leave the default that OpenPaige has initialized.

This method should be used when you want to set one or more style functions to be used for all styles.

This is done by simply setting up the default style in pg_globals with the appropriate function pointer(s). The default style is called "def_style" and is a style_info record which OpenPaige will copy for every new style insertion.

NOTE (Windows 3.1 Users): Function pointers you use to override the

OpenPaige functions must be created with MakeProcInstance().

### 27.6 Function Definitions

The following is a definition and description of each of these functions, what each parameter means and general comments on how you might use the function to create special features.

style_init_proc

PURPOSE: To initialize the style_info record.

STANDARD FUNCTION: The default function, pgStylelnitProc, figures out the text ascent, descent and default leading; for the Mac version, it also figures out a 16-bit "style" word it can give to QuickDraw for the text style; for Windows it builds a Font Object HANDLE.

PG_FN_PASCAL (void, style_init_proc) (paige_rec_ptr pg, style_info_ptr style, font_info_ptr font);

The style record is to be initialized; the font parameter is a pointer to the font_info record associated with this style.

NOTE 1. Important: OpenPaige determines character heights solely from the information in the style_info record; specifically, style_info.ascent, style_info.descent and style_info.leading. Additionally, top_extra, bot_extra and superscript/subscript values will affect the height of characters. It is therefore important that you initialize these fields to reflect the appropriate vertical "bounding" areas of text drawn in that format.

NOTE 2. When computing the height for offscreen bitmap display, OpenPaige uses only the ascent, descent and leading fields. You can therefore avoid excessive memory usage for bitmap drawing by using top_extra and bot_extra to define "non text" boundaries such as picture frames.

install_font_proc

PURPOSE: To set the font and style as the "current font" for subsequent drawing.

STANDARD FUNCTION: The default function, pglnstallFont, sets the current device to the font, style, color and char_extra values all taken from the style_info and font_info records. If include_offscreen is TRUE, all the same values are set in the internal offscreen port as well. If scaling is enabled (not $1: 1$ ), the point size is scaled accordingly and the scaled point size is used instead.

PG_FN_PASCAL (void, install_font_proc) (paige_rec_ptr pg, style_info_ptr style,font_info_ptr font, style_info_ptr composite_style, pg_boolean include_offscreen);

The $p g$ parameter is the paige_rec affected by this font. The style and font parameters point to the style and font record to install, respectively; neither will ever be a null pointer.

If composite_style is nonnull, this function must copy the contents of the style_info it actually used to set up the device (in certain cases, the original style_info may be temporarily altered, in the case of "superimpose" style variations, ALL CAPS, etc.).

If include_offscreen is TRUE, OpenPaige will want to use the same format when and if it draws to an offscreen bitmap during for the next text display (see "text_draw_proc" on page 27-501).

measure_proc

PURPOSE: To obtain the width of character(s).

STANDARD FUNCTION: The default function, pgMeasureProc, determines the character pixel positions as if each were drawn from left to right beginning at *positions. Special flags are set in each corresponding element in ${ }^{*}$ types.

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-495.jpg?height=198&width=1231&top_left_y=1018&top_left_x=436)

The walker parameter points to a style_walk record that contains pointers to the style and font to be measured.

NOTE: The paragraph formatting pointers in walker can be NULL or uninitialized.

data and length — define the text and length (in bytes), respectively.

slop — contains the number of "extra" pixels to include in the measurement, i.e., how much extra space to add for justification purposes (as in full justification).

positions - parameter points to an array of longs; the first long in that array will already be set to a number, which is the starting position, in pixels, for the first character in *data; the remaining elements in *positions will be uninitialized. The job of this function is to fill in the remaining longs with the position for all remaining characters as
they would appear on the screen if drawn from *positions pixel position, including the ending position of the last byte.

For example, if the text had 3 bytes, each of them 8 pixels wide, and the first element in positions was 70 , you would fill in the long's in positions as follows:

$$
70,78,86,92
$$

types - parameter is a pointer to an array of shorts; what you must fill in into this array is the character type of each byte in data. The character type is defined by setting any (or all, or none) of the following bits:

| \#define SOFT_HYPHEN_BIT | $0 \times 00000008$ | /* Soft hyphen char */ |
| :--- | :--- | ---: |
| \#define CTL_BIT | $0 \times 00000040$ | $/ *$ Char is a control code */ |
| \#define PAR_SEL_BIT | $0 \times 00000100$ | $/ *$ Char breaks a paragraph */ |
| \#define LINE_SEL_BIT | $0 \times 00000200 / *$ Char breaks a line (soft CR) */ |  |
| \#define TAB_BIT | 0x00000400 | /* Char performs a TAB */ |
| \#define CONTAINER_BRK_BIT | 0x00004000 | /* Break-container bit */ |
| \#define PAGE_BRK_BIT | 0x00008000 | /* Page breaking char */ |

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-496.jpg?height=47&width=1181&top_left_y=1232&top_left_x=421)
(meaning the character is none of the above), or a combination of these bits.

## EXAMPLE:

If there are 4 bytes to measure consisting of " $a$, b, $c$ " followed by $<\mathrm{CR}>$, the *types elements should be set to:

$$
0,0,0, \text { PAR_SEL_BIT } \mid \text { CTL_BIT }
$$

NOTE: types uses only the lower half of these \#defines.

The measure_verb parameter indicates one of two reasons measure_proc is being called, which are:

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-497.jpg?height=261&width=1138&top_left_y=283&top_left_x=434)

The difference between these two verbs is that measure_width_locs calls for the locations of characters to define the cascading width of each character, while measure_draw_locs calls for the literal locations (relative to horizontal screen *positions) that characters will appear on the screen. For standard Roman text these two measurements are identical. However, for right-to-left scripts they are not identical.

For example, Arabic characters aligned as "abcd" in memory will display as "cdba" on the screen. Hence, if each character were 10 pixels wide, the relative screen locations should be 40, 30, 20, 10, 0; if measuring purely for character widths, however, the *positions elements must be 0, 10, 20, 30, 40.

current_offset_parameter indicates the current offset into all text, which will match with the first byte of *data. For example, if OpenPaige is measuring text at absolute byte offset 1200, current_offset will be 1200.

scale_widths — parameter indicates whether or not the text is currently scaled: if TRUE, the measured text will eventually be used to convert a screen point to a character position that is scaled to something other than 1:1.

NOTE: If scale_widths is TRUE that does not mean the character position must be scaled, rather that the function must compensate for potential errors in scaling.

For example, suppose a document is currently scaled by $50 \%$ and the text to be measured is 24 point. However, the relative screen locations of scaled-down 12 point text do not necessarily correspond to exactly $50 \%$ of 24 -point text. Hence, there will be an inaccuracy if the text were being measured to compute a character from a scaled coordinate.

If scale_widths is TRUE, the Macintosh function computes the same scaled text it would normally draw in scaled mode, then upscales *positions.

## EXAMPLE:

If scaling is currently $50 \%$ and point size is 12 , then 6 point text is measured then each resulting character position is multiplied $\mathrm{X} 2$.

call_order — parameter indicates whether there will be one more measure_proc call for the current block of text or if this section of text is the end of the block: if call_order is -1 the text to be measured is the last part of the block.

NOTE: The "block" in this case is not necessarily all one style and in fact can be many styles (and potentially many different measure_proc functions). For Macintosh WorldScript measuring, it is necessary to know if measure_proc is being called for the last time in a sequence of bytes in order to set the last location of right-to-left script correctly. (Normally you won't care what the value of call_orderis).

## Multibyte Characters

The data given to measure_proc is always at a byte level, regardless of the character type. If you write your own measure_proc, it is your responsibility to return the correct responses anyway.

For example, suppose data had 10 bytes, but each byte represented $1 / 2$ of a two-byte character. If each double-byte character were 8 pixels wide, you would fill *positions with something like this:

$$
0,0,8,8,16,16,24,24,32,32,32
$$

NOTE (Macintosh): The style, font and point size will have already been set in the current GrafPort before measure_proc is called.

NOTE: The measure_proc never gets called if the text is hidden (styles[hidden_text_var] is TRUE and "hide hidden text" is on).

PURPOSE: To temporarily substitute the text for this style with something else.

STANDARD FUNCTION: The default function, pgMergeProc, does nothing. This particular function is mainly for application features for "mail merge" and similar functionality.

PG_FN_PASCAL (short, merge_proc) (paige_rec_ptr pg, style_info_ptr style, pg_char_ptr text_data, pg_short_t length, text_ref merged_data, long ref_con);

This function gets called only when the application has called pgMergeText (see "Mail Merging” on page 29-1).

style — parameter points to the style for which this call is intended; text_data will point to the first character affected by the style and length the number of bytes given in text_data.

merged_data — parameter will be an empty memory_ref. The job of this function is to fill merged_data with whatever new text it wishes to insert instead of the text that already exists. If merged_data is set to zero size, the "old" text is deleted with no new substitute.

ref_con — parameter will contain a value provided by the application.

FUNCTION RESULT: If this function returns FALSE, the existing text in this style is not affected (stays the same), otherwise the existing text is replaced with the text in merged_data even if merged_data is empty.

See “Mail Merging” on page 29-1.

PURPOSE: To return specific information about a byte of text.

STANDARD FUNCTION: The default function, $p g C h a r I n f o P r o c$, returns the various bit settings listed below for standard ASCII, Roman and non-Roman Macintosh character sets.

PG_FN_PASCAL (long, char_info_proc) (paige_rec_ptr pg, style_walk_ptr style_walker, pg_char_ptr data, long block_offset, long offset_begin, long offset_end, long char_offset, long mask_bits);

This function gets called at various times for OpenPaige to determine the nature of an arbitrary byte of text. Upon entry, the walker parameter points to a style_walk_ptr containing the following information:

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-500.jpg?height=818&width=1158&top_left_y=1049&top_left_x=432)

The style_walk record is provided for this function's reference only; the style_walk structure is described in more detail under "Style Walkers" on page 32-1.

CAUTION 1: You must not alter the contents of walker directly.

CAUTION 2: The paragraph format pointers might be NULL.

data - parameter will point to one or more bytes; the byte in question is at offset char_offset. For example, if char_offset is 7, the byte being questioned is data[7]. This function only gets called to learn about a single byte at a time; for multibyte characters you must respond with the appropriate bits set to indicate what part of a character the byte in question represents.

block_offset — parameter will be the absolute offset, relative to all text in $p g$, for the first byte of the text_block record from which the text is derived. For example, if block_offset were 1900, the data can be found at the text_block record, within pg, whose begin member is 1900; the purpose of this parameter is for special-case character testing for multiple language scripts (block_offset helps char_info find out if the text prior to data is a different script).

However, if block_offset is NO_BLOCK_OFFSET (value -1), the data does not belong to any text block. Some examples of when this can occur would be a byte stream that is about to be inserted but not yet part of a $p g \_r e f$, or a group of characters within an undo_ref, etc.

offset_begin and offset_end — parameters indicate the range of byte(s) within data that are valid for purposes of looking "ahead" and "before" data[char_offset]. Usually, offset_begin will be zero and offset_end will be some value >= char_offset. The purpose of these parameters is for checking adjacent character(s) for determining double versus single byte status of a character.

mask_bits — parameter indicates which character type(s) need to be checked; the bits set in mask_bits correspond one for one to the function result bits given below.

For example, if mask_bits contained BLANK_BIT and CTL_BIT, then the only characteristic that needs to be checked for data[local_offset] is whether or not the
character is blank and whether or not it is a control character. The purpose of mask_bits is to optimize the performance of char_info_proc by providing a hint as to which data types can be ignored.

FUNCTION RESULT: The appropriate combination of the following must be the function result (but the bits don't need to be set if they were not set in mask_bits):

\#define BLANK_BIT

\#define WORD_BREAK_BIT

\#define WORD_SEL_BIT

\#define SOFT_HYPHEN_BIT

\#define INCLUDE_BREAK_BIT

\#define INCLUDE_SEL_BIT

\#define CTL_BIT

\#define INVIS_ACTION_BIT

\#define PAR_SEL_BIT

\#define LINE_SEL_BIT

\#define TAB_BIT

\#define FIRST_HALF_BIT

\#define LAST_HALF_BIT

\#define MIDDLE_CHAR_BIT

\#define CONTAINER_BRK_BIT

\#define PAGE_BRK_BIT

\#define NON_BREAKAFTER_BIT

\#define NON_BREAKBEFORE_BIT

\#define NUMBER_BIT

\#define DECIMAL_CHAR_BIT

\#define UPPER_CASE_BIT

\#define LOWER_CASE_BIT

\#define SYMBOL_BIT

\#define EUROPEAN_BIT

\#define NON_ROMAN_BIT

\#define NON_TEXT_BIT

\#define FLAT_QUOTE_BIT

\#define SINGLE_QUOTE_BIT

\#define LEFT_QUOTE_BIT

\#define RIGHT_QUOTE_BIT

\#define PUNCT_NORMAL_BIT

\#define OTHER_PUNCT_BIT 0x00000001 /* Character is blank */

0x00000002 /* Word breaking char *

0x00000004 /* Word select char *

0x00000008 /* Soft hyphen char */

0x00000010 /* Word brk but include w/ word */

0x00000020 /* Select brk but include w/ word */

0x00000040 /* Char is a control code */

0x00000080/* Char implies command or action*/

0x00000100 /* Char breaks a paragraph *

0x00000200 /* Char breaks a line (soft CR) *

0x00000400 /* Char performs a TAB */

$0 \times 00000800$ /* 1 st half of a multi-byte char */

0x00001000 /* Last half of a multi-byte char *

0x00002000 /* Middle of a multi-byte char *

0x00004000 /* Break-container bit */

0x00008000 * Break-repeating-shape bit */ 0x00010000//Char must stay with char(s)after it 0x00020000 /* Char must stay with char(s) before it ${ }^{*}$ 0x00040000 / 1 Char is numeric * $0 \times 00080000 \quad 1 *$ Char is decimal (for decimal

0x00100000 /* Char is UPPER CASE */

0x00200000 / 1 Char is lower case */

0x00400000 /* Char is a symbol *

0x00800000 /* Char is ASCII-European */

0x01000000 /* Char is not Roman script */

0x02000000 /* Char is not really text *

0x04000000 /* Char is a "flat" quote */

0x08000000 / Q Quote char is single quote *

0x10000000 /* Char is a left quote */

$0 \times 20000000 \quad 1 *$ Char is a right quote */

$0 \times 40000000 \quad / *$ Char is normal punctuation *

0x80000000 /* Char is other punctuation */

tab) $\star /$

NOTE: Important: OpenPaige, as a whole, knows nothing at all about "character types" and therefore relies utterly on the information provided by char_info_proc:

For an example see "Control characters don’t draw” on page 24-12.

text_draw_proc

PURPOSE: To draw the text for a style.

FUNCTION RESULT: The default function, $p g D r a w P r o c$, draws the text using the standard styles and color as defined in the style_info record, plus any other styles that are appropriate (such as strikeout, word underline, etc.).

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-503.jpg?height=191&width=1189&top_left_y=946&top_left_x=448)

walker - parameter contains information about the text style, font and paragraph formatting (for more information about the style_walk record see "char_info_proc" on page 27-498 and “Style Walkers" on page 32-1).

data, offset and length — provide the pointer to text, offset into that text to start, and the number of bytes to draw, respectively. All text parameters are in bytes; if the text is something other than bytes it is this function's responsibility to work that out.

The text is guaranteed to never be zero length, and is guaranteed to not contain any "blanks" that should not be drawn (such as tabs, CR's, etc.). Only text for this style will be given: the data will never cross style boundaries nor will the text ever cross line boundaries (it will always be given for a whole line or a portion of a line).

The draw_position is a pointer to a record that defines exactly where to draw the text as follows:

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-504.jpg?height=846&width=1161&top_left_y=265&top_left_x=434)

The text is to be drawn at coordinate draw_position->from; after drawing, draw_position->to must be set to the ending "pen" position (ending coordinate where the text stops). The real_offset field contains the actual byte position (within $p g$ ) of the text to be drawn relative to all text in the $p g \_$ref.

The ascent and descent — fields define the line's height from the current vertical position (which will be draw_position->from.v).

compensate_ $h$ — field is currently not used (but reserved for future enhancement).
starts — field is a pointer to the current point_start record. The value in starts->flags can be useful for special drawing as any of the following might be set in this field:

| \#define LINE_BREAK_BIT | 0x8000 | /* Line ends here */ |
| :--- | :---: | :---: |
| \#define PAR_BREAK_BIT | $0 \times 4000$ | /* Paragraph ends here */ |
| \#define SOFT_PAR_BIT | $0 \times 2000$ | /* Soft CR ends line */ |
| \#define RIGHT_DIRECTION_BIT | $0 \times 1000$ | /* Text is right to left */ |
| \#define LINE_GOOD_BIT | $0 \times 0800$ | /* This line requires no recalc */ |
| \#define NEW_LINE_BIT | $0 \times 0400$ | /* New line starts here */ |
| \#define NEW_PAR_BIT | $0 \times 0200$ | /* New paragraph starts here */ |
| \#define WORD_HYPHEN_BIT | $0 \times 0100$ | /* Draw a hyphen after this text */ |
| \#define TAB_BREAK_BIT | $0 \times 0080$ | /* Tab char terminates this line */ |
| \#define HAS_WORDS_BIT | $0 \times 0040$ | /* One or more word breaks exist */ |
| \#define CUSTOM_CHARS_BIT | $0 \times 0020$ | /* Style(s) are custom */ |
| \#define SOFT_BREAK_BIT | $0 \times 0010$ | /* Start breaks on soft hyphen */ |
| \#define BREAK_CONTAINER_BIT | $0 \times 0008$ | /* Line breaks for next container */ |
| \#define BREAK_PAGE_BIT | 0x0004 | /* Line broke on page break */ |
| \#define LINE_HIDDEN_BIT | 0x0002 | /* Line is invisible (hidden) */ |
| \#define NO_LINEFEED_BIT | 0x0001 | /* Line does not advance vertically */ |
| \#define TERMINATOR_BITS | 0xFFFF | /* ALL ONES if terminator record */ |
|  |  |  |

bitmap_offset — field indicates how far the display is offset from its screen position to draw within the offscreen bitmap (if that is the drawing mode).

vis_offset — field indicates how much was added to the display to obtain the real screen position; usually, this is the "scroll position" of the text.

block — field contains a pointer to the current text_block record.

The two parameters in text_draw_proc — bextra and draw_mode — pdefine the amount of space (in pixels) to make up for justification and the current drawing mode, respectively.

NOTE (Macintosh): On scaling for Macintosh: If text is scaled, the fields draw_position will already reflect the scaling (i.e., the coordinates will already be scaled) and the point size will also have been scaled and set appropriately. Therefore you do not need to do anything "special" for scaling in this function.

NOTE: On blank characters: THE SIZE OF TEXT WILL ALWAYS EXCLUDE "INVISIBLE" CHARACTERS, including trailing spaces. Example: If a 4-byte line of text were 3 bytes followed by a carriage return, the length parameter
passed to text_draw_proc will be 3 (not 4). Also, if a line of text has embedded tab characters (or other similar controls), multiple calls to text_draw_proc will be made, each of which omit the tab characters. The function is not called at all if omitting non-displaying chars results in zero length.

NOTE: More about blank characters: "Blank" characters are determined to be so as a response from char_info_proc. OpenPaige knows nothing about the nature of characters. Hence, if you embed a stream of characters for special purposes, you can declare each of them as "visible" (or not) simply by writing your own char_info_proc.

Rather than actually drawing a character, sometimes all you need do is change the character attributes. See "Control characters don't draw" on page 24-12.

dup_style_proc

delete_style_proc

PURPOSE: To duplicate any structure(s) in a style_info record, or to delete any structure(s) in the style_info record.

STANDARD FUNCTION: The default functions, $p g$ DupStyleProc and pgDeleteStyleProc, do nothing (there are normally no allocated structures within a style_info record). These functions are mainly for special application features.

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-506.jpg?height=293&width=1176&top_left_y=1452&top_left_x=434)

Whenever OpenPaige duplicates (copies) a style_info record internally, dup_style_proc is called. This would give the application a chance to duplicate any structures it may have placed in the style_info record. For duplication, both source and target paige_rec's are given. The source will be the original OpenPaige document from
which the item is being copied; the target is the paige_rec for which the copy is being applied. Either might be NULL depending on the reason the function is being called.

Whenever OpenPaige is about to delete a style_info record, delete_style_proc is called giving the application a chance to dispose any structures it may have placed in the style_info record.

globals is sometimes used in cases where $p g$ can be NULL if style records are getting disposed from an undo_ref, but Windows version must have a pointer to the globals when a style is deleted.

For both duplicate and delete functions, a verb parameter is given to indicate the reason the function is being called, which will be one of the following:

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-507.jpg?height=879&width=582&top_left_y=787&top_left_x=434)

A good example of this would be if a picture structure were kept in one of the style_info fields, and if OpenPaige made a copy of the style, you would want to also duplicate the picture; for deletion, you would want to delete the picture, and so forth.

Neither function gets called when you "set" a style for the first time, but rather it is called for other situations such as Copy, Paste and deleting all the text for a style.

The parameter all_styles is the memory_ref containing all the style_info records (of which the style parameter is a part of). The purpose of this parameter is to provide the array of style records for special situations where $p g$ is a null pointer (see below).

CAUTION: In these particular functions, $p g$ can be a null pointer. This is because both of these functions can get called in relation to "undo" and "redo" functions that are not associated with a $p g \_r e f$ at the time they are required. If you need to examine the array of style_info records, use the all_styles parameter.

Source and destination pg_refs when duplicating or deleting

I'm doing some custom style work and I'm trying to follow the logic of how the hooks are called for duplicate and delete, etc. I understand that sometimes the pg_ref to these hooks is passed as NULL. However, it seems to me that the duplicate call should have BOTH the source and destination pg_refs, and BOTH the source and destination style records.

I can best answer all these questions by explaining what the duplicate/delete functions are all about.

The only real purpose of the duplicate and delete function hooks in a style is to handle memory structures that have been embedded in the style itself. Otherwise these hooks are not needed and shouldn't be used.

The style_info record is always "copied" or "removed" by openPaige as needed, without you doing anything. However, if you have embedded some kind of Handle or Pointer or memory_ref, etc. inside the style info you would need to be informed of duplications/deletions so you can handle the memory allocation(s).

For example, let's say you create a custom style that contains as one of its fields a PicHandle. You initially created this style something like:

style_info my_style;

style_info.user_var $=$ (long) MyPicture; /* put in a Picture handle ${ }^{\star /}$

As long as this style_info sits inside the pg_ref as-is, no problem. Later on, hōwever, the user does pḡcopy which causes

OpenPaige to make a duplicate of this particular style_info. In such a case, you would wind up with the same PicHandle in each copy of the style -- which would be a disaster. Hence, you need to be told that OpenPaige just made a copy of your style_info record and you would need to make a unique PicHandle to avoid this problem, something like:

HandToHand((Handle*) \&style_info.user_var);

If "user_var" were not a memory allocation you would NOT CARE that opēnPaige made a copy.

The same is true for deletions: if OpenPaige decides to dispose a style_info record (which ALWAYS occurs during pgDispose) you would need to be told so you could remove memory allocations, as:

KillPicture((PicHandle) style_info.user_var);

That is really all there is to it.

As to when and why OpenPaige calls these functions, it doesn't matter. There are lots of reasons and I've only named the obvious ones. All you need to know is:

1. The "duplicate" function gets called after OpenPaige has made an exact copy of a style_info for whatever reason.

2 The "delete" function gets called just before OpenPaige removes a style_info record for whatever reason.

Now, how can a pg_ref be "NULL" when these functions get called? That only occurs when the target $p g$ ref does not exist. The only situation where this exists is setting up an undo_ref, in which case there isn't any target pg_ref.

PURPOSE: To alter the style as necessary as it is about to be modified as "offspring" of some other file. For example, if the user selected text that was currently in Helvetica 12-point text, then changed the style to bold, OpenPaige creates a new style_info record exactly the same as Helvetica 12-point but with the bold attribute set. In this way, new styles can be "offspring" of parent styles and alter_style_proc gets called with the original style and offspring so custom styles can be altered appropriately.

STANDARD FUNCTION: The default function, pgAlterStyleProc, does nothing. This function is normally used for custom styles if the customized elements need to be changed before becoming offspring.

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-510.jpg?height=249&width=1223&top_left_y=897&top_left_x=433)

Upon entry, old_style is the "parent" (original) style and new_style is the altered "offspring" (new) style.

For alter_style_proc, the mask will indicate which field(s) of the style_info are actually being altered. Only the nonzero fields as indicated in mask will actually be changed.

A typical reason for using alter_style_proc is the following scenario: A custom style is developed by the application that places a "box" around text. The box effect is accomplished purely by changing the draw_proc to draw a frame around the surrounding text. The user, however, selects a range of text to become "plain," in which case you might want to clear out your custom function pointers. The alter_style proc would be best suited for this purpose: you could check the new_style to see if it is your custom "box" style and, if so, reinitialize all the function pointers to their defaults (so your box doesn't draw any more).
alter_par_procis identical to alter_style_proc except it does not have a "mask" parameter and is called for paragraph formats.

copy_text_proc

delete_text_proc

PURPOSE: To duplicate any structure(s) in within the text stream associated with a style_info record, or to delete any structure(s) in the text for style_info record.

NOTE: Standard text characters are automatically copied or deleted by OpenPaige. The purpose of this function is to copy anything that really isn't text, such as embedded picture allocations, controls, etc.

STANDARD FUNCTION: The default functions, $p g$ CopyTextProc and $p g D e l e$ teTextProc, do nothing (there are normally no allocated structures within the text stream). These functions are mainly for special application features.

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-511.jpg?height=292&width=1227&top_left_y=1126&top_left_x=438)

These functions are similar to dup_style_proc and delete_style_proc except they are intended for structures the application might have embedded into the text stream for the "style" in question. They get called for exactly the same reasons (and occurrences) as the dup_style_proc and delete_style_proc, but for the text stream.

On entry, style_position is the byte offset where the style begins (byte offset relative to all text within the $\left.p g \_r e f\right)$. The text_position parameter is the offset of the text in relation to all text in the $p g \_r e f($ this is not the offset of the text given in the text parameter, rather the absolute position from the first byte in $p g$ ). The text parameter is a pointer to the text to be copied or deleted and the length parameter is the number of bytes that text is pointing to.

The reason_verb for both functions indicates the reason for the function call; this value will be one of the values listed above for dup_style_proc and delete_style_proc.

For copy_text_proc, the text is being copied from src_pg to target_pg.

NOTE: Important: Neither of these functions get called unless

"REQUIRES_COPY_BIT" is set in the class_bits field of the style_info record.

CAUTION: In these particular functions, the paige_rec_ptr param(s) can be a null pointer. This is because both of these functions can get called in relation to "undo" and "redo" functions that are not associated with a pg_ref at the time they are required.

setup_insert_proc

PURPOSE: To set up for the next text insert. Custom style features sometimes require a "warning" before a new text insertion.

STANDARD FUNCTION: The default function, pgSetupInsertProc, does nothing. This function is mainly intended for custom style features, but OpenPaige will alter this hook when/if an embed_refis inserted.

PG_FN_PASCAL (short, setup_insert_proc) (paige_rec_ptr pg, style_info_ptr to_be_inserted, long position);

The main reason for this low-level function is to avoid "extending" a style beyond the range of what makes sense. A perfect example of this is when pictures have been embedded into the text stream as a "style". If the user selected the style at the end of the picture, you would not want the style to extend along with the text; rather, you would want to null out the "picture" part of the style and force OpenPaige to insert a normal, non-picture format from that point forward. The setup_insert_proc give you a chance to do that.

Upon entry, to_be_inserted is the style that would affect the next text, if you did nothing and position is the text position for the text insertion (relative to all text in pg).

If you change anything in to_be_inserted (which you are allowed to do), return TRUE from the function result. If to_be_inserted is untouched, return FALSE.

track_control_proc

PURPOSE: To track a "mouse" across a "control” type style.

NOTE: A style is considered a control simply if its class_bits field contains “STYLE_IS_CONTROL" setting.

STANDARD FUNCTION: Usually, the default function, $p g$ TrackCtIProc, does nothing, because this function only gets called if class_bits indicate it is a control style.

PG_FN_PASCAL (long, track_control_proc) (paige_rec_ptr pg, short verb, t_select_ptr first_select, t_select_ptr last_select, style_walk_ptr styles, pg_char_ptr associated_text, point_start_ptr bounds_info, short modifiers, long track_refcon);

This function gets called by OpenPaige only from within $p g$ DragSelect (text click/drag) and works precisely as follows: when text is clicked for the first time ( $p g$ DragSelect called with verb $=$ mouse_down), the class_bits field of the style applied to the character being "clicked" is examined for STYLE_IS_CONTROL; if that bit is set, track_control_proc gets called. Then, if track_control returns any nonzero result, OpenPaige "remembers" to make subsequent calls to track_control during every subsequent call to pgDragSelect for announcers until (and including) mouse_up is indicated.

NOTE: If track_controlreturns zero from its initial (mouse_down) call, it will not get called again until the next time pgDragSelect receives a mouse_down and the style's class_bits contain STYLE_IS_CONTROL.

Conversely, if track_control_proc returns a nonzero response during mouse_down (indicating that it is indeed a "control" as far as the application is concerned), pgDragSelect will neither track nor highlight nor "auto scroll" any text in the normal fashion. Rather, each new call to $p g D r a g S e l e c t$ will simply determine the location of
the new selection point and pass that information to track_control_proc. In other words, once this function indicates the style is a "control" the application becomes responsible for handling the mouse_down, mouse_moved and mouse_up activity.

For every track_control_proc call, the verb parameter contains the same verb given to pgDragSelect. The first_select and last_select parameters will contain selection information for the initial mouse_down point and the new (most recent) point, respectively (see record structure below).

styles — parameter is a pointer to a style_walk record containing the style information for the first, original selection point (which will be, of course, the same style for which track_control_proc is being called).

The text for which the (original) style is applied is given in associated_text.

NOTE: Associated text is a pointer to the first byte of text for which the style applies, not necessarily the character that is being "clicked."

bounds_info — parameter is a pointer to a point_start record that defines the boundaries of the portion of line originally selected. Among the fields in bounds_info that is probably the most useful is bounds_info->bounds, which is a rectangle defining the precise bounding area for the top and bottom of the text line and the left and right side of the character(s) enclosed by the style. However, this information is neither scrolled nor scaled.

modifiers and track_refcon — parameters are one and the same values given in the most recent call to $p g$ DragSelect.

FUNCTION RESULT: The initial call to track_control_proc (pgDragSelect got called for mouse_down) should return any nonzero result if the selection point is indeed a control and the application wishes to continue tracking it through more calls to track_control_proc.

For every call to track_control_proc, pgDragSelect returns the same function result. Hence your application can detect when a "control" has been clicked by what is returned from $p g D$ ragSelect.

## Selection Information

To obtain information about the first selection point (or most recent selection point) you can examine first_select or last_select, both which point to the following structure:

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-515.jpg?height=685&width=1125&top_left_y=425&top_left_x=435)

Among the fields in $t$ _select, probably the two of interest to the track_control_proc are control_offset and original_pt: the control_offset field contains the text position that corresponds to the selection coordinate. The original_pt contains the co_ordinate that was used to determine the selection point.

NOTE: Normally the "offset" field contains the selection position of text; when the track_control_proc is in progress, however, you should examine control_offset instead as it may be different than the actual text position.

NOTE: Important: The original_pt is not necessarily one and the same value given to $p g D$ ragSelect since it is backwards adjusted to the document's current scrolled position and scaling values.

By "backwards adjusted" is meant the following: Before pgDragSelect determines the text selection that corresponds with the co_ordinate given to it, the original co_ordinate values are first offset by the (positive) vertical and horizontal scrolled positions of the document; then the co_ordinate is reverse scaled to the document's
scaling factor, e.g. if the document is currently scaled by $1 / 2$, the co_ordinate is upscaled by $2 / 1$, or if the document is currently scaled by $2 / 1$ the co_ordinate is reversely scaled $1 / 2$, etc.

Hence to obtain the "real" co_ordinate of, say, first_select -> original_pt from your track_control_proc, perform the following:

co_ordinatereal_pt;

real_pt $=$ first_select $->$ original_pt;

pgScaleLong(-pg -> scale_factor.scale, pg->scale_factor.origin.h, \&real_pt.h);

pgScaleLong(- pg->scale_factor.scale, pg->scale_factor.origin.v, \&real_pt.v);

pgOffsetPt(\&real_pt, pg->scroll_pos.h, pg -> scroll_pos.v);

## Checking for Arrow Selection(s)

OpenPaige performs a "shift-arrow" selection by emulating a shift-click internally. When using the control tracking hook, your application can become confused by this action since the hook will be called if the user performs shift-arrow over a "control" style.

To avoid this problem, the modifiers parameter will contain the following attribute if arrow keys are causing the selection (instead of the mouse):

## ARROW_ACTIVE_BIT

\#define ARROW_ACTIVE_BIT0x8000/* Arrow key is actively down */

PURPOSE: To activate or deactivate "control" styles when the $p g \_$ref changes its highlight state.

STANDARD FUNCTION: The standard function, pgActivateStyleProc, does nothing.

PG_FN_PASCAL (void, style_activate_proc) (paige_rec_ptr pg, style_info_ptr style, select_pair_ptr text_range, pg_char_ptr text, short front_back_state, short perm_state, pg_boolean show_hilite);

Whenever the active/deactive state changes within a pg_ref, this function gets called for every style that has ACTIVATE_ENABLE_BIT set in class_bits. Note that the style is usually a "control" style that your application defined but does not really need to be a control.

The situation that causes this function to be called is (a) the document changes active/deactive states by virtual of pgSetHiliteStates, and (b) the style's class_bits contain ACTIVATE_ENABLE_BIT.

Upon entry, the style parameter is a pointer to the style_info record.

ext_range _ parameter defines the beginning and ending range of text for which this style applies, and text is a pointer to the first byte in the text stream for which this style applies.

font_back_state and perm_state — are the same values as given in pgSetHiliteStates; note that pgSetHiliteStates is the only way a pg_ref can change from active to inactive or visa versa.

If show_hilite is TRUE — the new highlight state is to be drawn, otherwise this function should not change anything on the screen.

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-518.jpg?height=147&width=1198&top_left_y=261&top_left_x=433)

This hook gets called during pgSaveDoc, just before style_to_save gets written to the file.

The intended purpose of this function is to change something in style_to_save before each of its fields get written to the file; when the style_info record is read later, the field(s) will contain the altered contents, if any.

An example of using this function would be to replace a memory structure that is stored in the style_info record with something that can be recognized to restore that structure later when the file is opened. If a pointer to a picture were placed in one of the style_info fields, for instance, the application might want to change that to some type of "picture ID" reference so it can restore the appropriate picture later.

NOTE: The actual style_info record within the $p g \_r e f$ is not altered, only the record that is written to the file is altered.

STANDARD FUNCTION: The standard save_style_proc does nothing.

### 27.7 Paragraph Style Functions

Paragraph formats — bpar_info records — palso contain their own set of low-level function pointers. For each par_info, OpenPaige sets default functions in the following record:

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-519.jpg?height=496&width=1176&top_left_y=279&top_left_x=434)

PURPOSE: To draw "ornaments” such as lines over paragraphs, line numbers, paragraph numbers, etc.

PG_PASCAL (void) line_glitter_proc (paige_rec_ptr pg, style_walk_ptr walker, long line_number, long par_number, text_block_ptr block, point_start_ptr first_line, point_start_ptr last_line, point_start_ptr previous_first, point_start_ptr previous_last, co_ordinate_ptr offset_extra, rectangle_ptr vis_rect, short call_verb);

This function gets called after each text line is drawn; information is available from the function parameters to determine whether or not the line is the beginning of a paragraph, the ending of a paragraph, and whether or not a page breaking characters terminates the line.

walker — parameter points to a style_walk record which contains pointers to the current text and paragraph style (see “Style Walkers” on page 32-1).

line_number and par_number — will contain the line and paragraph number that just displayed, respectively. Both of these numbers are one-based, i.e. the top line or paragraph of $p g$ is 1 , the second line is 2 , etc.

CAUTION: The line and paragraph numbers will be incorrect unless you have set COUNT_LINES_BIT as one of the attributes flags in the pg_ref.

block — parameter points to the text_block record containing the text for the line (if you need to access the text, see "Accessing Text" below).

first_line and last_line — parameters point to the first and last point_start records that compose the line (the format of an OpenPaige line is composed of one or more records called a point_start —psee "Line Records" on page 36-4 for information on this structure). Most of the information your application needs to know about the line is contained in one of these two parameters - psee "Determining Line Type" below).

previous_first and previous_last — parameters point to the previous line's beginning and ending point_start records, or they are null pointers if there is no previous line.

offset_extra — parameter points to a co_ordinate record whose $\mathrm{h}$ and $\mathrm{v}$ fields indicate the distance, in pixels, the text was adjusted when drawn; given the horizontal and vertical positions of the line as defined in its point_start record(s), OpenPaige will have added offset_extra-> $h$ and offset_extra- $>v$ to those locations when it drew the text (see "Determining Bounding Rectangle" below).

call_verb — indicates the nature of the function call, which will be one of the following

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-520.jpg?height=306&width=513&top_left_y=1271&top_left_x=429)

glitter_bitmap_draw — bThe line has been drawn through OpenPaige's offscreen bitmap; the bits have not yet been stamped to the screen.

glitter_post_bitmap_draw_bThe line_glitter function has been called a second time, after it stamped the bits to the screen in offscreen drawing mode.

glitter_normal_draw —pThe line has been drawn directly to the screen.

NOTE: If call_verb = glitter_post_bitmap_draw, the function would have been called once already for the same line. The purpose of this call_verb is to give the application a chance to draw directly to the screen after OpenPaige has displayed the offscreen bits. If call_verb = glitter_normal_draw, however, offscreen drawing did not occur.

## Determining Line Type

first_start and last_start — parameters point to the beginning and ending records that make up a line of OpenPaige text. Each of these are type point_start:

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-521.jpg?height=546&width=1162&top_left_y=759&top_left_x=436)

It is possible that first_start and last_start will be the same (both might point to the same record). For a single line of text, for example, that contains no style changes and no tab characters, OpenPaige maintains only one point_start; if the line contains style changes and/or tabs, a point_start record separates each style or tab separation.

The field you will probably need to examine the most often within a point_start is flags, which contains various bit settings that indicate almost everything you would need to know about the line, such as whether the line begins a new paragraph, whether it ends a paragraph, whether or not it breaks on a CR character or a page break character, etc.

The following bit values that exist in "flags" for a point_start record indicate the anatomy of the line:

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-522.jpg?height=910&width=1395&top_left_y=183&top_left_x=166)
/* New paragraph starts here */
/* Draw a hyphen after this text */
/* Tab char terminates this line */
/* One or more word separators exist */
$1 *$ Style(s) are known only to app
(so don't play games with display) */
/* Start breaks on soft hyphen */
/* Line breaks for next container */
/* Line broke for whole repeater shape */
/* Line is invisible (hidden text) */
/* Line does not advance vertically */
/* Flagged only as terminator record */

Some of these bits might be set in either first_start or last_start, while others will only be set in first_start, still others will only be in last_start.

The following code examples demonstrate common tests for flags:

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-523.jpg?height=205&width=712&top_left_y=275&top_left_x=487)

Testing end (last line) of paragraph:

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-523.jpg?height=212&width=704&top_left_y=832&top_left_x=485)

Testing for soft page break

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-523.jpg?height=254&width=1178&top_left_y=1350&top_left_x=487)

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-524.jpg?height=206&width=927&top_left_y=257&top_left_x=472)

Regarding WORD_HYPHEN_BIT: This bit will only be set if your application has provided a hyphenate hook and that hook has indicated a hyphenation break; except for "soft hyphen" characters, OpenPaige will never automatically hyphenate.

- or, for soft hyphens

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-524.jpg?height=208&width=905&top_left_y=989&top_left_x=475)

Regarding WORD_HYPHEN_BIT: This bit will only be set if your application has provided a hyphenate hook and that hook has indicated a hyphenation break; except for "soft hyphen" characters, OpenPaige will never automatically hyphenate.

## Determining Bounding Rectangle

It is common to want the bounding rectangle and/or the top or bottom dimensions of a line. This is accomplished by examining the bounds field of either first_start or last_start, or both.

When doing so, always add offset_extra to the side(s) of the bounding area to determine the exact drawing location. The following code examples show typical methods of determining bounding dimensions:

$$
\text { lineTop = first_start->bounds.top_left.v + offset_extra->v; }
$$

NOTE: Both first_start and last_start will have the same top and the same bottom, each reflecting the line's top and bottom

Line's Bottom

$$
\text { lineBottom = first_start->bounds.bot_right.v + offset_extra->v; }
$$

Line's left side

$$
\text { lineLeftSide = first_start->bounds.top_left.h + offset_extra->h; }
$$

Line's right side

$$
\text { lineRightSide = last_start->bounds.bot_right.h + offset_extra->h; }
$$

## Paragraph(s) versus Line(s)

Your application might use the line glitter hook to place ornaments around, or on the top of "paragraphs," yet this might not appear immediately intuitive since this hook is lineoriented (gets called for every line of text).

However, since an OpenPaige paragraph is merely composed of one or more lines, by interrogating the flags field in first_start or last_start you can immediately learn what portion of the paragraph the line belongs to.

For example, if NEW_PAR_BIT is set in first_start->flags, the line is first in a paragraph, while if last_start->flags has PAR_BREAK_BIT set, the line is last in a paragraph.

NOTE: Both NEW_PAR_BIT and PAR_BREAK_BIT can be set if the "paragraph" is composed of only one line.

## Finding "Real" Text Position

The feature you are implementing with line glitter might require you to learn the actual text position of the line, relative to the beginning of all text in the document. To do so, simply add first_start->offset to the text block's begin field.

## Real text position

longtextPosition;

textPosition $=$ (long) first_start->offset; // compilers often need this coercing textPosition += block->begin; // Add the block's begin, = real text position

## Line's Text Length (or ending position)

To learn how many bytes compose the line, subtract the offset value in the element AFTER last_start from the offset value in first_start.
pg_short_t lineTextSize;

lineTextSize = last_start[1].offset - first_start->offset;

NOTE: last_start[1] is guaranteed to exist even if last_start defines the end of the whole document, because OpenPaige always appends at least one point_start defining the ending position of all text.

To obtain the "real" text offset of the line's end, add the element after last_start to the block’s "begin” value:

Offset of line end

longendTextPosition;

endTextPosition $=$ (long)last_start[1].offset;

endTextPoistion += block->end;

ACCESSING TEXT

If it is necessary to examine the actual text of a line, you can do so by getting a pointer to block->text and its first byte in first_line->offset:

// The following example shows how to look at the text in the line:

pg_char_ptr text;

text $=$ UseMemory(block->text);

text $+=$ first_start->offset;// Points to FIRST BYTE of line

// .. be sure when you are through with "text" you call:

UnuseMemory(block->text);

I

tab_measure_proc

tab_draw_proc

PURPOSE: To measure the distance required for a tab and to draw "tab leaders."

STANDARD FUNCTION: The default function for tab_measure_proc, pgTabMeasureProc, returns the appropriate distance for every tab; the default tab_draw_proc, pgTabDrawProc, draws tab leaders if they exist.

PG_FN_PASCAL (long, tab_measure_proc) (paige_rec_ptr pg, style_walk_ptr walker, long cur_pos, long cur_text_pos, long line_left, pg_char_ptr text, pg_short_t text_length, long *char_positions, pg_short_t PG_FAR *tab_rec_info);

PG_FN_PASCAL (void, tab_draw_proc) (paige_rec_ptr pg, style_walk_ptr walker, tab_stop_ptr tab, draw_points_ptr draw_position);

For tab_measure_proc, walker is the current style and paragraph format; cur_pos is the current text width of the characters in the line preceding the tab; current_text_pos is the text position within $p g$ that contains the tab. The line_left parameter indicates the position where the line started, in pixels (which would include paragraph indentation, etc.).
text — parameter is a pointer to the text character immediately following the tab being measured and text_length holds the number of bytes remaining in that text. The char_positions parameter is a pointer to the character pixel positions that correspond to the text bytes (see "measure_proc" on page 27-493 for more information about character positions).

tab_info_rec — parameter points to the extra field of the current point_start record (see "line_glitter_proc" on page 27-517 for information about point_start). If the tab position does not correspond to any physical tab_stop record, *tab_info_rec should get cleared to zero; otherwise *tab_info_rec should be set to tab_stop element number OR'd with 0x8000. (Example: If tab corresponds to element 3 in the tab_stop array, *tab_info_rec should be set to 0x8003). This function should return the "width" of the tab character that, if added to cur_pos, would hit the appropriate tab position.

For tab_draw_proc, walker is the current style/paragraph info, tab is the tab_stop record and draw_position will contain the screen positions for the end of the text just drawn and the start of the tab position (hence, the two points to draw a tab leader). See "text_draw_proc" on page 27-501 for a description of a draw_points record.

dup_par_proc

delete_par_proc

PURPOSE: To duplicate any memory allocations, if any, that are present in the par_info record.

NOTE: The record itself is automatically duplicated by OpenPaige. The purpose of this function is to make copies of memory allocations that are embedded in the record.

STANDARD FUNCTION: In pgDupParProc the memory_ref containing a list of tabs is duplicated. In, pgDeleteParProc, they are deleted. Nothing else is cop-
ied or deleted (since there are no other memory structures in a standard par_info record).

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-530.jpg?height=244&width=1139&top_left_y=279&top_left_x=435)

These functions do exactly the same thing as dup_style_proc and delete_style proc except for par_info records. The all_pars parameter is the memory_ref containing all the paragraph formats (of which the style parameter is a part).

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-530.jpg?height=112&width=100&top_left_y=824&top_left_x=249)

CAUTION: In this function it is possible that the paige_rec_ptr(s) will be null.

This will happen if either function is getting called from the "undo" and "redo" function for which there is no $p g \_r e f$ associated. If you need to examine all the par_info records use all_pars.

## 27.8 "Global" OpenPaige Low-level Hooks

OpenPaige also has an additional set of low-level hooks that apply to all text and styles for that OpenPaige object; you can also replace any of these to enhance customized features (or, if you want all $p g \_r e f ' s$ to assume various functions other than the standard, you can change the default functions in $\left.p g \_g l o b a l s\right)$ :

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-531.jpg?height=1311&width=1445&top_left_y=248&top_left_x=224)

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-532.jpg?height=696&width=1371&top_left_y=140&top_left_x=219)

Setting pg_hooks

void pgSetHooks (pg_ref pg, pg_hooks PG_FAR *hooks, pg_boolean inval_text);

void pgGetHooks (pg_ref pg, pg_hooks PG_FAR *hooks);

To get the current pg_hooks, call pgGetHooks and the function pointers are copied to hooks.

To set new ones, call $p g S e t H o o k s$ with hooks containing new function pointer(s). The hooks <only in> $p g$ are changed. If inval_text is TRUE, all text in $p g$ is "invalidated" (marked to recalculated, reword wrap).

NOTE (Windows 3.1 Users): Hooks must be set from the result of MakeProcInstance() unless the function exists within a DLL.

NOTE: You can set the hook for all $p g \_$refs by changing OpenPaige globals "def_hooks".

CAUTION: No function pointers can be null, all must be valid. If you or OpenPaige attempts to access a proc that is NULL you will crash.

The names of the standard functions used by OpenPaige can be found in "Calling Standard Functions".

Setting a proc

The following is an example of setting a $p g$ function pointer for "wait_proc" but using the defaults for all other function pointers.

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-533.jpg?height=541&width=1232&top_left_y=776&top_left_x=436)

PURPOSE: To compute a line of text by setting up an array of point_start records.

STANDARD FUNCTION: The default function, $p g$ LineMeasureProc, computes a line of text, breaks on the appropriate word break and handles any "exclusion" that may exist (overlapping portions with exclude_area).

$$
\begin{aligned}
& \text { PG_FN_PASCAL (void, line_measure_proc) (paige_rec_ptr pg, } \\
& \text { pg_measure_ptr measure); }
\end{aligned}
$$

NOTE: This function requires highly complex handling and we do not recommend you use it. There are many alternative methods to force a line to calculate for customizing effects.

See also, "Anatomy of Text Blocks" on page 36-1 and "Standard Low-Level Function Access" on page 27-487 for better understanding of this area.

For alternatives to this see the next function pointer, line_adjust_proc, below.

line_adjust_proc

PURPOSE: To adjust a line (by adjusting an array of point_start records) after the line has been calculated. The intended purpose of this function is move an entire line somewhere else, for widows and orphan, or "keep- paragraphs-together" features, for instance. (See “Advanced Text Placement” on page 37-1).

STANDARD FUNCTION: The default function, $p g L i n e A d j u s t P r o c$, adjusts the line for non-left justification. Applications can use this function to move point_start records around for any purpose.

CAUTION: The point_start records must only be moved: new records must not be "inserted" nor can records be "deleted."

PG_FN_PASCAL (void, line_adjust_proc) (paige_rec_ptr pg, pg_measure_ptr measure, point_start_ptr starts, pg_short_t num_starts, rectangle_ptr line_fit, par_info_ptr par_format);
measure — parameter contains all the information about the line that was just built.

starts - parameter is a pointer to the first point_start for the line just computed, and num_starts indicates how many point_starts are in that array (which represent the whole line). The line_fit parameter contains the maximum rectangle that the line had to fit inside, and par_format is the current paragraph format.

NOTE: If you alter the bounding dimensions or location of the line in any way, be sure to also change measure_info->actual_rect to reflect the change.

## Line leading

## I need to implement the space between lines differently than openPaige does it now. How can I do this?

The only way I can think of to bypass line leading is to use the "line adjust proc" and change the physical baseline(s) of each line record once a line is figured out.

The default line_adjust_proc hook is used to alter the line for justification, but you can also use it to change the line leading.

When this gets called, you should:

1. First call pgLineAdjustProc itself (which is prototyped in defprocs. $h$ ) so OpenPaige can do its thing.

2 Then walk through line_starts for num_starts elements and make adjustments you need.

The "line starts" param points to one or more point start records (it points to num_starts records). The line "leading" will be reflected in either line starts->bounds (which defines the enclosing rectangle for that part of the line), and/or in line_starts $\rightarrow$ baseline (which defines the distance from bounds.bot_right.v where the text baseline sits).

3 For convenience, par_format is the current par info for this line. You can examine fields in this format if you need to.

4 The line fit param contains the overall rectangle for the whole liñe. MAKE SURE you adjust its height to the same dimension you changed the line_starts $\rightarrow$ bounds, if any. This is important, because when you return from the hook, openPaige uses the bottom of line_fit to know where the next line begins vertically.

PURPOSE: To verify that a built line of text is "valid," requiring no change of location, dimension or any other form and if so return TRUE. A result of FALSE tells OpenPaige to compute the text line over again. Hence, this low-level hook can be used to alter the form of a line under various conditions.

STANDARD FUNCTION: $p g$ LineValidate verifies that the new line fits within the boundaries of the current part of the page_area and does not intersect with any part of the exclusion area. If line fails to meet this criteria and line_validate_proc can't correct the line by mere adjustment, the function alters the line's parameters as necessary (such as adjusting the maximum width, changing to a new vertical position, etc.) and tells OpenPaige to recalculate the line by returning FALSE.

PG_FN_PASCAL (pg_boolean, line_validate_proc) (paige_rec_ptr pg, pg_measure_ptr measure_info);

The typical purpose of this hook would be to alter the form of a line after the "normal" line has been calculated.

For example, suppose the inclusion of a special character causes a line to dynamically change its physical location and/or its maximum allowable width. Using validate_line_proc, the necessary adjustments can be made, and if the function returns FALSE, OpenPaige will rebuild the line with the new information.

The measure_info parameter is a pointer to a large record structure which offers all available information about the line of text just computed; these are the fields you would
alter should the line need to be recalculated from new information. The $p g \_$measure record is defined as follows:

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-537.jpg?height=1682&width=1378&top_left_y=289&top_left_x=212)

styles;

block;

starts;

starts ctr;

num_starts;

*char_locs;

*char_types;

*positions;

*types;

fit_rect;

actual_rect;

wrap_bounds;

starts_ref;

tab_info;

wrap_r_base;

wrap_r_begin;

wrap_r_end;

r_num_begin;

r_num_end;

end_r;

exclude_ref;

wrap_r_save;

repeat_offset;

prev_bounds;

hook_refcon;

minimum_left;

maximum_right;

/* Total use of text in line */

/* Maximum text line can use */

/* Excess width not used by line */

/* Pointer to the style_walk */ $1 *$ Current text block *

/* Next point_start * record *

/* Number of starts remaining */ $1^{*} \#$ starts this line */ /* Current character locations */ /* Current character types */

I* Original character locations */ /* Original character types */ /* Rect in which line must fit */ $1 *$ Actual rect enclosing line */ /* Bounding rect for wrap area * /* Memory_ref of starts */

/* Contans tab_width_info */

/* Base for shape (first rect) */

/* Top wrap rectangle */

$1 *$ End wrap rectangle */

/* Current wrap-target rectangle */

$1 *$ Ending rect of line */

/* Ending record for wrap rects *

/* Holds "exclusion" rectangles *

/* Saves old bottom *

/* Amount to add for repeat */

/* Previous start's bounds *

/* Custom hooks can use this */

/* Minimum left side */

/* Maximum right side */

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-538.jpg?height=192&width=1358&top_left_y=192&top_left_x=215)

For the sake of simplicity and clarity we will discuss only the fields that would most likely apply to a custom line_validate_proc.

repeating — is TRUE if the page_area in $p g$ is a repeating shape.

extra_indents - extra pixel amounts to inset to the top, left, bottom and right of the line. By default, these are all zero but can be changed by hook(s) to adjust a line's bounding dimensions. For example, to force a line to fit within a smaller width, extra_indents.top_left. $h$ and/or extra_indents.bot_right. $h$ could be changed.

NOTE: extra_indents are inset values, i.e. extra_indents.top_left is added to the potential line's top-left bounds and extra_indents.bot_right is subtracted from the potential line's bottom-right.

line_text_size — pnumber of text bytes in this line.

max_text_size — the maximum number of text bytes the line can use from the main stream of text. This is not necessarily the total text bytes available, rather it is an optional maximum for special features to restrict all lines to, say, 80 characters. The max_text_size field might be useful if your line_validate_proc decided the line should be smaller: the max_text_size field could be reduced and the line forced to recalculate.

styles — pointer to the current style_walk which will hold information for the current style and paragraph formats.

block — pointer to the current text_block record (for which this line belongs).

starts — pointer to the next point_start record after the line being validated.

NOTE: A "line" in OpenPaige consists of one or more point_start_records; the starts field will be the next point_startrecord should the next line be calculated.

(To get the first point_start of the line being validated, subtract measure_info -> num_starts from the measure_info->starts pointer).

num_starts — number of point_start records in the line being validated. Since the starts field (above) points to the NEXT (not current) point_start, the first point_start of the line in question is measure_info->starts - measure_info -> num_starts.

fit_rect, actual_rect — contain the maximum rectangle for which the line must be contained and the actual bounding rectangle of the line after it was computed, respectively. These fields could prove useful in determining the potential and actual bounding rectangles for the line and/or to change the maximum dimensions and force a recalculation.

wrap_r_base — a pointer to the list of rectangle in the page area. For example, if the document had three "container" rectangles,

measure_info->wrap_r_base — would point to an array of those three rectangles.

r_num_begin —bthe rectangle index of the page area this line is contained in. By "rectangle index" is meant the nth rectangle of the page area shape. For repeating shapes, the index is a modulo value representing the rectangle number of the shape $\mathrm{X}$ page number (example: for a 3-column page area, a value of " 0 " represents the first column of the first page; a value of " 3 " would be the first column of the second (repeating) page, etc.). The rectangle index is zero-based. The actual rectangle that contains the line just calculated can be determined by first determining how many rectangles there are in the page area and indexing the array of rectangles:

measure_info->wrap_r_base[measure_info->r_num_begin $\%$ num_rects];

(where num_rects is number of rectangles in the page area).

hook_refcon — can be used for anything you choose. OpenPaige initially sets this to zero and does not alter it while lines are being calculated.

## NOTES:

1. If you alter the bounding dimensions or location of the line in any way, be sure to also change measure_info->actual_rect to reflect the change.
2. measure_info->fit_rect's height is often undetermined, i.e. OpenPaige only cares about its top, left and right sides; measure_info->actual_rect, on the other hand, will contain the true dimensions of the line.
3. If you want to force a different maximum bounding area and/or the line's vertical position, change $p g \_$measure->fit_rect

(not $p g \_m e a s u r e->$ actual_rect). If you return FALSE from

line_validate_proc, OpenPaige will recalculate the line based on the (new) information in pg_measure-> fit_rect.

hyphenate_proc

PURPOSE: To compute a word break at the end of a line.

STANDARD FUNCTION: The default function, pgHyphenateProc, figures out where to break a word, including handling soft hyphen characters if they exist. (A "soft hyphen" is a control character imbedded in the text stream that is normally invisible, but defines a word break if the enclosing word will wrap).

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-540.jpg?height=198&width=1185&top_left_y=1184&top_left_x=434)

This function gets called whenever a word at the end of a line will not fit. However, the term "word" in this case really means the next character, if added to the line of text, would overflow the maximum allowed width; there might not be any real "word" at all.

Nonetheless, it is the responsibility of this function to return the word break whether or not there are "real words" and whether or not any hyphenation is to be implemented (see Function Result below regarding actual hyphenation).

Upon entry, block is the current text_block record and styles is a pointer to a style_walk record which will be set to the style affecting the first byte following the character that overflowed the line.

The block_text parameter is a pointer to all the text in block, and line_begin contains the offset into that text where the line begins, while line_end points to the offset of text after the first byte that caused the line to overflow.

EXAMPLE : If the text "abcdefg" overflows the maximum line width after the "e", then *line_end will contain the offset of " $\mathrm{f}$ " (first byte after " $\mathrm{e}$ ").

NOTE: line_end will always be the offset after only one byte of overflow; hence, correct word breaking and/or hyphenation can assume that *line_end - 1 is the maximum text position for which the line can end.

When this function returns, it must have set *line_end to the correct location.

The positions and char_types parameters point to the character positions and the character types (both obtained from the measure_proc) for all character in text (the *position for start of the line would be positions[line_begin]). See "measure_proc" on page 27-493.

If zero_length_ok is TRUE, it is acceptable to return a "word break" that results in no text at all; i.e. the word won't fit on a line and cannot be divided. However, if zero_length_ok is FALSE, this function must break the text so at least one character exists on the line.

*line_width_extra value should be set by your function to the pixel width of the hyphenation character "-" if any.

NOTE: OpenPaige uses pg_globals.hyphen_char as the hyphenation character.

FUNCTION RESULT: If you want the line to be drawn with a "-" (hyphenation char), return TRUE, otherwise return FALSE. Note that you must still break the word by setting *line_end: the function result simply indicates whether or not the line must now include a hyphenation symbol to be drawn.

hilite_rgn_proc

draw_hilite_proc

draw_cursor_proc

PURPOSE: To compute the highlight region, draw a highlight region and to draw a "caret," respectively.

STANDARD FUNCTION: The default functions $p g$ HiliteProc, pgDrawHiliteProc, and $p g$ DrawCursorProc do each of the above.

PG_FN_PASCAL (void, hilite_rgn_proc) (paige_rec_ptr pg, t_select_ptr selections, pg_short_t select_qty, shape_ref rgn);

PG_FN_PASCAL (void, draw_hilite_proc) (paige_rec_ptr pg, shape_ref rgn);

PG_FN_PASCAL (void, draw_cursor_proc) (paige_rec_ptr pg, t_select_ptr select, short verb);

For hilite_rgn_proc — selections contains an array of select_qty t_select record pairs from which to compute the highlight region). The region must be returned in $r g n$, which is a standard OpenPaige shape.

For draw_hilite_proc — the highlighting in rgn is to be drawn; this function must adjust the region for any scrolled positions and scaling factors (neither of those have been considered when computing $\operatorname{rgn}$ ).

For draw_cursor_proc — selections contains the information as to where the cursor should go and verb is the cursor drawing mode. The cursor position and height needs to be computed from the information in select.

text_load_proc

PURPOSE: To initialize the text within a text_block.

STANDARD FUNCTION: The default function, pgTextLoadProc, does nothing. The intended purpose of this low-level hook is to implement "text paging" from a file.

PG_FN_PASCAL (void, text_load_proc) (paige_rec_ptr pg, text_block_ptr text_block);

This function gets called any time OpenPaige wants to do a UseMemory on
text_block->text. Hence, a text-paging feature is given the chance to load the text for this block.

A text block record follows:

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-543.jpg?height=826&width=1356&top_left_y=440&top_left_x=197)

For more information about text blocks, see "Anatomy of Text Blocks" on page 36-1.

CAUTION: In the above record, only the text field should be changed by this function (it should be filled with the appropriate text).

pt2_offset_proc

PURPOSE: To compute a text offset belonging to a specified Coordinate.

STANDARD FUNCTION: The default function, $p g P t 2 O f f s e t P r o c$, computes the text offset from a point received in $p g D r a g S e l e c t$.

PG_FN_PASCAL (void, pt2_offset_proc) (paige_rec_ptr pg, co_ordinate_ptr point, short conversion_info, t_select_ptr selection);

point — is the coordinate from which to compute the offset.

conversion_info — indicates additional attributes to apply to the logic; this value can be either (or both) of the following bits:

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-544.jpg?height=106&width=1155&top_left_y=855&top_left_x=434)

If NO_HALFCHARS is set,-— the offset must not shift to the character's right side unless it is completely to the right of the character. In other words, if a character were 10 pixels wide, the computed offset for NO_HALFCHARS must equal the left side of that character until the point was at least 10 pixels to its right.

If NO_BYTE_ALIGN is set, — possibly landing in the middle of a multibyte characters should be ignored. In other words, the text position should be computed as-is without any consideration to adjust or align for multibyte character boundaries.
selection — points to a $t$ select record which this function must completely initialize. The $t$ _select record is defined as follows:

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-545.jpg?height=598&width=1321&top_left_y=283&top_left_x=216)

Upon entry, none of the fields will be initialized. When this function returns, each field must contain the following:

offset $-\mathrm{pT}$ The absolute offset, in bytes, representing the point.

word_offsets — pThe function does NOT need to initialize this field.

original_pt — bShould be a copy of the point parameter.

line —-bThe point_start element number within the text_block record that applies to offset. (Each text_block record contains an array of point_start records. The line field in $t$ _select should be the element number for the appropriate text_block, the first line for each block being zero).

flags — bShould be set to zero.

control_offset —bShould be same as offset or, if tracking a "control" style, this should be set to whatever is appropriate for the control-tracking feature.

primary_caret —pThe pixel position relative to the point_start's left bounds. In other words, caret should be the amount relative to the point_start's bounds.top_left.h indicated above (line field).
secondary_caret — bThe pixel position relative to the point_start's left bounds for a "secondary" insertion point for mixed directional scripts. If one direction only, secondary_caret must be set to the same value as primary_caret.

font_init_proc

PURPOSE: To initialize a font_info record.

STANDARD FUNCTION: The default function for the Windows version. The font name is changed to a pascal string (if info->environs has NAME_IS_CSTR set). For the Macintosh version, pgInitFont, determines the font ID code, the script code (e.g., Roman, Kanji, etc.), the language and sets most other fields to zeros. It also converts the "name" field to a Pascal string if necessary. For Windows-32, the appropriate code page and language ID is determined and the name is adjusted to a pascal string, if necessary.

PG_FN_PASCAL (void, font_init_proc) (paige_rec_ptr pg, font_info_ptr info);

special_char_proc

PURPOSE: To draw "invisible” characters.

STANDARD FUNCTION: The default function, pgSpecialCharProc, draws the symbols as specified in $p g \_g l o b a l s$ in the font specified in $p g \_g l o b a l s$.

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-547.jpg?height=194&width=1192&top_left_y=292&top_left_x=445)
style_walk_ptr walker, pg_char_ptr data, pg_short_t offset, pg_short_t length, draw_points_ptr draw_position, long extra, short draw_mode);

This function gets called after any text is drawn, but only if SHOW_INVIS_CHAR_BIT is set as an attribute in pg (see "Changing Attributes" on page 3-1 for information on $p g$ SetAttributes).

walker — contains the current format info.

data — is a pointer to the text in the current text block and offset/length are the byte position and length of the text that has just been drawn.

draw_position, extra and draw_mode — parameters are the same parameters just given to text_draw_proc.

auto_scroll_proc

PURPOSE: To perform an automatic scroll during pgDragSelect() (mouse drag).

STANDARD FUNCTION: The default function, $p g$ AutoScrollProc, performs automatic scrolling. If EXTERNAL_SCROLL_BIT is set in $p g$, only an internal adjustment is made (no visual scrolling occurs). If you need to autoscrolling in a different way than the default, use this hook to override it.

PG_FN_PASCAL (void, auto_scroll_proc) (paige_rec_ptr pg, short h_verb, short v_verb, co_ordinate_ptr mouse_point, short draw_mode);

This only gets called during pgDragSelect(), and then only if auto_scroll == TRUE. Upon entry, $h \_v e r b$ and $v \_v e r b$ indicate the direction to scroll (same possible values as given to $p g S c r o l l$ function). The mouse_point will be the current point given to pgDragSelect().

draw_scroll_proc

PURPOSE: To draw additional items when updating a scroll region.

STANDARD FUNCTION: The default function, $p g D r a w S c r o l l P r o c$, does nothing. This low-level function has been provided for special features where the application needs to update something on the screen and, since OpenPaige can do autoscrolling, this provides a way to add "ornaments" to the scrolled area. The draw_scroll_proc now gets called twice, once before updating the scrolled area and once after updating the scrolled area.

PG_FN_PASCAL (void, draw_scroll_proc) (paige_rec_ptr pg, shape_ref update_rgn, co_ordinate_ptr scroll_pos, pg_boolean post_call);

This only gets called immediately after a physical scroll. The update_rgn contains the shape that requires updating (the "blank" part of the screen after a scroll). The scroll_pos parameter contains the current horizontal and vertical scrolled position (which always gets subtracted from drawing coordinates when updating the screen).

However, draw_scroll_proc gets called twice: after physical scrolling occurs and before any text is drawn inside the scrolled region, draw_scroll_proc is called and passes FALSE for post_call; then once all text is redrawn, draw_scroll_proc gets called again with post_call as TRUE.

NOTE: To further understand the relationship between scrolling, display and the scrolling "hooks" please see “scroll_adjust_proc" on page 27-552.

PURPOSE: To add additional graphics to the offscreen bitmap before stamping such bits to the screen.

STANDARD FUNCTION: The default function, pgBitmapModifyProc, does nothing. This low-level function has been provided for special features where the application needs to display something in the "background" such as a picture for which text overlays, which normally would get "erased".

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-549.jpg?height=150&width=1104&top_left_y=778&top_left_x=448)

If OpenPaige does offscreen drawing, this function gets called twice: once after the bitmap is set up but before any text is drawn, and once after the text is drawn into the bitmap but before transferring to the screen.

bits_port — the offscreen bitmap port (see"Graphic Devices" on page 3-8). If post_call is TRUE, the function is getting called the second time (after the text is drawn into the bitmap but before transferring to the screen).

bits_rect — the target rectangle that the bits will eventually get copied to. In other words, the target rectangle is the bounding area on the screen for the text line being prepared for eventual bits transfer. The "local" rectangle for the bitmap area itself (whose top-left coordinate is typically 0,0 ) is bits_rect offset by screen_offset->h and screen_offset->V.

text_position — parameter is the text position (relative to all text in $p g$ ) for the line about to be drawn or just drawn.

NOTE: This is the hook you use to do "backgrounding", i.e. to display some kind of graphic or pattern behind editable text.

The purpose of the bitmap_modify_proc is to alter the contents of OpenPaige's offscreen bitmap before it transfers those bits to the screen. Note that this only occurs
when OpenPaige is drawing in one of the "bits" draw modes: bits_copy, bits_or, bits_xor, etc. The bit transfer will always be targeted to the graf_device currently set in the $p g \_r e f$ - pthere is no way to change what device will receive the bitmap other than assigning a different device to a $p g \_r e f($ using $p g S e t D e f a u l t D e v i c e)$ before calling any function that might draw text. (The exception to this is when the function has as one of its parameters an optional graf_device such as pgPrintToPage).

NOTE: The bitmap_modify function will also get called for the whole window if bits_emulate_or, bits_emulate_xor or bits_emulate_copy are indicated as the drawing mode. In this case, the "bitmap" is really the entire drawing area for the $p g \_$ref on the screen. For maximum performance, you should use "bits_emulate_xx" modes for backgrounding something if the entire document is repainted on an erased window.

NOTE (Windows): The device context of the bitmap (or screen if bits_emulate_xx drawing mode) is device->machine_ref.

NOTE: bitmap transfer mode might still occur even if you did not explicitly pass one of the "bits" draw modes. If a function was called that suggested best_way for the drawing mode, OpenPaige often decides that bitmap transfer is best_way and assumes that mode.

## wait_process_proc

PURPOSE: To inform the application when something that can take a bit of time is being performed.

STANDARD FUNCTION: The default function, $p g$ WaitProc, does nothing. This low-level function has been provided so the application can display messages, put up "thermometers," etc. when something is going on that can take a while.

PG_FN_PASCAL (void, wait_process_proc) (paige_rec_ptr pg, short wait_verb, long progress_ctr, long completion_ctr);
wait_verb — defines what OpenPaige is doing and will be one of the following:

typedef enum \{

paginate_wait,

copy_wait, insert_wait, save_wait, open_wait

progress_counter - some number less than or equal to completion_ctr; however, the first time this function gets called, progress_counter will be zero; the final call (when the operation has completed) progress_ctr will equal completion_ctr.

A "percentage completion" can be calculated as:

(progress_ctr * 100) / completion_ctr

draw_page_proc

PURPOSE: To inform the application when the whole screen (or part of the screen) gets repainted. The purpose of this is to draw any special page or document ornaments such as page break or "margin" lines, page numbers, container outlines, headers and footers, etc.

STANDARD FUNCTION: The default function, pgDrawPageProc, does nothing. This low-level function has been provided so the application can draw "pagination" and other document items. See "Display Proc" on page 16-16.

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-552.jpg?height=153&width=1229&top_left_y=195&top_left_x=438)

OpenPaige calls this function only after a general display (from $p g$ Display), after a scroll (pgScroll or any scrolling function), and printing (pgPrintToPage). This function is not called for any other display, including keyboard character insertions. The assumption is that page ornament items are "clipped" for regular typing, but require updating for general display.

Upon entry, page_shape is a pointer to the first rectangle of the page area in $p g$. The r_qty parameter will contain the number of rectangles within that shape (it will always be at least one).

page_num — indicates the "page" you are being asked to draw, the first page being zero. For a new $p g \_r e f$ in which only the defaults are used, page_num will always be zero. If $p g$ is set for repeating shapes (V_REPEAT_BIT or H_REPEAT_BIT set in $p g \_d o c \_i n f o$ ), page_num will indicate the page number (beginning at zero) you are asked to draw which can be a multiple repeat of the original shape. The draw_page_proc will get called for every "page" that is visible, one at a time.

vis_offset — will contain horizontal and vertical pixel amounts that should offset the rectangle(s) in page_shape to obtain the exact screen location for the "page" that is being drawn. In other words, if page_shape pointed to a single rectangle, the onscreen page dimensions are precisely *page_shape offset horizontally by vis_offset$>h$ and offset vertically by vis_offset $->v$.

For example, if an OpenPaige object is set for a repeating shape resulting in five "pages" on the screen (which is to say, the original shape repeats itself five times), draw_page_proc would be called five consecutive times, the first time passing zero for page_num, the second time a 1, then 2, 3, 4 and 5. Also, for each consecutive call, vis_offset would contain the appropriate pixel amount to adjust the rectangles in page_shape to draw each page at the correct screen location.

The draw_mode_used parameter indicates which drawing mode was used for text display before draw_page_proc was called (note that all text is drawn to the screen first, then draw_page_proc).

This function is not called for "pages" that fall completely out of the vis area in $p g$.

NOTE (Windows): The device context for drawing the page(s) is available as $p g$->globals->current_port->machine_ref.

CAUTION: Warning! The page_ptr is literally a pointer to the contents of $p g$ 's page_area shape. Do not alter these rectangles!

## CLIPPING

Upon entry, the "clip region" will be set to pg's vis_area boundaries. Normally, you should not need to change the clipping area.

## PRINTING NOTE

The draw_page_proc also gets called for printing. In certain cases, you might not want to draw page ornaments (such as page break lines) while printing. To detect printing mode, check $p g$->flags as follows:

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-553.jpg?height=208&width=799&top_left_y=1113&top_left_x=485)

See also, pgSetDocInfo in “Getting/Setting Document Info” on page 13-9.

text break_proc

PURPOSE: To find the best place to split apart a block of text. OpenPaige does not hold a large continuous block of text, rather it breaks text up into smaller sections. The text_break_proc is used to determine where in a section of text is a good breaking point.

STANDARD FUNCTION: $p g$ TextBreakProc: if $<\mathrm{CR}>$ characters exist in the text block, the closest CR to the middle of the block is returned as the best breaking point. If no CR's, the function recommends breaking on a line (word-wrap) boundary. If no lines (or one huge single line), the break occurs on a word boundary; if no words then text_break_proc has no other recourse than to break in the middle of text.

PG_FN_PASCAL (long, text_break_proc) (paige_rec_ptr pg, text_block_ptr block);

block - a pointer to the text block that must be split apart. The function result should be the relative offset to break the text (relative to the text in the block - pnot to all text in pg) .

scroll_adjust_proc

PURPOSE: To allow an application to adjust something before and after a document scrolls.

STANDARD FUNCTION: $p g$ ScrollAdjustProc does nothing.

PG_FN_PASCAL (void, scroll_adjust_proc) (paige_rec_ptr pg, long amount_h, long amount_v, short draw_mode);

Upon entry, amount_ $h$ and amount_ $v$ — are the amounts in pixels that will be scrolled horizontally and vertically, respectively. Negative amounts indicate the document contents will move upwards and/or left and positive amounts indicate the document contents will move down and/or to the right.

This function gets called twice, once before any physical scrolling occurs and once after scrolling occurs. You can detect the difference by the values in amount_ $h$ and amount_v: if scroll_adjust_proc is getting called after scrolling, both parameters will be zero.
draw_mode — indicates what the drawing mode will be. Note that it is possible for draw_mode t o be draw_none; it might be wise to observe that situation since it could make a difference in how you handle a scrolling adjustment.

## SEQUENCE OF SCROLL HOOKS \& DISPLAY

Scrolling hooks and display occurs in the following sequence:

- Application calls pgScroll (or any other function that causes a scroll).
- OpenPaige computes the amount to scroll, in pixels, and calls adjust_scroll_proc with those values.
- The window is scrolled.
- The scroll_adjust_proc is called again with 0,0 for "scroll amounts."

If draw_mode is not draw_none:

- The draw_scroll_proc is called with FALSE for post_call.
- Screen is refreshed (for scrolled area).
- The draw_scroll_proc is called once more with TRUE for post_call.

enhance_undo_proc

PURPOSE: To allow for custom "undo" and/or to modify an existing undo record prepared for undo.

STANDARD FUNCTION: pgEnhanceUndo does nothing.

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-555.jpg?height=157&width=1105&top_left_y=1602&top_left_x=445)

This function gets called from either pgPrepareUndo or pgUndo; the difference can be determined by the action_to_take verb which will be one of the following:

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-556.jpg?height=253&width=1143&top_left_y=265&top_left_x=434)

insert_ref — will be whatever was given to the same parameter for pgPrepareUndo (or NULL if enhance_undo_proc is being called from $p g U n d o$ ).

undo_rec — a pointer to an OpenPaige undo record as follows:

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-556.jpg?height=849&width=1463&top_left_y=855&top_left_x=209)

The fields of interest (for custom undo) are as follows:

verb — Holds the original undo verb as given to pgPrepareUndo. However, if verb is negative then the record is intended for a "redo." Example: — undo paste is "redo paste."
globals - A pointer to OpenPaige globals.

applied_range — If non-NULL this is a memory_ref containing select_pair's defining the selection range for which the undo/redo applies.

alt_range — Contains the range of text for which this undo/redo applies (if selection is simply two offsets). If more complex selection, the offsets will be in applied_range.

ref_con — Can be used by your application for anything.

The precise calling sequence of enhance_undo_proc in relationship to pgPrepareUndo and pgUndo is as follows:

- When pgPrepareUndo is called, the undo record is prepared with everything OpenPaige "knows" about. Then before returning from pgPrepareUndo, enhance_undo_proc is called passing the pg_undo_ptr (just prepared) and the action_to_take will be enhanced_prepared_undo.
- When $p g$ Undo is called, everything is "undone" that OpenPaige knows about. Then before updating anything on the screen, enhance_undo_proc is called, passing the undo record and action_to_take is enhanced_performed_undo.

NOTE: Both $p g$ PrepareUndo and $p g U n d o$ will only handle the verbs that it recognizes. It means that a completely foreign undo verb causes OpenPaige to do nothing at all —bbut it still calls enhance_undo_proc, passing your application an empty pg_undo record the "verb" field set to whatever you gave). Hence, a completely custom prepare undo/redo is possible by inventing undo verbs that OpenPaige doesn't understand.

For example, if you called $p g$ PrepareUndo( $p g, 9000, p t r)$, OpenPaige will have no idea what " 9000 " is —bbut it will create an empty undo record, place " 9000 " in the verb field and call enhance_undo_proc(pg, \&undo_rec, ptr, enhance_prepared_undo).

Of course you can also use enhance_undo_proc to simply modify existing undo operations. For example, additional information can be placed in the ref_con field while you let OpenPaige handle everything else.

CAUTION: While it is perfectly OK for you to set up a completely custom undo_ref, do not call pgDisposeUndo if you have set any of the fields besides the verb and ref_con, even if you have set an unrecognized undo verb. Instead, dispose the structure(s) yourself.

NOTE: There is no verb for "dispose undo" since there is not necessarily an associated $p g \_r e f$ when an undo_ref is disposed (thus there is no function pointer!). Therefore you must dispose your own data structures, if any, before calling pgDisposeUndo.

par_boundary_proc

PURPOSE: To quickly locate the text offsets of a paragraph.

STANDARD FUNCTION: $p g$ ParBoundaryProc locates the beginning and ending offsets that enclose a paragraph of text.

PG_FN_PASCAL (void, par_boundary_proc) (paige_rec_ptr pg, select_pair_ptr boundary);

This function gets called when OpenPaige wants to know the offsets of a paragraph. Upon entry, boundary->begin contains the text location in question; this function should initialize boundary->begin and boundary->end to the beginning and ending offsets of the paragraph.

change_container_proc

PURPOSE: To change, modify or enhance a "container" before erasing it, displaying text, calculating text or clipping its bounding region.

STANDARD FUNCTION: $p g$ ModifyContainerProc erases the container if the verb parameter so designates, otherwise does nothing.

PG_FN_PASCAL (void, change_container_proc) (paige_rec_ptr pg, pg_short_t container_num, rectangle_ptr container, pg_scale_ptr scaling, co_ordinate_ptr screen_extra, short verb, void PG_FAR *misc_info);

OpenPaige calls this function to give the application a chance to modify something to achieve desired affects for different text "containers" or areas in the page shape.

For example, OpenPaige always calls change_container_proc to "erase" a container. If your application wanted to provide different background colors for different containers, it could use this function to set and erase the appropriate backgrounds.

Upon entry, container_num is the rectangle number of the "container" and container is the pointer to the rectangle. The container number is one-based, i.e. the first rectangle is 1 . The rectangle itself is unscaled and not scrolled.

scaling — indicates any scaling (or can be NULL).

screen_extra — contains the amount of offset that would be required to obtain the actual rectangle on the screen. In other words, container offset by screen_extra-> $h$ and screen_extra- $>v$ is the actual container position in the scrolled document.

verb - indicates why the function is being called, which will be one of the following:

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-559.jpg?height=450&width=1156&top_left_y=1505&top_left_x=434)

What gets passed in ${ }^{*}$ misc_info depends on the value of verb, as follows:

If verb is clip_container_verb, misc_info is a pointer to a long which is initially set to zero.

If verb is unclip_container, misc_info points to the same long as in clip_container_verb. The purpose of this is to let you set *misc_info to something (such as the previous clip region) so you can use it when you unclip the area.

If verb is erase_rect_verb, will_draw_verb or will_delete_verb, misc_info is NULL.

NOTE: The term "containers" in this description really means a rectangle inside the page area. Complex, irregular shapes can therefore have hundreds of "containers" even though your application might think of the page shape as one object. Therefore do not confuse the logical "container" as a single unit shape or page with the OpenPaige meaning of a rectangle within a shape.

smart_quotes_proc

PURPOSE: To implement "smart quotes" for text insertions.

STANDARD FUNCTION: If the next insertion is a "flat" single or double quote character, the insertion is changed by pgSmartQuotesProc to the appropriate left or right quote characters per definitions in $p g \_g l o b a l s$.

PG_FN_PASCAL (void, smart_quotes_proc) (paige_rec_ptr pg, long insert_offset, long info_bits, pg_char_ptr char_to_insert, short PG_FAR *insert_length);

This function gets called if the byte about to be inserted into a pg_ref is a "quote character”.

NOTE: This is determined purely by the char_info hook for the insertion style returning such information about the character.

Upon entry, insert_offset is the byte_offset in text that will be inserted (relative to all text). char_to_insert points to the first byte to be inserted (whose first character will be at least one of the quote characters as defined in the OpenPaige globals). The info_bits parameters contains the results of char_info, indicating which quote character will be inserted.

Upon entry, *insert_length contains the number of bytes of the character to be inserted. To change the character to, say, an appropriate smart quote, this function should physically change *char_to_insert and set *insert_length to the new length if it is different.

NOTE: *char_to_insert will always be big enough to hold up to four bytes.

If it is decided that the character should change, smart_quotes_proc should literally alter ${ }^{*}$ char_to_change.

line_init

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-561.jpg?height=111&width=971&top_left_y=1085&top_left_x=447)

This function is called once before building lines of text, once before building an individual line and once when all lines have been built.

PURPOSE: Its intended purpose is to let an application set up whatever it needs to handle subsequent calls to other hooks (such as line_parse_proc below).

STANDARD FUNCTION: The default proc is pgInitLineProc which does nothing.

Upon entry, the information about the line to be calculated is contained in measure_info(see pg_measure_ptr in "line_validate_proc" on page 27-534).
![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-562.jpg?height=850&width=1160&top_left_y=226&top_left_x=434)

This hook gets called repetitively when a line of text in $p g$ is being built. Its intended purpose is to force a particular point_start record (many of which can compose a single line) to assume a certain length.

STANDARD FUNCTION: The default function is pgLineParse which does nothing.

One example of this would be building an array of "cells", in which an application needed to display a matrix of rows and columns for every "number" that appeared in a plain line of text. Normally, the line would be formatted as a single point_start record, yet a row/column display feature would require the line to break apart into one point_start for each "cell".

When this function is called, OpenPaige is essentially asking where the end of the current point_start record should break. The measure_info parameter is a pointer to the current position and point_start(s) of the line (see pg_measure_ptr "line_validate_proc" on page 27-534). The line_start parameter is the current point_start record (the one being asked about). The text parameter is the text to be included in the current point_start and remaining_length its maximum length.

What this function must do is return the new remaining length that the line builder should work with.

For example, in the cell/row application, the function would examine the text, and notice that the next "number" is 6 bytes in length. In this case, this function would return a " 6 " and the point_start for this portion of the line would be limited to 6 bytes.

The function would then be called again, after the 6-byte point_start had been formatted (if there is any more text available and the maximum line width permits it).

paginate_proc

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-563.jpg?height=211&width=1098&top_left_y=799&top_left_x=434)

This function is called every time after OpenPaige has paginated a block of text. By paginate is meant the computations of lines, their text widths and vertical/horizontal positions on the page.

STANDARD FUNCTION: The default proc, pgPostPaginateProc, implements widow and orphan control and "keep-paragraphs-together" if the document is set for repeating pages.

Upon entry, block is the text_block just paginated.

The update_info parameter is provided in case this function needs to change the recommended beginning of display. If so, update_info->suggest_begin and/or update_info->suggest_end can be altered

NOTE: .update_info might be null.

The lineshift_begin and lineshift_end values indicate a range of text that has moved vertically by virtue of pagination. These two values are text-line based (operate on line boundaries) and provide valuable information for performance purposes. For example, if
the user inserted a character causing no new wordwrapping, lineshift_begin will equal lineshift_end, which means none of the lines in the document have shifted vertically.

The action_taken_verb indicates what has occurred, which will be one of the following:

enum

\{

paginated_line_shift, /* Only shifted line locations vertically */

paginated_empty_block, /* Built an empty block */

paginated_hidden_block, /* Built block whose text is all invisible. */ paginated_fake_block, /* Built dummy block -- sits below last container */

paginated_partial_block, /* Rebuilt lines only partially */

paginated_full_block /* Rebuilt everything */ \};

click_proc

PG_FN_PASCAL(void, click_examine_proc) (paige_rec_ptr pg, short click_verb, short modifiers, long refcon_return, t_select_ptr begin_select, select_ptr end_select);

This function gets called after pgDragSelect has processed a "click" but before it returns control back to the application.

STANDARD FUNCTION: The default function, pgExamineClickProc, does nothing.

Upon entry, click_verb is the verb given to pgDragSelect (mouse_down, mouse_moved or mouse_up); modifiers is the same value given in pgDragSelect for modifiers. (See “Clicking \& Dragging” on page 2-43). The refcon_return field is the value that $p g D r a g S e l e c t$ is about to return.

The begin_select and end_select parameters indicate the beginning and ending points of the recent selection

NOTE: Until mouse_up has occurred, these points can be "backwards," i.e. end_select can reflect an offset < begin_select if the user clicked and dragged backwards.

text_increment

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-565.jpg?height=107&width=1184&top_left_y=683&top_left_x=446)

This function is called for every occurrence of inserting or deleting text. The intended purpose is to let OpenPaige extensions handle external "runs" when it becomes necessary to make adjustments for text insertions and deletions.

This function is always called after (not before) an insertion or deletion.

STANDARD FUNCTION: The default proc, pgTextIncrementProc, does nothing.

Upon entry, if the function is being called due to an insertion, base_offset is the text offset where the insertion occurred and increment_amount is positive (indicating the number of bytes inserted).

If the function is being called after a deletion, base_offset is the text offset before the first byte that got deleted and base_offset is negative (the absolute value is the number of bytes that got deleted).

NOTE: The function is called after the deletion, so the text that got deleted will no longer exist.

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-566.jpg?height=147&width=1217&top_left_y=262&top_left_x=434)

This function is called to prepare a device for drawing, clipping, and font selection(s), etc., or to report that the device is to be released. The verb parameter indicates whether to prepare the device or release the device.

PURPOSE: The purpose is to "prepare" or "release" a machine-specific graphics device for general use. Your application might use this hook, for instance, to prepare a special device or device characteristics. Calls to this function can be nested, so this function is responsible for handling multiple "prepare" and "release" situations.

STANDARD FUNCTION: If the verb indicates device preparation, the Windows version creates a device context (HDC), stores it into the machine_ref member of device and does a SaveDC(); the Macintosh version saves the current GrafPort then does a SetPort() with the GrafPtr in the machine_var field of device and saves all the GrafPort settings. If the verb indicates device release, the Windows version does a Restore $D C($ ) and/or a DeleteDC() if appropriate; the Macintosh version restores the original settings of the port, then sets the previous GrafPort.

Upon entry, the verb parameter will indicate one of the following:
set_pg_device,
/* Prepare the device */ unset_pg_device $1 *$ Release the device */

The device parameter points to the graf_device structure to prepare (this function alters that structure as necessary).

If $b k_{-}$color is nonnull, the background color should be set to that value.

## NESTED CALLS

OpenPaige will make frequent calls to this function, both set_pg_device and unset_pg_device, often nesting these pairs several levels deep. The standard set_device_proc handles this by incrementing/decrementing a counter in the graf_device structure, and handling the device accordingly.

For example, when verb $==p g \_s e t \quad d e v i c e$ the Windows version creates a new device context only if the counter is zero, otherwise it simply uses the previous DC (which is stored in the graf_device); then it increments the counter. Similarly, when verb == $p g \_$unset_device, the counter is decremented, and only when it decrements to zero does the DC get deleted.

page_modify_proc

$$
\begin{aligned}
& \text { PG_FN_PASCAL(void, page_modify_proc) (paige_rec_ptr pg, long } \\
& \text { page_num, rectangle_ptr margins); }
\end{aligned}
$$

This function is called for each repeating "page" during the text pagination process.

PURPOSE: Its intended purpose is to allow temporary modification(s) to any of the four sides of the page. A typical reason for doing this would be to add space for headers and footers, or extra "gutter" space based on document format and page number, etc.

STANDARD FUNCTION: The default function does nothing.

NOTE: This hook will not get called unless the $p g \_r e f$ is set for a "repeating page shape". See "Repeating Shapes" on page 13-9.

Upon entry, page_num will indicate the page number of the "page" about to be paginated; the first page is 1 (not 0 ). At this time, no text lines will have been calculated for this page, so any modifications made to the page boundary will affect the placement of text.

To "modify" the page boundary, this function should set margins to the desired amount of inset. For example, if margins->top_left.v were set to 20, the text would paginate on
this page beginning 20 pixels from the top; if margins->bot_right. $h$ were set to 50, text would wrap 50 pixels from the right side of the page, etc.

While text is being paginated, OpenPaige calls this function before it calculates the first line on a page. Each time this function is called, all four "margin" sides will be set to zero; hence if this function merely returns (does nothing), the page dimensions remain unchanged (they will retain their original dimensions as if you were not using this hook at all).

NOTE: The page_modify_proc also gets called when OpenPaige is computing the clipping region.

wordbreak_info_proc

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-568.jpg?height=207&width=1122&top_left_y=887&top_left_x=434)

This hook has been provided to allow special-case word breaking for various character sets, or multilingual scripts.

PURPOSE: OpenPaige calls this hook in response to its internal char_info function to learn about the nature of a particular character.

For example, in Japanese most characters can be considered "words" for purposes of breaking on a line, but there are several exceptions - pcertain characters must never end on a line and must stay grouped with character(s) that fall after them. Similarly there are characters that must never begin on a line and must stay grouped with character(s) before them. The purpose of this callback function is to determine of the character in question matches the application-defined table of these exceptions.

STANDARD FUNCTION: The default function returns current_settings.

Upon entry, the_char is a pointer to a character and charsize is the byte size of that character. The current style information is provided in the style parameter and the font parameter provides the current font information.

NOTE (Windows): pthe current code page for the character in question is available in the font parameter as font->code_page; the language ID (in LCID format) is available in the "language" member of font.

NOTE (Macintosh): pthe script code is in font->char_type.

The current_settings parameter contains a combination of bit settings that define what kind of character OpenPaige already thinks is appropriate, which can be a combination of any of the bit settings for the char_info_proc function.

FUNCTION RESULT: The new character bit settings should be returned. If no change to the existing settings are required, return current_settings.

key_insert_query

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-569.jpg?height=105&width=1052&top_left_y=992&top_left_x=445)

This function is called when $p g / n s e r t($ ) is called but before anything is inserted into $p g$.

PURPOSE: Its intended purpose is to speed up keyboard entry by determining if a character should be inserted immediately and redrawn, or temporarily buffered and inserted later.

STANDARD FUNCTION: The default function for Macintosh returns

key_insert_mode (signifying the character should be inserted now). The Windows version checks for a pending WM_CHAR message and, if any, returns key_buffer_mode (so the character is buffered).

Upon entry, the_char is the character to be inserted and charsize the number of bytes for the character.

FUNCTION RESULT: If the character should be inserted immediately, return key_insert_mode, otherwise return key_buffer_mode.

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-570.jpg?height=152&width=1165&top_left_y=261&top_left_x=434)

This function is called to determine the possible character subset or multilingual "class" of character.

PURPOSE: Its intended purpose is to determine a language or scripting break in the text for purposes of selection or wordwrapping. For all-Roman text, this functionality is not required (since OpenPaige handles text selection and wordbreaking automatically). For special scripts, character classes can become more complex and demand further attention.

STANDARD FUNCTION: The default function uses OS-specific functions to determine the character type of the_char. For Japanese, the function will determine which subset of the current script the_char belongs to.

Upon entry, the_char will be a single or double byte character, charsize its byte size. The style and font parameters provide the current style_info and font_info for the character.

FUNCTION RESULT: The character class type should be returned. This can be any value so long as each character of the same class return the same response. (OpenPaige simply compares the responses to each other, and if one of them is different, that is considered a change in script type. This is used to select (highlight) "words" from double-clicks, etc.

NOTE (Windows): pthe current code page for the character in question is available in the font parameter as font->code_page; the language ID (in LCID format) is available in the "language" member of font.

NOTE (Macintosh): pthe script code is in font->char_type.

### 27.9 Warning About "Re-Entrant" Functions

OpenPaige functions are generally reentrant and can therefore be called when you execute a custom hook. However, as a general rule you should never call anything that tries to change the internal structure(s) of a pg_ref. While the code itself is reentrant, data structures might be LOCKED and therefore cannot be resized. A function such as pgSetStyleInfo, for instance, often needs to add a style_info record; if the array of styles is temporarily locked, a call to pgSetStyleInfo could crash!

You should therefore restrict data changes to the parameters given to you in the low-level function itself or, if you absolutely must change something (and know what you are doing), change the structure(s) directly without resizing memory or adding/deleting records.

NOTE: All the "Get" functions, however ( $p g G e t S t y l e I n f o, p g G e t P a r I n f o$, etc.) are always safe to call.

One exception: It is OK (often anticipated) to call pgSetExtraStruct while executing a low-level hook.

### 27.10 Text Parsing \& Word Breaks

A new callback "hook" has been provided to allow special-case word breaking for various character sets (code pages). OpenPaige calls this hook in response to its internal char_info function to learn about the nature of a particular character.

For example, in Japanese most characters can be considered "words" for purposes of breaking on a line, but there are several exceptions - certain characters must never end on a line and must stay grouped with character(s) that fall after them. Similarly there are characters that must never begin on a line and must stay grouped with character(s) before them. The purpose of this callback function is to determine of the character in question matches the application-defined table of these exceptions.

PG_PASCAL (long) pgBreakInfoProc (paige_rec_ptr pg, pg_char_ptr the_char, short charsize, style_info_ptr style, font_info_ptr font, long current_settings);

When OpenPaige is determining where to break a line (word-wrap), it calls this function to determine any special-case information that might influence the word breaking result. The default function (the one that exists if your application does not set its own) does not do anything other than use the defaults, which in the case of double-byte characters it assumes that all such characters are valid word breaks.

Upon entry, the_char is a pointer to (usually) a double-byte character and charsize is the byte size of that character. The current style information is provided in the style parameter and the font parameter provides the current font information.

NOTE: The current code page for the character in question is available in the font parameter as font->code_page; the language ID (in LCID format) is available in the "language" member of font.

The current_settings parameter contains a combination of bit settings that define what kind of character OpenPaige already thinks is appropriate, which can be a combination of any of the following:

| \#define BLANK_BIT | 0x00000001 | "/ Character is blank |
| :--- | :--- | ---: |
| \#define WORD_BREAK_BIT | 0x00000002 | // Word breaking char |
| \#define WORD_SEL_BIT | $0 \times 00000004$ | // Word select char |
| \#define SOFT_HYPHEN_BIT | $0 \times 00000008$ | // Soft hyphen char |
| \#define INCLUDE_BREAK_BIT | $0 \times 00000010$ | // Include with word break |
| \#define INCLUDE_SEL_BIT | $0 \times 00000020$ | "/ Include with word select |
| \#define CTL_BIT | $0 \times 00000040$ | // Char is a control code |
| \#define INVIS_ACTION_BIT | $0 \times 00000080$ | // Char is an arrow, etc. |
| \#define PAR_SEL_BIT | $0 \times 00000100$ | // Char breaks a paragraph |
| \#define LINE_SEL_BIT | $0 \times 00000200$ | // Char breaks a line (soft CR) |
| \#define TAB_BIT | $0 \times 00000400$ | // Char is a TAB |
| \#define FIRST_HALF_BIT | $0 \times 00000800$ | // 1st half of a multi-byte char |
| \#define LAST_HALF_BIT | $0 \times 00001000$ | // Last half of a multi-byte char |
| \#define MIDDLE_CHAR_BIT | 0x00002000 | // Middle of a multi-byte char |


| \#define CONTAINER_BRK_BIT | 0x00004000 | // Break-container bit |
| :--- | :--- | :---: |
| \#define PAGE_BRK_BIT | 0x00008000 | // Break-repeating-shape bit |
| \#define NON_BREAKAFTER_BIT | $0 \times 00010000$ | // Must stay with char(s) after |
| \#define NON_BREAKBEFORE_BIT | $0 \times 00020000$ | // Must stay with char(s) before |
| \#define NUMBER_BIT | 0x00040000 | // Char is numeric |
| \#define DECIMAL_CHAR_BIT | $0 \times 00080000$ | // Char is decimal |
| \#define UPPER_CASE_BIT | $0 \times 00100000$ | // Char is lower case |
| \#define LOWER_CASE_BIT | $0 \times 00200000$ | // Char is a symbol |
| \#define SYMBOL_BIT | $0 \times 00400000$ | //Char is ASCII-European |
| \#define EUROPEAN_BIT | $0 \times 00800000$ | //Char is not Roman script |
| \#define NON_ROMAN_BIT | $0 \times 01000000$ | //Char is not really text |
| \#define NON_TEXT_BIT | 0x02000000 Char is a "flat" quote |  |
| \#define FLAT_QUOTE_BIT | 0x04000000 | //Quote char is single quote |
| \#define SINGLE_QUOTE_BIT | 0x08000000 |  |
| \#define LEFT_QUOTE_BIT | 0x10000000 | // Char is a left quote |
| \#define RIGHT_QUOTE_BIT | 0x20000000 | // Char is a right quote |
| \#define PUNCT_NORMAL_BIT | 0x40000000 | // Char is normal punctuation |
| \#define OTHER_PUNCT_BIT | 0x80000000 | // Char is other punctuation |

For purposes of the pgBreakInfoProc, the following bits are usually the only settings you will care about:

## WORD_BREAK_BIT

INCLUDE_BREAK_BIT

NON_BREAKAFTER_BIT

NON_BREAKBEFORE_BIT

If WORD_BREAK_BIT is set that character delineates a word for wrapping (breaking) purposes; if INCLUDE_BREAK_BIT is also set, the character is part of the word.

For example, in Roman text a space character would have WORD_BREAK_BIT set but not INCLUDE_BREAK_BIT (because the space is not part of a word). However, a “," (comma) character would have both WORD_BREAK_BIT and INCLUDE_BREAK_BIT set because it is a word break but must be included with the word.

NON_BREAKAFTER_BIT causes the character to always stay with the character(s) immediately after its position in text; NON_BREAKBEFORE causes the character to always stay with the character(s) immediately before its position in text.

FUNCTION RESULT: The callback function must return the new settings that OpenPaige should use for the character. Often, this return value will simply be whatever is in current_settings.

## EXAMPLE:

PG_PASCAL (long) MyBreakInfoProc (paige_rec_ptr pg, pg_char_ptr the_char, short charsize, style_info_ptr style, font_info_ptr font, long current_settings)

\{

if (SettingsOK(the_char, charsize))

return current_settings;

\}

Examples for changing the settings to something else are shown below.

## SETTING THE HOOK

The callback ("hook") function is embedded in the $p g \_r e f$ itself. If you want the same callback function to always get called from every $p g \_r e f$ you should establish the function pointer into OpenPaige globals early before any $p g \_$refs are created.

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-575.jpg?height=352&width=1118&top_left_y=218&top_left_x=434)

In the example above, "MyBreakInfoProc" is the name of your callback function (note, for 16-bit Windows you would need to call MakeProcInstance() to create a valid function pointer). By placing it in the globals structure, OpenPaige will initialize the wordbreaking callback for all $p g \_r e f s$.

## Examples of Parsing Multilingual Word Breaks

A typical multilingual word parsing feature would be to check the code page in the word breaking callback, then compare the character to a predefined table for that code page to determine if the character has any additional word breaking attributes.

In Japanese, for example, most characters can break on a line as a "word." In some cases, however, the character can break but must be included with the "word" immediately to the left or to the right. The following is an example of tagging these exceptions in "MyBreakInfoProc" (your application-defined word breaking callback):

\#define CP_JAPANESE932// Japanese code page

\#define CP_US 1252 // United states and Latin

PG_PASCAL (long) MyBreakInfoProc (paige_rec_ptr pg, pg_char_ptr the_char, short charsize, style_info_ptr style, font_info_ptr font, long current_settings)

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-576.jpg?height=1807&width=1184&top_left_y=177&top_left_x=433)

In the above example, the "table" variables can be simple resources that contain a list of characters. Usually you only care about double-byte characters (because OpenPaige handles most single byte characters correctly using its defaults - but that might change with certain languages). The "FindCharInTable" function would simply be a function that returns TRUE if the character in question was in the table.

### 27.11 Character/Language Subsets

Certain languages can have "subsets" of characters that belong together as a group - at least for purposes of double-clicking for a word selection. In Japanese script, for instance, each double-byte character can be Katakana, Hiragana or Ideograph.

Although OpenPaige handles these subsets appropriately for word-selection purposes, should it become necessary to alter the subset differences or add new ones, the following callback hook is available:

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-577.jpg?height=157&width=1048&top_left_y=1113&top_left_x=447)

OpenPaige calls this hook in addition (and after) the word break callback. The purpose of the function is to return the character class - or "subset" of the language. Upon entry, the_char points to the first byte of the character and charsize is the size of the character, in bytes. The style and font parameters are the current style and font of the character. Note that font->code_page and font->language will contain the character's code page and LCID, respectively.

The following is the default code that OpenPaige-Windows executes for this function (the function that gets called if you do not set your own):

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-578.jpg?height=590&width=1237&top_left_y=265&top_left_x=432)

NOTE: The default code uses GetStringTypeEx to determine the language subset of the character. OpenPaige does not actually examine the value or contents of the function result; rather, this function is called for adjacent characters to determine if a word selection (highlight) should continue.

For example, if the first five characters in a string returned $0 \mathrm{x} 0000$ from pgCharClassProc, then the sixth character returned $0 x 0001$, OpenPaige would only highlight the word(s) within the first five characters if the user double-clicked.

### 28.1 Inserting graphics \& user items

DEFINITION: A non-text character is a graphic display embedded into the text stream of an OpenPaige document, such as a picture, box or special string (such as a page number, footnote, etc.). It is not an ASCII byte as such, but otherwise looks and behaves like an ordinary character. It can be clicked, deleted, and subject to copy/paste.

The purpose of this chapter is to explain the built-in, high-level support for these special characters.

## DISCLAIMER

There are several undocumented references in $p g E m b e d . h$. If anything in that header file is not explained in this chapter, it is not supported. The purpose of these definitions is for possible future enhancement and/or custom development by DataPak Software, Inc.

## Description

OpenPaige provides a certain degree of built-in support for graphic characters. For the Macintosh version, PicHandles (pictures) can be inserted into the text stream with practically no support required from your application. For the Windows version, meta files can be inserted in the same way.

For other graphic types and/or "user items" (custom characters), OpenPaige supports a variety of user-defined non-text character insertions; your application can then handle the display and other rendering through a single callback function.

All the functions documented in this chapter are prototyped in pgEmbed.h. You therefore need to include this header file to use the structures, functions and callbacks.

### 28.2 The embed_ref

The first step to embedding a non-text character is to create an embed_ref:

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-580.jpg?height=206&width=1196&top_left_y=1123&top_left_x=433)

This function returns a special memory_ref that can be subsequently inserted into a $p g \_r e f$ as a "character". Once you have created an embed_ref, call pgInsertEmbedRef() below.

mem_globals — must be a pointer to your memory globals (same structure that was also given to $\mathrm{pgMemStartup}$ and $\mathrm{pg} / \mathrm{nit}$ ).

item_type — indicates the kind of object you want to create. This value can be any value shown in "Embed Ref Types” on page 28-583.
item_data, modifiers — What you provide in item_data and modifiers depends on the item_type; these are also described in "Embed Reference Types."

flags — should be set to zero (with some unusual exceptions - "Special Cases" on page 28-599).

vert_pos — Its purpose is to indicate a descent value for the object you will be inserting. By descent is meant the amount the item should be offset vertically below the baseline.

If vert_pos is positive, it is considered to be a percent of the item's total height. If vert_pos is negative, it is considered to be a pixel value. Carefully note that in both cases, vert_pos is a Fixed value — the high-order word is the whole integer and the low-order word a fractional part.

For example, if vert_pos is 0x000A0000, the embed_ref will be offset $10 \%$ of its total height from the text baseline. If vert_pos is 0xFFFEFFFF (negative 0x00020000), the item will be offset 2 pixels below the text baseline, etc.

The following illustrations show a typical embed_ref's descent values for different vert_pos values:

## This is text with

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-581.jpg?height=129&width=279&top_left_y=1148&top_left_x=1085)

The above shows the result of an embed_ref with vert_pos $=0$ (no descent from baseline).

\section*{This is text with

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-581.jpg?height=131&width=120&top_left_y=1532&top_left_x=1088)
inserted.}

The above shows the result of an embed_ref with vert_pos $=50.00$ (descent is $50 \%$ of height).

The above shows the result of an embed_ref with vert_pos $=-3$ (descent is 3 pixels)

user_refcon — is saved inside the embed_refitself, and can be anything.

keep_around — indicates whether or not the embed_ref can be automatically disposed once it is no longer being used within any existing $p g \_r e f$. If this value is FALSE then OpenPaige is authorized to dispose of it once it is no longer being used by any $p g \_r e f ;$ a TRUE value tells OpenPaige it must never dispose it even if no $p g \_r e f$ contains the embed_ref.

To understand the full meaning of keep_around, realize that an embed_ref can be "shared" by multiple positions in a document, and even between different documents. For example, if the user performs multiple copy/paste operations on a single embed_ref, OpenPaige won't actually duplicate the embed_ref, rather it simply creates multiple pointers to its data.

However, once the last remaining shared copy of the reference is deleted, OpenPaige will dispose the memory_ref (if keep_around = TRUE). Normally, this is what you would want.

NOTE: If keep_around is FALSE you should never dispose the embed_ref(OpenPaige will dispose of it at the appropriate time). If keep_around is TRUE then you need to eventually dispose the reference with pgDisposeEmbedRef().
pg_boolean pgInsertEmbedRef (pg_ref pg, embed_ref ref, long position, short stylesheet_option, embed_callback callback, long callback_refcon, short draw_mode);

This function inserts an embed_ref as a "character" into $p g$ at the specified text location. The position parameter indicates the text offset (relative to zero) to insert the embed_ref; the position parameter can also be CURRENT_POSITION which causes the insertion to occur at the current insertion point.

stylesheet_option — is an optional stylesheet ID that gets automatically applied to the embed_ref "character". If you merely want to use whatever style applies to the text position, pass zero for stylesheet_option, otherwise you need to create a new stylesheet and provide that stylesheet ID.

callback — is a pointer to a function (in your application) that will be called for various occurrences; the purpose of this callback is to handle custom characters and/or to modify the default behavior of standard embed_refs. However, if you want OpenPaige to handle the embed_ref in the default way, pass NULL for callback.

NOTE: Only certain embed_ref types are supported with a "default behavior", and therefore only those types will work correctly if you pass NULL for callback; see "Embed Ref Types" on page 28-583 below.

callback_refcon — value can be anything you want; this same value will be given to your callback function. If you have not supplied a callback function (callback = NULL), this parameter does not matter.

draw_mode — indicates whether or not to redraw the text after the embed_ref is inserted; the value you pass for this parameter is identical to all other OpenPaige functions that accept a draw_mode.

CLARIFICATION: Do not confuse callback_refcon given to pgInsertEmbedRef with user_refcon given to $p g N e w E m b e d R e f$. These are two separate values. The user_refcon given to pgNewEmbedRef is stored within the embed_ref itself (and the same embed_ref can exist as multiple copies throughout a document); the callback_refcon is specific to the insertion itself and can be different
for all occurrences of the embed_ref. The callback_refcon is stored in the style_info that is applied to the insertion, and is also passed to your callback function (if one exists).

The user_refcon will always be whatever value you passed to pgInsertEmbedRef() in the 'callback_refcon' parameter except for EMBED_READ_DATA and EMBED_WRITE_DATA, in which case the user_refcon will be the original user_refcon value given to pgNewEmbedRef().

CAUTION: Do not insert the same embed_refmore than once unless you have created it with keep_around = TRUE. Otherwise, OpenPaige can dispose it prematurely and your program will crash. Once it has been inserted, however, it is OK to copy and paste that character to as many documents as memory permits.

NOTE: Once the embed_ref has been inserted, OpenPaige "owns" its memory, i.e. you must not dispose of it as long as it exists in any $p g \_r e f$ (and, if you passed FALSE for keep_around in pgNewEmbedRef() you must not dispose of it at all, at any time).

## The Stylesheet Option

Any embed_refhas the option to alter the style and/or font of the text it applies to.

For example, a string-type embed_ref(embed_alternate_char) will normally assume the style and font of the text where it was embedded; however, by passing a nonzero stylesheet ID number in stylesheet_option in pgInsertEmbedRef or pgSetEmbedRef, the style or font can be overridden.

A stylesheet ID is obtained by first creating a new style_info record and adding it to the $p g \_r e f$ as a new stylesheet (see "Style Sheets" on page 31-1). The stylesheet ID is then given to the stylesheet_option parameter, in which case the text for which the embed_ref applies will assume that style.

However, a stylesheet applied to an embed_ref works slightly differently than normal styles: only the nonzero items in the style_info record of the stylesheet are applied.

For example, suppose a new stylesheet is created with every field in its style_info record set to zero except for the italic attribute. If this stylesheet is applied to an embed_ref, the text is forced to "italic" but retains all other attributes (keeps same point size as before, same font, etc.).

The following is an example of applying italic to an embed_ref; note that the embed_ref text retains all style and font characteristics except it changes to italic:

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-585.jpg?height=600&width=1040&top_left_y=511&top_left_x=444)

If you want to change only the font in the embed_ref text, use the same example as above except omit changing the italic attribute and pass a font_info record instead of NULL for pgNewStyle.

### 28.4 Embed Ref Types

The following table describes each possible embed_ref data type and what you should pass in the item_data and modifier parameters for pgNewEmbedRef(). The

Support column indicates which OpenPaige platform supports the data type. All the items listed are supported to some extent, i.e. none of them require a callback function to render a default behavior.

NOTE: IF THE DATA TYPE IS NOT LISTED, THERE IS NO CURRENT OpenPaige SUPPORT FOR THE TYPE. (You can, of course, support your own using the callback function).

| TABLE $\# 5$ | EMBED_REF | DATA TYPES |  |  |
| :---: | :---: | :---: | :---: | :---: |
| Data Type | Support | *item data <br> parameter | modifier | Note(s) |
| embed_rectangle | all platforms | rectangle_ptr | pen size (pixels) |  |
| embed_oval | all platforms | rectangle_ptr | pen size (pixels) |  |
| embed_roundrectangle | all platforms | rectangle_ptr | round corner + <br> pen size | 1 |
| embed_control | Macintosh <br> only | ControlHandle | not used | 2 |
| embed_polygon | all platforms | memory_ref of poly- <br> gon | pen size (pixels) | 3 |
| embed_mac_pict | Macintosh <br> only | PicHandle | not used |  |
| embed_mac_vm_pict | Macintosh <br> only | memory_ref of Pict <br> data | not used | 7 |
| embed_meta_file | Windows <br> only | metafile_ptr | not used | 4 |
| embed_alternate_char | all platforms | cstring (any size) | not used | 5 |
| embed_user_data | all platforms <br> (limitted) | pointer to data | data size (bytes) | 6 |


| Data Type | Support | *item_data <br> parameter | modifier | Note(s) |
| :--- | :--- | :--- | :--- | :--- |
| embed_dynamic_string | all platforms | cstring (any size) | max size (bytes) |  |
| embed_user_box | all platforms <br> (limitted) | rectangle_ptr | pen size (pixels) | 1 |

embed_roundrectangle — the low-order word of modifier is the pen size; the highorder word is the "rounded corner" value, e.g. FrameRoundRect(rect, value, value).

embed_user_box — The default behavior for embed_user_box is identical to embed_rectangle. To modify the default behavior use the callback function.

ControlHandle — is detached from any Window before it gets inserted.

data — is a memory_ref that must contain the following structure:

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-587.jpg?height=446&width=1161&top_left_y=1110&top_left_x=434)

width and height - members should contain the width and height of the bounding area of the polygon. The num_points member should contain the number of connecting points in the points $\square$ member array.

points — are represented by a series of co_ordinate pairs; the first pair is a line, the second pair is another line, etc.

## NOTES:

(F) The points array must therefore be in PAIRS.

(G) A Windows meta file must be represented by the following structure (pointed to by the item_data parameter);

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-588.jpg?height=398&width=1178&top_left_y=402&top_left_x=432)

metafile - The meta file HANDLE should be in the metafile member; the mapping mode for the meta file should be in mapping_mode. For most meta files the mapping mode is MM_ANISOTROPIC.

$x \_e x t$ and $y \_e x t$ _ members should contain the mapping mode-specific $x$ and $y$ extents, respectively. You can also set these to zero (in which case the default width and height of the meta file will be used, taken from the bounds member). Most often, the mapping_mode, $x$ and $y$ extents are taken from clipboard information.

bounds — member defines the meta file's dimensions in screen coordinates.

The metafile HANDLE should be in the metafile member; the bounds member must define the bounding area of the metafile (the enclosing rectangle as the metafile was recorded).

embed_alternate_char and embed_dynamic_string — draw a whole string to represent a single "character".

The embed_dynamic_string, — however, can be dynamically altered (changed or "swapped" with a different string) in the callback function for display and character measuring purposes.

embed_alternate_char and embed_dynamic_string — are treated as a single character and will therefore not wrap or word break in the middle.
embed_user_data — the item is considered custom (generally handled by your callback function), but OpenPaige will save and retrieve your data automatically when saving to files. The data is assumed to be a contiguous byte stream in *item_data.

item_data for embed_mac_vm_pict — must be a memory_refcontaining the data from a PicHandle. This type is behaves exactly the same as embed_mac_pict except the memory_ref provides virtual memory to the picture.

### 28.5 The Callback Function

Any custom user type embed_ref, embed_refs that are not supported, or items that require modification(s) to the default behavior will require a callback function. The callback function is a pointer to some code (that you write) that gets called for a number of occurrences:

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-589.jpg?height=146&width=1203&top_left_y=1069&top_left_x=446)

Each embed_ref you have inserted can have its own callback function (or they can all share the same callback if you so choose). The callback is set by passing the function pointer to the "callback" parameter in pgInsertEmbedRef().

NOTE (Windows 3.1 Users): you need to set a callback function that has been created with MakeProcInstance().

Upon entry, $p g$ is the OpenPaige record structure that owns the embed_ref; the embed_ptr parameter is a pointer to the embed_ref record structure (see below), and embed_type is the data type (same one you gave to pgNewEmbedRef() when it was initially created).

command — indicates why the function is being called, and param1/param2 will contain different values depending on what value is in command (see "Command Messages” below).
user_refcon — will be whatever value you passed to pgInsertEmbedRef() in the "callback_refcon" parameter except for EMBED_READ_DATA and EMBED_WRITE_DATA, in which case the user_refcon will be the original user_refcon value given to $p g N e w E m b e d R e f()$.

## Command Messages

When the callback function is called, the value in command will be one of the values below. Depending on the command, param1 and param 2 contain additional data. In each case, the embed_ptr will point to the embed_ref structure (see "The Embed Record" on page 28-596).

EMBED_INIT — occurs during a pgReadDoc() function (file read). The purpose of this command is to initialize an embed_ref that has been read from a file (typically, to set a callback function specific to the associated text style). See pgInitEmbedProcs on page 604 this chapter regarding the first $p g R e a d D o c$ callback function.

Upon entry, param1 indicates how many times EMBED_INIT has been sent to the callback function during $p g R e a d D o c$ for this particular embed_ref. (Since the same embed_ref can be shared by many places in the text, your initialization code might want to know this information so the embed_ref data is initialized only once). On the first callback for this embed_ref, param1 is zero.

The param2 — will be a style_info pointer that is associated to the embed_ref.

The callback function result is ignored.

EMBED_DRAW — occurs when the embed_refshould be drawn.

Upon entry, param1 is a rectangle_ptr (an OpenPaige rectangle) that defines the exact drawing bounds of the embedded item (which includes scrolled position and scaling).

param2 - is a draw_points_ptr containing additional information for drawing (see draw_points_ptr in OpenPaige manual and/or in Paige.h).

The callback function result is ignored.

NOTE (Windows): The device context that you should draw to can be obtained as:

$$
\begin{aligned}
& \text { HDC hdc; } \\
& \text { hdc = (HDC)pg->globals->current_port.machine_ref; }
\end{aligned}
$$

Do not assume that OpenPaige is drawing to the current window (it can be drawing to a bitmap DC or a printer DC, etc.). When the callback is called, the above code is guaranteed to return a valid device context to use for drawing.

NOTE (Macintosh): The GrafPort you should draw to is set as the current port before the callback is called. Do not assume that drawing will occur to the $p g \_r e f$ window (it can occur to an offscreen bitmap port).

EMBED_MEASURE - occurs when OpenPaige wants to know the character width(s) of the embedded item.

Upon entry, param1 is a pg_embed_measure_ptr, and param2 is not used. (See “The Measure Record” on page 28-598).

NOTE: This callback is only used to obtain the object's width. Its height must be initialized either before inserting the embed_ref or in response to EMBED_VMEASURE.

Before returning from this function you should set the embed_ptr->width to the embed_ref's width, in pixels.

NOTE: OpenPaige will determine the width automatically for embed types that are fully supported (requiring no callback)

The function result from the callback function is ignored.

EMBED_VMEASURE - poccurs when OpenPaige wants to know the height of the embedded item.

Upon entry, param1 is the style_info_ptr that "owns" the embed_ref, and param2 is not used.

Before returning from this function you should set the embed_ptr->height to the embed_ref's height, in pixels.

NOTE: OpenPaige will determine the height automatically for embed types that are fully supported (requiring no callback)

The function result from the callback function is ignored.

EMBED_SWAP — occurs (only) when the item type is embed_dynamic_str. The callback function is used to "swap out" (substitute) a string of bytes with something else. For example, this type of embed_ref can be used to indicate a date or time, or page number or footnote, all of which might change dynamically.

Upon entry, param1 is a pointer to the existing string (or empty buffer), and param2 is a long value indicating the maximum number of bytes that can be put into the buffer. The job of the callback function, in this case, is to fill the buffer pointed to by param1.

The function result of this callback must be the number of bytes placed in *param1 (i.e., the string length).

EMBED_CURSOR - occurs when the mouse is on top of the embed_ref. The purpose of the callback is to let your application change the cursor, if desired.

NOTE: This callback will never occur unless you call $p g P t I n E m b e d()$.

Upon entry, param1 will be a co_ordinate_ptr for the mouse point, and param2 is a rectangle_ptr to the enclosing bounds of the embed_ref.

The callback function result is ignored.

EMBED_MOUSEDOWN — occurs during a pgDragSelect() with mouse_down verb, and an embed_refhas been clicked.

Upon entry, param1 is a pointer to a pg_embed_click record with additional info (see Click Record below).

The function result from the callback should be any nonzero value if you want to continue tracking the embed_reflike a control button, otherwise return zero. (By
tracking like a control button is meant that OpenPaige will not try to drag-select surrounding characters, rather subsequent mouse movements will be passed to your callback function with EMBED_MOUSEMOVE and EMBED_MOUSEUP commands.

CAUTION: To achieve a "push-button control" effect, mouse-click behavior may not appear to work correctly unless you include EMBED_CONTROL_FLAG in the "flags" parameter for pgNewEmbedRef(). See"Acting Like a Control" on page 28-594 in this chapter.

EMBED_MOUSEMOVE — occurs during a pgDragSelect() with mouse_move given as the verb AND you returned nonzero from the previous callback for EMBED_MOUSEDOWN.

The parameters are identical to EMBED_MOUSEDOWN.

CAUTION: To achieve a "push-button control" effect, mouse-click behavior may not appear to work correctly unless you include EMBED_CONTROL_FLAG in the "flags" parameter for pgNewEmbedRef(). See "Acting Like a Control" in this chapter.

EMBED_MOUSEUP — occurs during a pgDragSelect() with mouse_up given as the verb AND you returned nonzero from the previous callback for EMBED_MOUSEMOVE.

The parameters are identical to EMBED_MOUSEDOWN.

EMBED_DOUBLECLICK - occurs during a pgDragSelect() with mouse_down and modifier containing WORD_MOD_BIT (“double click"). The parameters are identical to the callback for EMBED_MOUSEDOWN.

EMBED_DESTROY - occurs when the embed_ref is about to be disposed. Upon entry, param1 and param 2 are not used. The function result is ignored. NOTE: You will not receive this message if you dispose your own embed_ref (e.g., made a call to pgEmbedDispose()). The only time you will receive this callback command is when OpenPaige disposes the embed_ref; this happens when the last occurrence an embed_ref has been deleted (and you gave FALSE for "keep_around" when the embed_refwas created).

CAUTION: If you have created your own data and have placed it in embed_ptr-

$>$ data, you must first dispose it (if appropriate) then set that member to NULL. However, do not dispose the data if you gave that data to item_data and the data type is embed_user_data.

CAUTION: If the embed_ref data is not supported (i.e. fully custom), do NOT call the default callback function when command is EMBED_DESTROY.

CAUTION: Do not dispose the embed_refitself. You should only dispose memory structures that you created.

$E M B E D \_C O P Y$ _ Occurs when a style_info containing an embed_refis duplicated.

This callback only occurs for embed_refs that contain NOT_SHARED_FLAG (see “Special Cases”).

Upon entry, param1 and param2 are not used. The intended purpose of $E M B E D_{-} C O P Y$ is to duplicate any memory structures you might have stored in the embed_ref.

EMBED_WRITE_DATA — Occurs when an embed_ref is saved during pgSaveDoc().

When OpenPaige saves an embed_refto a file, all the "default" information is saved before this command is given to your callback. The intended purpose of EMBED_WRITE_DATA is for you to prepare any additional data that needs to be written to the file; this same data will then be retrieved when the file is read and issued to your callback as EMBED_READ_DATA.

Essentially, when you get the EMBED_WRITE_DATA command, you don't need to do anything unless there is extra data you have stored in the embed_ref that OpenPaige won't know about; all the other embed_ref contents are saved otherwise.

Open entry, param1 is a memory_ref of zero byte size, and param1 is not used. To save any additional data associated to the embed_ref, insert the bytes into this
memory_ref. When the function returns, OpenPaige will write

GetMemorySize(param1) bytes to the file; later when the file is opened, these same bytes will be read from the file and given to your callback with

$E M B E D \_R E A D_{-} D A T A$ as the command.

When the callback returns, if the memory size of (memory_ref) param1 is zero, no extra data is saved.

The function result from the callback is ignored.

NOTE: The EMBED_WRITE_DATA callback will only occur once for each embed_ref. In other words, if multiple "shared" copies of the embed_ref exist in the document, you will only be asked to save extra data once.

EMBED_READ_DATA — Occurs when an embed_ref is read from a file during pgReadDoc(). This command will always get sent for every embed_ref that is read even if you saved no extra data (from EMBED_WRITE_DATA).

Upon entry, param1 is a pointer to the same data bytes, if any, that you saved when the command was EMBED_WRITE_DATA, and param2 is the byte count.

The function result from this callback is ignored.

NOTE: The EMBED_READ_DATA callback will only occur once for each embed_ref. In other words, if multiple "shared" copies of the embed_ref exist in the document, you will only be asked to process the data once.

### 28.6 Default Callback

You should always call the default function, $p g$ DefaultEmbedCallback from your callback code if you do not handle the command (some exceptions - see CAUTION below).

For example, you might create a callback function only for the purpose of changing the cursor when the mouse is over the embed_ref. In this case, you would not want to handle any other command, rather you want the default handling. To do so, make a call to $\mathrm{pgDefaultEmbedCallback() \text {. }}$

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-596.jpg?height=489&width=1200&top_left_y=258&top_left_x=433)

## CAUTION: Never call $p g$ DefaultEmbedCallback() for EMBED_DESTROY if

 you have placed your own data in embed_ptr->data. If you have not directly altered the "data" field in any way, it is OK to call the default.
### 28.7 Acting Like a Control

In many cases, user-defined embed_refs need to act like a "control". For example, once the user clicks in the embed_ref, the mouse needs to be "tracked" as if the embed_ref were a push-button control.

To make your embedded item behave this way, include the following value in the "flags" parameter for pgNewEmbedRef():

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-596.jpg?height=107&width=1080&top_left_y=1868&top_left_x=433)

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-597.jpg?height=634&width=1235&top_left_y=274&top_left_x=433)

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-598.jpg?height=1407&width=1349&top_left_y=287&top_left_x=241)

The above structure is what all embed_refs look like internally. Most of the fields are maintained by OpenPaige and you must neither alter them, nor assume they are valid at any time, except as specified below.

The following fields, however, can be altered (and in some cases need to be initialized) by your application:

width, height - Define the width and height of the object. The width member gets set in the callback function when the command is EMBED_MEASURE; if the item type is unsupported or custom, the height member must be initialized before inserting the embed_ref.

minimum_width — Define the minimum width (smallest size) allowed for the embed_ref. Your application needs to set this, otherwise it is zero.

descent — Defines the distance the object should draw below the text baseline. You may alter this value for a descent other than the default.

top_extra through right_extra — Define optional extra white space on the top, left, bottom and right sides of the object. The default for each of these members is zero; if you want something else you should modify them before inserting the embed_ref.

data - You may place whatever data your application requires into this member. However, please observe the following cautions:

1. Do NOT alter the data field directly for embed_user_data type or any of the supported types listed above.
2. If you place anything directly in the data member, do not call the default callback function when the command is EMBED_DESTROY.
3. You must dispose your own data, if appropriate. Letting OpenPaige handle it as a default can result in a crash.

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-600.jpg?height=650&width=925&top_left_y=293&top_left_x=420)

The measure record is passed as a pointer in param1 for EMBED_MEASURE commands. Usually you won't need to use any of these values, but they are listed here for the sake of clarity.

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-601.jpg?height=390&width=1194&top_left_y=298&top_left_x=433)

A pointer to the above structure is provided in param1 for EMBED_MOUSEDOWN, EMBED_MOUSEMOVE, EMBED_MOUSEUP and EMBED_DOUBLECLICK commands.

The first_select and last_select members represent the current beginning and ending selection point(s) of the drag-select. (For EMBED_MOUSEDOWN these are typically the same). The point and modifiers member will contain the co_ordinate value and modifiers given to $p g D r a g S e l e c t()$, respectively. The bounds member will contain the WYSIWYG bounding rectangle of the embed_refthat is being clicked.

### 28.11 Special Cases

The "flags" parameter for pgNewEmbedRef has been briefly mentioned earlier. Normally, the value for "flags" should be zero. There are two possible bit settings you can provide for this parameter for specific situations, as follows:

NOT_SHARED_FLAG — If this bit is set, the embed_ref's data is always duplicated for any copy/paste operation. Normally, when an embed_ref is copied in the text stream, its contents are not actually copied; rather, only a pointer to its reference is copied. In essence, only one "real" embed_ref exists in the document even though there could be many occurrences of the reference throughout the text stream.

However, this may be undesirable in situations where copied embed_ref(s) must be unique (as opposed to pointer "clones" of the original). If this is the case, set NOT_SHARED_FLAG.

EMBED_CONTROL_FLAG — If this bit is set, the embed_ref responds like a control (such as a button). Otherwise, the embed_ref acts like a character. Note that the only significant difference between a "control" versus "character" is the way OpenPaige highlights the embed_ref when it is clicked. As a "control," the entire embed_ref is selected from a single click.

NO_FORCED_IDENTITY — If this bit is set, when the embed_ref is inserted, OpenPaige scans the document for any embed_ref that matches its type, its width and height, and its user_refcon value. If such a match is found, the (new) embed_ref is discarded and the matching (older) embed_ref is used in its place.

The purpose of NO_FORCED_IDENTITY is to minimize the amount of memory used by repeated insertions of the same embed_reftype.

For example, suppose your application is designed to insert a mathematical symbol (that can't otherwise be represented by a text character). To achieve this, an embed_ref is created to draw the symbol and it is inserted in many different places. Normally (without NO_FORCED_IDENTITY set), OpenPaige will create a unique style_info record and embed_ref for every insertion. If NO_FORCED_IDENTITY is set, however, only one record of this symbol would exist even though it may be inserted and display in many different text positions.

### 28.12 Hints \& Tips

- For all user items, custom or non-supported embed_refs, you must initialize at least the height of the embed_refbefore you insert it. Otherwise, OpenPaige has
no idea how tall the object is (but it will get the object's width from the callback function). To initialize the height, do the following:

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-603.jpg?height=258&width=1094&top_left_y=261&top_left_x=446)

NOTE: For supported types that require no special callback function, you do not need to initialize the height - OpenPaige initializes it for you. You would only need to change the height if you wanted something other than the default.

- If you need to create a custom embed_ref that requires a block of data larger than a long word, the recommended choice is to use embed_user_data because OpenPaige will at least store the data, present a pointer to it for your callback, and save/read the data for files.This minimal support assumes that nothing in your data stream needs to be de-referenced, i.e. if you have pointers inside of pointers OpenPaige has no way of knowing how to save them.

To create an embed_ref of type embed_user_data, pass a pointer to the data in item_data and the byte count in modifier; OpenPaige will them make a copy of the data (so you can then dispose the pointer, etc.).

CAUTION: If you let OpenPaige store the data, you should neither alter it nor dispose it. Let the default callback function handle the dispose (see "The Callback Function” on page 28-587).

- For all embed_refs (both supported items and custom/user items), OpenPaige normally keeps only one embed_ref around and creates pointers to the original when the text is copied/pasted. If this default behavior is unworkable for any particular feature, pass NOT_SHARED_FLAG for the flags field in pgNewEmbedRef() (see“Special Cases” on page 28-599).

In certain cases, you might want to apply an embed_ref to existing characters (as a "style") as opposed to inserting a new "character" by itself. One example of this would be to support hypertext links that apply to existing key words in the document; for such a feature, you will probably want to connect an embed_ref to an existing group of characters instead of inserting a new one. If this is the case, you should use the following function instead of $p g$ InsertEmbedRef:

void pgSetEmbedRef (pg_ref pg, embed_ref ref, select_pair_ptr selection, short stylesheet_option, embed_callback callback, long callback_refcon, short draw_mode);

This function's parameters are identical to pgInsertEmbedRef(), except the embed_ref is applied to existing text as a style. Hence, the selection parameter can be a pointer to a range of character, or NULL if you want to apply the reference to the current text selection.

NOTE: Relying on the default behavior of the embed_ref in this case can render the text "invisible". This is because the text within the specified selection becomes literally a custom style and the standard text drawing function within OpenPaige will no longer get called for those characters.

You can handle this by setting a callback function that responds to EMBED_DRAW, at which time you can call the standard text drawing function (see "The Callback Function" on page 28-587).

28.14 Non-sharing Embeds

By default, multiple occurrences of the same embed_ref are shared. For example, if you created a single embed_ref and inserted it as a character, subsequent copy/paste operations might duplicate the reference several times; yet, only one embed_ref is maintained by OpenPaige. Each copy is merely a pointer to the same (shared) memory.

In special cases, however, an application might need to force unique occurrences for each copy. For example, suppose the user is allowed to edit an embedded picture (such as changing its size or content). If multiple copies exist in the text, changing one of them would change the appearance of all — which may not be a desirable feature.

The work-around is to pass the following value in the flags parameter when pgNewEmbedRef() is called:

\#define NOT_SHARED_FLAG0x00080000// Embed_ref not

Setting flags to this value tells OpenPaige that for each copy/paste operation, the embed_ref needs to be newly created. Hence, each copy will be a unique reference and not shared.

### 28.15 File Saving

Embed_refs contained in a document do not automatically get saved to an OpenPaige file unless you call the function below immediately after calling pgSaveDoc:

pg_error pgSaveAllEmbedRefs ( $\mathrm{pg}$ _ref pg, file_io_proc io_proc, file_io_proc data_io_proc, long PG_FAR *file_position, file_ref filemap);

This function writes all embed_refs in $p g$ to the file specified. The $p g$, io_proc, file_position and filemap are the same parameters you just gave to $\operatorname{ggSaveDoc()~for~}$ pg, write_proc, file_position and filemap, respectively. The data_io_proc should be NULL (it is only used in very specialized cases).

This function is safe to call even if there are no e m b e d_re fs contained in $p g$ (if that is the case, nothing gets written to the file).

The reason this function is separate, as opposed to OpenPaige saving embed_refs automatically, is some OpenPaige developers will not be using the embed_ref
extension so the required library to handle this feature might not exist in every application.

For each embed_ref that is saved, the callback function will be called with EMBED_WRITE_DATA as the command.

The $p g$ SaveAlIEmbedRefs is to be used to save embed_refs already existing in $p g$; if you have embed_refs around that are not inserted anywhere, you need to save them discreetly using the following function:

pg_error pgSaveEmbedRef (pg_ref pg, embed_ref ref, long element_info, file_io_proc io_proc, file_io_proc data_io_proc, long PG_FAR *file_position, file_ref filemap);

The above function is similar to $p g$ SaveAllEmbedRefs except a single embd_refis saved to the file. The element_info value can be anything, and that value is returned to a read handler when the data is read later. If this function is successful, zero (NO_ERROR) is returned.

NOTE: You do not need to call this function unless you need to save an embed_ref that you have kept around that isn't inserted into a document.

### 28.16 File Reading

Since OpenPaige can not make the assumption that the embed_ref extension library is available in all applications, you must tell the file I/O mechanism that an OpenPaige file being read might contain embed_refs. You do so by calling the following function at least once before calling $p g R e a d D o c$ :

void pglnitEmbedProcs (pg_globals_ptr globals, embed_callback callback, app_init_read init_proc);

This initialize the embed_refread handler so it can process any embed_ref within the text stream during $p g R e a d D o c$. You only need to call this function once, some time after $p g I n i t$ and before the first $p g R e a d D o c$.

The callback parameter should be a pointer to a callback function that you want to set, as the default callback, for all embed_refs that are read. This function should either be NULL (for no callback) or a pointer to the same kind of function used for callback when inserting an embed_ref. The reason you need to provide this parameter when reading a file is the newly created embed_refs won't have a callback functions (hence there would be no way to examine the incoming data). Additionally, OpenPaige sets the callback given in pgInitEmbedProcs to become the callback for all the embed_refs read from the file.

An embed_ref is read from the file and processed as follows:

1. The embed_ref is created and the default contents are read,
2. The callback function is called with $E M B E D_{-} R E A D_{-} D A T A$ giving your app a chance to append additional data that might have been saved,
3. OpenPaige walks through all the style_info records and attaches the embed_ref to all appropriate elements; for each style_info that contains the embed_ref, the callback is called once more with EMBED_INIT.

The init_proc is an optional function pointer that will be called after an embed_ref is retrieved during file reading; the primary purpose for this function is to initialize an embed_ref that is not attached to the document. Normally you won't need to use this callback function so just pass NULL; but if for some reason you have saved an embed_ref discretely (using pgSaveEmbedRef()) and it is not applied to any character(s), the init_proc might be the only way you can get called back to initialize the embed_ref data.

The init_proc gets called immediately after an embed_refhas been read from a file:

$$
\text { PG_PASCAL (void) init_read(paige_rec_ptr pg, memory_ref ref); }
$$

When init_read is called, the newly read embed_ref will be given in ref.
embed_ref pgPtlnEmbed (pg_ref pg, co_ordinate_ptr point, long PG_FAR *ext_offset, style_info_ptr associated_style, pg_boolean do_callback);

This function returns an embed_ref, if any, that contains point. If no embed_ref contains point, MEM_NULL is returned.

If text_offset is non-NULL and an embed_ref containing point is found, ${ }^{*}$ text_offset is set to the text position for that ref. Likewise, if associated_style is non-NULL, then *associated_style is initialized to the style_info for that ref.

If do_callback is TRUE then the callback function for the embed_refis called with EMBED_CURSOR command when and if the point is contained in an embed_ref. (See "The Callback Function" on page 28-587 and EMBED_CURSOR command in"Command Messages" on page 28-588).

embed_ref pgGetEmbedJustClicked (pg_ref pg, long drag_select_result);

Returns the embed_ref that was clicked during the last call to pgDragSelect. If no embed_ref was clicked from the last pgDragSelect, the function returns MEM_NULL.

The drag_select_result should be whatever value was returned from the last call to pgDragSelect (which is actually how pgGetEmbedJustClicked knows which embed_refwas clicked).
embed_ref pgFindNextEmbed (pg_ref pg, long PG_FAR *text_position, long match_refcon, long AND_refcon);

This function does a search through all the embed_refs in $p g$ and returns the first one that matches the criteria specified. The search begins at ${ }^{*}$ text_position. If an embed_ref is found, it is returned and *text_position will be set to the text offset for that ref. If none found, MEM_NULL is returned and *text_position will be the end of the document.

For example, to search for an embed_refstarting at the document's beginning, set a long to 0 and pass a pointer to it as text_position.

Essentially, the function searches for the first occurrence of an embed_ref whose callback_refcon (the value given to pgInsertEmbedRef) matches match_refcon; the callback refcon value in the embed_refis first AND'd with AND_refcon, then compared to match_refcon. If the comparison is equal, that embed_ref is considered a true match and it is returned.

For example, if you wanted to find the next embed_refthat had a 1 set for the low-order bit of the callback refcon, you would pass 1 for both match_refcon and AND_refcon.

If you simply wanted to find the first occurrence of any embed_ref, pass 0 for both match_refcon and $A N D \_r e f c o n$.

To find an exact, specific embed_ref(per value in callback refcon), pass that exact refcon value in match_refcon and -1 for $A N D \_r e f c o n$.

embed_ref pgGetExistingEmbed (pg_ref pg, long user_refcon);

Returns the embed_refcurrently in $p g$, if any, that contains user_refcon. The user_refcon being searched for is the same value given to pgNewEmbedRef originally.

NOTE: The user_refcon is the value that was given to pgNewEmbedRef(), which can be different than the callback refcon.

If one is not found that matches user_refcon, this function returns MEM_NULL.

long pgNumEmbeds (pg_ref pg, select_pair_ptr selection);

Returns the total number of embed_refs contained in the specified selection of $p g$. If selection is a null pointer the current selection is used.

Once you know how many embed_refs are present in the specified range of text, you can access individual occurrences using pgGetIndEmbed (below).

embed_ref pgGetIndEmbed (pg_ref pg, select_pair_ptr selection, long index, long PG_FAR *text_position, style_info_ptr associated_style);

Returns the nth embed_refwithin the specified selection. If selection is a null pointer the current selection is used.

If text_position is a non-NULL pointer, then ${ }^{*}$ text_position get set to the text position of the embed_ref(relative to zero).

If associated_style is non-NULL, the style_info is initialized to the style the embed_refis attached.

If index embed_ref does not exist, the function returns MEM_NULL (and neither ${ }^{*}$ text_position nor *associated_style is set to anything).

NOTE: The index value is ONE-BASED, i.e. the first embed_ref is 1 (not zero).
long pgGetEmbedBounds (pg_ref pg, long index, select_pair_ptr index_range, rectangle_ptr bounds, short PG_FAR *vertical_pos, co_ordinate_ptr screen_extra);

This function returns the bounding dimensions of the embed_ref represented by index within the index_range; if index_range is NULL then the whole document is used.

The index is zero-based (first embed_ref in the document is zero). You can determine how many embed_ref exist by calling pgNumEmbeds().

This function returns the text position of the embed_ref (what character it applies to relative to the 0th char); the bounding rectangle of the refis returned in *bounds and the *vertical_pos parameter returns the item's descent value (distance from baseline to bottom).

NOTE: Any or all of these parameters can be NULL if you don't need the information.

The rectangle returned in *bounds will be the enclosing box of the embed_ref NOT scrolled, i.e. where it would be on the screen of $p g$ 's scroll position were 0,0 . If screen_extra is non-NULL then it will be set to the amount of pixels you would need to offset the bounding rectangle to obtain the physical location of its bounds. Hence, if you offset *bounds by screen_extra->h and screen_extra->v you obtain the WYSIWYG rectangle.

long pgEmbedStyleToIndex(pg_ref pg, style_info_ptr embed_style);

Returns the index value of the embed_ref attached to embed_style, if any. This function is useful for obtaining an "index number" for an embed_ref where only the style_info is known. If no embed_ref exists for embed_style, zero is returned; otherwise assume the function result is the related index.

This index value can then be used for functions that require it such as pgGetEmbedBounds, pgGetIndEmbed, etc.

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-612.jpg?height=154&width=1160&top_left_y=265&top_left_x=433)

This function changes the bounding dimensions and/or the baseline position (descent) of an embed_ref within a document.

The index parameter specifies which embed_ref to change (beginning at 1), and index_range indicates the range of text to consider. If index_range is NULL the current selection is used.

For example, if the current selection contained two embed_refs, an index of 1 would indicate the first embed_ref within that selection and a 2 would indicate the second embed_ref. The physical order of embed_refs is the order in which they appear in the text (not necessarily the order they were inserted).

The bounds rectangle indicates the embed_ref's new width and height. Note that width and height are taken from the rectangle dimensions - the physical top-left location of the embedded object is not altered. If bounds is NULL the embed_ref dimensions remain unaltered.

The vert_pos parameter should point to a value that indicates the amount of descent, in pixels, the embed_ref should be drawn relative to the text baseline. This is a positive value, i.e. a value of " 3 " will cause the embed_ref to draw three pixels below the text baseline.

The vert_pos parameter can also be NULL, in which case the object's descent remains unchanged.

If $d r a w \_m o d e$ is nonzero the text in $p g$ (including the changes to the embed_ref) is redrawn.

You can prepare for undoing an embed_refinsertion by calling pgPrepareUndo(), passing undo_embed_insert as the undo verb. You should do this just before inserting an embed_ref.

Otherwise, there is no specific undo support required for embed_ref(because after they are inserted, all the normal undo operations will work - pundo for Cut, Paste, format changes, etc.).

### 28.20 Relationship to Style Info

OpenPaige stores embed_refs directly into the style_info record. The following style_info fields contain embed_refinformation (from style_info struct):

long embed_entry; $\quad$ /* App callback rfunction for embed_refs *

long embed_style_refcon; $/ *$ Used by embed object extension *

long embed_refcon; /* Used by embedded object extension */

long embed_id; /* Used by embedded object extension */

memory_ref embed_object; / /* Used by embedded object extension */

The callback function is stored in embed_entry; embed_style_refcon is the callback refcon and embed_refcon is the user refcon (see refcon values for pgNewEmbedRef on page 578 versus pgInsertEmbedRef on page 581).

The embed_id will contain a unique ID number generated by OpenPaige; this value has no direct meaning except it is created to keep style_info's with embed_refs from running together.

The embed_refitself is in embed_object.

## Windows Example

The following is an example of inserting a metafile as a "character" into a pg_ref. It also shows how to prepare for an Undo.

/* This function embeds a meta file into the text.

The $x, y$ extents are given in $x_{-}$ext and $y_{-}$ext.

Note, the $x$ and $y$ extents should be given

in device units. The user_refcon param is whatever you want it to be for application reference. The callback param will become the embed callback, or NULL if you want to use the default. */

void InsertMetafile (pg_ref pgRef, HMETAFILE meta, int x_ext, int y_ext, long user_refcon, embed_callback callback)

\{

metafile_struct metafile;

embed_ref ref;

void PG_FAR *the_data;

// It is a good idea to fill struct with zeros

// for future compatibility.

memset(\&objData, 'IO', sizeof(PAIGEOBJECTSTRUCT));

metafile. metafile $=$ (long)meta;

metafile.bounds.top_left. $\mathrm{h}=$ metafile.bounds.top_left. $v=0$;

metafile.bounds.bot_right. $h=x \_e x t$;

metafile.bounds.bot_right. $v=y \_e x t ;$

metafile.mapping_mode $=$ MM_ANISOTROPIC;

metafile.x_ext $=$ x_ext;

metafile.y_ext $=$ y_ext;

the_data $=\left(\right.$ void PG_FAR $\left.{ }^{*}\right) \&$ metafile;
ref $=$ pgNewEmbedRef(\&mem_globals, embed_meta_file, the $\_$data, $0,0,0$, user_refcon, FALSE);

pgInsertEmbedRef(pgRef, ref, CURRENT_POSITION, 0, callback, 0 , best_way);

Macintosh Example

/* The following example shows inserting a PicHandle as a "character". The picture's descent (distance below baseline) is $20 \%$ of its height. We also prepare for an Undo. */

void InsertPicture (pg_ref pg, PicHandle picture)

## A Custom Embed Ref

This example shows how to create and manipulate a custom embed_ref. In this case we are creating a simple box for which we draw a frame, and we respond in some way if the user double-clicks in this box.

For purposes of demonstration, we also attach a data struct to the custom embed_ref. While this example doesn't do anything with that data, it shows how you would save and read your data to an OpenPaige file.

/* Insertion of a custom ref into a pg_ref "pg". Upon entry, width and height define the dimensions of the box; data is a pointer to some arbitrary data structure that gets attached to the ref (and eventually saved to the OpenPaige file) and dataSize is the size of that data. The callbackProc param is a pointer to our callback function (almost mandatory for any custom embeds).

The refCon value becomes the callback refcon. */

void makeCustomRef (pg_ref pg, short width, short height, char *data, long dataSize, embed_callback callbackProc, long refCon)

\{

embed_ref ref;

pg_embed_ptr embed_ptr;

// Create a custom ref, but if we specify embed_user_data // then OpenPaige will attach the data to the ref.

ref $=$ pgNewEmbedRef(\&mem_globals, embed_user_data, (void*) data, dataSize, $0,0,0$, FALSE);

// The following code is vital for a "custom" user type since

// OpenPaige has no idea how tall our embed item is, nor does

// it know how wide it is:

embed_ptr = UseMemory(ref); // Get the embed struct

embed_ptr->height = height;

embed_ptr $->$ width = width;

UnuseMemory(ref);

// Insert the ref. (Also add pgPrepareUndo() here if desired). pgInsertEmbedRef(pg, ref, CURRENT_POSITION, 0, callBackProc, refCon, best_way);

// The following code is the callback function for the // embed_ref. OpenPaige calls this with various "messages".

PG_PASCAL (long) callBackProc (paige_rec_ptr pg, pg_embed_ptr embed_ptr, long embed_type, short command, long user_refcon, long param1, long param2)

\{

memory_ref specialData;

Rect theBox;

char *extraBytes;

long result $=0 ; \quad / /$ Default function result

switch (command)

\{

case EMBED_DRAW:

// In this example we frame the box.

// param1 is a rectangle_ptr of the box RectangleToRect((rectangle_ptr)param1, NULL, \&theBox);

FrameRect(\&theBox);

break;

case EMBED_MOUSEDOWN:

case EMBED_MOUSEMOVE:

case EMBED_MOUSEUP:

case EMBED_DOUBLECLICK:

result $=$ pgDefaultEmbedCallback(paige_rec_ptr pg, pg_embed_ptr embed_ptr, long embed_type, short command, long user_refcon, long param1, long param2);

$$
\begin{aligned}
& \text { if (command == EMBED_DOUBLECLICK) } \\
& \text { HandleMyDoubleClick(pg, user_refcon); }
\end{aligned}
$$

// The "HandleMyDoubleClick() is whatever...

break;

## case EMBED_DESTROY:

/* Important note: Since our embed_ref type is embed_user_data, we can let OpenPaige dispose the data. However if we attached our own data directly we would NOT call the standard callback, or we would crash! */

result $=$ pgDefaultEmbedCallback(paige_rec_ptr pg, pg_embed_ptr embed_ptr, long embed_type, short command, long user_refcon, long param1, long param2);

break;

## case EMBED_WRITE_DATA:

/* NOTE, since our embed type is embed_user_data, OpenPaige will save that data automatically, so we don't need to do anything for this message. But purely for the sake of demonstration we will save two extra bytes to the file to show how it is done: */

specialData $=$ (memory_ref)param1;

SetMemorySize(specialData,sizeof(char) * 2); extraBytes $=$ UseMemory(specialData);

extraBytes $[0]=$ myCustomChar1;

extraBytes[1] = myCustomChar2;

UnuseMemory(specialData);

break;

/* NOTE, since our embed type is embed_user_data, OpenPaige will read that data automatically, so we don't need to do anything for this message. But purely for the sake of demonstration we will read the two extra bytes from the file that we saved in EMBED_READ_DATA: */

extraBytes $=\left(\right.$ char $\left.{ }^{*}\right)$ param 1 ; // Pointer to data

myCustomChar $1=$ extraBytes $[0]$; myCustomChar2 $=$ extraBytes[1]; break;

default:

result=pgDefaultEmbedCallback(paige_rec_ptr pg,

pg_embed_ptr embed_ptr, long embed_type, short command, long user_refcon, long param1, long param2);

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-619.jpg?height=104&width=374&top_left_y=1333&top_left_x=487)

## MAIL MERGING

"Mail merging" is a feature in which specific portions of text can be temporarily swapped with text from other sources. We are referring to it as "mail merge" because this feature is typically used to substitute special embedded symbols or fields within a document with data from a database for form letters, mailing labels, statements, etc.

### 29.1 How merging works

In OpenPaige, merging is essentially a custom style. For more about custom styles in general see "Creating a simple custom style" on page 30-37 and "Customizing OpenPaige" on page 27-1. Specifically, the merge feature is accomplished as follows:

1. A merge "symbol" is simply a specific style (set by the application) which is applied to a portion of text. It differs from other styles simply by the existence of a merge_proc other than the default (see \#2 below). Otherwise, such "symbols" can be any kind of characters, words or phrases the application wishes to embed in the text stream to convey "merge fields".

For the sake of discussion, we shall refer to this style attribute as a merge style.

2. A merge style must have the merge_proc function pointer set to an applicationdefined function (see "merge_proc" on page 27-13).
3. By itself, a merge style does nothing and text set to a merge style remains unchanged until the application calls pgMergeText (below). OpenPaige will then call the appropriate merge_procs, at which time the application makes the decision for substituting text (or not).
4. When pgMergeText is called, the text for which all merge styles applied is temporarily "pushed" (saved) into an internal memory_ref within the OpenPaige object. Later, when the application wishes to revert from "merge mode" the document can be completely restored to its original state, prior to any text substitutions.

## Sample merge text proc

This is called when the styles need to be initialized. Usually at the beginning of the program. This sets the merge style procs and user_id and the mask makes it so only the two desired procs, merge_text_proc and setup_insert, get set to our custom ones following:

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-622.jpg?height=886&width=1196&top_left_y=1088&top_left_x=433)

/* This function inserts my "mail merge" fields into the text.

I shall use only a couple of style hooks to make this work. */

void insert_merge_fields (doc_rec *doc)

\{

style_info style, mask;

short index, size_of_fld;

Str255 name_of_fld;

for (index $=0$; index $<$ NUM_MERGE_FLDS; ++index) \{

GetIndString(name_of_fld, MERGE_STRINGS, index + 1);

size_of_fld = name_of_fld[0];

pgGetStyleInfo(doc->pg, NULL, FALSE, \&style, \&mask);

pglnitStyleMask(\&mask, 0);

/* Set up everything I want in the style_info record: */

style.user_id = STYLE_IS_MERGE;

style.class_bits |= (STYLE_IS_CUSTOM | GROUP_CHARS_BIT);

style.char_bytes $=0$;

style.user_data $=$ index;

mask.user_id $=-1$;

mask.user_data $=-1$;

mask.class_bits $=-1$;

mask.char_bytes $=-1$;

/* Set desired function pointers: */

style.procs.merge = (pg_proc) merge_text_proc;

style.procs.insert_proc $=$ (pg_proc) setup_insert;

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-624.jpg?height=500&width=1108&top_left_y=149&top_left_x=472)

This is the hook that gets called when OpenPaige saves off the next style to apply from the next insert. The reason I need this for merge "characters" is because I don't want the user to "type" or extend text if the caret sits on one of my merge styles. Hence, this function must remove my own hooks from the style so it becomes just a regular style. */

static pascal short setup_insert (paige_rec_ptr pg, style_info_ptr style, long position) \{

pglnitStyleProcs(\&style->procs);// This sets all the standard procs style->class_bits $=0$; style->user_data $=$ style->user_id $=0$;

return TRUE;

/* Won't call me again (because I just nuked my own function ptr */

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-625.jpg?height=346&width=1193&top_left_y=1496&top_left_x=434)

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-626.jpg?height=829&width=1163&top_left_y=148&top_left_x=436)

### 29.2 Merge mode

Assuming all your merge styles have been set up (all the desired merge areas have a merge_proc set in their style_info record), placing the OpenPaige object in "merge mode" is accomplished by calling the following:
(pg_boolean) pgMergeText (pg_ref pg, style_info_ptr matching_style, style_info_ptr mask, style_info_ptr AND_mask, long ref_con, short draw_mode);

For every style_info that matches a specified criteria (based on the contents of matching_style, mask and AND_mask as described below), has its merge_proc called, at which time text can be substituted in place of the text that currently exists for each style involved.

Before any text is substituted, however, the "old" text is saved temporarily within the OpenPaige object. This is intended to allow the application to "revert" to the original document at some later time.

Styles that are affected by this call (in which the merge_proc gets called) are determined on the following bases:

- The fields in each style_info record in OpenPaige is compared to each field in matching_style if the corresponding field in mask is nonzero.
- Before the comparison, the corresponding field in AND_mask is temporarily AND'd with the target style_info field in OpenPaige before it is compared.
- If all fields that are checked in this way match exactly, the style is "accepted" and the merge_proc gets called.

Any of the three comparison styles — bmatching_style, mask and AND_mask can be null pointers to control the comparison procedure, in which case the following occurs:

- If matching_style is null, then all styles in $p g$ are considered "valid" with no comparisons made, hence all merge_procs are called.
- If mask is null, all fields in each style are compared to matching_style (none are skipped).
- If AND_mask is null, no "AND" occurs before the field comparisons (instead, the fields are compared as-is).

Using the various combinations of matching_style, mask and AND_mask you can selectively "merge" various styles based on a nearly infinite set of criteria.

The ref_con parameter can be anything; this value gets passed to the merge_proc.

If $d r a w \_$mode is nonzero, $p g$ is redisplayed with the new text. $d r a w \_$mode can be the values as described in "Draw Modes" on page 2-30:

draw_none,

best_way,

direct_copy,

direct_or,

direct_xor,

bits_copy,

bits_or,

bits_xor, $1 *$ Do not draw at all $* /$

/* Use most effecient method(s) $* /$

/* Directly to screen, overwrite */

$1 *$ Directly to screen, "OR" */

/* Directly to screen, "XOR" */

/* Copy offscreen */

$1 *$ Copy offscreen in "OR" mode */

/* Copy offscreen in "XOR" mode */

NOTE: The MERGE_MODE_BIT will be set in $p g$ 's attributes when the document has been "merged" in the above fashion.

You can check the attributes using pgGetAttributes.

FUNCTION RESULT:This function returns TRUE if anything merged at all; FALSE is returned if no text has been substituted from any merge_proc (hence the document remains unchanged).

NOTE 1. If you intend to revert to the original document using pgRestoreMerge (below), you must not insert any new text or allow any kind of editing by the user until you revert. It is OK, however, to do multiple pgMergeText calls before reverting the document.

NOTE 2. The original document is saved only once; subsequent pgMergeText calls will not save the merge styled text again. Hence, you can make multiple pgMergeText calls before reverting, then pgRestoreMerge (below) will revert the document to its state before the first merge.

NOTE 3. Even if you intend not to revert the text, you need to call pgRestoreMerge anyway, otherwise a memory leak can result.
(void) pgRestoreMerge (pg_ref pg, pg_boolean revert_original, short draw_mode);

This function "reverts" $p g$ to its original state, prior to the first $p g M e r g e T e x t$ call if revert_original $==$ TRUE.

If revert_original == FALSE, the previous text that has been saved within pg is simply disposed and the document is not reverted. The purpose of this parameter is to allow a document to "convert" to a merged state, but to keep it that way.

If draw_mode is nonzero, $p g$ is redisplayed. draw_mode can be the values as described in "Draw Modes" on page 2-30:

| draw_none, <br> best_way, <br> direct_copy, <br> direct_or, <br> direct_xor, <br> bits_copy, <br> bits_or, <br> bits xor, | /* Do not draw at all */ <br> /* Use most effecient method(s) */ <br> /* Directly to screen, overwrite */ <br> /* Directly to screen, "OR" */ <br> /* Directly to screen, "XOR" */ <br> /* Copy offscreen */ <br> /* Copy offscreen in "OR" mode */ <br> /* Copy offscreen in "XOR" mode */ |
| :---: | :---: |

This function only needs to be called once, even after multiple $p g M e r g e T e x t$ calls. Once you have reverted, however, a subsequent call to $p g$ RestoreMerge will do nothing (unless you have done another pgMerge Text).

This function also clears the MERGE_MODE_BIT from $p g$ 's attributes.

NOTE: Even if you do not wish to revert the text, you should call pgRestoreMerge anyway (with revert_original as FALSE) if anything has merged to dispose the saved text.

## \{

init_merge_strings();

if (pgGetAttributes(doc->pg) \& MERGE_MODE_BIT) \{

no_change_verb, activate_verb, FALSE);

pgSetHiliteStates(doc->pg, pgRestoreMerge(doc->pg, TRUE, draw_none);

\}

else

\{

pgMergeText(doc->pg, NULL, NULL, NULL, 0, draw_none);

no_change_verb, deactivate_verb, FALSE);

pgSetHiliteStates(doc->pg,

DoAllUpdates();

InvalRect(\&doc->w_ptr->portRect);

\}

## TECH STYLE_IS_CUSTOM bit set incorrectly

I looked into your code and found that you are correct that setting up the style info record is the problem. You need to REMOVE the class_bits setting, "STYLE_IS_CUSTOM." That's what is forcing the merge field to not draw.

STYLE_IS CUSTOM tells OpenPaige that only your app knows how to draw the style and measure its characters. Hence, if you call the standard draw/measure functions (which you are), they will DO NOTHING. I not only noticed the fields were invisible, on my machine, but the char widths would result in random garbage text sizes (which is CORRECT since the standard measuring does nothing for STYLE_IS_CUSTOM).

Technically, the style is not "custom" at all -- it has regular text chars and it draws like any other text. By strict definition, a STYLE_IS_CUSTOM means OpenPaige can't
understand the "text" stream, such as an embedded PicHandle or ControlHandle, etc.

## Merge fields and blank lines

Regarding merge fields and blank lines (and how to remove them) in items like addresses, I am not sure I have a perfect answer for that. I don't think you dare try deleting anything from within a hook, you will probably get a debugger break (because memory_ref's for the text and styles will be locked and "in use").

The only thing I can think of is to detect this situation and, after all is merged, go back and delete the "blank lines."

CAUTION: If you do this, I am not sure "pgRestoreMerge"will work correctly since it assumes you have not edited the document.

We had another customer doing extensive altering of a merged document for similar reasons, and he had to simply restore the original doc without using pgRestoreMerge. Rather, he would copy the document and then do pgUndo.

## Restore-all not yet implemented and the work-around

If I understand you correctly, the reason you need to throw away each document and reread it -- as opposed to relying on "restore merge" feature -- is due to your extra editing of the document and the fact that "restore merge" just restores the merge styles.

The supreme work-around would be, of course, for us to add "restore all" to the merge features -- which incidentally is not a bad idea. Sooner or later someone else will encounter the same problem.

In the meantime (since that feature is not currently available in pgMerge), I would suggest starting with a single pg_ref, as you are now, but use "pgCopy" to duplicate the doc siñce pgCopy can produce what you thought pgDuplicate did.

Here are some precautions / hints:

- To duplicate a whole document, simply use pgCopy with a selection parameter for whole text range. (Remember that
pgcopy returns a new pg_ref -- which is exactly what you want).
- You might have a problem displaying the copied pg_ref since I do not believe the exact vis and page areas are copied. In that case you might need to set those shapes before drawing (or printing) the merged document. I would get the shape(s) from the master document then do pgSetAreas to the copy. Even faster would be to get the master shapes once at the beginning, with pgGetAreas, then set them for each merge.
- I do know for sure that a copied pg_ref, from pgCopy, will have no grafdevice associated with it. I do not know if this is a problem if you intend to print, since pgPrintPage accepts a graf device (which would be a print port). But if you need to draw the merged doc to a window, you will need to set a window port using pgSetDefaultDevice, OR you will need to specify a grafdevice ptr in pgDisplay. Otherwise the drawing will be "invisible" and you will think you are going crazy. I believe our manual explains this (if not, I will be happy to provide more details).
- In OpenPaige's current stage, I do not believe anyone has yet to display (or print) a copied pgref returned from pgCopy. Usually they just paste with it. In that case, you may have unforeseen problems. However, all such cases (other than the precautions listed above) I would consider a bug, so be sure to let us know so the problem(s) can be corrected. We will make sure that a copied pg_ref displays correctly, one way or the other.
- You may encounter some slowness with this work-around. However, that will probably improve during future updates.


## 30 AdvANCED STYtes

This chapter unveils all of the style and font setting abilities within OpenPaige. For easier and quicker implementation of style setting, you will want to look at "Style Basics" on page 8-1.

NOTE (Windows): unlike a Windows font that defines the whole composite format of text, the term "font" as used in this chapter generally refers only to a typeface, or typeface name. OpenPaige considers a "font" to simply be a specific family such as Times, Courier, Helvetica, etc. while distinguishing other formatting properties such as bold, italic, underline, etc. as the text style.

### 30.1 Data Structures

For the sake of clarity and easier implementation of text formatting, the exact structure definitions and descriptions for style, font and paragraph formats are given at the end of this section. While you will need to set up these structures to effectively change text styles, they have been placed at the end for easier reference.

To understand the functions, however, let it suffice to declare the type for each of the four formats, as follows:

| Record Type | Pointer (to the record) | Description |
| :---: | :---: | :--- |
| style_info | style_info_ptr | Structure defining a style |
| font_info | font_info_ptr | Structure defining a font |
| par_info | par_info_ptr | Structure defining paragraph format |
| style_run | style_run_ptr | * Structure designating a style run. |

* A series of style_run records is maintained by OpenPaige to define all the style changes and associated text offsets. This record is much smaller than either style_info or par_info, thus requiring only one style_info record for every identical style change throughout the text and one par_info record for every identical paragraph format throughout the text. The style_run record is defined at the end of this section; most of the time you will not need to access style_runs.


### 30.2 More About Style Runs

For both style_info and par_info changes throughout the text, OpenPaige maintains a list of style_run records. There is one style_run array for style_info changes and one array for par_info changes.

The last element in a style_run array is a "dummy" entry whose offset field will be greater than the total text size of the $p g \_$ref. For example, if the total text size of a $p g \_r e f$ is 100 bytes, the final element in the array of style_run records will contain a value in style_run.offset of $>100$.

To simply set a style, font, size or paragraph format, see "Style Basics" on page 8-1. The following information is for those developers wanting more precise control of style, font and paragraph format setting.

### 30.4 Changing Fonts and Styles together

This sets the font and style at the same time.

(void) pgSetStyleAndFont (pg_ref pg, select_pair_ptr selection, style_info_ptr the_style, style_info_ptr style_mask, font_info_ptr font, font_info_ptr font_mask, short draw_mode);

selection - parameter defines the range of text that should be changed, or if you pass a null pointer the current selection range (or insertion point) in $p g$ is changed.

If you do give a pointer to selection, it must point to the following structure:

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-635.jpg?height=354&width=966&top_left_y=1392&top_left_x=434)

begin field of a select_pair — defines the beginning text offset and the end field defines the ending offset. Both offsets are byte offsets, not character offsets. Text offsets in OpenPaige are zero-based (first offset is zero).
info and mask — parameters must point to style_info records; info is the new style to apply to the text and mask defines which of the info fields to apply. For every nonzero field in mask the corresponding field in info gets applied to the text.

mask - parameter allows only partial style changes, which is almost always what you want to accomplish. For instance, if the user highlighted some text and changed it to "bold," all you want to change in the text range is the "bold" attribute, not anything else such as color, leading, or any other formatting. To do so, you would set info's style element for bold and the same field in mask to nonzero.

font and font_mask — is almost identical to the similar style parameters except a font_info record is used for font and font_mask.

info, mask, font nor font_mask_- Neither can be a null pointer.

draw_mode — parameter indicates whether or not to redraw the text once the style has changed: if draw_mode is nonzero, that drawing mode is used to redisplay the text. draw_mode can be the values as described in "Draw Modes" on page 2-30:

draw_none,

best_way,

direct_copy,

direct_or,

direct_xor,

bits_copy,

bits_or,

bits_xor,
/* Do not draw at all */

/* Use most efficient method(s) */

/* Directly to screen, overwrite */

/* Directly to screen, "OR" */ /* Directly to screen, "XOR" */

$1 *$ Copy offscreen */

/* Copy offscreen in "OR" mode */

/* Copy offscreen in "XOR" mode */

However, OpenPaige will only redraw the text if it is appropriate: if nothing changed (same styles applied as already exist), the text is not drawn, nor is it drawn if the new style applies only to an insertion point.

NOTE: The mask fields that indicate what to change must be set to -1 (all "1's). The reason is that the internal comparison function - pwhich checks the mask settings - pdoes one word at a time, hence the fields longer than 16 bits will not change correctly.

// The following example function converts a LOGFONT into a font_info, style_jnfo

// and "mask" record that can be given to pgSetStyleInfo():

static void convert_log_font (pg_ref pg, pg_globals_ptr paige_globals, LOGFONT PG_FAR *log_font, font_info_ptr font, style_info_ptr style, style_info_ptr stylemask)

\{

// Initialize the style to OpenPaige default:

${ }^{*}$ style $=$ paige_globals->def_style;

// Initialize other structs to zeros or -1's, etc.:

pgFillBlock(font, sizeof(font_info), 0);

pgFillBlock(stylemask, sizeof(style_info), 0);

pgFillBlock(stylemask->styles, MAX_STYLES * sizeof(short), -1);

stylemask->point $=$ (pg_fixed) -1 ;

CToPString(log_font->IfFaceName, font->name);

// (OpenPaige wants a pascal string)

font->family_id = log_font->IfPitchAndFamily;

font->machine_var[PG_OUT_PRECISION] = log_font$>$ IfOutPrecision;

font->machine_var[PG_CLIP_PRECISION] = log_font$>$ IfClipPrecision;

font->machine_var[PG_QUALITY] = log_font->|fQuality;

font->machine_var[PG_CHARSET] = log_font->lfCharSet;

if $\left(\left(\right.\right.$ style->point $=($ pg_ fixed $) \log _{-}$font $->\mid \mathrm{fH}$ eight $\left.)<0\right)$ style->point = -style->point;

style->point <<= 16; // Make sure point size is 0x000n0000

// Convert pointsize to fit the screen resolution style->point = pgScreenToPointsize (pg, style->point);

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-638.jpg?height=455&width=869&top_left_y=147&top_left_x=434)

### 30.5 Easier "Mask" Setups

Masks

The easiest way to initialize a style_info, font_info or par_info record for a "mask" is to call one of the following:

(void) pglnitStyleMask (style_info_ptr mask, short filler);

(void) pglnitFontMask (font_info_ptr mask, short filler);

(void) pglnitParMask (par_info_ptr mask, short filler);

These function fill mask with filler.

To set a section of text to a style, call the following:

(void) pgSetStylelnfo (pg_ref pg, select_pair_ptr selection, style_info_ptr info, style_info_ptr mask, short draw_mode);

selection defines the range of text that should be changed, or if you pass a null pointer the current selection range (or insertion point) in $p g$ is changed.

If you do give a pointer to selection, it must point to the following structure:

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-639.jpg?height=502&width=726&top_left_y=1061&top_left_x=433)

begin field of a select pair — defines the beginning text position and the end field defines the ending position. Both values are byte positions (not necessarily character positions, e.g. multilingual text can have double-byte characters, etc.). Text positions in OpenPaige are zero-based (first offset is zero).

info and mask — must point to style_info records; info is the new style to apply to the text and mask defines which of the info fields to apply. For every nonzero field in mask the corresponding field in info gets applied to the text.
mask — allows only partial style changes, which is almost always what you want to accomplish. For instance, if the user highlighted some text and changed it to "bold", all you want to change in the text range is the "bold" attribute, not anything else such as color, leading, or any other formatting. To do so, you would set info's style element for bold and the same field in mask to nonzero.

Neither info nor mask can be a null pointer.

draw_mode — indicates whether or not to redraw the text once the style has changed: if draw_mode is nonzero, that drawing mode is used to redisplay the text. draw_mode can be the values as described in "Draw Modes" on page 2-30:

draw_none, best_way, direct_copy, direct_or, direct_xor, bits_copy, bits_or, bits_xor,[^1]

However, OpenPaige will only redraw the text if it is appropriate: if nothing changed (same styles applied as already exist), the text is not drawn, nor is it drawn if the new style applies only to an insertion point.

NOTE: The mask fields that indicate what to change must be set to -1 (all "1's). The reason is that the internal comparison function — bwhich checks the mask settings — pdoes one word at a time. Hence, the fields longer than 16 bits will not change correctly.

NOTE (Windows): To convert a LOGFONT into a style_info and mask, see code example earlier in this chapter.

/* This function sets the current selection to Bold (Macintosh)*/

void set_to_bold (pg_ref pg)

\{

style_info mask, info;

pglnitStyleMask(\&info, 0);// Sets all to zero

pglnitStyleMask(\&mask, 0);// Sets all to zero

info.styles[bold_var] = -1;// sets styles[bold_var] to force bold mask.styles[bold_var] = -1 ;

pgSetStyleInfo(pg, NULL, \&info, \&mask, best_way);

\}

While the styles each contain shorts to indicate bold, italic, etc., this is generally done for future expansion. When OpenPaige was designed, new fonts were being created which would use "degrees of boldness", etc. Generally, this is not implemented in OpenPaige for Mac and Windows 1.0 except for the following style elements:

style_info->styles[small_caps_var]—bThe value in this style element indicates a percentage of the original point size to display lower case characters that get converted to ALL CAPS. Or, if this value is -1 , the default is used (which is $75 \%$ of the original style).

For example, if style_info->styles[small_caps_var] is 50 and style_info point size is 24, the lower case text is converted to uppercase 12 point; if style_info>styles[small_caps_var] is -1 , the lower case text is converted to 18 (which is $75 \%$ of 24).

style_info->styles[relative_point_var] —-pThe value in this style element indicates a point size to display the text which is a ratio relative to 12 point times the original point size. The ratio is computed as:

NOTE: style_info -> styles[relative_point_var] / 12. (The "original point size" is taken from the "point" field in style_info).

For example, if style_info->styles[relative_point_var] is 6 and the original point size is 12 , the point size that displays is $12 *(6 / 12)=6$ point. If style_info->styles[relative_point_var] is 6 and the original point size is 24, the point size that displays is $24 *(6 / 12)=12$ point.

---The relative_point_var element must not be "-1" as there is no default.

## NOTE:

style_info->styles[super_impose_var] — bIf nonzero, the value represents a stylesheet ID that gets "superimposed" over the existing style. What this means is all fields in the stylesheet style_info->styles[super_impose_var] that are nonzero are temporarily forced into style_info to form a composite style of both.

For example, if style_info->styles[super_impose_var] record had all fields set to zero EXCEPT for the "bold_var" element, the resulting style would be whatever the original style_info contained but with boldface text.

---(style_info->styles[super_impose_var] can only be zero or a positive number representing a stylesheet ID that actually exists in the pg_ref)

See the chapter "Style Sheets" on page 31-1 for more information.

## Insertion Point Changes

If $p g S e t S t y l e I n f o$ is called and the specified selection is a single insertion point, the style change occurs on the next $p g / n s e r t$. Furthermore, a processed mouse-click for change of selection invalidates the style_info set to the previous insertion point (i.e., the new style setting is lost).

Exception: Applying a style to a completely empty $p g \_$ref forces that style_info to become the default style for that $p g \_r e f$.

The point field in style_info is a "fixed" type value, i.e. the whole integer for the point size is the high-order word. For example, 12 point is (pg_fixed) $(12<<16$ ) or 0x000C0000

## TECH

## Changing point size

## I am having some difficulty in setting the point size of the font within OpenPaige.

Your code doesn't work because the point size in style info is a FRACTIONAL value, which means the whole-number point size needs to be in the high-order word -- and you're just setting a long integer (which is putting it in the low-order word). You must have skipped quite a few openPaige versions because that change has been there for a while.

So, your code is fine EXCEPT you need to put the point size in the high-order word, and it will work. Something like:

theStyle. point = fontSize;

theStyle.point <<= 16;

In case you're curious, openPaige only looks at the high-word of the point size, so setting only the low word results in "zero point," or the default - - which is 12 point .- which is why it never changed.

### 30.7 Changing Fonts

Changing the font applied to text range(s) requires a separate function call since fonts are maintained separate from text styles within a $p g \_r e f$.

NOTE (Windows): unlike a Windows font that defines the whole composite format of text, the term "font" as used in this chapter generally refers only to a typeface, or typeface name. OpenPaige considers a "font" to simply be a specific family such as Times, Courier, Helvetica, etc. while distinguishing other formatting properties such as bold, italic, underline, etc. as the text style.

To set a section of text to a new font, call the following:

(long) pgSetFontInfo (pg_ref pg, select_pair_ptr selection, font_info_ptr info, font_info_ptr mask, short draw_mode);

This function is almost identical to pgSetStyleInfo except a font_info record is used for info and mask.

selection and draw_mode — are functionally identical to the same parameters in pgSetStyleInfo. The same rules apply regarding insertion points versus selection range(s).

draw_mode_ can be the values as described in "Draw Modes" on page 2-30:

draw_none, best_way, direct_copy, direct_or, direct_xor, bits_copy, bits_or, bits_xor
/* Do not draw at all */ /* Use most efficient method(s) */ /* Directly to screen, overwrite */ /* Directly to screen, "OR" */ /* Directly to screen, "XOR" */ /* Copy offscreen */

/* Copy offscreen in "OR" mode */ /* Copy offscreen in "XOR" mode */

For detailed information on font_info records — pand what fields you should set up - pis available at the end of this section. But there is one important one you should be sure to set correctly, environs.

When you set a font_info record, only the name and environs fields should be changed; this is because OpenPaige initializes all the other fields when the font is applied to a pg_ref.

For Macintosh version, the font_info.name should be a pascal string terminated with the remaining bytes in font_info.name set to zeros; the font_info.environs field should be set to zero. For an example see below.

For Windows version, the font_info.name can be initially set to either a pascal string or a cstring, with all remaining bytes in font_info.name set to zeros. Usually, due to Windows programming conventions, you will set the name to a cstring. In this case, before passing the font_info record to pgSetFont/Info, you must set font_info.environs to NAME_IS_CSTR (see example below).

CAUTION: WINDOWS NOTE: OpenPaige converts font_info.name to a pascal string and clears the NAME_IS_CSTR bit when the font is stored in the $p g \_r e f$. This is done purely for cross-platform portability. This is important to remember, because if you examine the font thereafter with pgGetFontInfo, the font name will now be a pascal string (the first byte indicating the string length), not a cstring.

Setting font_info (Windows)

/* This example assumes we got a "LOGFONT" struct from a ChooseFont dialog, or something similar. */

LOGFONT

font_info pgFillBlock(\&font, sizeof(font_info), 0); // clear all to zeros

pgFillBlock(\&mask, sizeof(font_info), -1); // Set to all 1's Istrcpy((LPSTR)font.name, log.IfFaceName);

/* IMPORTANT! The following line is an absolute MUST or your code will fail: */

font_info.environs |= NAME_IS_CSTR;

/* Apply to the text: */

pgSetFontInfo(pg, NULL, \&font, \&mask, best_way);

pgFillBlock(\&font, sizeof(font_info), 0); // clear all to zeros pgFillBlock(\&mask, sizeof(font_info), -1); // Set to all 1's Istrcpy((LPSTR)font.name, log.IfFaceName);

/* IMPORTANT! The following line is an absolute MUST or your code will fail: */

font_info.environs |= NAME_IS_CSTR;

/* Apply to the text: */

pgSetFontInfo(pg, NULL, \&font, \&mask, best_way);

Responding to font menu (Macintosh)

/* In this example, we assume a "Font" menu whose MenuHandle is FontMenu, and "item" is the menu item selected by the user. */

font_info font, mask;

pgFillBlock(\&font, sizeof(font_info), 0); // clear all to zeros

pgFillBlock(\&mask, sizeof(font_info), -1);// Set to all 1's

Getltem(FontMenu, item, (StringPtr)font.name);

pgSetFontInfo(pg, NULL, \&font, \&mask, best_way);

### 30.8 Obtaining Current Text Format(s)

The three functions above return information for a specific range of text about its style or paragraph format, or font, respectively.

(long) pgGetStylelnfo (pg_ref pg, select_pair_ptr selection, pg_boolean set_any_match, style_info_ptr info, style_info_ptr mask);

(long) pgGetParInfo (pg_ref pg, select_pair_ptr selection, pg_boolean set_any_match, par_info_ptr info, par_info_ptr mask);

(long) pgGetFontInfo (pg_ref pg, select_pair_ptr selection, pg_boolean set_any_match, font_info_ptr info, font_info_ptr mask);

The three functions above return information for a specific range of text about its style or paragraph format, or font, respectively.

For all functions, if selection is a null pointer the information that is returned applies to the current selection range in $p g$ (or the current insertion point); if selection is non-null, pointing to select_pair record, information is returned that applies to that selection range (see "Copying and Deleting" on page 5-1 for information about select_pair pointer under pgGetStyleInfo).

Both info and mask must both point to a style_info, par_info or font_info record; neither can be a null pointer. When the function returns, both info and mask will be filled with information you can examine to determine what style(s), paragraph format(s) or font(s) exist throughout the selected text, and/or which do not.

If set_any_mask was FALSE: All the fields in mask that are set to nonzero indicate that the corresponding field value in info is the same throughout the selected text; all the fields in mask that are set to zero indicate that the corresponding field value in info is not the same throughout the selected text.

For example, suppose after calling pgGetStyleInfo, mask.styles[bold_var] has a nonzero value. That means that whatever value has been set in info.styles[bold_var] is the same for every character in the selected text. Hence if info.styles[bold_var] is -1 , then every character is BOLD; if info.styles[bold_var] is 0 , then NO character is bold.

On the other hand, suppose after calling pgGetStyleInfo, mask.styles[bold_var] is set to zero. That means that some of the characters in the selected text are BOLD and
some are NOT. In this case, whatever value happens to be in info.styles[bold_var] is not certain.

Essentially, any nonzero in mask is saying, "Whatever is in info for this field is applied to every character in the text," and any zero in mask is saying, "Whatever is in info for this field does not matter because it is not the same for every character in the text."

You want to pass FALSE for set_any_mask to find out what styles, paragraph formats or fonts apply to the entire selection (or not).

If set_any_mask was TRUE, all fields in mask get set to nonzero if the corresponding field value in info appears ANYWHERE within the selected text. In this case, you must first set all the info fields that you want to check before making the call.

The purpose for setting set_any_mask to TRUE is to find out what item(s) in info exist anywhere in the selected text (as opposed to finding out what items are all the same in the text).

NOTE: This important difference: if you pass FALSE for set_any_mask, OpenPaige sets the info fields; if you pass TRUE for set_any_mask, you set the info_fields before calling pgGetStyleInfo, pgGetParInfo or pgGetFontInfo.

For example, suppose you want to find out if BOLD exists anywhere in the selected text. To do so, you would set info.styles[bold_var] to a nonzero value, then call pgGetStyleInfo() passing TRUE for set_any_mask. Upon return, if mask.styles[bold_var] is "TRUE" (nonzero), that means BOLD exists somewhere in the selected text. On the other hand, had the function returned and mask.styles[bold_var] was FALSE, that would mean that BOLD exists nowhere in the text.

Usually, the reason you would want to pass TRUE for set_any_mask is to make some kind of notation on a menu or dialog, etc. as to which style(s) appear anywhere in a selection but not necessarily applied to the entire selection.

FUNCTION RESULT: Each function returns the text offset (which is a byte offset) of the first text that is examined. For example, if the current selection range in pg was offsets 100 to $500, p g G e t S t y l e I n f o$ would return 100; for the same selection range $p g G e t P a r I n f o$ would return the text offset of the beginning of the first paragraph (which would often be less than 100).

NOTE: If you want to get information about tabs, it is more efficient (and less code) to use the functions in the section below, See also, "Tab Support" on page 9-1.

### 30.9 Getting Info Recs

An additional way to obtain the current font that applies to a text range is to first obtain the style information that applies using pgGetStyleInfo, then get the font record by calling the following function:

(void) pgGetFontInfoRec (pg_ref pg, short font_index, font_info_ptr info);

font_index — should be whatever is in the font_index field in the style_info record (which you received from $p g G e t S t y l e I n f o$ ). The font record is put into info (which must not be a null pointer).

This function is used to fill in the whole font record if you already know its font index number, which you do after doing a $p g G e t S t y l e I n f o$.

Styles and fonts have the same functions that will fill in the appropriate record.

(void) pgGetStyleInfoRec (pg_ref pg, short style_item, style_info_ptr format);

(void) pgGetParInfoRec (pg_ref pg, short style_item, par_info_ptr format);

These functions take the style_item value from a style_run record and return a par_info or style_info record.

NOTE: This is a low-level function that you will rarely need but has been provided for documentation purposes. See style_run information at "More About Style Runs” on page 30-632.

/* This function is to obtain a font record that is "attached" to a style_info record. For example, you could get the whole font record after doing pgGetStyleInfo as follows:

*/

style_info info, mask;

font_info font;

pgGetStyleInfo(pg, NULL, FALSE, \&info, \&mask);

pgGetFontInfoRec(pg, info.font_index, \&font);

### 30.10 Other Style, Font and Paragraph Utilities

Set Insertion Styles

This function provides a convenient way to set both a style record and font for a single insertion point.

(void) pgSetInsertionStyles (pg_ref pg, style_info_ptr style, font_info_ptr font);

The style parameter will be the style that will apply to the next pgInsert; the font parameter will be the font that will apply to the next $\mathrm{pgInsert}$. Neither parameter can be null.

NOTE: This function is intended for single insertion points and will fail to work correctly if there is a selection range in $p g$.

Is pgSetInsertionstyles just a convenience function? or should I be using this to set font/style info when there is only an insertion point (no selection)? i.e. can I simply always use pgSetStyleInfo and pgSetFontInfo, and never use pgSetInsertionstyles?

This is only a convenience function; you will probably never use it. pgSetStylelnfo handles this for you. It checks the selection and, if only a "caret", it calls pgSetInsertionStyles for you.

Style info of a Mouse Point

long pgPtToStyleInfo (pg_ref pg, const co_ordinate_ptr point, short conversion_info, style_info_ptr style, select_pair_ptr range);

This function is useful for determining which style_info is applied to the character containing a specific "mouse" coordinate. For instance, pgPtToStyleInfo() can be used to detect what kind of text the cursor is moving through.

When this function returns, if is non-null it gets set to the range of text for which this style applies (see "Selection range" on page 8-2 for information about select_pair record).

conversion_info is used to indicate one or two special-case alignment instructions, which can be represented by the following bits:

\#define NO_HALFCHARS 0x0001_ Whole char only \#define NO_BYTE_ALIGN 0x0002/* No multibyte alignment

NO HALFCHARS instructs the function to select the right side of a character if the point is anywhere to the right of its left side (not having NO_HALFCHARS set results in the left side of the character if the point is within its first half width, or the right side of the character if the point is within its second half width).

NO_BYTE_ALIGN returns the absolute byte position regardless of multibyte character status. For example, in a Kanji system that contains double-byte characters, setting NO_BYTE_ALIGN can result in the selection of $1 / 2$ character.

NO_HALFCHARS instructs the function to select the right side of a character if the point is anywhere to the right of its left side (not having NO_HALFCHARS set results in the left side of the character if the point is within its first half width, or the right side of the character if the point is within its second half width).

NO_BYTE_ALIGN returns the absolute byte position regardless of multibyte character status. For example, in a Kanji system that contains double-byte characters, setting NO_BYTE_ALIGN can result in the selection of $1 / 2$ character.

FUNCTION RESULT: The function result is the text (character) position for the character found containing point. The function will always return a style and position even if the point is way beyond text (in which case the style for the last character is returned) or before text (where the first style is returned). Either style or range can be a null pointer if you don't need those values.

NOTE: This function always finds some style_info even if point is nowhere near any text. Hence, to detect "true" cursor-over-text situations you should also call $p g P t I n V i e w($ ) to learn of the point is actually over text (or not).

## Font table

(memory_ref) pgGetFontTable (pg_ref pg);

FUNCTION RESULT: This function returns the memory allocation in $p g$ that contains all the fonts used for the text. The memory_ref will contain an array or one or more font_info records.

NOTE: The actual memory_ref that OpenPaige used for this $p g \_r e f$ is returned, not a copy. Therefore do not dispose this allocation and do not delete any records it contains.

To learn how to access a memory_ref, see "The Allocation Mgr" on page 25-1.

### 30.11 Record Structures

style_info

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-653.jpg?height=1523&width=1374&top_left_y=512&top_left_x=220)

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-654.jpg?height=549&width=1385&top_left_y=144&top_left_x=215)

font_index — bThe record number of the font that goes along with this style. (To obtain the actual font, see "Getting Info Recs" on page 30-647 for information about pgGetFont/IfoRec).

NOTE: Do not change the font_index using pgSetStyleInfo. Instead, use pgSet-

FontInfo and the font_index values will be handled by OpenPaige appropriately.

stylesp - An array of shorts that correspond to 32 possible "standard" styles. Each element of styles, if nonzero, implies that style be applied to the text. An overall style of "plain” generally means all style elements are zero.

The standard styles supported by OpenPaige are defined by the following enumerates (each corresponding to one of the array elements):

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-655.jpg?height=1362&width=1160&top_left_y=222&top_left_x=434)

Superscript, subscript —If styles[super_script_var] or styles[subscript_var] apply, their values define the "amount" of shift.

For example, if styles[sub_script_var] contained a value of 3, the text is to be shifted down by 3 points (3 pixels). If styles[super_script_var] were 3, the text is to be shifted upwards by 3 points. However, the shift_verb (below) defines whether or not the super/subscript is relative to the text baseline or relative to a percent of the style's height.

Small caps — If styles[small_caps_var]—applies, the value in this style element indicates a percentage of the original point size to display lower case characters that get converted to ALL CAPS. Or, if this value is -1, the default is used (which is $75 \%$ of the original style).

For example, if style_info->styles[small_caps_var] is 50 and style_info point size is 24 , the lower case text is converted to uppercase 12 point; if style_info>styles[small_caps_var] is -1, the lower case text is converted to 18 (which is $75 \%$ of 24 ).

Relative size —If styles[relative_point_var] applies, the value in this style element indicates a point size to display the text which is a ratio relative to 12 point times the original point size. The ratio is computed as:

style_info->styles[relative_point_var] / 12

(The "original point size" is taken from the "point" field in style_info).

Or example: If style_info->styles[relative_point_var] is 6 and the original point size is 12 , the point size that displays is $12 *(6 / 12)=6$ point. If style_info>styles[relative_point_var] is 6 and the original point size is 24 , the point size that displays is $24 *(6 / 12)=12$ point.

NOTE: The relative_point_var element must not be -1 as there is no default.

Superimpose — If styles[super_impose_var] applies, the value represents a stylesheet ID that gets "superimposed" over the existing style. All fields in the stylesheet style_info>styles[super_impose_var] that are nonzero are temporarily forced into style_info to form a composite style of both.

For example, if style_info->styles[super_impose_var] record had all fields set to zero EXCEPT for the bold_var element, the resulting style would be whatever the original style_info contained but with boldface text.

NOTE: style_info->styles[super_impose_var] can only be zero or a positive number representing a stylesheet ID that actually exists in the $p g \_r e f$.

See “Style Sheets” on page 31-1 for more information.
char_bytes — Defines the number of bytes per character minus 1. For "normal" text, this field will be zero.

NOTE (Macintosh): Do not confuse this with double-byte script such as Kanji. This field is intended for situations where all character are char_bytes +1 in size, such as a feature in which a PicHandle is embedded as a "character." For Kanji, not every character is a double-byte so this field will always be zero.

max_chars —(** not supported currently **). Eventually this will be used for something fancy.

ascent, descent, leading — Define the style's ascent, descent and leading values. (For Macintosh, each value is obtained from the Toolbox call, GetFont/nfo).

NOTE: You do not need to set these fields for "normal" (non-custom) styles because the machine-specific portion of OpenPaige will initialize these fields according to the composite style and font. Additional note: If you need to implement a "set extra leading" feature, use top_extra and bot_extra below.

shift_verb—This value is used only if styles[sub_script_var] or styles[subscript_var] are nonzero. The shift_verb can be one of two values:

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-657.jpg?height=255&width=1172&top_left_y=1203&top_left_x=435)

For baseline_relative, values in styles[sub_script_var] or styles[subscript_var] are considered to be point (pixel) values; for percent_of_style, the super/subscript point values are computed as a percent (value of styles[sub_script_var] or styles[subscript_var]) of the style's total height (ascent + descent + leading). Example: If style's total height is 32 and styles[subscript_var] contained 50, the point value to shift the text will be $32 * 0.50$, or 16 .

class_bits — bContains a set of bits defining specific attributes and behaviors for this style. The current attributes supported by OpenPaige are as follows:

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-658.jpg?height=1014&width=1344&top_left_y=213&top_left_x=216)

Each of the above bits, if set, cause the following (only the bits currently supported are explained):

CANNOT_HILITE_BIT causes highlighting to NOT show for the characters; even if the user does a "Select All," text with this style attribute will not highlight.

CANNOT_BREAK is essentially a "non-breaking" style; characters with this attribute will not break in the middle (unless the line is too large).

STYLE_IS_CONTROL causes the track-control low-level function to be called when a "mouse" click is received (see “Customizing OpenPaige" on page 27-1).

GROUP_CHARS_BIT causes all text in this style to be highlighted as one, i.e. a single click selects the whole group.

STYLE_MERGE_BIT gets set by OpenPaige in "mail merge mode" (you should not set this bit).

STYLE_IS_CUSTOM causes the text to be invisible IF the standard display function is used. In other words, all text with this attribute will only display if you have provided your own display function.

HILITE_RESTRICT_BIT forces a click/drag on this style to stay inside the boundaries of the style.

CANNOT_WRAP_BIT causes text to NOT wrap regardless of width.

IS_NOT_TEXT_BIT tells OpenPaige the character(s) aren't really text. If this is set, the standard text measuring and drawing functions do nothing (hence you would need to set your own hooks for both functions).

REQUIRES_COPY_BIT causes the copy_text_proc (hook) to get called for these character(s); otherwise OpenPaige does not call this hook.

NO_SMART_DRAW_BIT disables the "second-guessing" for fast character display. If this bit is set, the whole text line is drawn (instead of a partial line).

ACTIVATE_ENABLE_BIT causes the style_activate_proc to get called, otherwise that hook is ignored.

CANT_UNDERLINE_BIT informs the text drawing function that the OS will not display an underline style (used for Kanji characters in Macintosh).

CANT_TRANS_BIT informs the "ALL CAPS" and "small caps" functions that the text can't be translated to upper/lower case. This bit might be important for text that is not really text, e.g. a pointer or memory reference.

RIGHTLEFT_BIT indicates the writing direction for the text is right-to-left.

NO_SAVEDOC_BIT causes this style_info to not be included in pgSaveDoc(). One reason you might want to do this is for special applications that want to construct their own styles or stylesheets without saving style_info to each file.

## style info fields (continued)

style_sheet_id p—Contains a unique ID used by style sheet support (see "Style Sheets" on page 31-1).

small_caps_index —pThis is used by OpenPaige when "small_caps_var" style is set; you should not alter this field.

fg_color, bk_color — bDefine the foreground and background color of the text. Both fields are a structure as follows:

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-660.jpg?height=401&width=1182&top_left_y=540&top_left_x=432)

NOTE: The "background" color applies to the text background, not necessarily the window's background.

For example, a line of text drawn with a "blue" background color on a whitebackground window will result in a blue "stripe" of line height's size with the text foreground overlaying the stripe.

machine_var, machine_var2-This is used by OpenPaige internally and must not be altered.

char_width —(Used only in Windows version). This becomes the "ffWidth" value when setting up a LOGFONT for font selection.

point —The point size for this style. This field is a "Fixed" fraction, i.e., the high-order word of the field is the whole integer point size and the low-order word the fractional value, if any. For more on setting point, see "Setting / Getting Point Size" on page 8-5 and "Changing point size" on page 30-641.

left_overhang, right_overhang — These are a form of indent for characters. These fields control how far a style overhangs to the left and/or right, the best example being italic that can overhang to the right.

NOTE: OpenPaige sets the default for these values when the style is initialized.

top_extra, bot_extra — Contains extra leading, in pixels, to add to the top or bottom of the style.

NOTE: You should use these fields — pnot the ascent/descent fields — pfor "add extra leading" features.

space_extra — The fractional amount to add to each space width. This value is a "fixed" fraction (high order word is the whole part and low order word the fraction part).

char_extra - pThe fractional amount to add to each non-space character. This value is a "fixed" fraction (high order word is the whole part and low order word the fraction part) and can be used for kerning.

NOTE (Macintosh): This field is only supported if Color QuickDraw exists.

user_id, user_data, user_data2 2 p-Your app can use these fields for anything it wants. These come in handy for customizing styles.

future - pan array of longs reserved for future enhancement. Do not use these fields.

embed_entry, embed_style_refcon, embed_refcon, embed_id, embed_object — These are used by the TEXT-embed extension and should not be altered. See chapter on "Embedding Non-Text Characters” on page 28-1.

user_var — This can be used for anything. It is intended mainly, however, for source code users who want to append to the style_info record.

procs - pThis is a record of many function pointers that get called by OpenPaige for drawing, text measuring, etc. The array of functions literally define the way this style behaves (OpenPaige will always call one of these functions to obtain information and/or to display text in this style). These are the essence and key to implementing special styles and text types. See "Customizing OpenPaige” on page 27-1.

maintenance, used_ctr — Both of these are used only by OpenPaige for internal maintenance and must not be altered (actually, you cannot alter them anyway; when calling pgSetStyleInfo, bOpenPaige ignores anything you put into these two fields).

A style_info is said to be user-defined if one or more fields contain information understood only by the application. Usually, in all other respects the style looks and feels like any other OpenPaige style.

For example, your application might want to "mark" various sections of text with some special attribute, but invisible to the user. You can set invisible "marks" for various sections of text by merely applying a style_info to the desired text with any of the user fields set to something your app will understand. The user fields are user_id, user_data and user_data2, each can be used for any purpose whatsoever.

font_info

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-662.jpg?height=839&width=1179&top_left_y=1004&top_left_x=433)

The font_info record is somewhat machine-dependent and what should be placed in each field may depend on the platform you are running.

When you set a font_info record, usually only the name, alternate_name and environs fields need be changed; this is because OpenPaige will initialize all the other fields to their defaults when the font is applied to a $p g \_$ref.

One exception to this is setting a Windows font and you require a special character set and/or special precision information (see "Additional Font Info for Windows" below).

NOTE (Macintosh): the font_info.name should be a pascal string terminated with the remaining bytes in font_info.name set to zeros; the font_info.environs field should be set to zero. For an example see "Responding to font menu (Macintosh)" on page 30-644.

NOTE (Windows): the font_info.name can be initially set to either a pascal string or a cstring, with all remaining bytes in font_info.name set to zeros. Usually, due to Windows programming conventions, you will set the name to a cstring. In this case, before passing the font_info record to pgSetFontInfo, you must set font_info.environs to NAME_IS_CSTR.

CAUTION: WINDOWS NOTE: OpenPaige converts font_info.name to a pascal string and clears the NAME_IS_CSTR bit when the font is stored in the $p g \_r e f$. This is done purely for cross-platform portability. This is important to remember, because if you examine the font thereafter with pgGetFontInfo, the font name will now be a pascal string (the first byte indicating the string length), not a cstring.

name - pThis should contain the name of the font. This can either be a pascal string (first byte is the length) or a cstring (terminated with zero). However, the assumption is made by OpenPaige that the string is a pascal string. Hence, you need to set the environs field accordingly if name is a cstring (see below).

alternate_name —-pThis should contain a font name to use as a second choice if the font defined in name does not exist. If OpenPaige can't find the first font, it will try using alternate_name. If you do not have an alternative, set alternate_name to all zeros.
environs - bAdditional information about the font, which contains the following bit (or not):

$$
\text { \#define NAME_IS_CSTR } 1 \quad / * \text { Font name is a cstring */ }
$$

All the other fields in font_info are initialized by OpenPaige when you set a font.

NOTE: You should fill the font name with all zeros before setting the string. This will allow applications to more easily shift between pascal strings and cstrings (because a pascal string will also be terminated with a zero).

NOTE (Macintosh): For your reference, the family_id will get initialized to the font ID and char_type will get set to the font script code (e.g., Roman, Kanji, etc.).

The remaining fields are not supported for any particular purpose and might be used for future enhancements.

language - pThis will contain the language ID for the font. In Windows NT and 95, this contains the langID and code page.

## Additional Font Info for Windows

In certain cases it is necessary to map certain members of the font information to obtain the appropriate character set and drawing precision. The machine_var field in font_info is used for this purpose, the first four elements defined as follows:

machine_var[PG_OUT_PRECISION] should contain output precision. machine_var[PG_CLIP_PRECISION] should contain clipping precision. machine_var[PG_QUALITY] should contain output quality. machine_var[PG_CHARSET] should contain the character set code.

/* This code snippet shows the members of LOGFONT you should map across to font_info: */

font->machine_var[PG_OUT_PRECISION] = log_font$>$ IfOutPrecision;

font->machine_var[PG_CLIP_PRECISION] = log_font$>$ IfClipPrecision;

font->machine_var[PG_QUALITY] = log_font->IfQuality; font->machine_var[PG_CHARSET] = log_font->lfCharSet;

par_info

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-665.jpg?height=739&width=1215&top_left_y=1146&top_left_x=436)

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-666.jpg?height=786&width=1178&top_left_y=175&top_left_x=433)

Field descriptions

justification - pThe justification (alignment) for the paragraph. This value can be any of the following:

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-666.jpg?height=454&width=1149&top_left_y=1453&top_left_x=434)
force_left and force_right are used to force an alignment to one side or the other regardless of the writing direction.

direction - $\mathrm{p}$ Defines the writing direction (left to right or right to left), and can be one of the following:

typedef enum

\{

right_left_direction $=-1, \quad / *$ Right-to-left */

system_direction, /* Direction defined by OS */

left_right_direction /* Left-to-right */

\};

NOTE: The direction parameter defines the writing direction of the paragraph(s) affected by the par_info style. In such paragraphs, bidirectional scripts can exist such as English and Hebrew. While each script has its own direction attribute, the writing direction defines the point of origin for all lines in the paragraph. If writing direction is right-to-left, all text is aligned to the right; if writing direction is left to right, all text is aligned to the left. In both cases, however, individual blocks of text can go either direction relative to the text it is aligned to. For more information on bidirectional scripts, see Inside Macintosh Volume 6 and Apple's Inside Macintosh Text manual.

class_info — b Contains various bit setting(s) defining special attributes. Currently, the following attribute bits are supported:

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-667.jpg?height=258&width=875&top_left_y=1677&top_left_x=433)
num_tabs, tabs — bDefine the tab stop(s). The tabs field contains an array of tab_stop records and num_tabs contains the number of valid elements. Tabs are described in "Tab Support” on page 9-1.

style_sheet_id — pContains a unique ID for paragraph style sheets (see "Style Sheets" on page 31-1).

def_tab_space — bDefines the default tab spacing (when no preset tab stops exist). You can set this to anything.

NOTE: The initial (default) setting is taken from pg_globals (see "Changing Globals” on page 3-21 for more information about pg_globals).

indents — pThese are the paragraph indentations; for information about indents see “Set Indents” on page 9-11 and “Get Indents” on page 9-12.

spacing - Defines the line spacing for the paragraph. This value is a fraction, a "fixed" type in which the whole amount is in the high-order word and the fraction in the loworder word. This value is multiplied times the current line height (ascent + descent) and the result becomes the new height.

For example, to obtain $2 \mathrm{X} 1$ line spacing, the spacing value should be $0 \mathrm{x} 00020000$. For 1.5 X 1 spacing, the value should be $0 \times 00018000$ (low-order word is $1 / 2$ of an unsigned short).

NOTE: A spacing value of zero implies "auto" spacing (lines spaced according to their style). You would also get the same effect if spacing $=0 x 00010000$.

leading_extra, leading_fixed — bBoth of these can also control line spacing. The leading_extra field is added to the line's height. The leading_fixed field, if nonzero, is forced as the line height. Both should never be set to nonzero at the same time since that would make no sense.

top_extra, bot_extra, left_extra, right_extra — pThese are all added to the top, bottom, left and right of the paragraph, respectively.

NOTE: These values are all pixel amounts (point) and they are added to the paragraph's boundaries in addition to everything else (in addition to indentations and spacing, etc.). Note: Use these fields to obtain "space before" and "space after" for paragraphs.
user_id, user_data, user_data2 — bYour app can use these fields for anything it wants. These come in handy for customizing paragraphs.

partial_just, future — bThese are reserved for future enhancement. Do not alter these fields.

procs - pThis is a record of many function pointers that get called by OpenPaige for paragraph formatting. The array of functions literally define the way this format behaves. See “Customizing OpenPaige" on page 27-1.

user_var — pThis can be used for anything. It is intended mainly, however, for source code users who want to append to the par_info record.

maintenance, used_ctr — bBoth of these are used only by OpenPaige for internal maintenance and must not be altered (actually, you can't alter them anyway with pgSetParInfo - bOpenPaige simply ignores anything you put into these two fields)

### 30.12 Creating a simple custom style

One of the most important features of OpenPaige is the ability to create custom styles. There are several issues to be understood when doing custom styles. They involve customizing how OpenPaige draws and measures the text. This is accomplished by using hooks, described in "Customizing OpenPaige" on page 27-1.

However, here simple custom styles can be created by changing just a few functions. The following example shows how to create a custom style that draws a box around some text. In this case, the only thing changing is how the text is drawn.

First of all, I must set the text to my custom style and install the hooks I will need. Second, I show how to initialize my style and my drawing hook. I even get to use the default OpenPaige functions for simply drawing the characters.

Other custom styles may have to use other custom hooks, including measure_proc. But nearly every custom style can be built changing only three:

1. The measure_proc. The (replaced) function must not only measure the character widths correctly, it must also fill in the "types" pointer (see "measure_proc" on page 27-9).
2. The text_draw_proc. The (replaced) function must be able to draw the text on demand (see "text_draw_proc" on page 27-17).
3. The style_init_proc. The (replaced) function probably needs to determine the style's ascent, descent and leading if that functionality for the character set in question does not already exist inherently in the OS. (See "style_init_proc" on page 27-7).

NOTE: Many improvements could be made to this code, such as drawing a single box around the text when boxes are adjacent, setting the box so the offset on the left and right of the style is not right next to the first and last character, using the styles[var] amount for various offsets or widths of the line or both, and implement scaling.

Set some text to a custom style (Cross platform)

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-670.jpg?height=1023&width=1162&top_left_y=836&top_left_x=433)

/* This does the actual box and text drawing. */

/* Note: this does not handle multiple custom styles to do that we will need to build our own myMasterDrawProc with the major changes being 1) a huge if/then for each styles $[$, 2) possibly the order in which these are called AND 3) that the pgDrawProc be called only once. */

static PG_PASCAL (void) box_draw_proc (paige_rec_ptr pg, style_walk_ptr walker, pg_char_ptr data, pg_short_t offset, pg_short_t length, draw_points_ptr draw_position, long extra, short draw_mode)

style_info_ptr original_style $=$ walker->cur_style;

pg_scale_factor scale $=$ pg -> scale_factor;

$/ \star$ this is not implemented ${ }^{\star /}$

Point start_pt;

Point end_pt;

pgDrawProdog,walkerdataoffsetlengthdraw_positionextradraw_mode); /* OpenPaige's standard draw */

start_pt.h = pgLongToShort(draw_position->from.h);

start_pt.v = pgLongToShort(draw_position->from.v);

end_pt.h = pgLongToShort(draw_position->to.h);

end_pt.v = pgLongToShort(draw_position->to.v);

draw_a_box_around_rectangle ( start_pt.h, start_pt.v - original_style>ascent + 1, end_pt.h. end_pt.v+original_style->descent - 1 );

/* on Mac use FrameRect */

Figures out new line heights due to the box (Cross platform)

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-672.jpg?height=835&width=1232&top_left_y=257&top_left_x=434)

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

style_info.procs.bytes_to_unicode;// Non-Unicode to Unicode style_info.procs.unicode_to_bytes;// Unicode to Non-Unicode

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
