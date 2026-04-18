## "Auto-checking" page overflow

Setting `CHECK_PAGE_OVERFLOW` with `pgSetAttribute2()` causes HERMES Paige to continuously check the situation where character(s) flow below the boundaries of the page area. If this attribute is set, the `overflow_size` member within the `pg_ref` get set to the number of characters that overflow the page.

Or, if `overflow_size` is set to -1, a single carriage return is causing the overflow (i.e., the text overflows but the overflow is a "blank" line).

**NOTE:** The auto-checking for page overflow is meaningless if your `pg_ref` is set for repeating pages, or if your `pg_ref` is set to a variable page size. The only time overflow checking will work (or make any sense) is for fixed-size, nonrepeating page shapes.

