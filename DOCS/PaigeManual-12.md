## ALL ABOUT SHAPES

The quickest way to get "Up \& Running” with shapes is to see "Up \& Running Shapes” on page 2-20. This shows how to get a document up within rectangles to display and/or edit.

This chapter provides more details should you want to provide the users with more complex shapes.

### 12.2 Basic shape areas

As mentioned in several places in this document, an OpenPaige object maintains three basic shape areas.

The exact description and behavior for each of these shapes is as follows:
vis_area —pThe "viewable" area of an OpenPaige object. Stated simply, anything that OpenPaige displays that is even one pixel outside the vis_area gets clipped (masked out). Usually, the vis_area in an OpenPaige object is some portion (or all) of a window's content area and remains unmoving and stationary. (See Figure 8 on page 12213).

page_area — pThe area in which text will flow. For the simplest documents, the page_area can be considered a rectangle, or "page" which defines the top-left position of text display as well as the maximum width. For example, if you wanted to create a document representing an 8 " wide page, you simply specify a page_area that is 8 inches wide. Hence, text will wrap within those boundaries.

The page_area may or may not be the same size as the vis_area, and may or may not align with the vis_area's top-left position. In fact, a large document on a small monitor would almost always be larger than the vis_area (see Figure 8 on page 12-213).

exclude_area - An optional area of an OpenPaige object in which text flow must avoid. An good example of implementing an exclude_area would be placing a picture on a document in which text must wrap over (or wrap around from left to right). The easiest way to do this would be to build an exclude_area that contains the picture's bounding frame, resulting in the forced avoidance of text for that area.

All three shapes can be changed dynamically at any time. Changing the page_area would force text to rewrap to match the new shape; changing the exclude_area would also force text to rewrap in order to avoid the new areas.

If you are specifically implementing "containers", see "Containers Support" on page 141 which might provide an easier path.

If you are implementing any kind of exclusion shapes, see "Exclusion Areas" on page 15-1.

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-215.jpg?height=813&width=1284&top_left_y=297&top_left_x=439)

As stated, the simplest documents are rectangles; however, the page_area can be nonrectangular. A good example of this would be columns in which text must flow from one column to the other. In this case, the page_area would look something like is shown in Figure 9 on page 12-213.

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-215.jpg?height=459&width=845&top_left_y=1569&top_left_x=649)

### 12.3 Coordinates \& Graphic Structures

For purposes of cross-platform technology, OpenPaige defines its own set of structures to represent screen positions (coordinates) and shapes. Except for machine-specific source files, no reference is made to, say, Macintosh “QuickDraw” structures.

The main components ("building blocks") of shapes are the following record structures:

Rectangle

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-216.jpg?height=303&width=1147&top_left_y=906&top_left_x=427)

Co_ordinate

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-216.jpg?height=300&width=1170&top_left_y=1656&top_left_x=429)

Shapes are simply a series of rectangles. A very complex shape could be represented by thousands of rectangles, the worst-case being one rectangle surrounding each pixel.

All shape structures consist of a bounding rectangle (first rectangle in the array) followed by one or more rectangles; the bounding rectangle (first one) is constantly updated to reflect the bounding area of the whole shape as the shape changes.

Hence, the shape structure is defined simply as:

typedef rectangle shape; /* Also a "shape", really */

typedef rectangle_ptrshape_ptr;

A shape is maintained by OpenPaige, however, as a memory_ref to a block of memory that contains the shape information. In the header it is defined as:

typedef memory_refshape_ref;

/* Memory ref containing a "shape" */

### 12.5 Rules for Shapes

The following rules apply to shapes with respect to the list of rectangles they contain:

1. If rectangle edges are connected exactly (i.e., if two edges have the same value), they are considered as "one" even if such a union results in a non-rectangular shape (see Figure 10 on page 12-216).
2. If rectangle edges are not connected, they are considered separate "containers;" even if they overlap. (Overlapping would result in overlapping text if the shape definition was intended for the area where text is drawn).

## FIGURE\#10 “CONNECTING” RECTANGLES

Rectangles

Effec tive Result
![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-218.jpg?height=332&width=656&top_left_y=368&top_left_x=738)

FIGURE \#11 “NON-CONNECTING” RECTANGLES

Rectangles
![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-218.jpg?height=334&width=280&top_left_y=1081&top_left_x=740)

Effec tive Result
![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-218.jpg?height=336&width=276&top_left_y=1076&top_left_x=1124)

12.6 Building Shapes

