#ifndef CPLOT_H
#define CPLOT_H

#include "acb.h"
#include "image.h"
#include <stdbool.h>

typedef void (*cplot_func_t)(acb_t res, const acb_t z, slong prec);

/*
    Structure for metadata of plot, specifies:
    - Maximum Precision for calculation
    - Range of plot
*/
typedef struct cplot_meta
{
    slong maxprec;
    arf_struct xa, xb, ya, yb;    
} cplot_meta;
typedef cplot_meta cplot_meta_t[1];

void cplot_meta_init(cplot_meta_t meta);
void cplot_meta_range_set_d(cplot_meta_t meta, double xa, double xb, double ya, double yb);
void cplot_meta_clear(cplot_meta_t meta);

void cplot_domain_plot(cplot_img_t res, cplot_func_t func, cplot_color_func_t color_func, cplot_meta_t meta);

#endif