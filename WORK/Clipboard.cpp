#include "Paige.h"
#include "pgMemMgr.h"

extern paige_rec_ptr paigeDoc;

void CopyText() {
    if (paigeDoc) {
        if (OpenClipboard(NULL)) {
            if (pgScrapAvail(RegisterClipboardFormat("HERMES Paige"))) {
                pg_ref scrap = pgGetScrap(&globals->mem_globals, RegisterClipboardFormat("HERMES Paige"), NULL);
                if (scrap) {
                    pgPaste(paigeDoc, scrap, CURRENT_POSITION, false, best_way);
                    pgDispose(scrap);
                }
            }
            CloseClipboard();
        }
    }
}

void PasteText() {
    if (paigeDoc) {
        if (OpenClipboard(NULL)) {
            if (pgScrapAvail(RegisterClipboardFormat("HERMES Paige"))) {
                pg_ref scrap = pgGetScrap(&globals->mem_globals, RegisterClipboardFormat("HERMES Paige"), NULL);
                if (scrap) {
                    pgPaste(paigeDoc, scrap, CURRENT_POSITION, false, best_way);
                    pgDispose(scrap);
                }
            }
            CloseClipboard();
        }
    }
}

void CutText() {
    if (paigeDoc) {
        pgPrepareUndo(paigeDoc, undo_cut, NULL);
        pgCutToClipboard(paigeDoc, NULL, 0, best_way);
    }
}

void DeleteText() {
    if (paigeDoc) {
        long start, end;
        pgGetSelection(paigeDoc, &start, &end);
        if (start != end) {
            pgDelete(paigeDoc, &start, best_way);
        }
    }
}
