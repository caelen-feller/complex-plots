#ifndef CPLOT_ANIM_H
#define CPLOT_ANIM_H

#include "plot.h"

typedef struct cplot_anim cplot_anim;
typedef cplot_anim * cplot_anim_t;

// TODO: Implement saving mathods

// TODO Implement parallel animation
void animated_plot(cplot_anim_t res, cplot_func_t func, cplot_color_func_t color_func, cplot_meta_t meta);

#endif