Placing data into the shape_ref is the subject of discussion in this section. However, you will not normally manipulate the shape_ref data directly.

The easiest way to create a new shape is to use the following function:

(shape_ref) pgRectToShape (pgm_globals_ptr globals, rectangle_ptr rect);

This returns a new shape_ref (which can be passed to one of the "area" parameters in pgNew). The globals parameter must be a pointer to the same structure given to pgMemStartup() and pgInit().

The rect parameter is a pointer to a rectangle; this parameter, however, can be a null pointer in which case an empty shape is returned (shape with all sides $=0$ ).

Setting a Shape to a Rectangle

If you have already created a shape_ref, you can "clear" its contents and/or set the shape to a single rectangle by calling the following:

(void) pgSetShapeRect (shape_ref the_shape, rectangle_ptr rect);

The shape the_shape is changed to represent the single rectangle rect. If rect is a null pointer, the_shape is set to an empty shape.

Adding to a New Shape

(void) pgAddRectToShape (shape_ref the_shape, rectangle_ptr rect);

The best way to build a shape requiring more than one rectangle is to call the following:

The rectangle pointed to by rect is added to the rectangle list in the_shape, combining it with other rectangles if necessary. When a rectangle is added, $p g$ A $d d R e c t T o S h a p e$ first explores all existing rectangles in the_shape to see if any of them can "merge" with rect (see "Rules for Shapes" on page 12-215). If none can be combined, rect is appended to the end of the list.

If the_shape is empty, the_shape gets set to the dimensions of rect (same as if you called $p g$ SetShapeRect above).

## FIGURE\#12 RESULT OF ADDING A SHAPE

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-220.jpg?height=330&width=826&top_left_y=679&top_left_x=664)

retumed in the_shape

Disposing a Shape

To dispose a shape, call:

(void) pgDisposeShape (shape_ref the_shape);

Two utilities exist that make it easier to create OpenPaige rectangles:

\#include "pgTraps.h"

(void) RectToRectangle (Rect PG_FAR *r, rectangle_ptr pg_rect);

(void) RectangleToRect (rectangle_ptr pg_rect, co_ordinate_ptr offset, Rect PG_FAR ${ }^{*}$ r);

RectToRectangle converts Rect $r$ to rectangle $p g \_$rect. The $p g \_$rect parameter must be a pointer to a rectangle variable you have declared in your code.

Rectangle ToRect converts $p g \_$rect to $r$, also, if offset is non-null the resulting Rect is offset by the amounts of the coordinate (for example, if offset. $h$ and offset. $v$ were 10, 5 the resulting Rect would be the values in $p g \_$rect with left and right offset by 10 amount and top and bottom amounts offset by -5 .

NOTE (Macintosh): Since a Mac Rect has a +- 32K limit for all four sides, OpenPaige rectangle sides larger than $32 \mathrm{~K}$ will be intentionally truncated to about $30 \mathrm{~K}$.

NOTE: Important: You must include " $p g T r a p . h$ " in any code that calls either function above.

### 12.7 Manipulating shapes

Moving shapes

(void) pgOffsetShape (shape_ref the_shape, long h, long v);

Offsets (moves) *the_shape by $h$ (horizontal) and $v$ (vertical) distances. These may be negative. Positive numbers move to the right horizontally and down vertically as appropriate.

FIGURE \#13 RESULT OF OFFSETTING A SHAPE

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-222.jpg?height=424&width=778&top_left_y=1010&top_left_x=606)

Shrinking or expanding shape

(void) pglnsetShape (shape_ref the_shape, long h, long v);

Insets (shrinks or expands) *the_shape by $h$ and $v$ amounts. Positive numbers inset the shape inwards and negative numbers expand it.
(pg_short_t) pgPtInShape (shape_ref the_shape, co_ordinate_ptr point, co_ordinate_ptr offset_extra, co_ordinate_ptr inset_extra, pg_scale_ptr scaling);

pgPtInShape returns "TRUE" if point is within any part of the_shape (actually, the rectangle number is returned beginning with \#1). The point is temporarily offset with offset_extra if offset_extra is non-null before checking if it is within the_shape (and the offset values are checked in this case, not the original point).

If scaling is non-NULL, the_shape is temporarily scaled by that scale factor. For no scaling, pass NULL.

Also, each rectangle is temporarily inset by the values in inset_extra if it is non-NULL. Using this parameter can provide extra "slop" for point-in-shape detection. Negative values in inset_extra enlarge each rectangle for checking and positive numbers reduce each rectangle for checking.

NOTE: For convenience the_shape can be also be MEM_NULL, which of course returns FALSE.

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-223.jpg?height=108&width=1203&top_left_y=1295&top_left_x=446)

