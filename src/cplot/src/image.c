#include "cplot/image.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAGICKCORE_QUANTUM_DEPTH 64
#define MAGICKCORE_HDRI_ENABLE 1
#include <MagickWand/MagickWand.h>


struct cplot_img
{
    slong x,y;
    cplot_RGB ** data;
};

cplot_img_t cplot_img_init(slong x, slong y) 
{
    cplot_img_t z = malloc(sizeof(cplot_img));
    z->x = x; z->y=y;

    z->data = malloc(y*sizeof(*z->data));
    for(slong i = 0; i < y; i++) 
    {
        z->data[i] = malloc(x*sizeof(**z->data));
        for(int j=0; j<x; j++) z->data[i][j] = (cplot_RGB){};
    }
    return z;
}

void cplot_img_set(cplot_img_t res, cplot_img_t src)
{
    res = cplot_img_init(src->x, src->y);
    res->x = src->x; res->y=src->y;

    res->data = malloc(res->y*sizeof(*res->data));
    for(slong i = 0; i < res->y; i++) 
    {
        res->data[i] = malloc(res->x*sizeof(**res->data));
        for(int j=0; j<res->x; j++) res->data[i][j] = src->data[i][j];
    }
}

void cplot_img_set_rgb(cplot_img_t img, cplot_RGB_t rgb, slong x, slong y)
{
    img->data[y][x] = *rgb;
}

cplot_RGB_t cplot_img_get_rgb(cplot_img_t img, slong x, slong y)
{
    return &(img->data[y][x]);
}

slong cplot_img_get_x(cplot_img_t img)
{
    return img->x;
}

slong cplot_img_get_y(cplot_img_t img)
{
    return img->y;
}

void cplot_img_clear(cplot_img_t z)
{
    for(int i=0; i<z->y; i++)
        free(z->data[i]);
    free(z->data);
    free(z);
}

/*  
    TODO:
    - Restructure with output flag
*/

#define ThrowWandException(wand) \
{ \
char \
    *description; \
\
ExceptionType \
    severity; \
\
description=MagickGetException(wand,&severity); \
(void) fprintf(stderr,"%s %s %lu %s\n",GetMagickModule(),description); \
description=(char *) MagickRelinquishMemory(description); \
exit(-1); \
}

void convert_ppm(char* tmpfilename, char* filename)
{
    MagickBooleanType status;

    MagickWand *magick_wand;

    MagickWandGenesis();
    magick_wand=NewMagickWand();

    status=MagickReadImage(magick_wand,tmpfilename);
    if (status == MagickFalse)
        ThrowWandException(magick_wand);
        
    status=MagickWriteImage(magick_wand,filename);
    if (status == MagickFalse)
        ThrowWandException(magick_wand);
    magick_wand=DestroyMagickWand(magick_wand);
    MagickWandTerminus();
}

void fputimg(cplot_img_t img, FILE * fp, int (*func)(const int,FILE *)) 
{
    for(slong y = img->y-1; y >= 0; y--)
    {
        for(slong x = 0; x < img->x; x++)
        {
            func(FLINT_MIN(255, floor(img->data[y][x].R * 255)), fp);
            func(FLINT_MIN(255, floor(img->data[y][x].G * 255)), fp);
            func(FLINT_MIN(255, floor(img->data[y][x].B * 255)), fp);
        }
    }
}

void cplot_img_save(cplot_img_t img, char* filename, char* format)
{
    char fullname[100];
    char tmpfilename[100];
    snprintf(fullname, 100, "%s%s%s", filename,".",format);
    snprintf(tmpfilename, 100, "%s%s", filename,".ppm");
    
    if(!strcmp(format, "stdout")) 
    {
        printf("%s in RGB", filename);
        fputimg(img, stdout, fputc);
    }
    else 
    {        
        FILE *fp;
        fp = fopen(tmpfilename, "w"); 
        fprintf(fp, "P6\n%ld %ld 255\n", img->x, img->y);
        fputimg(img, fp, fputc);
        fclose(fp);

        if(strcmp(format, "ppm")) 
        {
            convert_ppm(tmpfilename, fullname);
            remove(tmpfilename);
        }
    }
}