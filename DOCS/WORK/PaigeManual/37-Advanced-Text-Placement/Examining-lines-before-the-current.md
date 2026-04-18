## Examining line(s) before the current

It may become necessary to examine one or more lines prior to the current line given in the `adjust_proc`.

One example might be a situation where the current line is the middle of a paragraph but you need to know the position of the first line in the paragraph.

Since the starts pointer actually points to a specific element in the entire `array` of `point_starts` that have been computed thus far, you can simply decrement it to examine line(s) before the current position, if they exist.

However, the only `point_start` elements that are *guaranteed* to exist in the `array` are all the elements for the current paragraph; this is due to the fact that HERMES Paige breaks apart large blocks of text into smaller sections—but never in the middle of a paragraph.

### Obtaining the point_starts of the current paragraph

To obtain the first `point_start` of the current paragraph, you can decrement the `starts` pointer until the `flags` field contains `NEW_PAR_BIT`. Here is an example:

	for (;;)
	{
		if (starts -> flags & NEW_PAR_BIT)
			break;
		--starts;
	}

You can do the same thing to back up to start of the previous line, with a slight alteration:

	for (;;)
	{
		--starts;
		if (starts -> flags & NEW_LINE_BIT)
			break;
	}

**CAUTION:** Be sure there are truly previous `point_start` elements before backing up the `starts` pointer. The simplest way to check this is to examine the `offset` field of the `start`; if it is zero, there are no elements before it.

#### Example

	if (starts -> offset == 0)
		/* We must not "back up" because starts is the FIRST START.*/

**NOTE:** The "first `start`" does not necessarily mean the start of the whole document, rather the start of the current block of text. The first `start` however will always be the beginning of a paragraph.

### 37.6 Page Rectangles

For purposes of custom pagination of paragraphs, you probably need to compute the vertical location of page boundaries.

**CAUTION:** The information given here assumes that *repeating shapes are enabled* to achieve a multiple-page effect. If you are using some other method for page breaks, this information might not apply (mainly because we do not know how you have implemented page sizes and breaks).

The following is a list of very useful low-level utility functions that you can use to find out about the current "page" that a line will display in:

	#include "pgShapes.h"
	pg_short_t pgGetWrapRect (paige_rec_ptr pg, long r_num, co_ordinate_ptr offset_extra);

This function returns sufficient information to construct the exact "page" rectangle for a given line of text. (**NOTE:** it is prototyped in `pgShapes.h` and is intended to be called from low-level hooks such as `adjust_proc`).

The `r_num` field must be the value in `r_num` from the first `point_start` of the line. When this function returns, `offset_extra` gets set to the amount to adjust the original page rectangle to obtain the actual, physical page location (remember we are dealing with "repeating shapes", which means the `pg_ref` has only one page shape which repeats; this function computes the physical page position based on that information).

For purposes of obtaining only the vertical positions of the page, the function result can be ignored.

Here is an example of obtaining the page rect for a line of text in question (while in the adjust_proc):

	rectangle page;
	co_ordinate offset_adjust;
	
	pgShapeBounds(pg -> wrap_area, &page);
	/* start with actual page area */
	
	pgGetWrapRect(pg, starts -> r_num, &offset_adjust);
	pgOffsetRect(&page, offset_adjust.h, offset_adjust.v);
	
	/* We now have the "real" page area for the line beginning at "starts" */