Checks to see if a rectangle is within the_shape. First, offset_extra, if non-null, moves rect by the amount in offset_extra. $h$ and offset_extra. $v$, then checks if it intersects any part of the_shape. The result is TRUE if any part of rect is within the shape, FALSE if it is not. If the_shape is empty, the result is always FALSE.

Actually, a "TRUE" result will really be the rectangle number found to intersect, beginning with 1 as the first rectangle.

NOTE: A result of TRUE does not necessarily mean that rect doesn't intersect with any other rectangle in the_shape; rather, one rectangle was found to intersect and the function returns.

If sect_rect is not MEM_NULL, it gets set to the intersection of rect and the first rectangle in the_shape found to intersect it.

Shape Bounds

(void) pgShapeBounds (shape_ref the_shape, rectangle_ptr bounds);

Returns the rectangle bounds of the outermost edges of the_shape. The bounds is placed in the rectangle pointed to by bounds (which cannot be null).

## FIGURE \#14 $\quad$ GRAY RECTANGLE REPRESENTS BOUNDS RETURNED

 BY pgShapeBounds.![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-224.jpg?height=206&width=227&top_left_y=942&top_left_x=939)

Comparing Shapes

(pg_boolean) pgEmptyShape (shape_ref the_shape);

FUNCTION RESULT: This function returns TRUE if the_shape is empty (all sides are the same or all zeros).

(pg_boolean) pgEqualShapes (shape_ref shape1, shape_ref shape2);

FUNCTION RESULT: Returns TRUE if shape 1 matches shape 2 exactly, even if both are empty.

## Intersection of shapes

(pg_boolean) pgSectShape (shape_ref shape1, shape_ref shape2, shape_ref result_shape);

Sets result_shape to the intersection of shape1 and shape2. All shape_ref parameters must be valid shape_ref's, except result_shape can be MEM_NULL (which you might want to pass to just check if two shapes intersect). Additionally, result_shape cannot be the shape shape_ref as shape 1 or shape 2 or this function will fail.

If either shape 1 or shape 2 is an empty shape, the result will be an empty shape. Also, if nothing between shape 1 and shape 2 intersects, the result will be an empty shape.

FUNCTION RESULT: The function result will be TRUE if any part of shape1 and shape2 intersect (and result_shape gets set to the intersection if not MEM_NULL), otherwise FALSE is returned and result_shape gets set to an empty shape (if not MEM_NULL).

## FIGURE \#15 NON-INTERSECTING SHAPES RETURN FALSE AND

 MEM_NULL IN RETURN SHAPE![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-225.jpg?height=390&width=502&top_left_y=1532&top_left_x=662)

shape_2
![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-226.jpg?height=484&width=1094&top_left_y=274&top_left_x=514)

Shape of intersec tion returned in result

FUNCTION RESULT: Neither shape1 or shape 2 are altered by this function.

(void) pgDiffShape (shape_ref shape1, shape_ref shape2, shape_ref result_shape);

FUNCTION RESULT: This function places the difference in result_shape between shape 1 and shape2.

Unlike pgSectShape, result_shape cannot be MEM_NULL; however, it CAN be the same shape_ref as shape1 or shape2.

The "difference" is computed by subtracting all portions of shape1 from shape2, and the geometric difference(s) produce result_shape. If shape1 is an empty shape, result_shape will be a mere copy of shape2; if shape 2 is empty, result_shape will be empty.
![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-227.jpg?height=510&width=582&top_left_y=290&top_left_x=590)

result_shape

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-227.jpg?height=445&width=278&top_left_y=342&top_left_x=1253)

shape1

shape2

result_shape
![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-227.jpg?height=382&width=922&top_left_y=938&top_left_x=590)

## Erase a Shape

(void) pgEraseShape (pg_ref pg, shape_ref the_shape, pg_scale_ptr scale_factor, co_ordinate_ptr offset_extra, rectangle_ptr vis_bounds);

FUNCTION RESULT: This function will erase the_shape (by filling it with the background color of the device in $p g$ ).

The scale_factor parameter defines scaling, if any; for no scaling, pass zero for this parameter. If you want scaling, see "Scaling an OpenPaige Object" on page 16-2.

If offset_extra is non-null, the_shape is temporarily offset by offset_extra> $h$ and offset_extra- $>v$ amounts before the erasure occurs.

