# 1 Introduction

## 1.1 Purpose of this document

The purpose of this document is to provide initial programming information for the OpenPaige developers. Comments are welcome, as are useful example code submissions. Questions and answers from OpenPaige developers may be used in subsequent editions of the manual.

This function sets a new tab that applies to the specified selection.

## 1.2 How to use this manual

The OpenPaige technology is quite extensive, so we recommend that you do not simply dive into the middle of this manual and start implementing complex features.

Our advice is to implement this software by following these gradient steps:

1. Follow the information in chapter 2, "Up & Running". During this phase, ignore all other information in the manual.
2. Follow chapter 3, "Beyond the Defaults", which discusses implementation of additional, common features above and beyond the bare minimum covered in \#1 above.
3. If you need to implement virtual memory, do that by following chapter 4, "Virtual Memory".
4. Implement all remaining simple functionality not covered in \#1 or \#2 above, such as text formatting (fonts and styles), paragraph formatting (indents and justification) and possibly tab settings and color. See chapter 8, "Style Basics".
5. Depending on what you wish to accomplish with OpenPaige, find section(s) that deal with your particular requirements — we have tried to break down this manual into the most likely application requirements.

You should also consult the index to locate the topic(s) of interest quickly.

Generally, we have placed the parts of OpenPaige that most users will want and that are the most straight forward in the front. As you move to the back of the manual, the functionality will become more complex.

#### CAUTION
It is important to remember that no user will need the entire functionality. If you are contemplating a complex feature, or one in which you will need detailed knowledge of OpenPaige or working in the chapters toward the rear of the manual, please contact OpenPaige Tech Support via electronic mail for an evaluation and suggestions on how you can easily accomplish your goal. We can often suggest the easiest way to do something if we are consulted before you are buried in buggy code. Also knowing what you are doing and why you are doing it "that way" helps us to build better features.

## 1.3 Implementation Tips \& Hints
<!-- very out of date -->

- If you are a Word Solution Engine customer: the OpenPaige technology is very different than DataPak's Word Solution Engine. We therefore recommend strongly to "forget" all you know about Word Solution in order to understand the implementation of OpenPaige.
- Use the index to find small items, and Summary of Functions for quick-reference to function syntax.
- Consult the demo program. The OpenPaige package you received includes all the source files for the "demo" which contains a wealth of information and examples. If you think something does not work correctly, before reporting a bug or otherwise reach an impasse, consult that area of the demo against the way you have implemented the code. One of the first questions we will ask when you contact our Technical Support is, "Does it work correctly in the demo?"

#### NOTE (WINDOWS USERS)
If you are using the OpenPaige API directly, consult the source files in the Control directory (the "demo" simply uses the OpenPaige Custom Control; the Custom Control source files show how to access the API).

#### NOTE
You may contact our Technical Support service if the above suggestions fail to help. However, we do not accept any telephone support questions whatsoever. All questions must be submitted by email; we will always attempt to handle your questions as quickly and as thoroughly as possible. You can email your support questions to <!--support@OpenPaige.com-->.

## 1.4 Certain Conventions

Since OpenPaige is designed to be a multi-platform, multi-application processing editing library, we have had to make certain conventions in how the functions are described.

### "FAR" Pointers

Certain platforms require pointers which are outside the current segment to be designated as `far` pointers, such as Windows. Other platforms, such as the Macintosh do not require this. For the Macintosh, `PG_FAR` has been declared as nothing and these differences can be ignored.

### `pascal` keyword

The `pascal` keyword has been left out of the function definitions in this document; the actual header file(s) will contain that keyword. All external OpenPaige functions are declared using the Pascal calling conventions.

### Redefinition of types

To maintain compatibility across all platforms, certain new types have been declared as follows:

#### Unicode Version

| OpenPaige Type | Typedef'd From |
| :--- | :--- |
| `pg_short_t` | unsigned short |
| `pg_char` | unsigned short |
| `pg_char_ptr` | pointer to `pg_char` |
| `pg_bits8` | unsigned char |
| `pg_bits8_ptr` | pointer to `pg_bits8` |
| `pg_boolean` | short |
| `pg_error` | short (for error codes) |
| `memory_ref` | unsigned long |
| `PGSTR` | `pg_char_ptr` |

#### Non-Unicode Version

| OpenPaige Type | Typedef'd From |
| :--- | :--- |
| `pg_short_t` | unsigned short |
| `pg_char` | unsigned char |
| `pg_char_ptr` | pointer to `pg_char` |
| `pg_bits8` | (same as `pg_char`) |
| `pg_bits8_ptr` | (same as `pg_char_ptr`) |
| `pg_boolean` | short |
| `pg_error` | short (for error codes) |
| `memory_ref` | unsigned long |
| `PGSTR` | `pg_char_ptr` |

### NULL Reference

Frequent use of the term `M_NULL` exists throughout this manual. This is an OpenPaige macro that simply expands to (value of) zero. It is used for indicating a "null" for an OpenPaige memory reference.

### Machine Definitions

A single header file, `cpudefs.h` controls basic definitions for the platform in which the source files are intended.

## 1.5 Debug Mode

Windows users can ignore the "debug mode" libraries described below. This method of debugging applies only to Macintosh versions.

OpenPaige is compiled in both "debug" and "non debug" modes. Two sets of libraries are provided for this purpose.

When you use the "debug" libraries, you must also include `pgdebug.c` in your project. This lets you break into a source-level debugger to learn why OpenPaige is raising an exception. To use the OpenPaige debugger, open `pgdebug.c` and place a break point at the suggested spot (source comments indicate the spot).

If you break into the debugger, the message parameter is a `pascal` string.

Source code users: Debug mode is controlled by a single `#define` in `CPUDefs.h`, `#define PG_DEBUG`.

Debug mode slows the performance down substantially. It is recommended to use OpenPaige in debug mode during your development, but to turn it off for your final release, if for no other reason than increased performance.

# 2 Up & running

## 2.1 OpenPaige Custom Control

<!-- these documents are missing so this section is invalid -->

If you are using OpenPaige for a new application, or integrating OpenPaige for the first time, it would be wise to consider implementing the OpenPaige Custom Control. Documentation for this subset of OpenPaige is contained in a separate, smaller manual.

The Custom Control can potentially save you substantial amounts of development time, particularly to get "up and running" quickly. To make this decision, consider the following:

- Using the Control immediately eliminates the need to know very little — or any — of the detailed information in this Programmer's Guide.
- Most of the samples we provide use the Control (not the direct API).
- You can still call the OpenPaige API directly, when and if you need to.

If you decide to use the OpenPaige Custom Control, you do not need to read this manual any further! Immediately proceed to the OpenPaige Control manual; use this (larger) Programmer's Guide only when/if you need to call the API directly.

## 2.2 Bare necessities

This section provides the bare minimum code to get up and running with OpenPaige. This minimum functionality assumes one single default font and style, a single rectangle for display and word wrapping, no scrolling, nothing fancy.


#### CAUTION
Be sure to consult the release note and individual installation instructions included in each release. OpenPaige installation will change with versions and even interim releases. This makes checking the latest notes on the disk critical.

## 2.3 Libraries \& Headers

Regardless of whether you are a source code user or an object-code-only user, all source files in your application that call OpenPaige functions must include, at a minimum:

	#include "Paige.h"

As for the OpenPaige software itself, the minimum configuration is given below.

### Windows

The Windows version provides several library options; choose the appropriate libraries based upon the information provided below.

#### NOTE
Most libraries include the option between DLL(s) and static libraries.

#### Windows 3.1

Multilingual (will handle double-byte codes such as Kanji)

	(DLL Version Only)
	
	PGML16.DLL (Main OpenPaige)
	PGMLCT16.DLL (Custom control)
	
Non-Multilingual (no requirements for double-byte codes)

	DLL Libraries
	^^^^^^^^^^^^^
	PAIGE.DLL (Main OpenPaige)
	PGCNTL.DLL (Custom control)
	
	Static Libraries
	^^^^^^^^^^^^^^^^
	PG16LIB.LIB (Main OpenPaige)
	PGCTL16.LIB (Custom control)

#### Windows NT (XP, 10, 11)

##### Unicode

	DLL Libraries
	^^^^^^^^^^^^^
	PGUNICOD.DLL (Main OpenPaige)
	PGUNICTL.DLL (Custom control)
	
	Static Libraries
	^^^^^^^^^^^^^^^^
	PGUNILIB.LIB (Main OpenPaige)
	PGUNCTLB.LIB (Custom control)

