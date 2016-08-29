#ifndef __ALPHA_FLAME_H___
#define __ALPHA_FLAME_H___

#include<QtCore>

void changePixel(unsigned int current_filterx, int width, int height, QImage &full_buffer, int i, int z, unsigned char *buffer, double pos, double *count, int reverse, bool negated, unsigned int red, unsigned int green, unsigned int blue, unsigned int randomNumber);

void alphaFlame(QImage &image, int filter_num, int iteration);

#endif