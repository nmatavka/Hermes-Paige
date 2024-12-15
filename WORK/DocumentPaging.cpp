#ifndef DOCUMENTPAGING_H
#define DOCUMENTPAGING_H

#include "Paige.h"
#include "pgMemMgr.h"
#include <sys/stat.h>

extern paige_rec_ptr paigeDoc;

#endif // DOCUMENTPAGING_H

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

file_ref GetCacheFileRef(pg_ref doc) {
    if (doc) {
        return pgGetCacheFileRef(doc);
    }
    return NULL;
}

pg_error SaveDocumentWithPaging(pg_ref doc, const char* file_path, const pg_file_key_ptr keys, pg_short_t num_keys, file_io_proc write_proc, long doc_element_info) {
    if (!doc) {
        return BAD_TYPE_ERR;
    }

    int file_ref = _lcreat(file_path, 0);
    if (file_ref == -1) {
        return FILE_NOT_FOUND_ERR;
    }

    memory_ref file_map = MemoryAlloc(&globals->mem_globals, sizeof(int), 1, 0);
    int* f_ptr = (int*)UseMemory(file_map);
    *f_ptr = file_ref;
    UnuseMemory(file_map);

    long position = 0;
    pg_error error = pgCacheSaveDoc(doc, &position, keys, num_keys, write_proc, file_map, doc_element_info);

    DisposeMemory(file_map);
    _lclose(file_ref);

    return error;
}