If vis_bounds is non-null, then only the parts of the_shape that intersect with vis_bounds get erased, otherwise the whole shape is erased (see illustration below).

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-228.jpg?height=679&width=1271&top_left_y=493&top_left_x=450)

Moving a Shape in a pg_ref

(void) pgOffsetAreas (pg_ref pg, long $\mathrm{h}$, long v, pg_boolean offset_page, pg_boolean offset_vis, pg_boolean offset_exclude);

This function "moves" the page area and/or visual area and/or the exclusion area of $p g$. If offset_page is TRUE the page area is moved; if offset_vis is TRUE the visual area is moved; if offset_exclude is TRUE the exclusion area is moved.

Each area is moved horizontally and vertically by $h$ and $v$ pixels, respectively. What occurs is $h$ gets added to the left and right sides of all rectangles enclosed in the shape while $v$ gets added to top and bottom. Hence the shape is moved left or right, up or down with negative and positive values, respectively.

NOTE: The contents of $p g$ are not redrawn.

### 12.8 Region Conversion Utilities

void ShapeToRgn (shape_ref src_shape, long offset_h, long offset_v, pg_scale_factor PG_FAR *scale_factor, short inset_amount, rectangle_ptr sect_rect, RgnHandle rgn);

This function sets region rgn to src_shape. In addition, the region is offset by offset_ $h$ and offset_vamounts. If scale_factor is non-NULL, the resulting region is scaled by that scaling factor (see "Scaling" on page 16-1).

Each rectangle added to the region is inset by inset_amount (inset_amount is added to the top and left and subtracted from right and bottom).

If sect_rect is non-null, every rectangle in the shape is first intersected with sect_rect and the intersection (only) is output to the region.

NOTE: You must include "pgTraps. $h$ " to use this function.

NOTE (Windows): - b"RgnHandle" is typedef'd in pgTraps. $h$ and is the same as HRGN.

CAUTION: Converting a huge complex shapes to a region can be slow.

The following function is available only for Macintosh that takes a picture and produces a shape that encloses the picture's outside edges:

\#include "pgTraps.h"

(void) PictOutlineToShape (PicHandle pict, shape_ref target_shape, short accuracy)

Given a picture in pict and a shape_ref in target_shape, this function sets target_shape to surround the outside bit image of the picture.

The accuracy parameter can be a value from 0 to 7 and indicates how "accurate" the shape should be: 0 is the most accurate (but consumes the most memory) and 7 is the least accurate (but consumes the least memory). The accuracy value actually indicates how many pixels to skip, or "group" together in forming the image. If accuracy is 0 , the image is produced to the nearest pixel — bwhich theoretically can mean a rectangle might be produced for every pixel surrounding the image (which is why so much memory can be consumed).

The picture does not need to be a bitmap image, and it can be in color (the image is produced around the outside edges of all nonwhite areas for color).

NOTE: Large, complex images can not only consume huge amounts of memory but can take several seconds to produce the image, so use this function sparingly!

NOTE: Important: You must \#include "pgTraps. $h$ " to use this function.

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-230.jpg?height=60&width=371&top_left_y=1570&top_left_x=434)

### 12.9 Page Area Background Colors

OpenPaige will support any background color (which your machine can support) even if the target window's background color is different.

The page area (area text draws and wraps) will get filled with the specified color before text is drawn; hence this features lets you overlay text on top of nonwhite backgrounds (or, if desirable, will also let you overlay white text on top of dark or black backgrounds).

Note that this differs from the $b k_{-}$color value in style_info. When setting the style_info background, OpenPaige will simply turn on that background color only for that text. Setting the general background color (using the functions below) sets the background of the entire page area.

## COLOR TEXT AND TEXT BACKGROUND

NOTE: See "Setting / Getting Text Color" on page 8-10 or "Changing Styles" on page 30-7 for information about setting text color and text background color.

OpenPaige will also recognize which color is considered "transparent." Normally, this would be the same color as the window's normal background color, typically "white."

"Transparent" is simply the background color for which OpenPaige will not set or force. Defining which color is transparent in this fashion lets you control the background color(s) for either the entire window and/or a different color for the window versus the pg_ref's page area.

The color that is specified as "transparent" is basically telling OpenPaige, "Leave the background alone if the page area's background is the transparent color."

For most situations, you can leave the transparent color to its default — pwhite.

Here is an example, however, where you would need to change the transparent color. Suppose your whole window is always blue but you want OpenPaige to draw on a white
background. In this case, you would set the transparent color to something other than "white" so OpenPaige is forced to set a white background. Otherwise, OpenPaige will not change the background at all when it draws text since it assumes the window is already in that color.

