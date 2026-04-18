## Sample `merge_text_proc`

This is called when the styles need to be initialised (*i.e.*, usually at the beginning of the program). This sets the merge style `proc`s and `user_id` and the mask makes it so only the two desired `proc`s, `merge_text_proc` and `setup_insert`, get set to our custom ones following:

```
void InitMergeStyles(pg_ref pg)
{
	style_info style, mask;
	pg_style_hooks style_functions;
	pgInitStyleMask(&mask, 0);
	
	style.user_id = STYLE_IS_MERGE;
	mask.user_id = -1;
	
	// The idea is to change only the styles that have pictures:
	
	InitStyleProcsToDefaults(&style_functions); // Init standard procs first.
	style_functions.merge = (pg_proc) merge_text_proc;
	style_functions.insert_proc = (pg_proc) setup_insert;
	pgSetStyleProcs(pg, &style_functions, &style, &mask, NULL, 0, STYLE_IS_MERGE, FALSE, draw_none);
}
```

### Mail merge fields are inserted into text

```
/* This function inserts my "mail merge" fields into the text. I shall use only a couple of style hooks to make this work. */

void insert_merge_fields (doc_rec *doc)
{
	style_info style, mask;
	short index, size_of_fld;
	Str255 name_of_fld;
	for (index = 0; index < NUM_MERGE_FLDS; ++index)
	{
		GetIndString(name_of_fld, MERGE_STRINGS, index + 1);
		size_of_fld = name_of_fld[0];
		pgGetStyleInfo(doc -> pg, NULL, FALSE, &style, &mask);
		pgInitStyleMask(&mask, 0);
		
		// Set up everything I want in the style_info record:
		
		style.user_id = STYLE_IS_MERGE;
		style.class_bits |= (STYLE_IS_CUSTOM | GROUP_CHARS_BIT);
		style.char_bytes = 0;
		style.user_data = index;
		mask.user_id = -1;
		mask.user_data = -1;
		mask.class_bits = -1;
		mask.char_bytes = -1;

		// Set desired function pointers:

		style.procs.merge = (pg_proc) merge_text_proc;
		style.procs.insert_proc = (pg_proc) setup_insert;
		mask.procs.merge = (pg_proc) - 1;
		mask.procs.insert_proc = (pg_proc) - 1;
		pgSetStyleInfo(doc -> pg, (pg_char_ptr) &name_of_fld[1], size_of_fld, CURRENT_POSITION, data_insert_mode, 0, draw_none);
	}
		
	InvalRect(&doc -> w_ptr -> portRect);
  DoAllUpdates();
}
```

### Sample `setup_insert` hook for merging

```
/* This is the hook that gets called when HERMES Paige saves off the next style to apply from the next insert. The reason I need this for merge "characters" is because I don't want the user to "type" or extend text if the caret sits on one of my merge styles. Hence, this function must remove my own hooks from the style so it becomes just a regular style. */

static pascal short setup_insert (paige_rec_ptr pg, style_info_ptr style, long position)
{
	pgInitStyleProcs(&style -> procs);
	// This sets all the standard procs 
	style -> class_bits = 0; 
	style -> user_data = style -> user_id = 0;
	
	return TRUE; 
	/* Won't call me again (because I just nuked my own function ptr */
}
```

### `merge_text_hook`

```
// This gets called by page to swap out text during pgMergeText.
static pascal short merge_text_proc (paige_rec_ptr pg, style_info_ptr style, pg_char_ptr text_data, pg_short_t length, text_ref merged_data, long ref_con)
{
	short field_size
	char *str_to_merge;
	field_size = *merge_text[style -> user_data];
	if (!merged_data)
		return TRUE;
	SetMemorySize (merged_data, field_size);
	if (!field_size)
		return TRUE;
	str_to_merge = (char*) merge_text[style -> user_data];
	++str_to_merge;
	BlockMove(str_to_merge, UseMemory(merged_data), field_size);
	UnuseMemory(merged_data);
	return TRUE;	
}
```

