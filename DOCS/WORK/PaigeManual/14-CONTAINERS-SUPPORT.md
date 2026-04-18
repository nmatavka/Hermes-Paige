# 14 CONTAINERS SUPPORT

HERMES Paige has some built-in support for this purpose by providing several functions to insert, delete and change a list of rectangles that constitute `page_area`, as well as the ability to attach an application-defined reference to each "container" of the shape.

The term *container* is used to describe a rectangular portion of the `page_area`. For an application to support page-layout text containers, the typical method is to build the `page_area` (the shape in a `pg_ref` in which text will flow) with the desired series of rectangles.

