#include "cplot/plot.h"
#ifdef CPLOT_USE_PARALLEL_LOOPS
#include <omp.h>
#endif

void cplot_meta_range_set_d(cplot_meta_t meta, double xa, double xb, double ya, double yb)
{
    arf_set_d(&meta->xa, xa); arf_set_d(&meta->xb, xb);
    arf_set_d(&meta->ya, ya); arf_set_d(&meta->yb, yb);
}

void cplot_meta_init(cplot_meta_t meta)
{
    meta->maxprec = 128;
    arf_init(&meta->xa); arf_init(&meta->xb);
    arf_init(&meta->ya); arf_init(&meta->yb);  
    cplot_meta_range_set_d(meta, -5, 5, -5, 5);
}

void cplot_meta_clear(cplot_meta_t meta)
{
    arf_clear(&meta->xa); arf_clear(&meta->xb);
    arf_clear(&meta->ya); arf_clear(&meta->yb);
}

void cplot_domain_plot(cplot_img_t res, cplot_func_t func, cplot_color_func_t color_func, cplot_meta_t meta)
{
    slong sx,sy;
    sx = cplot_img_get_x(res); sy = cplot_img_get_y(res);
    
    #ifdef CPLOT_USE_PARALLEL_LOOPS
    #pragma omp parallel 
    #endif
    {
        acb_t z, w;
        acb_init(z); acb_init(w);
    
        #ifdef CPLOT_USE_PARALLEL_LOOPS
        #pragma omp for collapse(2)
        #endif
        for (slong y = 0; y < sy; y++)
        {
            for (slong x = 0; x < sx; x++)
            {
                for (slong prec = 10; prec < meta->maxprec; prec *= 2)
                {
                    arf_sub(arb_midref(acb_imagref(z)), &meta->yb, &meta->ya, prec, ARF_RND_DOWN);
                    arf_mul_ui(arb_midref(acb_imagref(z)), arb_midref(acb_imagref(z)), y, prec, ARF_RND_DOWN);
                    arf_div_ui(arb_midref(acb_imagref(z)), arb_midref(acb_imagref(z)), sy-1, prec, ARF_RND_DOWN);
                    arf_add(arb_midref(acb_imagref(z)), arb_midref(acb_imagref(z)), &meta->ya, prec, ARF_RND_DOWN);

                    arf_sub(arb_midref(acb_realref(z)), &meta->xb, &meta->xa, prec, ARF_RND_DOWN);
                    arf_mul_ui(arb_midref(acb_realref(z)), arb_midref(acb_realref(z)), x, prec, ARF_RND_DOWN);
                    arf_div_ui(arb_midref(acb_realref(z)), arb_midref(acb_realref(z)),sx-1, prec, ARF_RND_DOWN);
                    arf_add(arb_midref(acb_realref(z)), arb_midref(acb_realref(z)), &meta->xa, prec, ARF_RND_DOWN);
                    
                    func(w, z, prec);

                    if (acb_rel_accuracy_bits(w) > 4) 
                        break;
                }

                color_func(cplot_img_get_rgb(res,x,y), w, 32);
            }
        }
        
        acb_clear(z);
        acb_clear(w);
        flint_cleanup();
    }
}
