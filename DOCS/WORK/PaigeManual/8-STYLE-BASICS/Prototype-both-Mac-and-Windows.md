## Prototype (both Mac and Windows)

```c
#include "pgHLevel.h"
void pgGetStyleBits (pg_ref pg, long PG_FAR *style_bits, long PG_FAR *consistent_bits);
```

This function returns the style(s) that are applied to the text currently highlighted in `pg` (or, if nothing is highlighted, the style(s) that apply to the current insertion point are returned).

The style(s) are returned in `*style_bits` (which must not be a null pointer); the value of `*style_bits` will be a composite of one or more of the style bits as defined in `pgSetStyleBits` (above).

The `*consistent_bits` parameter will also get set to the style(s) that remains consistent throughout the selected text; if a style bit in `consistent_bits` is set to a "1", that corresponding bit value in `*style_bits` is the same throughout the selected text.

For example, if `*style_bits` returns with all 0's, yet `*consistent_bits` is set to all 1's, the selection is purely "plain text" (no styles are set). However, if `*style_bits` returned all 0's but `*consistent_bits` was *not* all 1's, the text is not "plain text," rather the bits that are 0 in `*consistent_bits` reveal that style is not the same throughout the whole selection.

NOTE: The `consistent_styles` parameter must not be a null pointer.

