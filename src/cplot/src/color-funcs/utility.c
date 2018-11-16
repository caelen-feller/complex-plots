#include "cplot/color.h"

/* HLS algorithm from python's colorsys module */
double cplot_color_vv(double m1, double m2, double hue)
{
    hue = hue - floor(hue);

    if (hue < 1/6.)
        return m1 + (m2-m1)*hue*6.0;
    if (hue < 0.5)
        return m2;
    if (hue < 2/3.)
        return m1 + (m2-m1)*(2/3.-hue)*6.0;
    return m1;
}

void cplot_color_hls_to_rgb(double * R, double * G, double * B, double * H, double * L, double * S)
{
    double m1, m2;

    if (*S == 0.0)
    {
        *R = *G = *B = *L;
        return;
    }

    if (*L <= 0.5)
        m2 = *L * (1.0+*S);
    else
        m2 = *L+*S-(*L * (*S));

    m1 = 2.0**L - m2;

    *R = cplot_color_vv(m1, m2, *H + 1/3.);
    *G = cplot_color_vv(m1, m2, *H);
    *B = cplot_color_vv(m1, m2, *H - 1/3.);
}

void cplot_color_rgb_to_hls(double * H, double * L, double * S,double * R, double * G, double * B)
{
    double h, l, s, hi, lo, d;

    hi = FLINT_MAX(FLINT_MAX(*R, *G), *B);
    lo = FLINT_MIN(FLINT_MIN(*R, *G), *B);

    l = 0.5 * (lo + hi);
    d = hi - lo;

    if (hi == lo)
    {
        s = 0.0;
        h = 0.0;
    }
    else
    {
        if (l <= 0.5)
            s = d / (hi + lo);
        else
            s = d / (2.0 - hi - lo);

        if (d == 0.0)
            d = 1.0;

        if (*R == hi)
            h = (*G - *B) / d;
        else if (*G == hi)
            h = (*B - *R) / d + 2.0;
        else
            h = (*R - *G) / d + 4.0;

        h = h / 6.0;
        if (h < 0.0)
            h += 1.0;
    }

    *H = h;
    *L = l;
    *S = s;
}

void cplot_color_rgb_to_hsv(double * H, double * S, double * V,double * R, double * G, double * B)
{
    double h, s, v, hi, lo, d;

    hi = FLINT_MAX(FLINT_MAX(*R, *G), *B);
    lo = FLINT_MIN(FLINT_MIN(*R, *G), *B);
    
    v= hi;
    d = hi - lo;

    if (hi == lo)
    {
        s = 0.0;
        h = 0.0;
    }
    else
    {
        s = d/hi;

        if (d == 0.0)
            d = 1.0;

        if (*R == hi)
            h = (*G - *B) / d;
        else if (*G == hi)
            h = (*B - *R) / d + 2.0;
        else
            h = (*R - *G) / d + 4.0;

        h = h / 6.0;
        if (h < 0.0)
            h += 1.0;
    }

    *H = h;
    *V = v;
    *S = s;
}

void cplot_color_hsv_to_rgb(double * R, double * G, double * B, double * H, double * S, double * V)
{
    double f,p,q,t;
    long i;

    if (*S == 0.0)
    {
        *R = *G = *B = *V;
        return;
    }

    i = floor(*H *6.0);
    f = (*H *6.0) - i;
    p = *V *(1.0-*S);
    q = *V *(1.0-*S * f);
    t = *V *(1.0 - *S*(1.0-f));

    if(i == 0)
    { *R = *V; *G = t; *B = p; }
    if(i == 1)
    { *R = q; *G = *V; *B = p; }
    if(i == 2)
    { *R = p; *G = *V; *B = t; }
    if(i == 3)
    { *R = p; *G = q; *B = *V; }
    if(i == 4)
    { *R = t; *G = p; *B = *V; }
    if(i == 5)
    { *R = *V; *G = p; *B = q; }

    
}

/* color balance algorithm from gimp */
double cplot_color_balance_channel(double value, double l,
    double shadows, double midtones, double highlights)
{
    double a = 0.25, b = 0.333, scale = 0.7;

    shadows    *= CLAMP((l - b) / (-a) + 0.5) * scale;
    midtones   *= CLAMP((l - b) / ( a) + 0.5) *
                  CLAMP((l + b - 1.0) / (-a) + 0.5) * scale;
    highlights *= CLAMP((l + b - 1.0) / ( a) + 0.5) * scale;

    value += shadows;
    value += midtones;
    value += highlights;
    return CLAMP(value);
}

void cplot_color_balance(double * R, double * G, double * B,
    double ra, double rb, double rc,  /* red shadows, midtones, highlights */
    double ga, double gb, double gc,  /* green */
    double ba, double bb, double bc)  /* blue */
{
    double h, l, s;
    double h2, l2, s2;

    cplot_color_rgb_to_hls(&h, &l, &s, R, G, B);

    *R = cplot_color_balance_channel(*R, *R, ra, rb, rc);
    *G = cplot_color_balance_channel(*G, *G, ga, gb, gc);
    *B = cplot_color_balance_channel(*B, *B, ba, bb, bc);

    /* preserve lightness */
    cplot_color_rgb_to_hls(&h2, &l2, &s2, R, G, B);
    cplot_color_hls_to_rgb(R, G, B, &h2, &l, &s2);
}
