#include "Paige.h"
#include "pgMemMgr.h"
#include "Document.h"

extern paige_rec_ptr paigeDoc;

void SetTableAttributes(paige_rec_ptr doc, long start, long end, const pg_table& table_attrs, pg_boolean redraw) {
    if (doc) {
        select_pair selection = {start, end};
        par_info info;
        pgGetParInfo(doc, &selection, &info);
        info.table = table_attrs;
        pgSetParInfo(doc, &selection, &info, NULL, redraw);
    }
}

pg_boolean GetTableAttributes(paige_rec_ptr doc, long position, pg_table& table_attrs) {
    if (doc) {
        select_pair selection = {position, position};
        par_info info;
        if (pgGetParInfo(doc, &selection, &info)) {
            table_attrs = info.table;
            return TRUE;
        }
    }
    return FALSE;
}

void InsertTable(paige_rec_ptr doc, long position, const pg_table& table_attrs, long row_qty, short draw_mode) {
    if (doc) {
        pgInsertTable(doc, position, &table_attrs, row_qty, draw_mode);
    }
}

void SetColumnWidth(paige_rec_ptr doc, long position, short column_num, short width, short draw_mode) {
    if (doc) {
        pgSetColumnWidth(doc, position, column_num, width, draw_mode);
    }
}

void SetColumnBorders(paige_rec_ptr doc, long position, short column_num, long border_info, short draw_mode) {
    if (doc) {
        pgSetColumnBorders(doc, position, column_num, border_info, draw_mode);
    }
}

void SetColumnShading(paige_rec_ptr doc, long position, short column_num, long shading, short draw_mode) {
    if (doc) {
        pgSetColumnShading(doc, position, column_num, shading, draw_mode);
    }
}

void SetColumnAlignment(paige_rec_ptr doc, long position, short column_num, short alignment, short draw_mode) {
    if (doc) {
        pgSetColumnAlignment(doc, position, column_num, alignment, draw_mode);
    }
}

pg_boolean IsTable(paige_rec_ptr doc, long position) {
    if (doc) {
        return pgIsTable(doc, position);
    }
    return FALSE;
}

pg_boolean PointInTable(paige_rec_ptr doc, co_ordinate_ptr point, select_pair_ptr offsets) {
    if (doc) {
        return pgPtInTable(doc, point, FALSE, offsets);
    }
    return FALSE;
}

memory_ref TableColumnWidths(paige_rec_ptr doc, long position) {
    if (doc) {
        return pgTableColumnWidths(doc, position);
    }
    return MEM_NULL;
}

void CellOffsets(paige_rec_ptr doc, long position, select_pair_ptr offsets) {
    if (doc) {
        pgCellOffsets(doc, position, offsets);
    }
}

void TableOffsets(paige_rec_ptr doc, long position, select_pair_ptr offsets) {
    if (doc) {
        pgTableOffsets(doc, position, offsets);
    }
}

void InsertColumn(paige_rec_ptr doc, long position, short column_num, tab_stop_ptr info, short draw_mode) {
    if (doc) {
        pgInsertColumn(doc, position, column_num, info, draw_mode);
    }
}

void InsertRow(paige_rec_ptr doc, long position, long row_num, short draw_mode) {
    if (doc) {
        pgInsertRow(doc, position, row_num, draw_mode);
    }
}

void DeleteColumn(paige_rec_ptr doc, long position, short column_num, short draw_mode) {
    if (doc) {
        pgDeleteColumn(doc, position, column_num, draw_mode);
    }
}

void DeleteRow(paige_rec_ptr doc, long position, long row_num, short draw_mode) {
    if (doc) {
        pgDeleteRow(doc, position, row_num, draw_mode);
    }
}

short NumColumns(paige_rec_ptr doc, long position) {
    if (doc) {
        return pgNumColumns(doc, position);
    }
    return 0;
}

long NumRows(paige_rec_ptr doc, long position) {
    if (doc) {
        return pgNumRows(doc, position);
    }
    return 0;
}
