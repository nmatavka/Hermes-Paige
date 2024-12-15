#include <windows.h>
#include "Paige.h"
#include "pgMemMgr.h"
#include "pghtext.h"

extern paige_rec_ptr paigeDoc;

void pgDrawScrollProc(paige_rec_ptr pg, shape_ref update_rgn, co_ordinate_ptr scroll_pos, pg_boolean post_call) {
    if (pg && update_rgn) {
        HDC hdc = GetDC((HWND)pg->port.window);
        if (hdc) {
            // Repaint the uncovered area using the update region
            FillRgn(hdc, (HRGN)update_rgn, (HBRUSH)(COLOR_WINDOW + 1));
            ReleaseDC((HWND)pg->port.window, hdc);
        }
    }
}

void pgDrawPageProc(paige_rec_ptr pg, shape_ptr page_shape, pg_short_t r_qty, pg_short_t page_num, co_ordinate_ptr vis_offset, short draw_mode_used, short call_order) {
    HDC hdc = GetDC((HWND)pg->port.window);
    if (hdc) {
        for (pg_short_t i = 0; i < r_qty; ++i) {
            RECT rect;
            rect.left = page_shape[i].left + vis_offset->h;
            rect.top = page_shape[i].top + vis_offset->v;
            rect.right = page_shape[i].right + vis_offset->h;
            rect.bottom = page_shape[i].bottom + vis_offset->v;

            // Draw a rectangle around each page area
            FrameRect(hdc, &rect, (HBRUSH)GetStockObject(GRAY_BRUSH));

            // Draw additional ornaments or floating images
            if (call_order < 0) {
                // Example: Draw floating images or other elements
                // Custom drawing logic for floating images
            }
        }
        ReleaseDC((HWND)pg->port.window, hdc);
    }
}

void PrintDocument(HDC out_dc, pg_ref pg, rectangle_ptr page_rect, double scale_factor) {
    graf_device print_port;
    pgInitDevice(&globals->mem_globals, MEM_NULL, out_dc, &print_port);

    // Adjust the print resolution for scaling
    AdjustPrintResolution(&print_port, out_dc, scale_factor);

    long starting_position = 0;
    short draw_mode = best_way;

    // Set the print device to match the printer's resolution
    pgSetPrintDevice(pg, (generic_var)out_dc);

    // Print the document
    while (starting_position != -1) {
        starting_position = PrintToPage(pg, &print_port, starting_position, page_rect, draw_mode);
    }

    // Clear the print device after printing
    pgSetPrintDevice(pg, MEM_NULL);
}

void AdjustPrintResolution(graf_device_ptr print_port, HDC out_dc, double scale_factor) {
    if (print_port) {
        long print_x = GetDeviceCaps(out_dc, LOGPIXELSX);
        long print_y = GetDeviceCaps(out_dc, LOGPIXELSY);

        // Adjust resolution based on the scale factor
        print_x = static_cast<long>(print_x * scale_factor);
        print_y = static_cast<long>(print_y * scale_factor);

        print_port->resolution = (print_x << 16) | print_y;
    }
}
