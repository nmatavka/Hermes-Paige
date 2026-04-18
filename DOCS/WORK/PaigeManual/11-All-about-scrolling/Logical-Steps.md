## Logical Steps

The following pseudo instructions provide an example for any HERMES Paige platform when determining the values that should be set for both horizontal and vertical scrollbars:

	if (I just made a major text change and did not draw) 
		pgPaginateNow(pg, CURRENT_POSITION, FALSE);
	if (pgGetScrollValues(pg, &h, &v, &max_h, &max_v)) returns "TRUE" then 
		I should change my scrollbar values as:
		Set horizontal scrollbar maximum to max_h
		Set horizontal scrollbar value to h
		Set vertical scrollbar maximum to max_v
		Set vertical scrollbar value to v
	else
		Do nothing.

### Update scrollbar values (Windows)

	void UpdateScrollbars (pg_ref pg, HWND hWnd)
	{
		short max_h, max_v;
		short h_value, v_value;
		
		if (pgGetScrollValues(pg (short far *) &h_value, short far, (short far *) &max_h, short far *) &max_v));
		{
			if max_v < 1)
				max_v = 1; // For Windows I don't want scrollbar disappearing
			SetScrollRange (hWnd, SB_VERT, 0, max_v, FALSE);
			SetScrollRange (hWnd, SB_HORZ, 0, max_h, FALSE);
			SetScrollPos (hWnd, SB_VERT, v_value, TRUE);
			SetScrollPos (hWnd, SB_HORZ, h_value, TRUE);

### Update scrollbar values (Macintosh)

	void UpdateScrollbarValues (doc_rec *doc)
	{
		short h, v, max_h, max_v;
		
		if (pgGetScrollValues(doc -> pg, &h, &v, &max_h, &max_v))
		{
			SetCtlMax(doc -> v_ctl, max_v);
			SetCtlValue(doc -> v_ctl, v);
			SetCtlMax(doc -> h_ctl, max_h);
			SetCtlValue(doc -> h_ctl, h);
		}
	}

### TECH NOTE: "Wrong" Scroll Values

> In my application I need to scroll to certain characters or styles in the document. I noticed, however, that the visual location of these special characters are often "wrong", so when I attempt to scroll to these places I do not wind up at the correct place.

Regarding the scrolling issues, you've touched upon a classic problem that I have been handling with support for years and years. "To Paginate or Not To Paginate, that is the question", *pace* Shakespeare.

When dealing with potentially large word-wrapping text, the editor must avoid repaginating the whole document *at all costs*; otherwise, performance is major dog-slow.

Most of our users that have graduated from TextEdit (Macintosh) or EDIT controls (Windows) are limited in their document size and never understand this problem, because TextEdit maintains an array of line positions at all times. That's because it doesn't handle a lot of text so it can get away with it. Our text engines, on the other hand, support massive documents, changing point sizes, irregular wrapping and who knows what else. Hence, to learn the exact document height at any given time, HERMES Paige must calculate every single word-wrapping line to come up with a good answer.

To avoid turning into a major dog, HERMES Paige (and its predecessors) elect to repaginate only at the point they *display*. There are several good reasons for this, the most important one being a typical HERMES Paige-based app applies all kinds of inserts, embedding, style changing and the like before displaying; if HERMES Paige decided to repaginate each time you set a selection or inserted a piece of text or made any changes whatsoever, it would become unbearably slow.

The reason I'm explaining all of this is so you understand WHY your document behaves the way it does with regards to scrolling. Your problem is simply: you have not yet drawn the part of the document that you will scroll to, hence it is unpaginated, hence the "wrong" answer from `pgGetScrollvalues`. That is also why `auto-scroll-to-cursor` works a wee bit better, because the `auto-scroll` forces a redisplay, which forces a paginate, which forces new information about the doc's height which can then return the "right" answer.