### 12.11 Setting/Getting the Background Color

(void) pgSetPageColor (pg_ref pg, color_value_ptr color);

(void) pgGetPageColor (pg_ref pg, color_value_ptr color);

To change the page area background color, call pgSetPageColor. The new background color will be copied from the color parameter.

To obtain the current page color, use pgGetPageColor and the background color of pg is copied to *color.

After changing the background, subsequent drawing will fill the page area with that color before text is drawn.

NOTE: $p g S e t P a g e C o l o r$ does not redraw anything.

### 12.12 Getting/Changing the Transparent Color

The "transparent color" is a global value, as a field in $p g \_g l o b a l s$. Hence, all pg_ref's will check for the transparent color by looking at this field.

If you need to swap different transparent colors in and out for different situations, simply change $p g \_g l o b a l s$->trans_color to the desired value.

NOTE: Usually the only time you need to change the transparent color to something other than its default (white) is the following scenario: Nonwhite back-
ground color for the whole window, but white background for a pg_ref's page area. In every other situation it is safe to leave the transparent color in pg_globals alone.

### 12.13 Miscellaneous Utilities

(void) pgErasePageArea (pg_ref pg, shape_ref vis_area);

This function fills $p g$ 's page area with the current page background color of $p g$.

The fill is clipped to the page area intersected with the shape given in the vis_area parameter. However, if vis_area is a null pointer, then the vis_area in $p g$ is used to intersect instead.

NOTE: You do not need to call this function, normally. OpenPaige fills the appropriate areas(s) automatically when it draws text. This function exists for special situations where you want to "erase" the page area.

### 12.14 OpenPaige Background Colors

The purpose of this technical note is to provide some additional information about OpenPaige "background" colors and their relationship to the window's background color.

First, let's clarify the difference between three different aspects of background:

Page background color -- is the color that fills the background of your page area. The "page area" is the specific area in the $p g \_$ref in which text flows, or wraps. This is not necessarily the same color as the window's background color. For instance, if the page area were smaller than the window that contained it, the page background would fill only the page area, while the remaining window area would remain unchanged.

Window background color -- is the background color of the window itself. This can be different than the window's background color.

Text background color -- is the background color of text characters, applied as a style (just as italic, bold, underline, etc. is applied to text characters). Text background color applies only to the text character itself. This can be different from both window background and page background.

### 12.15 Who/What Controls Colors

When creating new OpenPaige objects, the page area background color is purely determined by the " def_bk_color" member of OpenPaige globals. Afterwards, this color can be changed by calling pgSetPageColor().

The window background color is purely controlled by your application and no OpenPaige functions alter that color.

Text background is controlled by changing the $b k_{-}$color member of style_info, and that color applies only to the character(s) of that particular style.

### 12.16 What is "trans_color" in OpenPaige globals?

The purpose of $p g \_g l o b a l s . t r a n s \_c o l o r$ is to define the default WINDOW background. Since OpenPaige is a portable library, the trans_color member is provided as a platform-independent method for OpenPaige to know what the "normal" background color is.

OpenPaige uses trans_color only as a reference. Essentially, trans_color defines the color that would appear if OpenPaige left the window alone, or the color that would be used by the OS if the window were "erased".

The value of trans_color becomes the most significant when you have set the page and/or text color to something different than the window color, because OpenPaige compares the page and text colors to trans_color to determine whether or not to ERASE the background.

Its reasoning is, "... If the background color I am to draw is NOT the "normal" background color [trans_color], then I need to force-fill the background.”

Conversely, "... If the background color I am to draw IS the same as trans_color, then I don't have to set anything special”.

Herein is most of the difficulty that OpenPaige users encounter with background colors: they set the window to a nonwhite background, yet they usually leave pg_globals.trans_color alone. This is OK as long as trans_color and the page area color are different.

But if you want the page background and window background to be the same, make sure pg_globals.trans_color is the same as the page background color. The general rules are:

1. Always set $p g$ _globals.trans_color to the same value as the window's background color. Do this regardless of what the page area background color will be.
2. The only time you need to change pg_globals.trans_color is when/if you have changed the window's background color to something other than what is already in pg_globals.trans_color.
3. Setting page and/or text color has nothing to do with the window's real background color. These may or may not be the same, and OpenPaige only knows if they match the window by comparing them to trans_color.
4. To make the page area AND the window backgrounds match each other, you must set pg_globals.trans_color, pgSetPageColor() and the window background color to the same color value.