##### Non-Unicode

	DLL Libraries
	^^^^^^^^^^^^^
	Paige32.DLL (Dynamic Linked Library for main (OpenPaige)
	Pgentl32.DLL (Dynamic Linked Library for custom control)
	
	Static Libraries
	^^^^^^^^^^^^^^^^
	PGLIB32.LIB (Main OpenPaige)
	PGCTLLIB.LIB (Custom control)

##### Multilingual

All versions for Windows 95 and NT are multilingual-compatible.

##### Borland Libraries (DLL libraries only)

	Single Thread
	^^^^^^^^^^^^^
	PAIGE32B.DLL (Main OpenPaige)
	PGCTL32B.DLL (Custom Control)

	Multithread
	^^^^^^^^^^^
	PG32BMT.DLL (Main OpenPaige)
	PGC32BMT.DLL (Custom control)

### Program Linking with DLL Libraries

When using any of the DLL libraries, add the file with the same name plus the ".LIB" extension. For example, if using `PAIGE.DLL` for the runtime library, add `PAIGE.LIB` to your project.

### Macintosh

### Macintosh Object Code Users

If you are using Think C or Metrowerks CodeWarrior, add all libraries to your project from the "Debug Libraries" OR "Runtime Libraries" folder (not both). Running in debug mode is suggested for general development, while non-debug is suggested for performance testing (for speed) and/or for final release of your product. Debug mode will reduce the program's performance substantially.

If you are using Metrowerks CodeWarrior, you must be sure to remove all previous versions of header files. Compiler complaints may be the result of CodeWarrior finding the incorrect header or object file.

The source code package includes "make" files for building OpenPaige libraries with MSVC++. If you need to create your own project file to build an OpenPaige library, the following information may prove useful:

1. Include `.C` files from the `pgsource` directory. None of them should be excluded.
2. Include `pgdebug` from the `pgdebug` directory. (**NOTE:** This file compiles to zero bytes of code unless `#define PG_DEBUG` is present in `CPUDEFS.H` [see “Compiler Options" below].)
3. Include the following `.C` files from `pgplatfo` regardless of the target platform: `pgio.c`, `pgmemmgr.c`, `pgosutl.c`, `pgscrap.c`.
4. Depending upon your target platform, include the following files from `pgplatfo`: `pgwin.c` and `pgdll.c` (the latter if compiling as a DLL) for Windows, and `pgmac.c` and `pgmacput.c` for Macintosh.
5. For **Windows 3.1** you may be asked to include a `.DEF` file. With MSVC 1.5x you can ask to generate a default `.DEF`, in which case you should choose to do so and rebuild.
6. The OpenPaige source code is not always friendly to certain C++ compilers due to `void*` type casting (or lack thereof). In most cases, you can work around this problem by compiling your project as straight C with an output for static or dynamically-linked library, then include that library in your main project. For Metrowerks CodeWarrior (Macintosh) you can work around this problem by turning OFF the option, "Invoke C++ Compiler".
7. To compile for Unicode, define `UNICODE` and `_UNICODE` in the preprocessor option(s). Do not define these constants in the header file(s) or you won't necessary achieve an accurate Unicode library.
8. If you compile for **Windows 3.1-Multilingual**, you must also include the following Windows library (for National Language Support): `OLENLS.LIB`.

### Compiler Options

All options for different target platforms and library types are controlled in `CPUDEFS.H`. Generally, only the first several lines in `CPUDEFS.H` need to be changed to compile for different platforms. The following guidelines should be followed:

#### Compiling for Windows 3.1

	#define WIN16_COMPILE (should be ON)
	#define WIN32_COMPILE (should be OFF)

#### Compiling for Windows 3.1-Multilingual (double-byte)

In addition to above:

	#define WIN_MULTILINGUAL (should be added to the file or preprocessor)

#### Compiling for Windows NT (7, 8, 10, 11)

	#define WIN16_COMPILE (should be OFF)
	#define WIN32_COMPILE (should be ON)

##### NOTE

There are other miscellaneous options that may imply a requirement to be enabled (by their names) such as `WIN95_COMPILE`. Do not turn these on, regardless of platform! Enable only `WIN32_COMPILE` for all 32-bit versions.

You do not need to define anything other than WIN32_COMPILE to support doublebyte multilingual editing for Windows NT and Windows 95; that support is generated automatically.

For Unicode, you must define `UNICODE` and `_UNICODE` in your preprocessor options of the compiler. (If no preprocessor option, `#define UNICODE` somewhere in your sources or headers to allow all system header files to recognize the Unicode option).

#### DLL versus Static Library (all platforms)

To compile as a DLL:

	#define CREATE_MS_DLL (should be ON)

If compiling as a static library or non-DLL:

	#define CREATE_MS_DLL (should be OFF)

#### Debug versus Runtime

OpenPaige has a built-in debugger which can be enabled by compiling with the following:

	#define PG_DEBUG (OpenPaige debugger compiles if ON)

When this is defined, all OpenPaige exceptions or debugging errors jump into the code in `pgdebug.c`.

##### NOTE
Compiling with `PG_DEBUG` will dramatically reduce the performance!

#### Special Resource (Macintosh only)

A special resource has been provided on your OpenPaige disc which the Macintosh-specific code within OpenPaige uses to initialize default character values (such as arrow keys, backspace characters, invisible symbols, etc.). You may copy and paste this resource into your application's resource and you may modify its contents if you want different defaults.

This resource is not required to use OpenPaige successfully. If it is missing, initialization simply sets a hard-coded set of defaults.

See also [Changing Globals](#3.8-changing-globals).

### 2.4 Software Startup

Some place early in your application you need to initialize the OpenPaige software; the recommended place to do so is after all other initializations have been performed for the main menu, Mac Toolbox, etc. To initialize, you need to reserve a couple blocks of memory that OpenPaige can use to store certain global variables (OpenPaige does not use any `globals` and therefore requires you to provide areas it can use to store required global structures).

To initialize OpenPaige you must call two functions in the order given:

	#include "Paige.h"
	(void) pgMemStartup (pgm_globals_ptr mem_globals, long max_memory);
	(void) pgInit (pg_globals_ptr globals, pgm_globals_ptr mem_globals);

Calling `pgMemStartup` initializes OpenPaige's allocation manager. This call must be made first before `pgInit`. The `mem_globals` parameter must be a pointer to an area of memory which you provide. The usual (and easiest) method of doing this is to define a
global variable that will not relocate or unload during the execution of your program, such as the following:

	pgm_globals 	memrsrv; // <--somewhere that will NOT unload

You do not need to initialize this structure to anything—`pgMemStartup` initializes this structure appropriately.

`max_memory` should contain the maximum amount of memory OpenPaige is allowed to use before purging memory allocations. If you want OpenPaige to have access to all available memory, pass 0 (RECOMMENDED) for `max_memory`.

For example, suppose you only wanted to use 200 kB of memory for all OpenPaige documents, combined. In this case, you would pass 200000 to pgInit. If you don't care, or want it to use all memory available, you would pass 0.

After `pgMemStartup`, call `pgInit`, which initializes every other part of OpenPaige.

`globals` is a pointer to an area of memory which you provide. The usual (and easiest) method of doing this is to define a global variable that will not relocate or unload during the execution of your program, such as the following:

	pg_globals	paigersrv; // <-- somewhere that will NOT unload

The structure `pg_globals` is defined in `paige.h` (and shown in [Changing Globals](#3.8-changing-globals)). You do not need to initialize this structure to anything—OpenPaige will initialize the globals structure as required. It is only necessary that you provide the space for this structure and pass a pointer to it in `pgInit`.

`mem_globals` parameter in `pgInit` must be a pointer to the same structure passed to `pgMemStartup`.

##### MFC NOTE

The best place to initialize OpenPaige in the constructor of the `CWinApp` derived class. Also the best place to put the OpenPaige globals and memory globals is in the CWinApp derived class.

##### EXAMPLE
	(.H)

	class MyWinApp : public CWinApp
	{
		...
		public:
		pgm_globals m_MemoryGlobals;
		pg_globals m_Globals;
		...
	}
	
	(.CPP)
	
	MyWinApp::MyWinApp()
	{
		pgMemStartup(&m_MemoryGlobals, 0);
		pgInit(&m_Globals, &m_MemoryGlobals);
		...
	}

#### TECH NOTE 

> `pgInit` crashes

It is possible to crash in `pgInit`. This is very rare however. Here are the main possibilities:

- A wrong library is linked in, i.e. version mismatch. (This includes all "updates" from compiler vendors who have changed the format of their object code libraries).
- It is called without calling `MemStartup`.
- You are out of memory. OpenPaige can require up to 60 kB to build itself and get ready to accept text.
- **Windows 3.1 platform only**: you are building a DLL with a memory model mismatch. The PAIGE DLL was built for large modal; try building your DLL the same.

### 2.5 OpenPaige Shutdown

For applications that require a `shutdown` of all allocations it has created, call the following functions, in the order shown, before terminating your application:

	(void) pgShutdown (pg_globals_ptr globals);
	(void) pgMemShutdown (pgm_globals_ptr mem_globals);

`globals` and `mem_globals` parameters must be pointers to the same structures given to `pgInit` and `pgMemStartup`, respectively. After `pgShutdown`, you must not call any OpenPaige functions (except for `pgInit`). After `pgMemShutdown`, all allocations placed in globals are de-allocated.

##### CAUTION

All `pg_ref`s and all memory references allocated anywhere by OpenPaige become invalid after `pgShutdown`, so make sure this is the very last OpenPaige function you call.

##### CAUTION (WINDOWS USERS)

Be sure to call both `pgShutdown` and `pgMemShutdown`, in that order, before `EXIT`, or you will have memory leaks and resources that are never released.

##### NOTE

`pgShutdown` and `pgMemShutdown` actually dispose every memory allocation made by OpenPaige since `pgMemStartup`; you therefore don't really need to dispose any `pg_ref`s, `shape_ref`s or other OpenPaige allocations.

##### NOTE (Macintosh)

The shutdown procedure is completely unnecessary if you will be doing an `ExitToShell` using the app version. Mac developers working with code resource libraries will still need to call `pgShutdown` and `pgMemShutdown`.

##### NOTE (MFC)

The best place to shutdown OpenPaige is in the destructor of the `CWinApp` derived class. Example:

	(.CPP)
	MyWinApp::~MyWinApp()
	{
		...
		pgShutdown(&m_Globals);
		pgMemShutdown(m_MemoryGlobals);
	}

##### NOTE (MFC)

For Microsoft Foundation Class applications, the appropriate method to shut down OpenPaige is to override `CxxAppxExitInstance()` and call `::pgShutdown` and `::pgMemShutdown`.

##### NOTE

You must not call either shutdown function if you are using the OpenPaige Control.

### 2.6 Creating an OpenPaige Object

By "OpenPaige object" is meant a single item that can edit, display and otherwise manipulate a block of text, large or small.

Calling `pgNew`, below, returns a reference of type `pg_ref`. This `pg_ref` can then be passed to all the other functions given in this manual.

	(pg_ref) pgNew (pg_globals_ptr globals, generic_var def_device, shape_ref vis_area, shape_ref page_area, shape_ref exclude_area, long attributes);

The above function returns a new `pg_ref`; the `pg_ref` can then be passed to other functions to insert text and edit text.

`globals` parameter must be a pointer to the same `pg_globals` structure you passed to `pgInit` at startup time.

Attributes are described in [Attribute Settings](#2.8-Attribute-Settings) and [Changing Attributes](#3.1-Changing-Attributes), but can be set here as well.

`def_device` parameter defines what graphics port this OpenPaige object should draw to by default; what is actually passed to `def_device` can slightly vary between platforms as follows:

#### Macintosh & PowerPC

If `def_device` is `NULL` then current `GrafPort` is used as the default device; if `def_device` is non-`NULL` and not "-1" it is assumed to be a `GrafPtr` and that port is used for subsequent drawing.

#### Windows (PC)

If `def_device` is `0L` then the current window of focus is used as the default window where drawing will occur (e.g., `GetFocus` is used to determine the window); if `def_device` is non-`NULL` and not `-1` it is assumed to be type `HWND` and that window is used for subsequent drawing.

This `HWND` in the def_device is NOT a Device Context.

Essentially, the `dev_device` should be the window (or child window) that is receiving the message to create the OpenPaige object, e.g. `WM_CREATE`.

##### CAUTION
If you pass `MEM_NULL` to `def_device`, OpenPaige will obtain the window of current focus. You should only use this method if your document window is known to be the window of focus, otherwise passing `MEM_NULL` can result in a crash.

#### Microsoft Foundation Classes (MFC)

The best place to put `pgNew()` is in the `OnCreate()` member of the `CView` derived class. It is important to call the `CView::OnCreate()` **before** calling `pgNew()`. Examples follow:

	(.H)
	class MyView : public CView
	{
	...
	public:
		pg_ref m_Paige;
	...
	}
	
	(.CPP)
	int MyView::OnCreate(LPCREATESTRUCT lpCreateStruct)
	{
		pgm_globals_ptr memory_globals = ((MyWinApp*)AfxGetApp())->m_MemoryGlobals;
		int return_value = 0;
		CRect client_rect;
		rectangle client_paige_rect;
		if(CView::OnCreate(lpCreateStruct == -1)
			return -1;
		ASSERT(m_hWnd);
		ASSERT(isWindow(m_hWind));
		
		// Non-OpenPaige initialization here!
		
	GetClientRect(&client_rect);
	RectToRectangle(&client_rect, &client_paige_rect);
	
	shape_ref window = pgRectToShape(AfxGetMemoryGlobals(), &rect);
	
	PG_TRY(AfxGetApp()->m_MemoryGlobals // See Chapter 19 of the OpenPaige manual.
	{
		m_Paige = pgNew(AfxGetApp()->m_Globals,(generic_var)(LPVOID)m_hWnd, window, window, MEM_NULL, 0);
	};
	
	PG_CATCH
	{
		return_value = -1;
	};
	
		PG_ENDTRY;
	
		pgDisposeShape(window);
	
		return return_value;
	}
		
#### All Platforms

If `def_device` is `-1` then no device is assumed (which implies you will not be drawing anything and/or will specify a drawing port later). If you need to pass `-1` for the `def_device` parameter, you can use the following predefined macro:

	#define USE_NO_DEVICE (generic_var) -1 // pgnew is with no device

If `def_device` is neither `-1` nor a null pointer it is assumed to be an OpenPaige drawing port to be used for the default (see `graf_device`, `pgSetDefaultDevice`).

For "Up & Running", pass a null pointer for `def_device` (for Macintosh and PowerPC) or the `HWND` associated with the current message for Windows-PC.

Parameters `vis_area`, `page_area` and `exclude_area` define the literal shapes for which text will display, wrap and jump over, respectively. Each of these define how the text will appear within the OpenPaige object as follows:

`vis_area` defines the visible area that shows text, or the "hole" in which it displays. This area may be physically smaller than the document containing the text; any physical area of the screen that is outside the boundary of `vis_area` will *clip* (mask) the text from view.

`page_area` defines the container in which text will wrap and flow. It is referred to as the `page` area since it literally defines the `page` size of your document. The width of `page_area` also defines the boundaries for which text must wrap. The `page_area` can be any size, larger or smaller than `vis_area`.

`exclude_area` is an optional shape which defines an area or areas in which text must avoid. In other words, if a line of text were to intersect any part of the `exclude_area`, it must jump over that area in some way to avoid it.

For `pgNew`, you can pass `MEM_NULL` for `exclude_area`, but you must pass a valid `shape_ref` for `vis_area` and `page_area`.

See "Up \& Running Shapes" on page 2-26 to create a `shape_ref`.

`attribute`s can contain different bit settings which define specific characteristics for the OpenPaige object. For the purpose of getting "Up \& Running" quickly, pass 0 for this parameter (or see “Changing Attributes” on page 3-1).

The initial font and text format used by the `pg_ref` returned from `pgNew` will be taken from `pg_globals`. To change what font, style or paragraph format that a new `pg_ref` assumes, set the appropriate information in `pg_globals` after calling `pgNew`.

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-027.jpg?height=571&width=653&top_left_y=494&top_left_x=439)

vis_area is where text "shows through". Text may wrap or flow beyond this area but it is clipped during display

page_area defines where text flows and wraps. At any time, text outside

vis_area is clipped

exclude_area is optional. If used, text avoids the shape, hopping across the area

#### MEM_NULL Definition

The value `MEM_NULL` is a defined value in OpenPaige header files that you should use to imply a "null" `shape_ref` or `memory_ref`—see "The Allocation Mgr" on page 25-1.

##### Error checking `pgNew` 

OpenPaige provides excellent error checking for `pgNew`. See "Exception Handling" on page 26-1.

### 2.7 Up \& Running Shapes

To avoid a lengthy discussion at this time regarding OpenPaige shapes, we will assume at this time you wish to display text within a simple rectangle (as opposed to some other non-rectangular shape or multiple "container" rectangles).

#### Creating a shape using rectangle

The easiest way to create a new shape is to use the following function:

	(shape_ref) pgRectToShape (pgm_globals_ptr mem_globals, rectangle_ptr rect);

This returns a new `shape_ref` (which can be passed to one of the `area` parameters in `pgNew`). The globals parameter must be a pointer to the same structure given in `pgInit` and `pgNew`.

The *rect* parameter is a pointer to a structure consisting of a top-left and bottom-right coordinate that encloses a rectangle. The coordinate and rectangle definitions are as follows:

	typedef struct
	{
		long	v;	// vertical position
		long	h;	// horizontal position
	}
	co_ordinate;
	
	typedef struct
	{
		co_ordinate	top_left;	// Top-left of rect
		co_ordinate	bot_right;	// Bottom-right of rect
	}
	rectangle, *rectangle_ptr;

Hence, if you set a rectangle to the desired dimensions and pass a pointer to that rectangle in `pgRectToShape`, a new memory reference is returned which contains a shape of that rectangle.

##### NOTE
The reason `pgNew` requires a `shape_ref` instead of rectangles is that an OpenPaige object can have non-rectangular shapes for any of its three areas.

For further information regarding shapes, particularly non-rectangular shapes, see "All About Shapes" on page 12-1.

#### Disposing a Shape

The `pgNew` function makes a copy of the shape you pass to its parameters. Once you have received a new `pg_ref` you can dispose the shape. To do so, call:

	void pgDisposeShape (shape_ref the_shape);

#### Rect to Rectangle

Two utilities exist that make it easier to create OpenPaige rectangles:

	\#include "pgTraps.h"
	(void) RectToRectangle (Rect PG_FAR *r, rectangle_ptr pg_rect);
	(void) RectantleToRect (rectangle_ptr pg_rect, co_ordinate_ptr offset, Rect PG_FAR *r);

`RectToRectangle` converts `Rect r` to rectangle `pg_rect`. The `pg_rect` parameter must be a pointer to a rectangle variable you have declared in your code.

`RectangleToRect` converts `pg_rect` to `r`. Also, if offset is non-null, the resulting `Rect` is offset by the amounts of the `co_ordinate` (for example, if `offset.h` and `offset.v` were 10, -5 the resulting `Mac Rect` would be the values in `pg_rect` with left and right amounts offset by 10 and top and bottom amounts offset by -5.

##### NOTE (Windows)
Type `Rect` is identical to type `RECT`, and both can be used interchangeably.

##### NOTE (Macintosh)
Since a Mac Rect has a ±32K limit for all four sides, OpenPaige rectangle sides larger than 32K will be intentionally truncated to about 30K.

#### About Windows, Graphic Ports and Origins

Although OpenPaige is designed to be platform-independent, is does assume a target graphics device that all drawing is transferred to.

**When a `pg_ref` is created, the default target device is set to whatever is appropriate for the running platform.** For Macintosh, the default device is the current `GrafPort` set when `pgNew` is called.

##### NOTE (Word Solution Engine for Macintosh)

Unlike WSE, OpenPaige "remembers" what port it should draw to and all subsequent drawing will occur in that port unless you specifically override it.

For the purpose of getting "Up & Running", just make sure you create your window first and have it set as the current port before calling `pgNew`. In subsequent sections, we will provide different ways to change the target port.

#### Origins

OpenPaige does not care what a window's origin is set to (top-left `co_ordinate` values). OpenPaige only cares about the `area` parameters you provide for `pgNew`; remember, OpenPaige doesn't really know what a window is and doesn't know anything about origins. OpenPaige simply and only follows the coördinates you have set for `vis_area`, `page_area` and `exclude_area`. If your `page_area` shape passed to `pgNew`, for instance, had a top-left of -10000,-9999, the first character of the first line will be drawn at that coördinate location regardless of where the top-left of your window might physically exist. In other words, OpenPaige coordinates are always relative to the associated window's coördinates.

### 2.8 Attribute Settings

As mentioned earlier, `pgNew` will accept certain characteristics defined in the "attributes" parameter. The current version supports the following:

	#define NO_WRAP_BIT			0x00000001	// Wraps only on <CR> or <LF>
	#define NO_LF_BIT			0x00000002	// Do not add font
	#define NO_DEFAULT_LEADING	0x00000004	// Do not add font leading
	#define NO_EDIT_BIT			0x00000008	// No editing (display only)
	#define EXTERNAL_SCROLL_BIT	0x00000010	// App controls scrolling
	#define COUNT_LINES_BIT		0x00000020	// Keep track of line/para count
	#define NO_HIDDEN_TEXT_BIT	0x00000040	// Do not display hidden text
	#define SHOW_INVIS_CHAR_BIT	0x00000080	// Show control characters
	#define SMART_QUOTES_BIT	0x00000800	// Do "smart quotes"
	#define NO_SMART_CUT_BIT	0x00001000	// Do not do "rt cut/paste"
	#define NO_SOFT_HYPHEN_BIT	0x00002000	// Ignore soft hyphens
	#define NO_DUAL_CARET_BIT	0x00004000	// Do not show dual carets
	#define SCALE_VIS_BIT		0x00008000	// Scale vis_area when scaling
	#define BITMAP_ERASE_BIT	0x00010000	// Erase page(s) with bitmap drawing
	#define TABS_ARE_WIDTHS_BIT	0x10000000	// Fixed-width tab characters
	#define LINE_EDITOR_BIT		0x40000000	// Document is line editor mode

`NO_WRAP_BIT` turns off word wrapping (which means a line of text will continue horizontally until a carriage-return or line-feed character is encountered).

`NO_LF_BIT` causes OpenPaige to ignore line-feed characters. The usual purpose of this setting is for imported text that contains both CR and LF at the end of every line; setting the `NO_LF_BIT` attribute will cause LF characters to be invisible and have no effect of any kind.

`NO_DEFAULT_LEADING` prevents any extra leading reported by the system for font attributes. In Windows, *extra leading* is the external leading value reported by `GetTextMetrics`; in Macintosh, it is the leading value reported by `GetFontInfo`. By default, OpenPaige adds the extra leading to every line unless this attribute is set.

`NO_EDIT_BIT` disables editing. In effect, if `NO_EDIT_BIT` is set, the "caret" will not blink and the user can't insert characters.

`EXTERNAL_SCROLL_BIT` tells OpenPaige that your application will control all scrolling. (This fairly complex subject is discussed elsewhere.)

`COUNT_LINES_BIT` tells OpenPaige to keep track of line and paragraph numbers, in which case you can use the line and paragraph numbering features in OpenPaige (see "Line and Paragraph Numbering"<!--on page 24-16-->). Please note that constantly counting lines and paragraphs, particularly if the document is large and contains wordwrapping with style changes, can consume considerable processing time. Hence, COUNT_LINES_BIT has been provided to enable/disable this feature.

`NO_HIDDEN_TEXT_BIT` suppresses the display of all text that is "hidden" (OpenPaige will accept a hidden text attribute as a style). If this bit is not set, hidden text is displayed with a grey strike-through line; if it is set, the text is completely invisible and ignored for line width computations.

`SHOW_INVIS_CHAR_BIT` causes all invisible characters (control codes such as CR and LF) to be displayed using special character symbols. These symbols are defined in `pg_globals` (see “Changing Globals”<!--on page 3-21-->).

`EX_DIMENSION_BIT` tells OpenPaige to include the exclusion area as part of the "document height".

`NO_WINDOW_VIS_BIT` – Do not respect window's clipped area.

`SMART_QUOTES_BIT` – Do "smart quotes" (curly quotation marks).

`NO_SMART_CUT_BIT` – Do not do "smart cut/paste"

`NO_SOFT_HYPHEN_BIT` – Ignore soft hyphens

`NO_DUAL_CARET_BIT` – Do not show dual carets

`SCALE_VIS_BIT` tells OpenPaige to scale the `vis_area` along with the text when scaling has been enabled. By default, the `vis_area` is left alone when an OpenPaige document is scaled, leaving the text "behind" the visual boundaries reduced or enlarged. In certain cases—particularly when employing multiple `pg_ref`s into the same document as "edit boxes"—you need this attribute set; for single `pg_ref` documents that fill all or most of the window, you generally do not want this attribute set.

`BITMAP_ERASE_BIT` tells OpenPaige to erase area(s) on the page using offsetting bitmap drawing, otherwise the same portions of the screen are erased directly. The purpose of this attribute is to draw "background" graphics in the window when/if OpenPaige needs to erase the screen.

`TABS_ARE_WIDTHS_BIT` causes all <TAB> characters to display as no more or less than "wide" blanks. For example, if this attribute is not set, a <TAB> character aligns the character(s) that follow to the next logical tab stop; if this attribute is set, the a tab character is simply a fixed-width space (the default tab spacing per OpenPaige globals).

`LINE_EDITOR_BIT` tells OpenPaige that you intend to maintain the document as a "line editor", defined as one where words will not wrap and all lines remain the same height. If OpenPaige knows this in advance, it can bypass the usual "pagination" functions and you can achieve substantially increased performance for line editors.

##### NOTE
If you set `LINE_EDITOR_BIT` you must not set any attributes to wrap the text, nor should you vary the point size(s) or attempt any irregular page shapes or page breaks. You can still produce multi-styled text as long as the text height(s) are consistently the same.

Any (or all) of the above settings can exist at once.

##### NOTE
You can always change these attributes after an OpenPaige object is created (see section 3.1, "Changing Attributes").

#### Example – `pgNew`

	/* This creates a new OpenPaige object */
	#include <Paige.h>
	#include "pgTraps.h"
	extern pg_globals paige_rsrv;
	
	/* Routine: Open_Window */
	/* Purpose: Open our window */
	/* Note: the window has already been made and will be shown and 
		selected immediately after this function */
		
	void Open_Window(WindowPtr win_ptr)
	{
		if (win_ptr!=nil) /* See if opened OK */
		{
		pg_ref result;
		shape_ref vis, wrap;
		rectangle rect;
		
		/* this sets vis_area and wrap_area to the shape of the window 
		itself */
			
		RectToRectangle(win_ptr->portRect, &rect);
		vis = pgRectToShape(&paige_rsrv, &rect);
		wrap = pgRectToShape(&paige_rsrv, &rect);
		result = pgNew(&paige_rsrv, NULL, vis, wrap, NULL, EX_DIMENSION_BIT);
		}		/* End of IF */
	}

### 2.9 Disposing an OpenPaige Object

Once you are completely through with a `pg_ref` (e.g., user closes the window), dispose it with:

	(void) pgDispose (pg_ref pg);

This function disposes all data structures within `pg`; the `pg_ref` will no longer be valid.

Be certain you have not shut down the OpenPaige library before disposing a `pg_ref`, or you will crash.

##### NOTE (Microsoft Foundation Classes)

The best place to destroy the OpenPaige object is in the OnDestroy() member of your `CView` derived class. Example:

	(.CPP)
	
	void PGView::OnDestroy()
	{
		pgDispose(m_Paige);
		CView::OnDestroy();
	}

### 2.10 Getting the "Globals" Pointer

If you need to obtain the pointer to `pg_globals` (originally given to `pgInit` and to `pgNew`), you can get it from a `pg_ref` using the following:

	(pg_globals_ptr) pgGetGlobals (pg_ref pg);

The typical use for `pgGetGlobals` is to obtain a pointer to `pgGlobals` in places where the original global structure, given to `pg_init`, is not easily accessible.

**FUNCTION RESULT:** This function returns the globals pointer as saved in pg.

To change globals, see section 3.8, "Changing Globals".

## 2.11 Displaying

To draw the text in a `pg_ref` to a window, use the following function:

	(void) pgDisplay (pg_ref pg, graf_device_ptr target_device, shape_ref vis_target, shape_ref wrap_target, co_ordinate_ptr offset_extra, short draw_mode);

The `pg_ref`'s contents are drawn to the `target_device`. If, however, you pass a null pointer to `target_device` the text will be drawn to the default device set during `pgNew`. (For the purposes of getting "Up \& Running", we will assume you want to draw to the default device, which will typically be a window that was created prior to `pgNew`, so pass a null pointer).

`vis_target` and `wrap_target` parameters are optional shapes which will temporarily redefine the OpenPaige object's `vis_area` and `wrap_area`, respectively. Using these two parameters, you can temporarily control and/or change the way an OpenPaige object will display. Text gets clipped to `vis_target`, or, if `vis_target` is a null pointer, to the original `vis_area`, and text will wrap within `wrap_target`, or, if `wrap_target` is `MEM_NULL`, within the original `wrap_area`. (For the purposes of getting "Up \& Running", pass MEM_NULL for these two parameters.)

If `offset_extra` is `non-null`, all drawing is offset by the amounts in that coördinate (all text is offset horizontally by `offset_extra->h` and vertically by `offset_extra->v`. If `offset_extra` is a null pointer, no extra offset is added to the text.

The `draw_mode` parameter defines the way text should be transferred to the target device. The `draw_mode` selections are shown below.

See "Display Proc"<!-- on page 16-16--> about how to add ornaments to the text display.

##### NOTE

You do not need to specify any drawing device for `pgDisplay` if you intend to display in the window given to `pgNew`. In this case, just pass NULL to the `target_device` parameter.

If for some reason you need to redirect the display to some other window or device (such as a bitmap), you can create a `graf_device` record for that purpose and pass a pointer to that structure for the `target_device`.

Creating a `graf_device` for this purpose is the same as the `graf_device` record used for `pgPrintToPage`. See "Printing in Windows"<!-- on page 16-9-->.

#### Draw Modes

	typedef enum
	{
		draw_none,			// Do not draw at all
		best_way,			// Use most efficient method(s)
		direct_copy,		// Directly to screen, overwrite
		direct_or,			// Directly to screen, "OR"
		direct_xor,			// Directly to screen, "XOR"
		bits_copy,			// Copy offscreen
		bits_or,			// Copy offscreen in "OR" mode
		bits_xor,			// Copy offscreen in "XOR" mode
		bits_emulate_copy	// Copy "fake" offscreen
		bits_emulate_or	// "Fake" offscreen in "OR" mode
		bits_emulate_xor	// "Fake" offscreen in "XOR" mode
	};

#### "Bits-emulate" Mode

The drawing modes `bits_emulate_copy`, `bits_emulate_or`, and `bits_emulate_xor` are identical to `bits_copy`, `bits_or` and `bits_xor` save that no bitmaps are used and the drawing is directly to the screen. Unlike the non-bitmap drawing modes, however, OpenPaige's standard callback hooks are called to allow modification to its "bitmap", which in this case is the direct screen. Bitmap modification is typically used to render background images, patterns, and other forms of graphics.

##### NOTE
Unless you need to create a special or unusual effect, always pass `direct_or` or `bits_emulate_or` when responding to `WM_PAINT` (Windows) or an update
event (Macintosh), and `best_way` for all other functions requiring a `draw_mode`.

![](https://cdn.mathpix.com/cropped/2024_04_30_f87dba95664e91f9803eg-039.jpg?height=679&width=1309&top_left_y=242&top_left_x=417)

Additional draw modes require the developer to use the custom draw hook and draw his own. See "`text_draw_proc`"<!-- on page 27-17--> for information on how to do custom drawing.

A value of `draw_none` will disable all drawing and visual scrolling. In other words, if the OpenPaige document changes in some way, nothing would change on the screen until the application re-displayed the OpenPaige text contents. The "draw nothing" feature is used only for special cases where an application wants to change without drawing anything yet.

##### Responding to `WM_PAINT` Event (Windows)

	{
	PAINTSTRUCT ps;
	BeginPaint(hWnd, &ps);
	pgDisplay(pg, NULL, MEM_NULL, MEM_NULL, NULL, direct_or);
	}
	EndPaint(hWnd, &ps);

To display the OpenPaige object in MFC, use `OnPaint()`. Do not try to use `OnDraw()` or it will not draw correctly.

## EXAMPLE:

	(.CPP)
	
	void PGView::OnPaint()
	{
	CWnd::OnPaint();
	
	// If you don't use the OnEraseBkgnd() member of the MFC class, 
	// you must erase the background of the window first.
	
	pgDisplay(m_Paige, NULL, MEM_NULL, MEM_NULL, NULL, bits_emulate_or);
	}

### 2.12 Key Insertion

OpenPaige actually makes very little distinction between keyboard entry and any other text insertion, and in both cases the following function is used:

	(pg_boolean) pgInsert (pg_ref pg, pg_char_ptr data, long length, long position, short insert_mode, short modifiers, short draw_mode);

This function will insert length bytes pointed to by data. The insertion will occur at byte offset position if it is positive or zero; if position is `CURRENT_POSITION` (a `#define`d constant of -1), the insertion occurs at the current insertion point.

The `insert_mode` parameter defines the type of data being inserted, which can be any of the following:

	typedef enum
	{
		key_insert_mode,		// Typing insertion
		key_buffer_mode,		// Typing-buffer insertion
		data_insert_mode,		// Raw data insertion
	}

For keyboard entry, pass `key_insert_mode` or `key_buffer_mode`; for any other data insertion, pass `data_insert_mode`.

The difference between the two "key" insert modes and `data_insert_mode` is that a key insertion can contain special controls such as arrow keys and backspace (delete). For `data_insert_mode`, the bytes will be inserted as is.

If `key_insert_mode` is used, the new character(s) will draw immediately if `draw_mode` is nonzero.

If `key_buffer_mode` is used, character(s) will be buffered (temporarily saved) and drawn later by OpenPaige; the purpose of this mode is to avoid "getting ahead" of keyboard entry on complex document entry. It is also useful for Macintosh double-byte script entry, in which the text is entered all at once from a floating palette window.

##### NOTE (Windows)

The `key_buffer_mode` is usually meaningless in the Windows environment; instead, you should always use `key_insert_mode` when processing keyboard characters. Using `key_buffer_mode` (where chars are stored and inserted later) requires a call to `pgIdle` which, under the Windows messaging system, would require you to set up a "timer" message that occurs every few milliseconds, which is probably not implemented in most applications.

If keys are buffered, OpenPaige will display the new text during the first `pgIdle` function call (see “Blinking Carets \& Mouse Selections”<!-- on page 2-48-->).

##### NOTE
"Arrows" and other control codes are defined (and changeable) in the `pg_globals` record (see "Changing Globals"<!-- on page 3-21-->); these special controls will be processed correctly for `key_insert_mode` and `key_buffer_mode` only.

The `modifiers` parameter can change the way the `pg_ref` will respond to special control characters for `key_insert_mode` (`modifiers` is ignored for the other insertion modes). In the current version, the following value is supported:

	#define EXTEND_MOD_BIT 0x0001	// Extend the selection

If `modifiers` is `EXTEND_MOD_BIT`, the selection range is extended if an arrow key is "inserted." Other selection modifier bits are explained in "Modifiers"<!-- on page 2-51-->.

The `draw_mode` for `pgInsert` performs identically to `pgDisplay` and can be any of the verbs defined for drawing. If you just want to insert but not display, pass `draw_none` for `draw_mode`. If `key_buffer_mode` is used for insertion, the `draw_mode` is saved and used later when the text is displayed.

For keyboard insertions, the recommended `draw_mode` is `best_way`.

##### CAUTION (Macintosh)
> Mac developers should not confuse these modifier bits with the modifiers given in the event record. There is no similarity. The modifiers shown here are the ones OpenPaige supports.

##### NOTE
The insertion will assume either the text format of the current insertion point OR the format of the last style/font/format change, whichever is more recent. This is true even if you specify an insert position other than the current point. If you want to force the insertion to be a particular font or style, simply call the appropriate function to change the text format prior to your insertion.

##### FUNCTION RESULT
The function returns TRUE if the text and/or highlighting in `pg` changed in any way. Note that no change occurs only if `key_buffer_mode` is passed as the insert mode, in which case the characters are stored and not drawn until the next call to `pgIdle`. Another situation that will not change anything visually is passing `draw_none` as the `draw_mode`. In both cases, `pgInsert` would return FALSE. The purpose of this function result is for the application to know whether or not it should update scrollbar values or scroll to the insertion point, etc. (i.e., it is a waste of processing time to check or change scroll positions if nothing changed on the screen).

#### Running Unicode

If you are using the Unicode-enabled OpenPaige library, the "data" to be inserted is expected to be one or more 16-bit characters. The data size in this case is assumed to be a character count (not a byte count). This is due to the fact that if UNICODE is defined in your preprocessor or header files (which it should be for a true Unicode-enabled application), a `pg_char_ptr` changes from a byte pointer to a 16-bit character pointer.

For example, to insert the Unicode value 0x0041 (letter "A") you would pass the value of 1 in the `length` parameter even though the character size is technically 2 bytes long.

#### TECH NOTE: Insert Positions

The specified insertion position is a zero-relative byte offset. Note that this is a byte—not a "character" offset (characters in OpenPaige can be more than one byte), rather a byte offset from the beginning of all text in `pg`, starting at zero.

**EXCEPTION:** The pure Unicode version measures everything as 16 bit characters. Hence, the insertion point in this case is a character position.

If one or more characters are currently selected (selection range ≥ one character), those characters are deleted before the insertion occurs. Note that if the specified insertion position were `CURRENT_POSITION`, the insertion will occur to the immediate left of the previously selected text (which will have been deleted).

After the insertion, the new insertion position in `pg` is advanced to length `bytes` from the original specified position. Example: If 100 bytes were inserted at text position 500 when `pgInsert` returns the current insertion position will be 600.

##### APPLIED STYLE(S) AND INSERTION

If `pgInsert` occurs at the current insertion point, whatever the last style and/or font that was applied to that insertion point will be applied to the next insertion.

For example, suppose all text in `pg` is currently "Helvetica" font, and `pg` has a single insertion point (not a selected range of characters). Before inserting new text, a call is made to `pgSetFontInfo` with "Times Roman" font; the very next subsequent `pgInsert` would apply Times Roman—not Helvetica—to the new text.

However, if the insertion occurs somewhere other than the current insertion, the font/style that is applied will be whatever font/style applies to that position in text.

Hence, to implement the insertion of specific, multi-stylized text, the logic to perform should be as follows:

	pgSetStyleInfo(...) - and/or - pgSetFontInfo(...); 
	pgInsert(..., CURRENT_POSITION,...); 
	pgSetStyleInfo(...) - and/or - pgSetFontInfo(...); 
	pgInsert(..., CURRENT_POSITION,...); etc.

**NOTE:** For repetitive insertions, the insertion point will automatically advance the number of bytes you insert, so normally you should not need to set a new position if you are doing repetitive, sequential insertions.

**WARNING:** If you need to apply a specific font or style to a text insertion (such as in the logic above), do not set the insertion point after you set the style/font or that style/font attribute may be lost. If you must set position, do so BEFORE calling pgSetFont/nfo or pgSetStyleInfo.

##### EXAMPLE 

WRONG WAY:

	pgSetStyleInfo(...);
	pgSetSelection(pg, 0, 0); //<-- previous style setting is lost!
	pgInsert(...);

RIGHT WAY:

	pgSetSelection(pg, 0, 0);
	pgSetStyleInfo(...); // <-- Style gets applied to next insertion 
	pgInsert(...);

#### TECH NOTE: Nothing happens

> Nothing seems to happen when I insert text.

If you are doing inserts with `key_insert_mode`, OpenPaige won't do anything if the `pg_ref` is deactivated. That might be the problem. If so, you need to use `data_insert_mode`, not `key_insert_mode`, and it will then work; `pgInsert` does nothing.

### 2.13 Keyboard Editing with MFC (Windows)

To get Up and Running with basic keyboard editing you must add the following code to your MFC view class:

	(.H) 
	// Declare the following private variables. 
	short m_KeyModifiers;
	
	(.CPP)
	
	// Respond to the windows message WM_KEYDOWN... 
	void PGView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
	pg_globals globals = ((MyWinApp*)AfxGetApp())->m_Globals;
	pg_short_t verb;
	
		switch(nChar)
		{
			case VK_SHIFT:
				m_KeyModifiers != EXTEND_MOD_BIT;
				break;
			case VK_CONTROL:
				m_KeyModifiers != CONTROL_MOD_BIT;
				break;
			case VK_LEFT:
				SendMessage(WM_CHAR, globals->left_arrow_char);
				break;
			case VK_UP:
				SendMessage(WM_CHAR, globals->up_arrow_char);
				break;
			case VK_RIGHT:
				SendMessage(WM_CHAR, globals->right_arrow_char);
				break;
			case VK_DOWN:
				SendMessage(WM_CHAR, globals->down_arrow_char);
				break;
			case VK_HOME:
				verb = begin_line_caret;
				if(m_KeyModifiers & CONTROL_MOD_BIT)
					verb = home_caret;
				if(m_KeyModifiers & EXTEND_MOD_BIT)
					verb != EXTEND_CARET_FLAG;
				pgSetCaretPosition(mPaige, verb, TRUE);
				pgScrollToView(m_Paige, CURRENT_POSITION, 0, 0, TRUE, bits_emulate_or;
				break;
			case VK_END:
				verb = end_line_caret;
				if(m_KeyModifiers & CONTROL_MOD_BIT)
					verb = doc_bottom_caret;
				if(m_KeyModifiers & EXTEND_MOD_BIT)
					verb != EXTEND_CARET_FLAG;
				pgSetCaretPosition(m_Paige, verb, TRUE);
				pgScrollToView(m_Paige, CURRENT_POSITION, 0, 0, TRUE, bits_emulate_or;
				break;
			case VK_PRIOR:
				SendMessage(WM_VSCROLL, SB_PAGEUP);
				break;
			case VK_DELETE:
				if(m_KeyModifiers & EXTEND_MOD_BIT)
				{
					long start, end;
					pg_ref scrap;
					pgGetSelection(m_Paige, &start, &end);
					if(start == end)
						return;
					scrap = pgCut(m_Paige, &start, &end);
					assert(scrap);
					OpenClipboard();
					pgPutScrap(scrap, 0, pg_void_scrap);
					CloseClipboard();
					pgDispose(scrap);
					scrap = MEM_NULL;
					SetChanged();
				}
				else
				{
					SendMessage(WM_CHAR, globals->fwd_delete_char);
				}
			case VK_NEXT:
				SendMessage(WM_VSCROLL, SB_PAGEDOWN);
				break;
				pgScrollToView(m_Paige, CURRENT_POSITION, 0, 0, TRUE, bits_emulate_or);
				break;
		}
	}
				break;
			case VK_INSERT:
			{
			if(m_Key_Modifiers & CONTROL_MOD_BIT
				{
					long start, end;
					pg_ref scrap;
					pgGetSelection(m_Paige, &start, &end);
					if(start == end)
						return;
					scrap = pgCopy(m_Paige, NULL);
					assert(scrap);
					OpenClipboard();
					pgPutScrap(scrap, 0, pg_void_scrap);
					CloseClipboard();
					pgDispose(scrap);
				}
			else if(m_KeyModifiers & EXTEND_MOD_BIT)
				{
					pg_ref scrap = MEM_NULL;
					OpenClipboard();
					scrap = pgGetScrap(globals, 0, HookEmbedProc);
					CloseClipboard();
					if scrap
					{
						pgPaste(m_Paige, scrap, CURRENT_POSITION, false, best_way);
						pgDispose(scrap)
					}
				}
			}
			pgScrollToView(m_Paige, CURRENT_POSITION, 0, 0, TRUE, bits_emulate_or);
			break;
		}
	}
	
	// Respond to the Windows message WM_KEYUP...
	void MyView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		switch(nChar)
		{
			case VK_SHIFT:
				m_KeyModifiers &= (~EXTEND_MOD_BIT);
				break;
			case VK_CONTROL:
				m_KeyModifiers &= (~CONTROL_MOD_BIT);
				break;
		}
	}
	
	// Respond to the Windows message WM_CHAR...
	void MyView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		pg_char the_char = (pg_char)nChar;
		pgInsert(mPaige, &the_char, 1, CURRENT_POSITION, key_insert_mode, m_KeyModifiers, best_way);
		pgScrollToView(m_Paige, CURRENT_POSITION, 0, 0, TRUE, bits_emulate_or);
	}

### 2.14 Pending Buffer Insertions

As mentioned in `pgInsert`, if `key_buffer` mode is used, the characters get stored in an internal buffer and get inserted during the next `pgIdle`.

There might be an occassion, however, that requires immediate insertion of anything pending in this buffer. To do so, call the following:

	(pg_boolean) pgInsertPendingKeys (pg_ref pg);

Calling this function will immediately "empty" any pending characters, inserting and displaying them as appropriate. If there aren't any pending characters, `pgInsertPendingKeys` does nothing. The function returns TRUE if one or more characters were inserted.

##### NOTE

The display mode used when OpenPaige displays the pending buffer will be the original display mode passed to ``pgInsert``.

### 2.15 Blinking Carets \& Mouse Selections

#### Caret blinking (Macintosh only)

To cause the "caret" to blink in a `pg_ref`, call the following as often as possible:

	(pg_boolean) pgIdle (pg_ref pg);

##### NOTE (Macintosh): 

`pgIdle` should be called repeatedly while you are waiting for an event.

The `pg` parameter must be a valid `pg_ref` (can not be a null pointer).

**FUNCTION RESULT:** The function returns TRUE if character(s) were inserted and displayed that were stored previously from `pgInsert` calls with `key_buffer_mode`. This will only happen if you had called `pgInsert`, passing `key_buffer_mode` as the data transfer parameter. A result of TRUE or FALSE from `pgIdle` can help your application know whether or not it should update scrollbar values (since new text has been inserted). For Windows `key_buffer_mode` is not usually necessary, see “Key Insertion”<!-- on page 2-39-->.

##### NOTE (Windows)
You do not need to call `pgIdle()` since the blinking caret is maintained by the OS. Calling `pgIdle` by "accident" however is harmless.

## Clicking \& Dragging

Clicking and dragging is accomplished by using the following function:

	(long) pgDragSelect (pg_ref pg, co_ordinate_ptr location, short verb, short modifiers, long track_refcon, short auto_scroll);

To change the insertion point in a `pg_ref` (i.e., in response to a mouse click), call `pgDragSelect` with the `location` parameter set to the location of the "click." The coördinate values must be local to the window's coördinate system (relative to the top-left window origin).

For **Macintosh**, location should be the same as the "where" member of the `EventRecord`, converted to local coördinates.

For **Windows**, location is usually the coördinates given to you in `IParam` when responding to `WM_LBUTTONDOWN`, `WM_LBUTTONDBLCLK`, or `WM_MOUSEMOVE`.

The `verb` parameter defines what action should occur, which must be one of the following:

	enum
	{
		mouse_down,	// First-time click
		mouse_moved,	// Mouse moved
		mouse_up,		// Mouse button released
	}

NOTE: `pgDragSelect()` does not retain control at any time—it always returns control immediately regardless of what `verb` is passed.

For the first click, pass `mouse_down` in verb.

In a `Macintosh-specific` application, while the user is holding down the mouse button, wait for the mouse location to change and, if it does, call `pgDragSelect` with the new location but with `verb` as `mouse_moved`.

In a `Windows-specific` application, call `pgDragSelect(mouse_moved)` in response to a `WM_MOUSEMOVE` if the mouse button is still down.

When the mouse button is released, pass the final location and `mouse_up` for `verb`.

##### NOTE
It is important to call `pgDragSelect` with `mouse_up` after the user releases the mouse button *even if the mouse never moved from its original location*. This is because OpenPaige performs certain housekeeping chores when `mouse_up` is given.

The `modifiers` parameter controls the way text is selected. For "normal" click/drag, pass zero for this parameter; for added effects (such as responding to double-clicks, shift-clicks, etc.), see “Modifiers"<!-- on page 2-51-->.

If `auto_scroll` is "TRUE", OpenPaige will automatically scroll the document if `pgDragSelect` (with `verb` as `mouse_moved`) has gone beyond the `vis_area`. See "All About Scrolling"<!--on page 11-1-->. For getting "Up \& Running", you can pass TRUE for this parameter.

`track_refcon` is used when and if OpenPaige makes a call to the track-control-callback function. If a style is a "control" (the control bit set for the style class bits field), OpenPaige calls the tracking control function hook and passes the `track_refcon` to the app. In other words, this value is application-defined and OpenPaige does nothing with it. For getting "Up \& Running", you can pass 0 for this parameter.

##### FUNCTION RESULT

For "normal" mouse tracking, ignore the function result of `pgDragSelect`. The only time the function result is significant is when you have customized a style to be a "control" (information is available on "control" styles under "Customizing OpenPaige”). If you have not customized OpenPaige in any way, `pgDragSelect` will always return zero.

## Modifiers

The following bit settings are supported for the modifiers parameter in this release:

	#define EXTEND_MOD_BIT		0x0001	// Extend the selection
	#define WORD_MOD_BIT		0x0002	// Select whole words only
	#define PAR_MOD_BIT			0x0004	// Select whole paragraphs only
	#define LINE_MOD_BIT		0x0008	// Highlight whole lines
	#define VERTICAL_MOD_BIT	0x0010	// Allow vertical selection
	#define DIS_MOD_BIT			0x0020	// Enable discontinuous selection
	#define STYLE_MOD_BIT		0x0040	// Select whole style range
	#define WORD_CTL_MOD_BIT	0x0080	// Select "words" delimited by ctrl chars
	#define NO_HALF_CHARS_BIT	0x0100	// Do not go left/right on half chars
	#define CONTROL_MOD_BIT		0x0200	// Word advance for arrows

Various combinations of these bits can generally be set to create the desired effect such as word selections, paragraphs selections, etc., save that vertical selection does not work with the other modifiers. If misused regardless, it will produce unpredictable results.

The following is a description of how text is highlighted in response to each of these bits:

`EXTEND_MOD_BIT` will extend the selection for `verb` of `mouse_down` (otherwise the previous selection is removed). For Macintosh, this is the same as "shift-click" (but you need to determine that from your application and set this bit).

`WORD_MOD_BIT` will select whole words, otherwise only single characters are selected.

`PAR_MOD_BIT` will select whole paragraphs.

This is different than `LINE_MOD_BIT` (below) since a paragraph could contain several lines if word wrapping exists.

`LINE_MOD_BIT` will select whole lines. This differs from `PAR_MOD_BIT` since a paragraph might consist of many lines.

`VERTICAL_MOD_BIT` allows vertical selection. This bit really causes a rectangular region that selects all characters intersecting that region and will not follow any particular character. `VERTICAL_MOD_BIT` is mainly useful for tables and tabular columns.

`DIS_MOD_BIT` allows discontinuous selections. If this bit is set, the previous selection remains and a new selection range is started (OpenPaige can have multiple selection ranges).

`STYLE_MOD_BIT` causes whole style ranges to become selected. This is similar to word/paragraph/line highlighting except style changes are considered the delimiters (which also means the whole document could be selected in one click if only one style exists).

`WORD_CTL_MOD_BIT` causes text between control characters to be selected. This is similar to word/paragraph/line highlighting except control codes are considered the delimiters.

> **NOTE:** In OpenPaige "control codes" or "control characters" are not necessarily limited to standard ASCII symbols. Control characters in the OpenPaige context are defined in `pg_globals` (see "Changing Globals”<!--on page 3-21-->).

`NO_HALF_CHARS_BIT` controls whether or not dragging can change the selection point half way into a character. Normally, if this bit is not set, once the mouse moves half way into a character, that character is considered to be "selected" (or unselected if moving in the opposite direction). Setting this bit, however, instructs `pgDragSelect` not to select the character until it has completely crossed over its area.

`CONTROL_MOD_BIT` is used mainly with arrow keys. This causes the selection to advance to the next word (right arrow) or to the previous word (left arrow).

For additional information about highlighting and selection range(s), see "All About Selection”<!-- on page 10-1-->.

### 2.16 Click & Drag using Microsoft Foundation Classes (Windows)

To get Up and Running with simple mouse drag select in MFC, use the following code as a starting point:

	(.H)
	/* Declare the following private variables. Make sure to set m_Dragging to FALSE in the construct to avoid the unintialized variable bug!! *
	
	short m_MouseModifiers;
	BOOL m_Dragging;
	
	(.CPP)
	// Respond to the Windows message WM_LBUTTONDOWN...
	void MyView::OnLButtonDown(UINT nFlags, CPoint point)
	{
		CView::OnLButtonDown(nFlags, point);
		co_ordinate mouse { point.y, point.x };
		SetCapture();
		m_Dragging $=$ TRUE;
		if(nFlags & MK_SHIFT)
			m_MouseModifiers != EXTEND_MOD_BIT;
		if(nFlags & MK_CONTROL)
			m_MouseModifiers != PAR_MOD_BIT;
			pgDragSelect(m_Paige, &mouse, mouse_down, m_MouseModifiers, 0, TRUE);
	}
	
	// Respond to the Windows WM_MOUSEMOVE message...
	void MyView::OnMouseMove(UINT nFlags, CPoint point)
	{
		CView::OnMouseMove(nFlags, point);
		co_ordinate pg_mouse = {point.y, point.x};
		if(m_Dragging)
		{
			pgDragSelect(m_Paige, &mouse, mouse_up, m_MouseModifiers, 0, FALSE);
			m_MouseModifiers = 0;
			ReleaseCapture();
			m_Dragging = FALSE;
		}
	}
				
#### Responding to Windows mouse events

	case WM_LBUTTONDBLCLK:
	pg_modifiers = pg_modifiers = WORD_MOD_BIT;
	
	// fall through to WM_LBUTTONDOWN
	
	case WM_LBUTTONDOWN:
	if(pgRef)
	{
			co_ordinate pg_mouse;
			mouse_contact = TRUE;
			SetCapture(hWnd);
			pg_mouse.h = IParam & OxFFFF;
			pg_mouse.v = ((IParam & OxFFFF0000) >> 16);
		
		if (wParam & MK_SHIFT)
		pg_modifiers != EXTEND_MOD_BIT;
		
		if (wParam & MK_CONTROL)
		pg_modifiers != DIS_MOD_BIT;
		pgDragSelect(pgRef, &pg_mouse, mouse_down, pg_modifiers, 0, TRUE);
	}
	return 0 ;
	
	case WM_LBUTTONUP:
		if(pgRef)
		{
			co_ordinate pg_mouse;
			pg_mouse.h = lParam & OxFFFF;
			pg_mouse.v = ((lParam & OxFFFF0000) >> 16);
			mouse_contact = FALSE;
			pgDragSelect(pgRef, &pg_mouse, mouse_up, pg_modifiers, 0, FALSE);
			pg_modifiers = 0;
			ReleaseCapture();
		}
	return 0;
	
	case WM_MOUSEMOVE:
		if(mouse_contact)
			pgDragSelect(pgRef, &pg_mouse, mouse_moved, pg_modifiers, 0, TRUE);
		else
		{
			pg_view = pgPtInView(pgRef, &pg_mouse, NULL);
			
			if (pg_view & WITHIN_TEXT)
				SetCursor(LoadCursor(NULL, IDC_IBEAM));
			else
				SetCursor(LoadCursor(MULL, IDC_ARROW));
		}
	}
	return 0;

### TECH NOTE

#### Turn automatic scroll off

To prevent selecting/scrolling you would simply pass FALSE for `pgDragSelect` so it doesn't try to auto-scroll. As far as not letting the user select text outside the visual area, I would simply check to see if the coordinate that will get passed to `pgDragSelect` is outside of the view area and if it is, just force it to some other point that is within the view area.

In fact, you wouldn't even need to turn off auto-scroll if you forced the coördinate to always be within the visual area. Remember, you have complete control over `pgDragSelect` (control always comes back to you unlike, say, `TrackControl` on Macintosh) so there is no reason you can't adjust the "mouse" point for each pass.

#### (Mac-specific) Problems with mouse clicks -1

> I have big troubles handling mouse clicks in the openPaige object within my class library. If I get a click (with `GetMouse(&hitPt)`) and do the following (testing a response to a simple click)...

Your test code sample should work. Therefore, I have to conclude there is something wrong with the mouse point you obtain with `GetMouse()`.

I would guess that you are doing a `GetMouse()` without regards to the current `GrafPort`. Since `GetMouse()` returns a `LOCAL` point (based on current port's coördinates), if you don't have the correct `GrafPort` set you will get some other coördinate system. Worst case, you are getting "global" coordinates which will be completely different than what you expect.

Or, another possibility might have to do with the window's "origin." I know that some class libraries muck with this to create scrolling effects.

What you need to do is, check what the actual values of `point.h` and `point.v` really are. I know that pgDragSelect works, in fact you should see the caret immediately appear at the point you give for `mouse_down` verb.

BTW, the usual (best) way for dragging the mouse in a `pg_ref` is to get the click right out of the `EventRecord.where` field (first doing a `GlobalToLocal` on it). That is by far the most accurate -- but I do not know if that `EventRecord` is easily available in your class library.

## 2.17 Activate/Deactivate

To deactivate a `pg_ref` (to cause highlighting or the "caret" to disappear), call the following function:

	(void) pgSetHiliteStates (pg_ref pg, short front_back_state, short perm_state, pg_boolean show_hilite);

In a "window" environment, where different windows can overlap, it is usually desirable to disable any OpenPaige objects that are not contained in the front most window. To do so, `pgSetHiliteState` can be called to turn off the highlighting or the "caret."

An OpenPaige object, however, contains two highlight states, one for "front/back" activate and deactivate and one to disable a `pg_ref` in both states. For "normal" applications, you will only be changing the front/back highlight state (activate or deactivate a `pg_ref` according to its window position). The purpose of the alternate highlight state is to provide a way to disable a `pg_ref` completely regardless of its window position.

The `front_back_state` should be one of the following values:

	typedef enum
	{
		no_change_verb,	// State stays the same
		activate_verb,	// Set to activate mode
		deactivate_verb,	// Set to deactivate mode
	}
	

The `perm_state` parameter provides an alternate highlight state setting; this parameter can also be any of the above. For getting "Up \& Running," however, pass `no_change_verb` for this parameter.

If `show_hilite` is "TRUE", the highlighting (or caret) will redraw according to `pg`'s new state. A "FALSE" value will activate or deactivate `pg` internally (by setting special flags within the `pg_ref`) but the highlighting or caret will remain unchanged. For getting "Up & Running", always pass TRUE for `should_draw`.

See also "Additional Selection Support" <!--on page 10-4--> and "Activate/Deactivate with shape of selection still showing"<!--on page 10-13-->.

### Responding to `WM_SETFOCUS` and `WM_KILLFOCUS` messages

	{
	case WM_KILLFOCUS:
		pgSetHiliteStates(pgRef, deactivate_verb, no_change_verb, TRUE);
	
	case WM SETFOCUS:
		pgSetHiliteStates(pgRef, activate_verb, no_change_verb, TRUE);
	}

### Getting the Highlight State

If you want to know what state a `pg_ref` is in, call the following:

	(void) pgGetHiliteStates (pg_ref pg, short PG_FAR *front_back_state, short PG_FAR *perm_state);

The front/back highlight state will be returned in `front_back_state` and the alternate state in `perm_state`. Both parameters will be set to either `activate_verb` or `deactivate_verb`.

##### NOTES

1. If the highlight status is already set to what is specified in your parameters (e.g., if you are deactivating a `pg_ref` that is already deactivated or vice versa), this function does nothing.
2. A `pg_ref` returned from `pgNew` is set to an active state.
3. If a `pg_ref` is in a deactivate state, the caret will not blink even if you call pgIdle and highlighting will not draw.

### TECH NOTE

> Why two activate states?

One is for regular activate/deactivate for a window; the other is to FORCE deactivation regardless of the window's front/behind state. Haven't you ever been in a situation where you want to deactivate selections but the window is still in front? Using two possible states, it becomes easier to do that. The two states are logically "AND'd" logic for activation: both must be true or the document is deactivated.

#### MFC NOTE 

**IMPORTANT:** You must activate and deactivate the OpenPaige object in the MFC OnSetFocus() and OnKillFocus() before any of the functions in this chapter will work.

#### Example:

	(.CPP)
	
	// Respond to Windows message WM_SETFOCUS...
	void MyView::OnSetFocus(CWnd* pOldWnd)
	{
		CView::OnSetFocus(pOldWnd);
		pgSetHiliteStates(m_Paige, activate_verb, no_change_verb, TRUE);
	}
	
	// Respond to Windows message WM_KILLFOCUS...
	void MyView::OnKillFocus(CWnd* pNewWnd)
	{
		pgSetHiliteStates(m_Paige, deactivate_verb, no_change_verb, TRUE);
		CView::OnKillFocus(pNewWnd);
	}
	
	

# 3 BEYOND THE DEFAULTS

The purpose of this section is to explain some of the more common additions and/or changes to the "bare minimum" implementation discussed in the previous section, "Up \& Running."

## 3.1 Changing Attributes

There will be situations where you want to change the attributes of an OpenPaige object after it is created (these are the bits initially passed to `pgNew` for the "attributes" parameter). There are also situations where you want to examine the current attributes (to check mark a menu item, for instance). To do so, use the following:

	(long) pgGetAttributes (pg_ref pg);
	(pg_boolean) pgSetAttributes (pg_ref pg, long attributes);

To obtain the current attribute bits, call `pgGetAttributes`.

**FUNCTION RESULT:** The function result will be the current setting(s) of pg.

To change the attributes, call `pgSetAttributes` with attributes set to the new bit value(s).

OpenPaige "attributes" are defined as bit settings which can be a combination of any bit values shown below:

	#define NO_WRAP_BIT			0x00000001	// Wraps only on CR or LF
	#define NO_LF_BIT			0x00000002	// <LF> char ignored
	#define NO_DEFAULT_LEADING	0x00000004	// Do not add font leading
	#define NO_EDIT_BIT			0x00000008	// No editing (display only)
	#define EXTERNAL_SCROLL_BIT	0x00000010	// App controls scrolling
	#define COUNT_LINES_BIT		0x00000020	// Track line/para count
	#define NO_HIDDEN_TEXT_BIT	0x00000040	// Do not display hidden text
	#define SHOW_INVIS_CHAR_BIT	0x00000080	// Show invisible character(s)
	#define EX_DIMENSION_BIT	0x00000100	// Exclude width/height
	#define NO_WINDOW_VIS_BIT	0x00000200	// Do not respect clipped area
	#define SMART_QUOTES_BIT	0x00000800	// Do "smart" quotes
	#define NO_SMART_CUT_BIT	0x00001000	// Do not do "smart" cut/paste
	#define NO_SOFT_HYPHEN_BIT	0x00002000	// Ignore soft hyphens
	#define NO_DUAL_CARET_BIT	0x00004000	// Do not show dual carets
	#define SCALE_VIS_BIT		0x00008000	// Scale vis_area when scaling
	#define BITMAP_ERASE_BIT	0x00010000	// Erase page(s) with bitmap drwg
	#define TABS_ARE_WIDTHS_BIT	0x10000000	// Tab chars are merely wides
	#define LINE_EDITOR_BIT		0x40000000	// Doc is line editor mode

These are described under "Attribute Settings"<!-- on page 2-23-->.

**FUNCTION RESULT:** After calling `pgSetAttributes`, the function result will be "TRUE" if `pg` should be redrawn. The only time "TRUE" is returned is when one or more attributes have been set that will affect the way text is drawn or the way word wrap is computed.

**WARNING:** Before setting attributes, first get the current settings from the function `pgGetAttributes` and change the bits you require and pass that whole long value to `pgSetAttributes`. Otherwise, the `view only` bits will get changed erroneously.

Additional attributes can be set for more advanced features using the following `set` and `get` functions:

	(pg_boolean) pgSetAttributes2 (pg_ref pg, long attributes); (long) pgGetAttributes2 (pg_ref pg);

To obtain the current, extended attribute bits, call `pgGetAttributes2`.

**FUNCTION RESULT:** The function result will be the current setting(s) of the extended attributes of `pg`.

To change the extended attributes, call `pgSetAttributes2` with attributes set to the new bit value(s).

OpenPaige "extended attributes" are defined as bit settings which can be a combination of any of the following.

	#define KEEP_READ_STYLES	0x00000200	// Keep existing style_infos for pgReadDoc()
	#define KEEP_READ_PARS		0x00000400	// Keep existing par_infos for pgReadDoc()
	#define KEEP_READ_FONTS		0x00000800	// Keep existing font_infos for pgReadDoc()
	#define CHECK_PAGE_OVERFLOW	0x00002000	// Constantly check page overflow
	#define NO_HAUTOSCROLL		0x00080000	// Do not autoscroll horizontally
	#define NO_VAUTOSCROLL		0x00100000	// Do not autoscroll vertically

`KEEP_READ_STYLES` tells OpenPaige to not remove existing `style_info` records from the `pg_ref` when a file is read. Normally, all existing style records are replaced with the styles read from an OpenPaige file. This attribute is used to retain the existing styles.

`KEEP_READ_PARS` tells OpenPaige to not remove existing `par_info` records from the `pg_ref` when a file is read. Normally, all existing paragraph records are replaced with the paragraph records read from an OpenPaige file. This attribute is used to retain the existing paragraph records.

`KEEP_READ_FONTS` tells OpenPaige to not remove existing `font_info` records from the `pg_ref` when a file is read. Normally, all existing font records are replaced with the fonts read from an OpenPaige file. This attribute is used to retain the existing fonts.

`CHECK_PAGE_OVERFLOW` tells OpenPaige to constantly test the position of the last character in the document and, if it overflows the bottom of the `page_area`, sets an internal field to the number of characters that have overflowed. The purpose of this attribute is to allow an application to implement features that require "page overflow checking", but since this requirement requires constant pagination and extra processing, set this attribute only when absolutely necessary.

`NO_HAUTOSCROLL`, `NO_VAUTOSCROLL` tells OpenPaige not to automatically scroll horizontally or vertically, respectively, when `pgDragSelect()` is called.

## "Auto-checking" page overflow

Setting `CHECK_PAGE_OVERFLOW` with `pgSetAttribute2()` causes OpenPaige to continuously check the situation where character(s) flow below the boundaries of the page area. If this attribute is set, the `overflow_size` member within the `pg_ref` get set to the number of characters that overflow the page.

Or, if `overflow_size` is set to -1, a single carriage return is causing the overflow (i.e., the text overflows but the overflow is a "blank" line).

**NOTE:** The auto-checking for page overflow is meaningless if your `pg_ref` is set for repeating pages, or if your `pg_ref` is set to a variable page size. The only time overflow checking will work (or make any sense) is for fixed-size, nonrepeating page shapes.

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

Regarding LF/CR characters, OpenPaige handles both of them as a "new line" except a CR. It also starts a new paragraph, but for LF it just does a line feed.

Note that lines that terminate both in LF and CR will cause "two" lines on the screen -- at least in OpenPaige default mode.

You can turn that off, however, if you want LF/CR to be treated as only one líne feed.' To do so, just set `NO_LF_BIT` in the OpenPaige attribute flags during `pgNew`. When this attribute is set, OpenPaige ignores all LFs embedded in the text (they become invisible).

Note that I haven't mentioned what the values are for LF and CR, because those are whatever values sit in OpenPaige globals. Also as he mentioned, MPW will compile `\r` etc. differently than Symantec so watch out for that. See "Changing Globals"<!-- on page 3-81--> and "CR/LF Conversion"<!-- on page 3-88-->.

## 3.3 A Different Default Font, Style, Paragraph

Any time a new `pg_ref` is created, OpenPaige sets the initial `style_info`, `font_info` and `par_info` (style, font and paragraph format) to whatever exists in the corresponding field from `pg_globals`.

Hence, to set default style, font or paragraph format, simply change the respective information in `pg_globals` (see example below).

To change the default style information, change field(s) in `pg_globals.def_style`; to change the default font, change field(s) in `pg_globals.def_font`; to change the default paragraph format, change field(s) in `pg_globals.def_par`.

You can also set the default low-level callback "hook" functions for style or paragraph records, and even the general OpenPaige functions by placing a pointer to the new function in the respective `pg_globals` field. See "Customizing OpenPaige"<!-- on page 271-->.

For example, if you wanted to override the draw-text callback function always for all styles, you would change the default draw-text function in the default style found in `pg_globals` before your first call to `pgNew` (but after `pgInit`:

	pg_globals.def_style.procs.draw = myTextDrawProc;

... where `myTextDrawProc` is a low-level callback to draw text (see "Setting Style Functions”<!-- on page 27-4-->). If you did this, every new `style_info` record created by OpenPaige will contain your callback function.

The default hooks for general callbacks not related to styles or paragraph formats are in `pg_globals.def_hooks`.

See a complete description of `style_info`, `font_info` and `par_info` records under "Style Basics”<!-- on page 8-1-->.

### Change defaults after they are created using `pgInit`.

These changes will apply to all `pgNew`s that are called later.

	void ApplInit()	// Initialisation of the App
	{
		pgMemStartup(&mem_globals, 0);
		pgInit(&paige_rsrv, &mem_globals);
		
		/* change to make the default for all pg_refs created herein after 
		9 point instead of 12 point is a fraction with hi word being a 
		point is a fraction with hi word being the whole point value /*
		
		paige_rsrv.def_style.point = 0x00090000;
	}


### Default tab spacing

You can also change the default spacing for tabs (the distance to the next tab if no specific tab stops have been defined in the paragraph format). To do so, change `globals.def_par.def_tab_space`.

	/* The following code changes the default tab spacing (for all subsequent pg_refs) to $32. */
	
	pgMemStartup(&mem_globals, 0);
	pgInit(&paige_rsrc, &mem_globals);
	paige_rsrv.def_par.def_tab_space $=32$;

## 3.4 Graphic Devices

As mentioned earlier, a newly created OpenPaige object will always draw to a default device; in a Macintosh environment, for instance, the default device will be the current port that is set before calling `pgNew`. In a Windows environment, the default device will be an HDC derived from `GetDC(hWnd)`, where `hWnd` is the window given to `pgNew`.

### Setting a device

It is possible that you will want to change that default device once an OpenPaige object has been created. To do so, call the following function:

	(void) pgSetDefaultDevice (pg_ref pg, graf_device_ptr device);

The device parameter is a pointer to a structure which is maintained internally (and understood) by OpenPaige. (Generally, you won't be altering its structure directly but the record layout is provided at the end of this section for your reference).

The contents and significance of each field in a `graf_device` depends on the platform in which OpenPaige is running. However, a function is provided for you to initialize a `graf_device` regardless of your platform:

	(void) pglnitDevice (pg_globals_ptr globals, generic_var the_port, long machine_ref, graf_device_ptr device);

The above function sets up an OpenPaige graphics port which you can then pass to `pgSetDefaultDevice` (you can also use `pgInitDevice` to set up an alternate port that can be passed to `pgDisplay`).

The `globals` parameter is a pointer to the same structure you passed to `pgInit`.

The actual (but machine-dependent) graphics port is passed in `the_port`; what should be put in this parameter depends on the platform you are working with, as follows:

* **Macintosh** (and PowerMac) — `the_port` should be a `GrafPtr` or `CGrafPtr`; `machine_ref` should be zero.

* Windows (all OS versions) — `the_port` should be an `HWND` and `machine_ref` should be `MEM_NULL`. Or, if you only have a device context (but no window), `the_port` should be `MEM_NULL` and `machine_ref` the device context. See sample below.

The `device` parameter must be a pointer to an uninitialised `graf_device` record. The function will initialise every field in the `graf_device`; you can then pass a pointer to that structure to `pgSetDefaultDevice`.

#### NOTES

1. If you specified a window during `pgNew()` and want the `pg_ref` to continue displaying in that window, the "default device" is already set, so you do not need to use these functions. The only reason you would/should ever set a default device is if you want to literally change the window or device context the `pg_ref` is associated with.

2. OpenPaige makes a copy of your `graf_device` record when you call `pgSetDefaultDevice`, so the structure does not need to remain static. But the graphics port itself (`HWND` or `HDC` for Windows, or `GrafPtr` for Mac) must remain “open” and valid until it will no longer be used by OpenPaige.

3. If you need to temporarily change the `GrafPtr` (Macintosh) or device context (Windows), see "Quick & easy `set-window`"<!-- on page 3-72 in this chapter-->.

**CAUTION:** Do not set the same `graf_device` as the "default device" to more than one `pg_ref`. If you need to set the same window or device context to more than one `pg_ref`, create a new `graf_device` for each one.

### Setting up a `graf_device` for Windows

#### EXAMPLE 1: Setting up a `graf_device` from a Window handle (HWND)

	graf_device device;
	
	pgInitDevice(&paige_rsrv, (generic_var)hWnd, MEM_NULL, &device);
	pgSetDefaultDevice(pg, &device);
	
	//... other code, draw, paint, whatever.
	
	pgCloseDevice(&paige_rsrv, &device);

#### EXAMPLE 2: Setting up a `graf_device` from a device context only (HDC):

	graf_device device;
	
	pgInitDevice(&paige_rsrv, MEM_NULL, (generic_var)hDC, &device);
	pgSetDefaultDevice(pg, &device);
	
	//... other code, draw, paint, whatever.

### Setting default device on the Macintosh

	/* This function accepts a pg_ref (already created) and a Window pointer. The Window is set to pg's default drawing port, so after a call to this function, all drawing will occur in a new window. */
	
	void set_new_paige_port (pg_ref pg, WindowPtr new_port)
	{
		graf_device paige_port;
		pgInitDevice(&paige_rsrv, new_port, 0, &paige_point);
		pgSetDefaultDevice(pg, &paige_port);
	}
	
	/* Done. OpenPaige makes a copy of paige_port so it does not need to be static */

If you want to obtain the current default device for some reason, you can call the following:

	(void) pgGetDefaultDevice (pg_ref pg, graf_device_ptr device);

The device is copied to the structure pointed to by `device`.

### Disposing a device

If you have initialized a `graf_device`, followed immediately by `pgSetDefaultDevice()`, you do not need to uninitialize or dispose the `graf_device`.

But if you have initialized a `graf_device` that you are keeping around for other purposes, you must eventually dispose its memory structures. To so call the following:

	(void) pgCloseDevice (pg_globals_ptr globals, graf_device_ptr device);

This function disposes all memory structure created in device when you called `pgInitDevice`. The `globals` parameter should be a pointer to the same structure given to `pgInit`.

#### NOTES:

1. `pgCloseDevice` does not close or dispose the `GrafPort` (Macintosh) or the HWND/HDC (Windows) — you need to do that yourself.

2. You should never dispose a device you have set as the default device because `pgDispose` will call `pgCloseDevice`. The only time you would use `pgCloseDevice` is either when you have set up a `graf_device` to pass as a temporary pointer to `pgDisplay` (or a similar function that accepts a temporary port) in which OpenPaige does not keep around, OR when you have changed the default device (see note below).

3. Additionally: OpenPaige does not dispose the previous default device if you change it with `pgSetDefaultDevice`. Thus, if you change the default you should get the current device (using `pgGetDefaultDevice`), set the new device then pass the older device to `pgCloseDevice`.

### Quick \& easy `set-window`

In certain situations you might want to temporarily change the window or device context a `pg_ref` will render its text drawing. While this can be done by initializing a `graf_device` and giving that structure to `pgSetDefaultDevice()`, a simpler and faster approach might be to use the following functions:

	generic_var pgSetDrawingDevice (pg_ref pg, const generic_var draw_device);
	void pgReleaseDrawingDevice (pg_ref pg, const generic_var previous_device);

The purpose of `pgSetDrawingDevice` is to temporarily change the drawing device for a `pg_ref`. The `draw_device` parameter must be a `WindowPtr` (Macintosh) or a device context (Windows).

The function returns the current device (the one used before `pgSetDrawingDevice`).

**NOTE:** "device" in this case refers to a machine-specific device, not a `graf_device` structure.

You should call `pgReleaseDrawingDevice` to restore the `pg_ref` to its previous state. The `previous_device` parameter should be the value returned from `pgSetDrawingDevice`.

### Temporarily changing the HDC (Windows)

	/* This function forces a pg_ref to display inside a specific HDC instead of the default. */
	
	void DrawToSomeHDC (pg_ref pg, HDC hDC)
	{
		generic_var old_dc;
		old_dc = pgSetDrawingDevice(pg, (generic_var)hDC);
		pgDisplay(pg, NULL, MEM_NULL, MEM_NULL, NULL, best_way); 
		pgReleaseDrawingDevice(pg, old_dc);
	}

### Setting a Scaled Device Context (Windows only)

On a Windows platform, in certain cases you will want to preset a device context that needs to scale all drawing. However, using the standard function to set a device into an OpenPaige object (`pgSetDrawingDevice`) will not work in this case because OpenPaige will want to clear your mapping mode(s) and scaling factor(s).

The solution is to inform OpenPaige that you wish to set your own device context, but to include a scaling factor:

	generic_var pgSetScaledDrawingDevice (pg_ref pg, const generic_var draw_device, pg_scale_ptr scale);

This is identical to `pgSetDrawingDDevice()` except that it contains the additional parameter `scale` which specifies the scaling factor. For more information on OpenPaige scaling, see the appropriate section(s).

### 3.5 Color Palettes (Windows-specific)

	void pgSetDevicePalette (pg_ref pg, const generic_var palette); generic_var pgGetDevicePalette (pg_ref pg);

These Windows-specific functions are used to select a custom palette into the device context of a `pg_ref`. To set a palette, call `pgSetDevicePalette()` and pass the `HPALETTE` in `palette`. If you want to clear a previous palette, pass `(generic_var)O`.

Setting a palette causes OpenPaige to select that palette every time it draws to its device context.

To obtain the existing palette (if any), call `pgGetDevicePalette()`

**CAUTION:** Do not delete the palette unless you first clear it from the `pg_ref` by calling `pgSetDevicePalette(pg, (generic_var)0)`.

**CAUTION:** If you change the default device (`pgSetDefaultDevice`), you need to set the custom palette again.

**NOTE:** OpenPaige does not delete the `HPALETTE`, even during `pgDispose()`. It is your responsibility to delete the palette.

### 3.6 Changing Shapes

You can change the `vis_area`, the `page_area` and/or the `exclude_area` of an OpenPaige object at any time (see "Creating an OpenPaige Object"<!-- on page 2-14 about pgNew<--> for a description of each of these parameters):

	(void) pgSetAreas (pg_ref pg, shape_ref vis_area, shape_ref page_area, shape_ref exclude_area);

The `vis_area`, `page_area`, and `exclude_area` are functionally identical to the same parameters passed in `pgNew`. Of course, you could have passed any of these shapes in `pgNew`, but the purpose of `pgSetAreas` is to provide a way to change the visual area and/or wrap area and/or exclusion areas some time after an OpenPaige object has been created.

Any of the three "`_area`" parameters can be `MEM_NULL`, in which case that shape remains unchanged.

Subsequent drawing of `pg`'s text will reflect the changes, if any, produced by the changed shape(s).

A typical reason for changing shapes would be, for example, to implement a "set columns" feature. The initial OpenPaige object might have been a simple rectangle ("normal" document), but let us suppose that the user later wishes to change the document to three columns. To do so, you could set up a `page_area` shape for three columns and pass that new shape to `page_area` and null pointers for the other two areas. The OpenPaige object, on a subsequent `pgDisplay`, would rewrap the text and flow within these "columns."

#### NOTES

1. If your area(s) are simple rectangles, it may prove more efficient to use `pgSetAreaBounds()` in this chapter.

2. If you simply want to "grow" the `vis_area` (such as responding to a user changing the window’s size), see “'Growing' The Visual Area”<!-- on page 3-76--> for information on `pgGrowVisArea`.

3. OpenPaige makes a copy of the new shape(s) you pass to `pgSetAreas`. You can therefore dispose these shapes any time afterwards.

For information on constructing various shapes, see "All About Shapes"<!-- on page 12-1-->.

If you are implementing containers, see “Containers Support”<!-- on page 14-1-->.

## "Growing" The Visual Area

If you want to change the `vis_area` (area in which text displays) in response to a user enlarging the window's width and height, call the following:

	(void) pgGrowVisArea (pg_ref pg, co_ordinate_ptr top_left, co_ordinate_ptr bot_right);

The size of `vis_area` shape in `pg` is changed by adding `top_left` and `bot_right` values to `vis_area`'s top-left and bottom-right corners, respectively.

By "adding" is meant the following: `top_left.v` is added to `vis_area`'s top and `top_left.h` is added to `vis_area`'s left; `bot_right.v` is added to `vis_area`'s bottom and `bot_right.h` is added to `vis_area`'s right.

**NOTE:** This function adds to (or "subtracts" from, if coordinate parameters are negative) the visual area rather than setting or replacing the visual area to the given coördinates.

Either `top_left` or `bot_right` can be null pointers, in which case they are ignored.

**NOTE:** This function only works correctly if `vis_area` is rectangular; if you have set a non-rectangular shape, you need to reconstruct your `vis_area` shape and change it with `pgSetAreas`.

#### Responding to WM SIZE message (Windows)

	case WM_SIZE:
		if (pgRef)
		{
			rectangle vis_bounds;
			co_ordinate amount_to_grow;
			long old_width, new_width, old_height, new_height;
			pgAreaBounds(pgRef, NULL, &vis_bounds);
			new_width = (long) LOWORD(lParam);
			new_height = (long) HIWORD(lParam);
			old_width = vis_bounds.bot_right.h - vis_bounds.top_left.h;
			old_height = vis_bounds.bot_right.v - vis_bounds.top_left.v;
			amount_to_grow.h = new_width - old_width;
			amount_to-grow.v = new_height - old_height;
			pgGrowVisArea(pgRef, NULL, (co_ordinate_ptr) &amount_to_grow);
		}
	break;

## 3.7 Getting information about shapes

### Getting Current Shapes

To obtain any of the three shapes in an OpenPaige object, call the following:

	(void) pgGetAreas (pg_ref pg, shape_ref vis_area, shape_ref page_area, shape_ref exclude_area);

The `vis_area`, `page_area`, `exclude_area` must be pre-created `shape_ref`s (see below). Any of them, however, can be `MEM_NULL` (in which case that parameter is ignored).

This function will copy the contents of `pg`'s visual area, wrap area, and exclude area into `vis_area`, `page_area` and exclude_area, respectively, if that parameter is non-null.

Helpful hint: The easiest way to create a `shape_ref` is to call `pgRectToShape` passing a null pointer to the `rect` parameter, as follows:

	shape_refnew_shape;
	new_shape = pgRectToShape(&paige_rsrv, NULL);

The `paige_rsrv` parameter in the above example is a pointer to the same `pg_globals` passed to `pgInit`. By providing a null pointer as the second parameter, a new `shape_ref` is returned with an empty shape (all sides zero).

### 'Get/Set Areas' Trick

If you are using simple rectangles for the visual area or wrap (page) area in an OpenPaige object, and/or if you simply want to know the bounding rectangular area of either shape, use the following instead of `pgGetAreas`:

	(void) pgAreaBounds (pg_ref pg, rectangle_ptr page_bounds, rectangle_ptr vis_bounds);

When `pgAreaBounds` is called, `page_bounds` gets set to a rectangle that encloses the entire `page_area` and `vis_bounds` gets set to a rectangle that encloses the entire `vis_area` of `pg`.

If you don't want one or the other, either `page_bounds` or `vis_bounds` can be a null pointer.

This function is useful when you simply want the enclosing bounds of either shape because you do not need to create a `shape_ref`.

You can also set the page area and/or vis area by calling `pgSetAreaBounds`, which accepts a pointer to a rectangle in `page_bounds` and `vis_bounds` (of which either can be a null pointer). Note that this is faster and simpler than `pgSetAreas`, except that it only works provided that the shape(s) are single rectangles.

### Direct Shape Access

You can also access the `shape_ref`s in an OpenPaige object directly using any of the following:

	(shape_ref) pgGetPageArea (pg_ref pg);
	(shape_ref) pgGetVisArea (pg_ref pg);
	(shape_ref) pgGetExcludeArea (pg_ref pg);

These three functions will return the `shape_ref` for `page_area`, `vis_area` and `exclude_area`, respectively. Neither will ever return `MEM_NULL` (even if you provided `MEM_NULL` for `exclude_area` in `pgNew`, for instance, OpenPaige will still maintain a `shape_ref` for the exclusion, albeit an empty shape).

The purpose of these functions is for special applications that need to look inside of OpenPaige shape as quickly and as easily as possible.

**CAUTION:** These functions return the actual memory_ref's for each shape. You must therefore never dispose of them, nor should you alter their contents (or else OpenPaige won't know you have changed anything and word wrapping and display will fail). If you want to alter the contents of OpenPaige shapes, see "Containers Support"<!-- on page 14-1--> and "Exclusion Areas"<!-- on page 15-1-->.

### Getting Shape Rectangle Quantity

You can find out how many rectangles comprise any shape by calling the following:

	(pg_short_t) pgNumRectsInShape (shape_ref the_shape);

The function will return the number of rectangles in `the_shape`.

**NOTE:** The result will always be at least 1, even for an empty shape. Any "empty" shape is still one rectangle whose boundaries are $0,0,0,0$. If you need to detect whether or not a shape is empty, call:

	(pg_boolean)pgEmptyShape(the_shape); /* Returns TRUE if empty */


## 3.8 Changing Globals

As mentioned several times, your application provides a pointer to `pgInit` (and other places) to be used by OpenPaige to store certain global variables. This structure is initially set to certain default values, but you can make certain changes that apply to your particular application.

For example, OpenPaige globals contain the values for special control codes such as CR, LF, and arrow keys, but there are instances when you need to change some of these "characters" to a different value.

Another (more common) reason to change OpenPaige globals is to force a default text or paragraph format for all subsequent `pgNew()` calls.

Since your application maintains the globals record, there are no functions provided to change its contents; rather, you alter the structure's contents directly some time after `pgInit`.

**NOTE:** The entire OpenPaige globals structure can be viewed in `paige.h`. Only the members of this structure that you are allowed to alter are shown unless noted otherwise.

	/* Paige "globals" (address space provided by app): */
	
	struct pg_globals
	{
		pgm_globals_ptr	mem_globals;			// Globals for pgMemManager
		long			max_offscreen;			// Maximum memory for offscreen
		long			max_block_size;			// Maximum size of text block
		long			minimum_line_width;		// Minimum size line width
		long			def_tab_space;			// Default tab spacing for pgNew
		pg_short_t		line_wrap_char;			// <CR> character
		pg_short_t		soft_line_char;			// Soft <CR> character
		pg_short_t		tab_char;				// Tab character
		pg_short_t		soft_hyphen_char;		// Soft hyphen character
		pg_short_t		bs_char;				// Backspace character
		pg_short_t		ff_char;				// Form feed chr (for page breaks)
		pg_short_t		container_brk_char;		// Container break character
		pg_short_t		left_arrow_char;		// Left arrow
		pg_short_t		right_arrow_char;		// Right arrow
		pg_short_t		up_arrow_char;			// Up arrow
		pg_short_t		down_arrow_char;		// Down arrow
		pg_short_t		fwd_delete_char;		// Forward delete character
		pg_char			hyphen_char[4];			// Hard hyphen character
		pg_char			decimal_char[4];		// "." char (for decimal tabs)
												/* Visible surrogate for:
												-------------------------/*
		pg_char			cr_invis_symbol[4];		// carriage return
		pg_char			lf_invis_symbol[4];		// line feed
		pg_char			tab_invis_symbol[4];	// horizontal tab
		pg_char			end_invis_symbol[4];	// end-of-document
		pg_char			pbrk_invis_symbol[4];	// break-of-page
		pg_char			cont_invis_symbol[4];	// container break
		pg_char			space_invis_symbol[4];	// space
												//------------------------
		pg_char			flat_single_quote[4];	// Single "typewriter" quote
		pg_char			flat_double_quote[4];	// Double "typewriter" quote
		pg_char			left_single_quote[4];	// Single left smart quote
		pg_char			right_single_quote[4];	// Single right smart quote
		pg_char			left_double_quote[4];	// Double left smart quote
		pg_char			right_double_quote[4];	// Double right smart quote
		pg_char			elipse_symbol[4];		// Char to draw for ellipse
		long			invis_font;				// Machine-specific invisible char font
		pg_char			unknown_char[4];		// Used for unsupported characters
		long			embed_callback_proc;	// Used internally by embed_refs
		font_info		def_font;				// Default font for all pgNew's
		style_info		def_style;				// Default style for all pgNew's
		par_info		def_par;				// Default para for all pgNew's
		color_value		def_bk_color;			// Default background colour
		color_value		trans_color;			// Transparent colour (default is white)
		pg_hooks		def_hooks;				// Default general hooks
		// miscellaneous fields not to be altered by app.
	};

The following is a description for each field that you can change directly:

`max_offscreen` — defines the maximum amount of memory, in bytes, that can be used for offscreen bit map drawing. The purpose of this field is to avoid excessive, unreasonable offscreen bit maps for huge text on high-density monitors.

`max_block_size` — defines the largest size for contiguous text (OpenPaige breaks down text into blocks of `max_block_size` as the OpenPaige object grows).

`minimum_line_width` — pdefines the smallest width allowed, in pixels, for a line of text. The purpose of this field is for OpenPaige to decide when a portion of a wrap area is too small to even consider placing text.

`def_tab_space` — not used in version 1.3 and beyond. (To change default tab spacing, change `globals.def_par.def_tab_space`).

`line_wrap_char` through `down_arrow_char` — defines all the special characters recognized by OpenPaige. Any of these can be changed to something else if you don't want the default values. See **Warning** below. See also "Double Byte Defaults"<!-- on page 388-->.

`text_brk_char` — defines an alternate character to delineate text blocks (OpenPaige partitions large blocks of text into smaller blocks; by default, a block will break on a \<CR\> or \<LF\>, but if neither of those are found in the text, the `text_brk_char` will be searched for). For additional information, see "Anatomy of Text Blocks"<!-- on page 36-1--> in the Appendix.

`null_char` — defines a special character that, if inserted, merely causes word-wrap to recalculate and the `null_char` itself is not inserted.

`cr_invis_symbol` through `space_invis_symbol` — define all the character values to draw when OpenPaige is in "show invisibles" mode. Each character is represented by a null-terminated Pascal string (first byte is the length, followed by the byte(s) for the character, followed by a zero). Note that these characters can be zero, one or two bytes in length. See also “Double Byte Defaults”<!-- on page 3-88-->.

`flat_single_quote` through `right_double_quote` - define single and double quotation characters for "smart quotes" implementation. The "flat_" quote characters should be the standard ASCII characters for single and double quotes, while the "left_" and "right_" quote characters are to be substituted for "smart quotes" if that feature has been enabled.

`elipse_symbol` — contains the character to draw an ellipsis “…" symbol. However, this character definition has been provided only for future enhancement: the current version of OpenPaige does not use this character for any built-in feature.

`invis_font` — defines the font to be used for drawing invisibles. This is machine dependent. For Macintosh, this is the QuickDraw font ID that gets set for invisible characters. For Windows, this is a font `HANDLE` (which you can alter by replacing it with you own font `HANDLE`)

`unknown_char` — Contains the symbol to be used when importing unsupported characters. For example, importing a file with OpenPaige's import extension may include characters that to not cross over to any available character set, in which case unknown_char will be substituted.

**WARNINGS**

* *(Windows only)* If you replace the `invis_font` member with your own font object, do not delete the object that was there before, if any. Moreover, OpenPaige will not delete your `invis_font` object either, so you are responsible for deleting your own object before your application quits.
* The default machine-specific functions within OpenPaige are assuming ASCII control codes for the special character values in `pg_globals` (ASCII chars \< 20).

`def_font`, `def_style`, `def_par` — define the default font, text and paragraph formatting, respectively. Whenever you call `pgNew()`, these three structures are literally copied into the new `pg_ref`. Hence, to change the default(s) for text formatting, you simply change the members of these three structures prior to calling `pgNew()`.

`def_bk_color`, `trans_color` — define the default background colour to be used for drawing all text and the colour that is considered "transparent", respectively. The background colour is not necessarily the same as the window's background colour (OpenPaige will make the necessary adjustment if window colour does not equal the `pg_ref`'s background colour). By "transparent" color is meant which colour is considered the normal screen background colour (default is white).

The purpose of defining the transparent colour is to inform OpenPaige when and if the background of its drawing needs to be "erased" with a different colour other than the regular background of the window. If the background colour for an OpenPaige object is set to the same value as `trans_color` in `pg_globals`, OpenPaige won't do any special color filling of background since it assumes normal erasing of the window will take care of it (for instance, responding to WM_PAINT). If OpenPaige's background color is not the same as `trans_color`, then the `pg_ref`'s background shape will be pre-filled with a different color other than the window's default.

`def_hooks` — define the default function pointers to be used for a `pg_ref`'s general hooks. Essentially, `pgNew` copies these pointers. (DSI and other developers can change these defaults for special extensions).

### Default Values

After you have called $p g I n i t$, the following defaults are set for all the fields mentioned above:

| Global Field | About the field | Windows | Macintosh |
|--------------|-----------------|---------|-----------|
| `max_offscreen` | bit map size (bytes) | 48,000 | 48,000 |
| `max_block_size` | max paragraph size in number of `<br>` characters | 4096 | 4096 |
| `minimum_line_width` | in pixels | 16 | 16 |
| `line_wrap_char` | carriage return character | 0x0D | 0x0D |
| `soft_line_char` | soft carriage return char | 0x0A | 0x0A |
| `tab_char` | tab char | 0x09 | 0x09 |
| `hyphen_char` | hard hyphen char | 0x2D | 0x2D |
| `soft_hyphen_char` | soft hyphen char | 0x1F | 0x1F |
| `decimal_char` | decimal point char | 0x2E | 0x2E |
| `bs_char` | back space (delete) char | 0x08 | 0x08 |
| `lf_char` | line feed char | 0x0C | 0x0C |
| `container_brk_char` | container break char | 0x0E | 0x0E |
| `left_arrow_char` | left arrow key | 0x1C | 0x1C |
| `right_arrow_char` | right arrow key | 0x1D | 0x1D |
| `up_arrow_char` | up arrow key | 0x1E | 0x1E |
| `down_arrow_char` | down arrow key | 0x1F | 0x1F |
| `text_brk_char` | alternative carriage return char `<br>` (form feed) | 0x1B | 0x1B |
| `fwd_delete_char` | forward delete key | 0x7F | 0x7F |
| `elipse_symbol` | displayed when OpenPaige encounters an unknown symbol | '.' | 0x85 |
| `flat_single_quote` | straight apostrophe – ' | 0x27 | 0x27 |
| `flat_double_quote` | straight double quote – " | 0x22 | 0x22 |
| `left_single_quote` | curly left quote – ' | 0x91 | 0xD4 |
| `right_single_quote` | curly right quote – ' | 0x92 | 0xD5 |
| `left_double_quote` | curly left quotes – “ | 0x93 | 0xD2 |
| `right_double_quote` | curly right quotes – " | 0x94 | 0xD3 |
| `cr_invis_symbol` | carriage return when invisibles <br> are displayed | ¶ (0xB6) | ¶ (0xA6) |
| `lf_invis_symbol` | line feed when invisibles are displayed | ¼ (0xB5) | ¼ (0xB9) |
| `tab_invis_symbol` | tab when invisibles are displayed | 0x95 | 0x13 |
| `end_invis_symbol` | end of document when invisibles <br> are displayed | × (0xB5) | × (0xB0) |
| `cont_invis_symbol` | container break when invisibles <br> are displayed | `|` (0xA5) | `|` (0xAD) |
| `space_invis_symbol` | space symbol when invisibles are <br> displayed | . (0x2E) | . (0x2E) |
| `invis_font` | font in which invisibles are dis- <br> played | default font* | 0 (Chicago) |
| `def_font` | font (name) used for `pgNew()` | “System” | Application font |
| `def_style` | text format used for `pgNew()` | Plain, 12 point | Plain, 12 point |
| `def_par` | paragraph format used for <br> pgNew () | Indents all zero, tab spacing 24 pixels | Indents all zero, tab spacing 24 pixels |
| def_bk_color | background color used to fill page <br> area for all pg_refs | white | white |
| trans_color | color assumed also to be window’s background | white | white |


If the default font is zero, then OpenPaige creates a font object using the default found in `pg_globals` record that was created with `pgNew`. If you want to change this you can change the default font in the `pg_globals`.

**NOTE (Macintosh):** The `pgdf` Resource: During initialization, the machine-specific code for Macintosh searches for a special resource to determine the character defaults (above). If it does not find this resource, the values given above are used. Hence, you can change the defaults by changing the contents of this resource:

| TABLE \#2 | MACINIOSH RESOURCE TYPE \& ID |
| :---: | :---: |
| Resource Type | Resource ID |
| "pgdf' | 128 |

The OpenPaige package we provide should contain this resource as well as a ResEdit template to change its contents.

### Double Byte Defaults

Each character default in `pgGlobals` can be "double byte" such as Kanji, if necessary. Although this manual references these defaults as "characters," in truth these global values are ALL double-byte, that is they are unsigned integers. An ASCII CR, for instance, is considered to be 0x000D and not 0x0D, etc. To set a double byte default, such as a Kanji decimal for instance, simply place the whole 16-bit value into the appropriate global field.

### TECH NOTE (CR/LF Conversion)

> I have read all the stuff so far about carriage return line feeds. What exactly do I have to do to make sure my documents are portable between the PC which uses `<CR><LF>`, and the Mac which uses only a `<CR>`?

OpenPaige normally formats text using only CR for paragraph endings (NOT CR/LF), hence for documents created from scratch on any of the platforms, where all text has been entered by the user via the keyboard, documents between platforms are generally portable with respect to CR/LF or just CR.

The only time this can become even remotely an issue is when raw text is inserted which contains both CR and LF, which if left "as is" would cause OpenPaige to draw two line feeds for each paragraph ending (one for CR and one for LF).

To avoid this situation, the `NO_LF_BIT` should be set as one of the "flag" bits in `pgNew` (or, if the `pg_ref` has already been created, NO LF BIT can be set by calling pgGetAttributes, ORing `NO_LF_BIT` to the result and setting that value with `pgSetAttributes`. By setting this bit, OpenPaige will essentially ignore all LF characters and they will become virtually invisible.

See also "Carriage return/line feeds causing problems"<!-- on page 3-65-->.

## 3.9 Cloning an OpenPaige Object

To create a new OpenPaige object based on an existing `pg_ref`'s `vis_area`, `page_area`, `exclude_area` and attributes, use the following:

	(pg_ref) pgDuplicate (pg_ref pg);

**FUNCTION RESULT:** This function returns a new `pg_ref`, completely independent of, but using the same shapes and attributes as, `pg`. No text is copied and the default text formatting is used.

## 3.10 Storing Arbitrary References and Structures

You can store any arbitrary long value or pointer into a `pg_ref` any time you want, and with as many different values as you want by using the following:

	(void) pgSetExtraStruct (pg_ref pg, void PG_FAR *extra_struct, long ref_id);
	(void PG_FAR *) pgGetExtraStruct (pg_ref pg, long ref_id);

By "storing" an arbitrary value within a `pg_ref` is meant that OpenPaige will save longs or pointers — which only have significance to your application — which can be retrieved later at any time.

To store such items, call `pgSetExtraStruct`, passing your long (or pointer) in `extra_struct` and a unique ID number in `ref_id`. The purpose of the unique id is to reference that item later in `pgGetExtraStruct`.

However, if the value in `ref_id` is already being used by an "extra struct" item within `pg`, the old value is overwritten with `extra_struct`. (Hence, that is how you can "change" a value that had previously been stored).

To retrieve an item stored with `pgSetExtraStruct`, call `pgGetExtraStruct` passing the wanted ID in `ref_id` (which must be the same number given to `unique_id` for that item originally given to `pgSetExtraStruct`).

See "OpenPaige "Handler” Functions”<!-- on page 34-3-->.

### TECH NOTE (Removing ExtraStruct)

> Why is there no `pgRemoveExtraStruct()`?

Probably because of the way it was implemented and what it is/was intended for doesn't make sense to do a "remove."

An "extra `struct`", as far as OpenPaige is concerned, is a single element of an array of `long`s. Each of these `long`s are treated as `refcon` values that an application can use for whatever.

Literally, the list of extra `struct`s are maintained internally as `long[n]` where `n` is the number of extra `struct`s added.

The array number itself, e.g. 0, 1, 2, etc., is the "ID number" of the extra `struct`. That is what makes each one unique, really. Hence you can see why we could not really "delete" one of these elements since that would cause all subsequent extra `struct` elements to be a different "ID" number.

For example, if a `pg_ref` holds elements 0, 1, 2, 3, and 4 (all with same corresponding ID numbers), deleting 2 would make 3 become 2 and 4 become 3.

We realise a more elaborate system could have been implemented that contained indirect pointers, or some other scheme that is closer to what (I think) you are suggesting, so extra `struct`s could be deleted.

But, the original purpose of this feature was simply to add extra `refCon` possibilities. It might make more sense if we called the function something like `pgReserveAnotherLongRefCon`.

### Finding a Unique ID

If you aren't sure whether or not an ID number is unique for a `pg_ref`, or if you simply want to get an ID number that you know is unique, call the following:

	(long) pgExtraUniquelD (pg_ref pg);

The number this function returns will always be positive and is guaranteed to have not yet been used for `pgSetExtraStruct` with this `pg_ref`.

**CAUTION:** OpenPaige has no idea what you are storing with `pgSetExtraStruct`, and therefore will not dispose any memory allocations that you might have attached to "extra `struct`" storage. Be sure to dispose any such allocations before disposing the `pg_ref` or you will end up with a memory leak.

NOTE: Once you have stored something with `pgExtraStruct`, that item (and unique reference) stays in the `pg_ref` and never gets "removed" unless you
explicitly do another `pgSetExtraStruct` using the same ID (in which case the previous item associated with that ID will get overwritten).

### EXAMPLE (How to use and extra struct)

	/* This function adds a WindowPtr to the OpenPaige object using 
	   the extra struct feature and returns the ID of that struct */
	
	short add_window_to_pg (pg_ref pg, WindowPtr w_ptr)
	{
		short unique_id;
		unique_id = pgExtraUniqueID(pg);
		pgSetExtraStruct(pg, w_ptr, unique_id);
		return unique_id;
	}
	
	/* Later, the extra struct can be accessed using the ID returned above. */
	
	WindowPtr window_with_pg;
	window_with_pg = pgGetExtraStruct(pg, unique_id);

## 3.11 Cursor Utilities

If you want to know if a point (`co_ordinate`) sits on top of editable text (to change the mouse symbol to something else, for instance), call the following:

	(short) pgPtInView (pg_ref pg, co_ordinate_ptr point, co_ordinate_ptr offset_extra);

Given an arbitrary window coordinate (relative to that window's coordinate system) in point, `pgPtInView` returns information about what part of `pg`, if any, includes that point.

The `offset_extra` parameter is an optional pointer to a coördinate that holds values to temporarily offset everything in `pg` before checking intersections of the point. In other words, if `offset_extra` is non-null, this visual area in `pg` will first be offset by `offset_extra.h` and `offset_extra.v` amounts before checking the containment of point in `vis_area`; the wrap area will also be offset by this amount before checking if the wrap area contains the point, and so on.

If `offset_extra` is a null pointer, everything is checked as-is.

**FUNCTION RESULT:** The function result will be a word containing different bits set (or not) indicating what intersects the point as follows:

	#define WITHIN_VIS_AREA		0x0001	// Point within vis_area
	#define WITHIN_WRAP_AREA	0x0002	// Point within page_area
	#define WITHIN_EXCLUDE_AREA	0x0004	// Point within exclude_area
	#define WITHIN_TEXT			0x0008	// Point within actual text
	#define WITHIN_REPEAT_AREA	0x0010	// Point is in repeating gap of page
	#define WITHIN_LEFT_AREA	0x0020	// Point is left of document
	#define WITHIN_RIGHT_AREA	0x0040	// Point is right of document
	#define WITHIN_TOP_AREA		0x0080	// Point is above top of document
	#define WITHIN_BOTTOM_AREA	0x0100 // Point is below bottom of document
	
`WITHIN_VIS_AREA` means the point is within the bounding area of `vis_area`.

`WITHIN_WRAP_AREA` means the point is somewhere within the `page_area` shape.

`WITHIN_EXCLUDE_AREA` means the point is somewhere within the `exclude_area`.

`WITHIN_TEXT` means the point is somewhere within "real" text. This differs from `WITHIN_WRAP_AREA` since it is possible to have a large `page_area` shape with very little text (in which case, `WITHIN_TEXT` will only be set if the point is over the portion that displays text).

Each bit gets set notwithstanding the other settings. For example, `WITHIN_EXCLUDE_AREA` and `WITHIN_WRAP_AREA` can both be set, even though text cannot flow into the `exclude_area`.

Another setting that can be returned is `WITHIN_TEXT` set but `WITHIN_VIS_AREA` not set, which really means the point is over text that falls outside of `vis_area`. The function result is simply the setting for each case individually, so it is your responsibility to examine the combination of bits to determine what action you should take, if any.

**NOTE:** The best time to turn the cursor to an "i-beam" is when `pgPtInView` returns `WITHIN_VIS_AREA` and `WITHIN_TEXT` at the same time and `pg` is in an active state.

## 3.12 Getting Text Size and Height

To obtain the total size of text in an OpenPaige object (in bytes), call the following:

	(long) pgTextSize (pg_ref pg);

**FUNCTION RESULT:** This function returns the total size of text (byte size) in `pg`.

To find out how "tall" the text is, call the following:

	(long) pgTotalTextHeight (pg_ref pg, pg_boolean paginate)

**FUNCTION RESULT:** This function returns the distance between the top of the first line of text to the bottom of the lowest line, in pixels.

**NOTE:** The lowest line is not necessarily the last line in the document: had `pg` had a non-rectangular shape, such as parallel columns, the last (ending) line could have been vertically above some of the lines in other areas of the shape. Hence, `pgTotalTextHeight` really returns the bounding height between the highest and lowest points.

If paginate is "TRUE," all the text from top to bottom is recalculated (word wrap recomputed), if necessary. If paginate is "FALSE," the total text height returned is computed with the latest information available within pg. In essence, this would be OpenPaige's "best guess."

For example, suppose a large document changed from 12 point text to 18 point text and you wanted to know how tall the document had become. To get the exact height, to the nearest pixel, you should pass TRUE for paginate, otherwise OpenPaige might not have computed all the text to return an exact answer. However, computing large amounts of text can consume a great deal of time, which is why the choice to "paginate" or not has been provided.

##### NOTES:

1. If you will be using the built-in scrolling support in OpenPaige, you probably never need to get the height of an OpenPaige object — see "All About Scrolling"<!-- on page 11-1-->. If you do need an exact height for other reasons, see "Getting the Max Text Bounds”<!-- on page 24-24-->.
2. The "height" returned from this function does not consider any extra structures that aren't embedded in the text stream. For example, if you have implemented headers, footers, footnotes, or any other page "ornaments" their placement will not be considered in the text height computation.
