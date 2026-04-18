# 22 FILE STANDARDS, INPUT, AND OUTPUT

**NOTE:** If you will only be saving files as HERMES Paige native format or RTF and will be including no customised file formatting, see chapters 17, [HERMES Paige Import Extension](#17-hermes-paige-import-extension), and 18, [HERMES Paige Export Extension](#18-hermes-paige-export-extension). Importing and exporting may be a simpler approach.

The HERMES Paige technology includes a file handling system to help implement the following:

- *Platform-independent file transfers* — a proposed standard and function set that enables HERMES Paige software to read files saved by other C.P.O.S. as well as save or re-save files to be understood in reverse.
- *Upgrade/update independent file transfers* — the proposed standard guarantees upward and even backwards compatibility for future enhancements to HERMES Paige with regards to file transfer. For example, every internal record structure, including style records, can theoretically be altered and enhanced, yet older files will still be loaded correctly and older software will even be able to read the newer files (eliminating, of course, any new feature set that was inherently saved).
- *Application-independent file transfers* — diverse applications, even on the same platforms, are able to read file saved by other applications even if unknown elements have been saved. Using HERMES Paige's file transfer methods, application-specific data embedded in the file is simply "skipped" without any adverse consequences.
- *Subset of functions for app-specific saves* — HERMES Paige makes it fairly easy to save and read your own data structures along with the HERMES Paige object data, all the while maintaining compatibility with each concept listed above.
- *Preserves HERMES Paige structures that have `long`s* — If you roll your own I/O, HERMES Paige structures containing long words would get flipped around (backwards). For example, if you just slam an HERMES Paige `struct` to a file as a byte stream it won't work on the other end. Fortunately HERMES Paige's built-in I/O handler takes care of this problem. I strongly recommend you utilize the file "key" system provided. If special/custom I/O is required anywhere, take a look at the latest release notes regarding files—there are now ways to "roll your own" while still using HERMES Paige's system.

