#include "cplot/color.h"
#define PI 3.1415926535898

void Hcomplex(double* H, const acb_t z,slong prec)
{
    arb_t t, u;
    arb_init(t); arb_init(u);

    arf_set_round(arb_midref(t), arb_midref(acb_realref(z)), prec, ARF_RND_DOWN);
    arf_set_round(arb_midref(u), arb_midref(acb_imagref(z)), prec, ARF_RND_DOWN);

    arb_atan2(t, u, t, prec);

    *H = arf_get_d(arb_midref(t), ARF_RND_DOWN);
    *H = (*H) / (2 * PI);
    *H = *H - floor(*H);

    arb_clear(t);
    arb_clear(u);
}

#define CLAMP(y) FLINT_MAX(0.0, FLINT_MIN((y), 1.0))

#define FRACTIONAL_PART(x, m, M) ((m) + ((M) - (m)) * (-(x) + ceil((x)) ))
#define INVERSE_FRACTIONAL_PART(x, m, M) ((m) + ((M) - (m)) * (-(x) + ceil((x)) ))

void cplot_color_func_radial_log(cplot_RGB_t out, const acb_t z, slong prec) 
{
    double H, S, V;

    if (!acb_is_finite(z) || acb_rel_accuracy_bits(z) < 4)
    {
        out->R = out->G = out->B = 1;
        return;
    }

    Hcomplex(&H, z, prec);
    
    arb_t abs_z; arb_init(abs_z); 
    acb_abs(abs_z, z, prec);
    
    if (arf_cmpabs_2exp_si(arb_midref(abs_z), 200) > 0)
    {   
        S=0.0; V=1.0;
    } 
    else if (arf_cmpabs_2exp_si(arb_midref(abs_z), -200) < 0)
    {
        S=1.0; V=0.0;
    }
    else 
    {
        arb_log_base_ui(abs_z, abs_z, 2, prec);
        
        V = arf_get_d(arb_midref(abs_z), ARF_RND_DOWN);
        V = FRACTIONAL_PART(V, 0.0, 1.0);
        V = pow(V,0.2);
        
        S = 0.9;
    }

    cplot_color_hsv_to_rgb(&out->R, &out->G, &out->B, &H, &S, &V);
    arb_clear(abs_z);
}

void cplot_color_func_radial(cplot_RGB_t out, const acb_t z, slong prec) 
{
    double H, S, V;

    if (!acb_is_finite(z) || acb_rel_accuracy_bits(z) < 4)
    {
        out->R = out->G = out->B = 1;
        return;
    }

    Hcomplex(&H, z, prec);
    
    arb_t abs_z; arb_init(abs_z); 
    acb_abs(abs_z, z, prec);
    
    if (arf_cmpabs_2exp_si(arb_midref(abs_z), 200) > 0)
    {   
        S=0.0; V=1.0;
    } 
    else if (arf_cmpabs_2exp_si(arb_midref(abs_z), -200) < 0)
    {
        S=1.0; V=0.0;
    }
    else 
    {        
        V = arf_get_d(arb_midref(abs_z), ARF_RND_DOWN);
        V = FRACTIONAL_PART(V, 0.0, 1.0);
        V = pow(V,0.2);
        
        S = 0.9;
    }

    cplot_color_hsv_to_rgb(&out->R, &out->G, &out->B, &H, &S, &V);
    arb_clear(abs_z);
}

void cplot_color_func_conformal(cplot_RGB_t out, const acb_t z, slong prec)
{
    double H, S, V;

    if (!acb_is_finite(z) || acb_rel_accuracy_bits(z) < 4)
    {
        out->R = out->G = out->B = 1;
        return;
    }

    Hcomplex(&H, z, prec);
    
    arb_t abs_z; arb_init(abs_z); 
    acb_abs(abs_z, z, prec);
    
    if (arf_cmpabs_2exp_si(arb_midref(abs_z), 200) > 0)
    {   
        S=0.0; V=1.0;
    } 
    else if (arf_cmpabs_2exp_si(arb_midref(abs_z), -200) < 0)
    {
        S=1.0; V=0.0;
    }
    else 
    {   
        arb_log(abs_z, abs_z, prec);
        double M,m,n;
        m = 0.7; M = 1.0; n=15.0;
        V = arf_get_d(arb_midref(abs_z), ARF_RND_DOWN);
        V = FRACTIONAL_PART((V*n)/(2 * PI), m, M);
        V *= FRACTIONAL_PART(H*n,m,M);        
        S = 0.9;
    }

    cplot_color_hsv_to_rgb(&out->R, &out->G, &out->B, &H, &S, &V);
    arb_clear(abs_z);
}

void cplot_color_func_grid(cplot_RGB_t out, const acb_t z, slong prec)
{
    double H, S, V;

    if (!acb_is_finite(z) || acb_rel_accuracy_bits(z) < 4)
    {
        out->R = out->G = out->B = 1;
        return;
    }

    Hcomplex(&H, z, prec);
    
    arb_t abs_z; arb_init(abs_z); 
    acb_abs(abs_z, z, prec);
    
    if (arf_cmpabs_2exp_si(arb_midref(abs_z), 200) > 0)
    {   
        S=0.0; V=1.0;
    } 
    else if (arf_cmpabs_2exp_si(arb_midref(abs_z), -200) < 0)
    {
        S=1.0; V=0.0;
    }
    else 
    {   
        // arb_log(abs_z, abs_z, prec);
        double M,m,n;
        m = 0.7; M = 1.0; n=15.0;
        V = FRACTIONAL_PART(arf_get_d(arb_midref(acb_imagref(z)), ARF_RND_DOWN), m, M);
        V *= FRACTIONAL_PART(arf_get_d(arb_midref(acb_realref(z)), ARF_RND_DOWN), m, M);
        S = 0.9;
    }

    cplot_color_hsv_to_rgb(&out->R, &out->G, &out->B, &H, &S, &V);
    arb_clear(abs_z);
}