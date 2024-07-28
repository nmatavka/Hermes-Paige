# 17 OPENPAIGE IMPORT EXTENSION (for "RTF" and other types)

The OpenPaige import extension provides high-level functionality for importing ASCII files, other OpenPaige files and Rich Text Format (RTF) files. Although it is designed as a C++ framework, files can be imported from straight C if necessary.

## 17.1 Installation

**NOTE:** The installation procedure mentions the directory, "pgtxr".

If you are installing the importer for the Macintosh platform and/or for Win16 or Win32 and are not using MFC, simply add the following files from the "pgtxr" directory to your project:

##### Minimum configuration (import ASCII text only)
* `pgimport.cpp`
* `pgdeftbl.c`

##### Native OpenPaige file Import (in addition to above)
* `pgnative.cpp`

##### RTF File Import (in addition to Minimum Configuration)
* `pgrtfdef.c`
* `pgrtfimp.cpp`

### Header Files

##### If you will be importing files using C++:
* `#include "pgtxrcpp.h"`

##### If you will be importing only from straight C:
* `#include "pgtxr.h"`

## 17.2 Importing Files (from C++)

**CUSTOM CONTROL USERS:** There are (intentionally) no control messages that support the OpenPaige Import extension. Use the method shown below; also, see "Importing to the OpenPaige Custom Control"<!-- on page 17-311-->.

Loading a file with this extension can be accomplished in a few easy steps:

