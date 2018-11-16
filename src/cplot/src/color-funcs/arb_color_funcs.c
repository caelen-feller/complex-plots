#include "cplot/color.h"
#define PI 3.1415926535898

const double blue_orange_colors[][4] = {
  {-1.0,  0.0, 0.0, 0.0},
  {-0.95, 0.1, 0.2, 0.5},
  {-0.5,  0.0, 0.5, 1.0},
  {-0.05, 0.4, 0.8, 0.8},
  { 0.0,  1.0, 1.0, 1.0},
  { 0.05, 1.0, 0.9, 0.3},
  { 0.5,  0.9, 0.5, 0.0},
  { 0.95, 0.7, 0.1, 0.0},
  { 1.0,  0.0, 0.0, 0.0},
  { 2.0,  0.0, 0.0, 0.0},
};

void cplot_color_func_blue_gold_phase(cplot_RGB_t out, const acb_t z, slong prec)
{   
    double *R,*G,*B;
    R = &out->R;
    G = &out->G;
    B = &out->B;
    double H, L, S; 
    arb_t t, u;
    arb_init(t); arb_init(u);

    cplot_color_func_base(z, t, u, R, G, B, &H, prec);
    
    H = H / PI;
    H = FLINT_MAX(FLINT_MIN(H, 1.0), -1.0);

    for (int i = 1; ; i++)
    {
        if (blue_orange_colors[i][0] > H)
        {
            double a, ra, ga, ba, b, rb, gb, bb, s;

            a  = blue_orange_colors[i-1][0];
            ra = blue_orange_colors[i-1][1];
            ga = blue_orange_colors[i-1][2];
            ba = blue_orange_colors[i-1][3];
            b  = blue_orange_colors[i][0];
            rb = blue_orange_colors[i][1];
            gb = blue_orange_colors[i][2];
            bb = blue_orange_colors[i][3];

            s = (H - a) / (b - a);
            *R = ra + (rb - ra) * s;
            *G = ga + (gb - ga) * s;
            *B = ba + (bb - ba) * s;
            break;
        }
    }

    arb_clear(t);
    arb_clear(u);
}