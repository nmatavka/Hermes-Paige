## 5 Cut, Copy, Paste

This section explains how to implement Cut, Copy, Paste and Undo, including additional methods to copy "text only."

### 5.1 Copying and Deleting

	(pg_ref) pgCut (pg_ref pg, select_pair_ptr selection, short draw_mode);
	(pg_ref) pgCopy (pg_ref pg, select_pair_ptr selection);
	(void) pgDelete (pg_ref pg, select_pair_ptr delete_range, short draw_mode);

To perform a "Cut" operation — for which text is copied and then deleted — call `pgCut`. The `selection` parameter is an optional pointer to a pair of text offsets from which to delete text. This is a pointer to the following structure:

	typedef struct
	{
		long begin;	// Beginning offset of some text portion
		long end;	// Ending offset of some text portion
	}
	select_pair, *select_pair_ptr;

The `begin` field of a `select_pair` defines the beginning text offset and the `end` field defines the ending offset. Both offsets are byte counts, not character counts. Text offsets in HERMES Paige are zero-indexed (first offset is zero). The last character "end" is included in `selection`.

**FIGURE 3 SELECTION BEGIN AND END EXPLAINED**

![](figs/fig3.jpg)

**NOTE:** All offsets are byte counts. In the case of characters, they are each one byte.

If the selection parameter in `pgCut` is a null pointer, the current selection in `pg` is used instead (which is usually want you want).

**FUNCTION RESULT:** The function result of `pgRef` is a newly created HERMES Paige object containing the copied text and associated text formatting. You can then pass this `pg_ref` to `pgPaste`, below.

`draw_mode` can be the values as described in [Draw Modes](#draw-modes) under section 2.11:

	draw_none,		// Do not draw at all 
	best_way,			// Use most efficient method(s) 
	direct_copy,	// Directly to screen, overwrite 
	direct_or,		// Directly to screen, "OR" 
	direct_xor,		// Directly to screen, "XOR" 
	bits_copy,		// Copy offscreen 
	bits_or,			// Copy offscreen in "OR" mode 
	bits_xor			// Copy offscreen in "XOR" mode

##### NOTES:

1. The `pg_ref` returned from `pickup` is a "real" HERMES Paige object, which means you need to eventually dispose of it properly using `pgDispose`.
2. Shapes from the source `pg_ref` are used to "clone" the resulting `pg_ref` from a copy or cut regardless of the selection range. For example, if the source `pg_ref` that gets copied contained a `page_area` shape with dimensions 10, 10, 580, 800, the resulting `pg_ref` will have the same `pg_area` shape. The same is true for `vis_area` and `exclude_area`.

**CAUTION:** If there is nothing to copy (no selection range exists), both `pgCut` and `pgCopy` will return `MEM_NULL`.

**CAUTION:** It is wise never to display the resulting `pg_ref` unless you first set a default graphics device to target the display. For example, doing a `pgCopy` then drawing to a "clipboard" window later could result in a crash. This can happen if the original window containing the copied `pg_ref` has been closed (rendering an invalid window attached to the copied reference). Hence, before drawing to such a "clipboard", use `pgSetDefaultDevice`. See [Setting a device](#setting-a-device) under section 3.4.

The `pgCopy` function is identical to `pgCut` except that no text is deleted, only a `pg_ref` is returned which is the copy of the specified text and formatting and no `draw_mode` is provided (because the source `pg_ref` remains unchanged).

HERMES Paige provides excellent error checking for out-of-memory situations with `pgCopy`. See chapter 26, [Exception Handling](#26-Exception-Handling).

The `pgDelete` function is the same as `pgCut` in every respect except that a "copy" is neither made nor returned. Use this function when you simply want to delete a selection range but not make a copy (such as a *Clear* command from a menu).

