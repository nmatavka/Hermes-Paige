## Checking page overflow

**NOTE:** You should not implement this code if your `pg_ref` is set for repeating pages, or if your `pg_ref` is set for a variable document height.

	/* Call the function below after doing anything that can change the size of the document. This included insertions, deletions, style and font changes (which can cause new word wrapping) and page size changes. This function returns the number of characters that are overflowing the page area of pg. */
	
	/* Note: CHECK_PAGE_OVERFLOW must be set with pgSetAttributes2(pg). */
	
	long CheckPageOverflow (pg_ref pg)
	{
		paige_rec_ptr pg_rec;
		long_overflow_amount;
		
		pg_rec = UseMemory(pg);
		overflow_amount = pg_rec->overflow_size;
		UnuseMemory(pg);
		
		return overflow_amount;
	}

### TECH NOTE: Carriage return/line feeds causing problems

Regarding LF/CR characters, HERMES Paige handles both of them as a "new line" except a CR. It also starts a new paragraph, but for LF it just does a line feed.

Note that lines that terminate both in LF and CR will cause *two* lines on the screen — at least in HERMES Paige default mode.

You can turn that off, however, if you want LF/CR to be treated as only one líne feed.' To do so, just set `NO_LF_BIT` in the HERMES Paige attribute flags during `pgNew`. When this attribute is set, HERMES Paige ignores all LFs embedded in the text (they become invisible).

Note that I haven't mentioned what the values are for LF and CR, because those are whatever values sit in HERMES Paige globals. Also as mentioned, MPW will compile `\r` etc. differently than Symantec so watch out for that. See technical note [CR/LF Conversion](#tech-note:-cr/lf-conversion) and section 3.8, [Changing Globals](#3.8-changing-globals).

