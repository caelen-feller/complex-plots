#ifndef CPLOT_COLOR_FUNCS_H
#define CPLOT_COLOR_FUNCS_H

#include "acb.h"

/* 
    Output type for Color Function
    Consider restructure to hidden types or getters/setters to use fancier storage methods.
*/ 
typedef struct cplot_RGB
{
    double R,G,B;
} cplot_RGB;
typedef cplot_RGB *cplot_RGB_t;

typedef void (*cplot_color_func_t)(cplot_RGB_t, const acb_t,slong);

/* 
    Color Functions 
*/

/*    Standard Functions    */
void cplot_color_func(cplot_RGB_t out, const acb_t z, slong prec);
void cplot_color_func_phase(cplot_RGB_t out, const acb_t z, slong prec);

/*    Transformation Variants    */
void cplot_color_func_radial_log(cplot_RGB_t out, const acb_t z, slong prec);
void cplot_color_func_radial(cplot_RGB_t out, const acb_t z, slong prec);
void cplot_color_func_conformal(cplot_RGB_t out, const acb_t z, slong prec);
void cplot_color_func_grid(cplot_RGB_t out, const acb_t z, slong prec);

/*    Variants from Arb complex_plot example    */
void cplot_color_func_blue_gold_phase(cplot_RGB_t out, const acb_t z, slong prec);

/* 
    Utility Operations
*/

#define CLAMP(y) FLINT_MAX(0.0, FLINT_MIN((y), 1.0))
#define BLEND(x,y) (0.5*(x) + 0.5*(y))
#define DODGE(a,b) ((a) / ((1.0 - (b)) + 1/256.0))

/*    Change to GetARG    */
void cplot_color_func_base(const acb_t z, arb_t t, arb_t u, double* R, double* G, double* B, double* H, slong prec);

/* HLS/HLV algorithms based on python colorsys module */
double cplot_color_vv(double m1, double m2, double hue);
void cplot_color_hls_to_rgb(double * R, double * G, double * B, double * H, double * L, double * S);
void cplot_color_rgb_to_hls(double * H, double * L, double * S,double * R, double * G, double * B);
void cplot_color_hsv_to_rgb(double * R, double * G, double * B, double * H, double * S, double * V);
void cplot_color_rgb_to_hsv(double * H, double * S, double * V,double * R, double * G, double * B);


/* Color Balance algorithms (from GIMP) */
double cplot_color_balance_channel(double value, double l,
    double shadows, double midtones, double highlights);

void cplot_color_balance(double * R, double * G, double * B,
    double ra, double rb, double rc,  /* red shadows, midtones, highlights */
    double ga, double gb, double gc,  /* green */
    double ba, double bb, double bc);  /* blue */


#endif