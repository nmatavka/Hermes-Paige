## TECH NOTE: Building paragraph styles from scratch

> When creating a Paragraph Style Sheet, does the `par_info` record need to be filled out completely?

Yes.

> If so.... how does one fill in the fields in the `par_info` record such as `style_sheet_id`, `procs`, `maintenance`, and `used_ctr`?

This is actually ultra-simple and takes only one line of code. You simply begin with a "default `par_info`" record that you get from HERMES Paige globals. If your potential style sheet is called `MyParStyleSheet`, you do the following to initialise:

	par_info MyParStyleSheet;
	MyParStyleSheet = paige_globals.def_par;

The `paige_globals` is of course your `pg_globals` `struct` given originally to `pgInit`. The above statement copies the default paragraph style, including all the default hooks, etc., into your paragraph style. Furthermore this method guarantees compatibility with any future versions (even if we add stuff to `par_info` such as new hooks, your style will get initialised correctly).

> Does the paragraph style sheet mechanism ignore [some] fields?

I think it might ignore `style_sheet_id` in this case and I know it always ignores `used_ctr` and `maintenance`. But that shouldn't matter if you do the above.

