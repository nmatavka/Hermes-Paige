## Note

All the `Get` functions, however (`pgGetStyleInfo`, `pgGetParInfo`, etc.) are always safe to call.

One exception: It is OK (often anticipated) to call `pgSetExtraStruct` while executing a low-level hook.

