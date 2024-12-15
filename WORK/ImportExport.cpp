#include <windows.h>
#include "Paige.h"
#include "pgMemMgr.h"

extern paige_rec_ptr paigeDoc;

void ImportToPaigeControl(HWND hwnd, pg_file_unit f_ref, long feature_flags, long file_begin) {
    pg_ref pg = (pg_ref)SendMessage(hwnd, PG_GETPGREF, 0, 0);
    if (pg) {
        // Define a custom character mapping table
        unsigned char customCharTable[] = {
            0xAA, 0xBB, 0xCC, // Example mappings
            // Add more mappings as needed
        };

        // Create the import object and set the custom character table
        PaigeImportObject filter = (PaigeImportObject)new PaigeRTFImportFilter();
        filter->character_table = (pg_char_ptr)customCharTable;

        pg_error result = pgImportFileFromC(pg, pgDetermineFileType(f_ref, NULL, file_begin), feature_flags, file_begin, f_ref);
        if (result == NO_ERROR) {
            SendMessage(hwnd, PG_REALIZEIMPORT, TRUE, 0);
        } else {
            MessageBox(hwnd, "Failed to import file.", "Error", MB_OK | MB_ICONERROR);
        }
    }
}
