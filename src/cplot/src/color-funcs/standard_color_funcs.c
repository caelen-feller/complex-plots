#include "cplot/color.h"
#define PI 3.1415926535898

void cplot_color_func_base(const acb_t z, arb_t t, arb_t u, double* R, double* G, double* B, double* H, slong prec)
{
    if (!acb_is_finite(z) || acb_rel_accuracy_bits(z) < 4)
    {
        *R = *G = *B = 1;
        return;
    }

    arf_set_round(arb_midref(t), arb_midref(acb_realref(z)), prec, ARF_RND_DOWN);
    arf_set_round(arb_midref(u), arb_midref(acb_imagref(z)), prec, ARF_RND_DOWN);

    arb_atan2(t, u, t, prec);

    *H = arf_get_d(arb_midref(t), ARF_RND_DOWN);
}

void cplot_color_func(cplot_RGB_t out, const acb_t z, slong prec) 
{
    double *R,*G,*B;
    R = &out->R;
    G = &out->G;
    B = &out->B;
    double H, L, S; 
    arb_t t, u;
    arb_init(t); arb_init(u);

    cplot_color_func_base(z, t, u, R, G, B, &H, prec);

    H = (H + PI) / (2 * PI) + 0.5;
    H = H - floor(H);

    acb_abs(t, z, prec);

    if (arf_cmpabs_2exp_si(arb_midref(t), 200) > 0)
    {
        L = 1.0;
    }
    else if (arf_cmpabs_2exp_si(arb_midref(t), -200) < 0)
    {
        L = 0.0;
    }
    else
    {
        L = arf_get_d(arb_midref(t), ARF_RND_DOWN);
        L = 1.0 - 1.0/(1.0 + pow(L, 0.1));
    }

    S = 0.8;

    cplot_color_hls_to_rgb(R,G,B, &H, &L, &S);

    arb_clear(t);
    arb_clear(u);
}

void cplot_color_func_phase(cplot_RGB_t out, const acb_t z, slong prec)
{   
   double *R,*G,*B;
    R = &out->R;
    G = &out->G;
    B = &out->B;
    double H, L, S; 
    arb_t t, u;
    arb_init(t); arb_init(u);

    cplot_color_func_base(z, t, u, R, G, B, &H, prec);

    H = (H + PI) / (2 * PI) + 0.5;
    H = H - floor(H);

    acb_abs(t, z, prec);

    L = 0.5;
    S = 0.8;

    cplot_color_hls_to_rgb(R,G,B, &H, &L, &S);

    arb_clear(t);
    arb_clear(u);
}