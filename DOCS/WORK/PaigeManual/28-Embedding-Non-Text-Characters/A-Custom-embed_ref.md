## A Custom `embed_ref`

This example shows how to create and manipulate a custom `embed_ref`. In this case we are creating a simple box for which we draw a frame, and we respond in some way if the user double-clicks in this box.

For purposes of demonstration, we also attach a data struct to the custom `embed_ref`. While this example doesn't do anything with that data, it shows how you would save and read your data to an HERMES Paige file.

```C
/* Insertion of a custom ref into a pg_ref "pg". Upon entry, width and height define the dimensions of the box; data is a pointer to some arbitrary data structure that gets attached to the ref (and eventually saved to the HERMES Paige file) and dataSize is the size of that data. The callbackProc param is a pointer to our callback function (almost mandatory for any custom embeds). The refCon value becomes the callback refcon. */

void makeCustomRef (pg_ref pg, short width, short height, char *data, long dataSize, embed_callback callbackProc, long refCon)
{
	embed_ref ref;
	pg_embed_ptr embed_ptr;
	
	/* Create a custom ref, but if we specify embed_user_data then HERMES Paige will attach the data to the ref. */
	
	ref = pgNewEmbedRef(&mem_globals, embed_user_data, (void*) data, dataSize, 0, 0, 0, FALSE);
	
	/* The following code is vital for a "custom" user type since HERMES Paige has no idea how tall our embed item is, nor does it know how wide it is: */
	
	embed_ptr = UseMemory(ref); // Get the embed struct
	embed_ptr -> height = height;
	embed_ptr -> width = width;
	UnuseMemory(ref);

	// Insert the ref. (Also add pgPrepareUndo() here if desired). 
	pgInsertEmbedRef(pg, ref, CURRENT_POSITION, 0, callBackProc, refCon, best_way);
}

/* The following code is the callback function for the embed_ref. HERMES Paige calls this with various "messages". */

PG_PASCAL (long) callBackProc (paige_rec_ptr pg, pg_embed_ptr embed_ptr, long embed_type, short command, long user_refcon, long param1, long param2)
{
	memory_ref specialData;
	Rect theBox;
	char *extraBytes;
	long result = 0 ; // Default function result
	
	switch (command)
	{
		case EMBED_DRAW:
			// In this example we frame the box.
			// param1 is a rectangle_ptr of the box 
			RectangleToRect((rectangle_ptr)param1, NULL, &theBox);
			FrameRect(&theBox);
			break;
		case EMBED_MOUSEDOWN:
		case EMBED_MOUSEMOVE:
		case EMBED_MOUSEUP:
		case EMBED_DOUBLECLICK:
			result = pgDefaultEmbedCallback(paige_rec_ptr pg, pg_embed_ptr embed_ptr, long embed_type, short command, long user_refcon, long param1, long param2);
			if (command == EMBED_DOUBLECLICK) 
				HandleMyDoubleClick(pg, user_refcon);
				// The "HandleMyDoubleClick() is whatever...
				break;
		case EMBED_DESTROY:
			/* Important note: Since our embed_ref type is embed_user_data, we can let HERMES Paige dispose the data. However if we attached our own data directly we would NOT call the standard callback, or we would crash! */
			result = pgDefaultEmbedCallback(paige_rec_ptr pg, pg_embed_ptr embed_ptr, long embed_type, short command, long user_refcon, long param1, long param2);
			break;
		case EMBED_WRITE_DATA:
			/* NOTE, since our embed type is embed_user_data, HERMES Paige will save that data automatically, so we don't need to do anything for this message. But purely for the sake of demonstration we will save two extra bytes to the file to show how it is done: */
			specialData = (memory_ref) param1;
			SetMemorySize(specialData, sizeof(char) * 2); 
			extraBytes = UseMemory(specialData);
			extraBytes[0] = myCustomChar1;
			extraBytes[1] = myCustomChar2;
			UnuseMemory(specialData);
			break;
			/* NOTE, since our embed type is embed_user_data, HERMES Paige will read that data automatically, so we don't need to do anything for this message. But purely for the sake of demonstration we will read the two extra bytes from the file that we saved in EMBED_READ_DATA: */
			extraBytes = (char*) param1; // Pointer to data
			myCustomChar1 = extraBytes[0]; 
			myCustomChar2 = extraBytes[1]; 
			break;
		default:
			result = pgDefaultEmbedCallback(paige_rec_ptr pg, pg_embed_ptr embed_ptr, long embed_type, short command, long user_refcon, long param1, long param2);
			break;
		}
	return result;
}
```

