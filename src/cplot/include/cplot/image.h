#ifndef CPLOT_IMG_H
#define CPLOT_IMG_H

#include "color.h"

typedef struct cplot_img cplot_img;
typedef cplot_img * cplot_img_t;

cplot_img_t cplot_img_init(slong x, slong y);
void cplot_img_set(cplot_img_t res, cplot_img_t src);
void cplot_img_set_rgb(cplot_img_t img, cplot_RGB_t rgb, slong x, slong y);
cplot_RGB_t cplot_img_get_rgb(cplot_img_t img, slong x, slong y);
slong cplot_img_get_x(cplot_img_t img);
slong cplot_img_get_y(cplot_img_t img);
void cplot_img_clear(cplot_img_t img);

/**
 * @brief Saves image as file.
 * 
 * @param img Image to save.
 * @param filename Filename.
 * @param type File format to save to. Supports ImageMagick Formats
 */
void cplot_img_save(cplot_img_t img, char* filename, char* format);

#endif