Put simply, `pgGetScrollValues` has insufficient information about the whole doc if a part of the doc is "dirty" and undisplayed. That's why forced paginate fixes the problem. That's also why the "wrong" answer from `pgGetScrollValues` is intermittent—your doc won't always be "dirty" every time you call the function, and also sometimes HERMES Paige's best-guess in this case is correct anyway.

So yes, `pgPaginateNow` (see section 24.3, [Paginate Now](#24.3-paginate-now)) is the best approach; I would call it every time before getting the scrollbar info. The problem with your current logic—paginating *after* `pgGetScrollValues`—is that the document hasn't been computed yet for `pgGetScrollValues`, so it might return FALSE, thinking that the document is unchanged. Remember, `pgPaginateNow` isn't that bad since it won't do anything unless the document really needs it.

But, you should pass `CURRENT_POSITION` for the `paginate_to` parameter— that will help performance a bit.

### Setting scroll values

	(void) pgSetScrollValues (pg_ref pg, short h, short v, short align_line, short draw_mode);

This function is the reverse of `pgGetScrollValues`. It provides a way to do absolute position scrolling, if necessary.

For example, you would use `pgSetScrollValues` after the "thumb" is moved to a new location. As in `pgGetScrollValues`, the values are `short`s, but HERMES Paige computes the necessary distance to scroll. (Because of possible rounding errors, however, after you have called `pgSetScrollValues` you should immediately change the scroll indicator settings with the values from a freah call to `pgGetScrollValues`.

### Handling scrolling with mouse (Macintosh)

	/* ClickScrollBars gets called in response to a mouseDown event. If mouse is not within a control, this function returns FALSE and does nothing. Otherwise, scrolling is handled and TRUE is returned. */
	
	int ClickScrollBars (doc_rec *doc, EventRecord *event)
	{
		Point start_pt;
		short part_code;
		ControlHandle the_control;
		start_pt = event -> where;
		
		GlobalToLocal(&start_pt);
		
		if (part_code = FindControl(start_pt, doc-> w_ptr, &the_control))
		{
			scrolling_doc = doc;
			if (part_code == inThumb)
			{
				long max_h, max_v;
				long scrolled_h, scrolled_v;
				long scroll_h, scroll_v;
				short v_factor, old_h_position;
				
				if (TrackControl(the_control, start_pt, NULL))
				{
					old_h_position = GetCtlValue(doc -> h_ctl);
					pgSetScrollValues(doc -> pg, GetCtlValue(doc -> h_ctl), GetCtlValue(doc -> v_ctl), TRUE, best_way);
					UpdateScrollbarValues(doc);
					update_ruler(doc, old_h_position);
				}
				else
					TrackControl(the_control, start_pt, (ProcPtr) scroll_action_proc);
			}
			return (part_code != 0);
		}


### Maximum scroll value

Adjustments may be needed after large deletions; if so, call the following function.

	(pg_boolean) pgAdjustScrollMax (pg_ref pg, short, draw_mode);

This tells HERMES Paige that `pg` might need some adjustment after a large deletion or text size change.

For example, suppose you had a document in 24-point text, scrolled to the bottom. User changes the text to 12 point, resulting in a scrolled position way too far down! If you call `pgAdjustScrollMax`, this situation is corrected (by scrolling up the required distance).

If `draw_mode` ≠ 0, actual physical scrolling takes place (otherwise the scroll position is adjusted internally and no drawing occurs). `draw_mode` can be the values as described in [Draw Modes](#draw-modes) under section 2.11:

	draw_none,		// Do not draw at all 
	best_way,		// Use most efficient method(s) 
	direct_copy,	// Directly to screen, overwrite 
	direct_or,		// Directly to screen, "OR" 
	direct_xor,		// Directly to screen, "XOR" 
	bits_copy,		// Copy offscreen 
	bits_or,		// Copy offscreen in "OR" mode 
	bits_xor		// Copy offscreen in "XOR" mode

**FUNCTION RESULT:** The function returns TRUE if the scroll position changed.