1. Start with an existing `pg_ref` or OpenPaige control as the target document to receive the import. This may be a, empty document or a document which already has text (in which case the file contents will be inserted at the current insertion point).
2. To import from a disk file, open the file you wish to import. To import from memory, allocate the memory and fill its contents with the appropriate file data. If you are starting with a Macintosh Handle or Windows `HGLOBAL` you can convert it to a `memory_ref` by calling `HandleToMemory()`.
3. Create a new object (with `new` keyword) of the appropriate type for the file. (If you aren't sure about what type of file you just opened, see "Determining File Type"<!-- on page 17-301 --> in this document. Currently we support raw text files, RTF and OpenPaige files. The following is an example of creating an appropriate import object:

		#include "pgTxrCPP.h"
		PaigelmportObject filter;
	
		// To import a plain ASCII text file:
		filter = new PaigelmportFilter();
		
		// To import an RTF file:
		filter = (PaigelmportObject) new PaigeRTFImportFilter();
		
		// To import an OpenPaige file:
		filter =(PaigelmportObject) new PaigeNativeImportFilter();

4. Call the initialization member function `pgInitImportFile()`. This function is defined as follows:

		pg_error pglnitlmportFile (pg_globals_ptr globals, pg_file_unit fileref, memory_ref memory_unit, file_io_proc read_proc, long first_position, long last_position);
	
	This function prepares for importing a file, setting up whatever is necessary for the file's native format. A file can be imported from a physical file, or from memory. This is differentiated by the value(s) you pass in the function parameters, as follows:
	* `globals` - A pointer to your OpenPaige globals. Custom control users can get a pointer to the OpenPaige globals as follows: 
		1. Getting the `pg_ref` from the control by sending a `PG_GETPGREF` message, and
		2.  Calling the OpenPaige API, `pgGetGlobals()`.
	* `fileref` - If importing from a file on disc, this parameter must be a reference to the opened file (the `refNum` for *Macintosh* or a file handle for *Windows*). If importing from memory, `fileref` should be zero. **MFC** users on *Windows* should note that the `fileref` parameter must be a "real" `HFILE` (or `NULL` if importing from memory), not some other MFC-generated class member that you may assume is a file handle.
	* `memory_unit` — If importing from a file on disc, this parameter must be `MEM_NULL`. If importing from memory, this must be a `memory_ref` (see "The Allocation Mgr"<!-- on page 25-1-->). Importing from memory requires that `memory_unit` contains the same information in the same format as it would if it were a disk file.
	* `read_proc` — This is an optional I/O function to be used instead of the default low-level reading function. Refer to the OpenPaige Programmer's Guide for information about custom I/O functions. For reading a standard file from disk or memory, pass NULL for this parameter.
	* `first_position`, `last_position` — These two values indicate the beginning and ending file positions to import, respectively. The `first_position` can be zero or some other byte offset into the file to begin reading. If `last_position` is unknown (or if you want to read the file whole), pass `UNKNOWN_POSITION` for `last_position`. Otherwise, the file will be imported from byte offset `first_position` to, but not including the byte at `last_position`. <br> **FUNCTION RESULT:** If this function is successful, zero is returned, otherwise an error code is returned.

5. To read the file and insert its contents into an OpenPaige document, call the member function, `pgImportFile()`:

		pg_error pgImportFile (pg_ref pg, long pg_position, long import_flags, pg_boolean keep_selection, short draw_mode)

	* `pg` — The target document. Custom control users: obtain the `pg_ref` by sending a `PG_GETPGREF` message.
	* `pg_position` — The text position (in the OpenPaige document) to receive the insertion. If this value is `CURRENT_POSITION`, the file will be imported to the current insertion.
	* `import_flags` — A set of bits defining which item(s) to import, which can be any or all of the data types shown below. (Note, setting these bits causes that data item to import only if supported by the importer).
	
			#define IMPORT_EVERYTHING_FLAG			0x00FFFFFF	// Import everything
			#define IMPORT_TEXT_FLAG				0x00000001	// Import raw text
			#define IMPORT_TEXT_FORMATS_FLAG		0x00000002	// Import text formats
			#define IMPORT_PAR_FORMATS_FLAG		 	0x00000004	// Import paragraph formats
			#define IMPORT_PAGE_INFO_FLAG			0x00000008	// Import page information
			#define IMPORT_CONTAINERS_FLAG			0x00000010	// Import container boxes
			#define IMPORT_HEADERS_FLAG				0x00000020	// Import headers
			#define IMPORT_FOOTERS_FLAG				0x00000040	// Import footers
			#define IMPORT_FOOTNOTES_FLAG			0x00000080	// Import footnotes
			#define IMPORT_EMBEDDED_OBJECTS_FLAG	0x00000100	// Import embedded graphics
			#define IMPORT_PAGE_GRAPHICS_FLAG		0x00000200	// Import page pictures
			#define IMPORT_STYLESHEETS_FLAG			0x00000400	// Import style sheets
<br> In addition to the above, setting the following bit causes page dimensions (paper size, margins) to get applied:
			
			#define APPLY_PAGE_DIMENSIONS	0x02000000	// Apply page size(s)
			#define IMPORT_CACHE_FLAG		0x04000000	// Page-read the file
<br>If `APPLY_PAGE_DIMENSIONS` is set, the `pg_ref`'s page shape is changed per the import information (if such information is supported). For example, when importing an RTF file, setting `APPLY_PAGE_DIMENSIONS` will apply the page size(s) found in the RTF information. If this bit is not set, the page area remains unchanged. If `IMPORT_CACHE_FLAG` is set, the file is opened in "paging" mode, i.e. its text is not read all at once; rather, its text sections are read as needed. This is particularly useful for opening very large files. 
<br> **NOTE:** `IMPORT_CACHE_FLAG` is only supported for OpenPaige and ASCII text files. (See 2.0b release notes, "Huge File Paging”)

	* `keep_selection` — If TRUE, the selection point in the text does not advance, otherwise the selection point in the document advances by the number of bytes that were imported.
	* `draw_mode` — If non-zero, the document is redrawn showing the new data contents; otherwise, nothing is redrawn.
<br> **FUNCTION RESULT:** If this function is successful, zero is returned, otherwise an error code is returned.

6. Delete the object, or if you want to import another file, repeat steps 4 through 5.

#### Import File Example

	 #include "pgTxrCPP.h"
	 
	 /* This function imports a file into a pg_ref, first creating an object for the appropriate file type. If all is well, the document is re-drawn and NO_ERROR is returned. */
	 
	 pg_error ImportFile (pg_ref pg, pg_filetype filetype, long feature_flags, long file_begin, pg_file_unit f_ref)
	 {
	 	PaigeImportObject filter;
	 	pg_globals_ptr globals;
	 	long flags;
	 	pg_error result = NO_ERROR;
	 	
	 	if (!(flags = feature_flags))
	 		flags = IMPORT_EVERYTHING_FLAG;
	 		globals = pgGetGlobals(pg);

	 	switch (filetype)
	 	{
	 		case pg_text_type:
	 			filter = new PaigeImportFilter();
	 			break;
	 		case pg_rtf_type:
	 			filter = (PaigeImportObject) new
	 			PaigeRTFImportFilter();
	 			break;
	 		case pg_paige_type:
	 			filter = (PaigeImportObject) new
	 			PaigeNativeImportFilter();
	 			break;
	 		default:
	 			return (pg_ERROR) BAD_TYPE_ERR;
	 	}
	 	
	 	if((result = filter -> pgInitImportFile
	 	
## 17.3 Determining File Type

There might be cases where the file type is unknown and/or you want to verify that a file is truly the type that you expect. There is a function you can call to determine the type:

	pg_filetype pgDetermineFileType (pg_file_unit fileref, file_io_proc io_proc, long starting_position)

**NOTE:** Calling this function examines the appropriate contents of a file looking for a signature recognized by one of the support file import classes. The actual file contents are examined to determine the type.

`fileref` — An opened file reference (the "refNum" for Macintosh or file handle for Windows).

`io_proc` — The low-level function to perform I/O. This is described in the OpenPaige Programmer's Guide. Except for implementing very special features, usually you should pass NULL for this parameter.

`starting_position` — Indicates the file position you will (eventually) begin importing.

This function will always return one of the following types:

	#include "pgTxr.h"
	
	enum
	{
		pg_unknown_type,	// Unknown file type
		pg_text_type,		// Standard ASCII text
		pg_rtf_type,		// Rich text format
		pg_paige_type,	// Standard OpenPaige file type
	}

**NOTE:** An unrecognised file will usually return as `pg_text_type` because a `text` file is considered to be practically anything. For this reason, `pgDetermineFileType()` will first check for `pg_rtf_type` and `pg_paige_type` before deciding it is simply a text file.

## 17.4 Determining the Feature Set

You can determine what data type(s) are supported by the importer if you examine `object -> feature_bits` immediately after creating the import object. This member will be initialised to some combination in list shown on the following page:

	IMPORT_TEXT_FEATURE				Can import raw text
	IMPORT_TEXT_FORMATS_FEATURE		Can import styled text
	IMPORT_PAR_FORMATS_FEATURE		Can import paragraph formats
	IMPORT_PAGE_INFO_FEATURE		Can import page dimensions
	IMPORT_CONTAINERS_FEATURE		Can import containers
	IMPORT_HEADERS_FEATURE			Can import headers
	IMPORT_FOOTERS_FEATURE			Can import footers
	IMPORT_FOOTNOTES_FEATURE		Can import footnotes
	IMPORT_EMBEDDED_OBJECTS_FEATURE	Can import supported embed_refs
	IMPORT_PAGE_GRAPHICS_FEATURE	Can import page-layour graphics
	IMPORT_CACHE_FLAG				Can disc-page the file

#### Example

	PaigeImportObject filter;
	
	filter = (PaigeImportObject) new PaigeRTFImportFilter();
	
	if (!(filter -> feature_bits & IMPORT_EMBEDDED_OBJECTS_FEATURE))
		AlertUser("Any pictures in document will be lost. Open anyway?")

## 17.5 Cross-Mapping Font Tables

The OpenPaige importer extension provides a default mapping table for font names when you import a file generated from another platform. For any font name that is imported, if a match is found in the table then the suggested substitute name is used; if no match is found, the default font name (defined in OpenPaige globals) is used instead. The assumption is that the font name won't exist in the target platform.

You can override the defaults in one of two ways:

1. Substitute your own pointer to a font mapping table (see below). You can substitute your own table after the `PaigeImportFilter` object is created. For example:
		
		PaigeImportObject filter;
		
		filter = (PaigelmportObject) new PaigeRTFImportFilter();
		filter -> font_cross_table = (pg_char_ptr)MyOwnFontTable;
		
2. Override the font mapping member function. The function that maps font substitution can be overridden if you subclass the desired import structure. The font mapping function is declared as:

		virtual void pgMapFont (font_info_ptr font, long importing_os, long current_os);
-
Upon entry, "font" is the `font_info` pointer in question. The `importing_os` and `current_os` define the platform of the importing file and the current (runtime) platform, respectively. These platform definitions will be one of the following:

		#define MACINTOSH_OS	1
		#define WINDOWS_OS		2
		#define UNIX_OS			3
-
To substitute a font, simply change `font -> name` before returning from the function.

CAUTION: The font name, by default, is a pascal string (first byte is its length). If you replace it with a cstring you must set font->environs to NAME_IS_CSTR.

If you want no font mapping at all, set the object's member "font_cross_table" to NULL after creating it.

### Font Table Format

The font mapping table is a table of null-terminated text strings. Each entry (delimited by a null character) is ordered in ascending alphabetical order, the last entry is terminated with `\ff` (see default tables below). Each entry contains a font name (with possible asterisk `*` wildcard) followed by a substitute name in square brackets `[]`.

#### EXAMPLE 1:

	"WingDings[Zapf Dingbats]\0"

If the imported font name is "WingDings" then substitute "Zapf Dingbats".

#### EXAMPLE 2:

	"Times*[Times]\0"

If imported font's first five characters are "Times" then substitute "Times". (Hence, both “Times New Roman” and “Times Roman” would be subtitled with “Times”).

## 17.6 Default Font Tables

### Importing to Macintosh (and file is from Windows)

	static pg_char cross_font_table[] =
	{
		"Arial*[Helvetica]\0"
		"Book*[Bookman]\0"
		"Century Gothic[Avant Garde]\0"
		"Century Sch*[New Century Schoolbook]\0"
		"Courie*[Courier]\0"
		"Fixedsys[Chicago]\0"
		"Helvetic*[Helvetica]\0"
		"Monotype Cors*[Zapf Chancery]\0"
		"MS S*[Geneva]\0"
		"Roman[New York]\0"
		"Script[Zapf Chancery]\0"
		"Small Fonts[Monaco]\0"
		"Terminal[Monaco]\0"
		"Times*[Times]\0"
		"Wingdings[Zapf Dingbats]\0"
		"\ff"
	};
		
### Importing to Windows (and file is from Macintosh)

	static pg_char cross_font_table[] =
	{
		"Avant Garde[Arial]\0"
		"Bookman[Times New Roman]\0"
		"Chicago[FixedSys]\0"
		"Courier[Courier New]\0"
		"Geneva[MS Sans Serif]\0"
		"Helvetica[Arial]\0"
		"Monaco[Courier New]\0"
		"Helvetic*[Arial]\0"
		"New York[MS Serif]\0"
		"Palatino[Arial]\0"
		"Symbol[WingDings]\0"
		"Times[Times New Roman]\0"
		"Zapf Chancery[Script]\0"
		"Zapf Dingbats[WingDings]\0"
		"\ff"
	};

## 17.7 Character Mapping

The importing mechanism will also map ASCII characters > 0x7F. If you wish to override the defaults you should subclass the `import` class and override the following function:

	virtual void pgMapChars (pg_char_ptr c hars, long num_chars, long file_os, long current_os);

This function gets called after each block of text is imported. Upon entry, `chars` points to the block of text and `num_chars` defines the number of bytes. The `file_os` and `current_os` define the platform of the importing file and the current (runtime) platform. The possible values for these will be one of the following:

	#define MACINTOSH_OS	1
	#define WINDOWS_OS		2
	#define UNIX_OS			3

You can also override the character mapping by substituting your own character mapping table. The character mapping table is a series of unsigned characters, each entry representing consecutive characters from 0x80 to 0xFF.

For example, if the first three bytes being imported were 0x80, 0x81, and 0x82, the following character mapping table would cause 0xAA, 0xBB, and 0xCC to be inserted into the OpenPaige document:

	unsigned char mapping_table[]=
	{
		0xAA, 0xBB, 0xCC, ...
	}

An entry in the mapping table of null (zero value character) denotes that the character is not available in the current platform. If so, the `unknown_char` member in `paige_globals` is used.

To substitute your own mapping table, first create the import object then change `object -> character_table`.

#### EXAMPLE:

	PaigeImportObject filter;
	filter = (PaigeImportObject) new PaigeRTFImportFilter();
	filter -> character_table = (pg_char_ptr)MyOwnCharTable;

## 17.8 Importing from C

**CUSTOM CONTROL USERS:** There are (intentionally) no control messages that support the OpenPaige Import extension. Use the method shown below; also, see "Importing to the OpenPaige Custom Control"<!-- on page 17-311-->. If you need to import a file from a non-C++ environment—or if you want to import a file from a single line of code—you can do so by calling the following function:

	pg_error pgImportFileFromC (pg_ref pg, pg_filetype filetype, long feature_flags, long file_begin, pg_file_unit f_ref)

This function imports a file of type `filetype` into `pg`. The `filetype` parameter must be one of the following:

	pg_text_type, 	// Standard ASCII text
	pg_rtf_type,		// Rich text format
	pg_paige_type		// Standard OpenPaige file type

The `feature_flags` parameter indicates which data type(s) you want to import, which can be any of the following bit settings:

	#define IMPORT_EVERYTHING_FLAG			0x00FFFFFF	// Import everything
	#define IMPORT_TEXT_FLAG				0x00000001	// Import raw text
	#define IMPORT_TEXT_FORMATS_FLAG		0x00000002	// Import text formats
	#define IMPORT_PAR_FORMATS_FLAG		 	0x00000004	// Import paragraph formats
	#define IMPORT_PAGE_INFO_FLAG			0x00000008	// Import page information
	#define IMPORT_CONTAINERS_FLAG			0x00000010	// Import container boxes
	#define IMPORT_HEADERS_FLAG				0x00000020	// Import headers
	#define IMPORT_FOOTERS_FLAG				0x00000040	// Import footers
	#define IMPORT_FOOTNOTES_FLAG			0x00000080	// Import footnotes
	#define IMPORT_EMBEDDED_OBJECTS_FLAG	0x00000100	// Import embedded graphics
	#define IMPORT_PAGE_GRAPHICS_FLAG		0x00000200	// Import page pictures
	#define IMPORT_STYLESHEETS_FLAG			0x00000400	// Import style sheets

In addition to the above, setting the following bit causes page dimensions (paper size, margins) to get applied:

	#define APPLY_PAGE_DIMENSIONS	0x02000000	// Apply page size(s)
	#define IMPORT_CACHE_FLAG		0x04000000	// Page-read the file

The `file_begin` parameter indicates the first file position to begin reading.

The `f_ref` parameter must be a reference to an opened file (`refNum` for Mac, `file handle` for Windows).

If this function is successful, the contents of the file are inserted into the current position of `pg` and the document is redrawn and `NO_ERROR (0)` is returned. Otherwise the appropriate error code will be returned.

## 17.9 Importing to the OpenPaige Custom Control

There is no message-based support in the custom control to import a file using the methods shown in this document; the lack of message-based importing is an intentional omission to allow optional compiling of the import classes independent of the control. To import a file into the custom control, you may simply obtain the `pg_ref` using the `PG_GETPGREF` message.

However, importing a file into a control can cause an out-of-sync situation with scrollbar positions, pagination, etc., so you should always send the following message immediately after importing a file:

	SendMessage(hwnd, PG_REALIZEIMPORT, wParam, 0);

The `PG_REALIZEIMPORT` message informs the control that you have imported a file and that it should make any adjustments necessary to reflect those changes.

If `wParam` is TRUE the control repaints itself.

## 17.10 The `PaigeImportFilter`: Overrideables

	class PaigeImportFilter
	{
		public:
		pg_char_ptr font_cross_table;	// Table of cross-fonts
		pg_char_ptr character_table;	// Table of cross-chars
		
		// Overrideable member functions (higher level):
		
		virtual pg_error pgVerifySignature(void);
		virtual pg_error pgPrepareSignature(void);
		virtual pg_boolean pgReadNextBlock(void);
		virtual pg_error pgImportDone();
		virtual void PG_FAR * pgProcessEmbedData (memory_ref ref, long embed_type);
		virtual void pgMapFont (font_info_ptr font, long importing_os, long current_os);
		virtual void pgMapChars (pg_char_ptr chars, long num_chars, long file_os, long current_os);
	};

**NOTE:** All of the class definitions are not shown. Only the members of potential interest and usefulness are given. For a complete description of this class, see pgtxrcpp.h.

### Member-by-Member Description

`font_cross_table` — A pointer to the font mapping table. See "Cross-Mapping Font Tables" <!-- on page 17-303--> in this document.

`character_table` — A pointer to the character mapping table (for characters > 0x7F). See "Character Mapping"<!-- on page 17-308--> in this document.

`pg_error pgVerifySignature()` — Called to verify if the file about to be imported contains valid contents for the supported type. For example, `pgVerifySignature()` for the RTF class checks for the existence of the keyword `\rtf` at the start of the file to verify if it is truly an RTF file or some other format. If the file is valid, `NO_ERROR` should be returned, otherwise return `BAD_TYPE_ERR`.

`pgPrepareImport()` — Called to make any preparations for importing the file. No actual file transfer is performed, but this function should be used to initialize private members to perform the first "read". There are no parameters to this function. The values taken from the application's call to `pgInitImportFile()` will have been placed into the appropriate member values before `pgPrepareImport()` is called.

`pg_boolean pgReadNextBlock()` — Called to import (read) the next block of text. A "block of text" means a block of one or more characters that are rendered in the same consistent format.

For example, if the incoming text contained "**Bold**\_Plain\_*Italic*", the import class must consider `Bold_`, `Plain_` and `Italic_` as three separate blocks. The first time `pgReadNextBlock()` gets called, the text `Bold_` would be returned; the next time `Plain_` is returned, and so forth.

Most of the text and format information must be placed in the "translator" member of the class; this member is a record defined as follows:

	struct pg_translator
	{
		memory_ref data;				// Data transferred (read) or to-transfer (write)
		memory_ref stylesheet_table;	// Contains list of possible style sheets
		long bytes_transferred;			// Number of bytes in buffer
		long total_text_read;			// Total transferred to-moment
		style_info format;				// Style(s) and charcter format of text
		par_info par_format;			// Paragraph format(s) of the text
		font_info font;					// Font applied to this text
		pg_doc_info doc_info;			// General document information
		unsigned long flags;			// Attributes of last transfer
		pg_boolean format_changed;		// Set to TRUE - format has changed
		pg_boolean par_format_changed;	// Set to TRUE - para has changed
		pg_boolean font_changed;		// Set to TRUE - font has changed
		pg_boolean doc_info_changed;	// Set to TRUE if document info has changed

Imported text bytes are inserted into the `translator.data` `memory_ref` (using the appropriate OpenPaige Allocation Manager calls). The byte size returned is assumed to be `GetMemorySize(translator.data)`. Note, to implement special features, it is acceptable to return zero bytes for each call. Your function will be called repeatedly until you return FALSE.

For the best examples of `pgReadNextBlock()` consult the source code files for each import class.

**FUNCTION RESULT:** If there are no more bytes to transfer, return FALSE. Note that you can return FALSE even if the current function called transferred one or
more bytes, yet end-of-file comes after that position. A result of FALSE indicates that `pgReadNextBlock()` should not be called again.

`pgImportDone()` — Called when importing has completed. This function essentially balances `pgPrepareImport()`. Anything you allocated previously in `pgPepareImport()` should be disposed.

void PG_FAR * pgProcessEmbedData (memory_ref ref, } \\
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

NOTE: "Exporting", in many cases, is synonymous to "Save". We refer to the term "export" only to distinguish it from earlier methods of saving OpenPaige files (such as `pg` SaveDoc), but from an implementation viewpoint your application can respond to Save and Save As by "exporting" a file.

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

globals - A pointer to your OpenPaige globals. Custom control users: You can get a pointer to the OpenPaige globals as follows: (a) Get the `pg_ref` from the control by
sending a PG_GETPGREF message, and (b) Calling the OpenPaige API, "pgGetGlobals()".

fileref - If exporting to a disk file, this parameter must be a reference to the opened file (the "refNum" for Macintosh, or a file handle for Windows). If exporting to memory, fileref should be zero. Windows MFC users: The fileref parameter must be a "real" HFILE (or NULL if exporting to memory), not some other MFC-generated class member that you may assume is a file handle.

memory_unit — If exporting to a disk file, this parameter must be MEM_NULL. If exporting to memory, this must be a memory_ref of zero byte size (see "The Allocation Mgr” on page 25-1).

write proc — This is an optional I/O function to be used instead of the default lowlevel writing function. Refer to the OpenPaige Programmer's Guide for information about custom I/O functions. For writing to standard file from disk or memory, pass NULL for this parameter.

first_position — This value indicates the beginning file position to write. The first_position can be zero or some other byte offset into the file to begin writing.

FUNCTION RESULT: If this function is successful, zero is returned, otherwise an error code is returned.

4. Call the member function, "pgExportFile()":

pg_error pgExportFile (pg_ref pg, select_pair_ptr range, long export_flags, pg_boolean selection_only);

This exports the data from a `pg_ref` to the file (or memory_ref) specified in pgInitExportFile(). The parameters follow:

`pg` - The source document. Custom control users: obtain the `pg_ref` by sending a $P G \_G E T P G R E F$ message.
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

This function exports a file of type filetype into `pg`. The filetype parameter must be one of the following:

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
the appropriate member values before `pg` PrepareExport() is called.
pg_boolean pgWriteNextBlock() — Called to export (write) the next block of text. A "block of text" means a block of one or more characters that are rendered in the same consistent format.

For example, if the outgoing text contained "Bold_Plain_Italic", the export class must consider "Bold_", "Plain_" and "Italic_" as three separate blocks. The first time pgWriteNextBlock() gets called, the text "Bold_" would be provided; the next time "Plain_" is provided, and so forth.

The text and format information is placed in the "translator" member of the class; this member is a record as defined in the following example:

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-340.jpg?height=1177&width=1147&top_left_y=221&top_left_x=434)

Text byte(s) are available in translator.data; the byte size can be determined with GetMemorySize(translator.data).

For each consecutive call to pgWriteNextBlock(), if format_changed, par_format_changed or font_changed are TRUE then the text format, paragraph format or font is different than the last $\mathrm{p} g$ WriteNextBlock() call, respectively.

For the best examples of pgReadWriteBlock() consult the source code files for each import class.

FUNCTION RESULT: If TRUE is returned, `pg` WriteNextBlock() will get called again if there is any more text to export; if FALSE is returned, exporting is aborted.

$$
\text { pgExportDone() }
$$

Called after exporting has completed. This function essentially balances "pgPrepareExport()". Anything you allocated previously in pgPepareExport() should be disposed.

$$
\text { pg_error pgPrepareEmbedData() }
$$

Called to prepare embed_ref data to be exported. The purpose of this function is to make any data conversions necessary to provide a serialized, binary stream of data to be exported.

Upon entry, the refparameter is the embed_ref that is about to be exported. This function needs to return a pointer to byte stream to transfer and the byte count of the byte stream should be stored in *byte_count.

The local_storage parameter is a pointer to a long word; whatever is placed in *local_storage will be returned in pgReleaseEmbedData(), below. The purpose of this parameter is to provide a way for `pg` PrepareEmbedData() to "remember" certain variables required to un-initialize the embed_refdata (for example, *local_storage might be used to save a HANDLE that gets locked, hence it can be unlocked when pgReleaseEmbedData() is called).

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

NOTE: The `pg` g table record, generally used for defining table formats, also contains the definition for paragraph shading, if any. If table.table_columns is zero, border_shading is applied to the whole paragraph; if table.table_columns is nonzero then border_shading applies to default background shading of the table.

If border_shading is zero no shading is applied; otherwise, border_shading represents a "red-green-blue" component using the following bitwise fields:

## 0x00BBGGRR

The "BB" bits represent the blue component of the color, the "GG" bits represent the green component and "RR" represents the red component.

NOTE (Windows): These bits are identical to the bits in a COLORREF.
