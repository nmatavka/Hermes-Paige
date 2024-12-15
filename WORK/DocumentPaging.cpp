#include "Paige.h"
#include "pgMemMgr.h"
#include <sys/stat.h>

extern paige_rec_ptr paigeDoc;

pg_ref LoadDocument(const char* file_path) {
    struct stat file_stat;
    if (stat(file_path, &file_stat) != 0) {
        return NULL;
    }

    int file_ref = _lopen(file_path, OF_READ);
    if (file_ref != -1) {
        memory_ref file_map = MemoryAlloc(&globals->mem_globals, sizeof(int), 1, 0);
        int* f_ptr = (int*)UseMemory(file_map);
        *f_ptr = file_ref;
        UnuseMemory(file_map);
        long position = 0;
        pg_ref doc = pgNewShell(&globals->mem_globals);

        pg_error error;
        if (file_stat.st_size > 1024 * 1024) { // If file is larger than 1MB, use paging
            error = pgCacheReadDoc(doc, &position, NULL, 0, NULL, file_map);
        } else { // Otherwise, load the entire document
            error = pgReadDoc(doc, &position, NULL, 0, NULL, file_map);
        }

        DisposeMemory(file_map);
        _lclose(file_ref);
        if (error != NO_ERROR) {
            pgDispose(doc);
            return NULL;
        }
        return doc;
    }
    return NULL;
}
