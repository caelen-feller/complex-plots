#include "cplot/plot.h"
#include "flint/profiler.h"

void test_func(acb_t res, acb_t const z, slong prec)
{
    acb_set(res, z);
}

int main() 
{
    cplot_img_t image;
    cplot_color_func_t color_func = cplot_color_func;
    image = cplot_img_init(500, 500);
    
    double n = 1;
    cplot_meta_t meta;
    cplot_meta_init(meta);
    cplot_meta_range_set_d(meta, -n,n,-n,n);
    meta->maxprec = 128;

    TIMEIT_ONCE_START
    cplot_domain_plot(image, test_func, color_func, meta);
    TIMEIT_ONCE_STOP

    cplot_img_save(image, "t_plot", "jpeg");

    // Present to user, ask for input? 
    flint_printf("PASS\n");
    cplot_img_clear(image);
    cplot_meta_clear(meta);
    flint_cleanup();
    return EXIT_SUCCESS;
}