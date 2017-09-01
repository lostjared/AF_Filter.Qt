/*
 
 Alpha Flame Filter
 written by Jared Bruni
 http://github.com/lostjared
 
 License: GPL
 */
#ifndef __ALPHA_FLAME_H___
#define __ALPHA_FLAME_H___

#include<QImage>

extern QImage start_image;

void changePixel(unsigned int current_filterx, QImage &full_buffer, int i, int z, unsigned char *buffer, double pos, double *count, int reverse, bool negated, unsigned int red, unsigned int green, unsigned int blue, unsigned int randomNumber);
void alphaFlame(QImage &image, bool neg_state, unsigned int red, unsigned int green, unsigned int blue, int reverse, int filter_num, int iteration);
unsigned char *pixelAt(const QImage &image, int x, int y, unsigned int &value);
void ApplyOptions(unsigned char *buffer, bool negated, int red, int green, int blue, int reverse);
void GlitchSort(QImage &image, bool neg, int iteration, int red, int green, int blue, int rev);
void GlitchSortByChannel(QImage &image, bool neg, int iteration, int red, int green, int blue, int rev);
void GlitchSortVertical(QImage &image, bool neg, int iteration, int red, int green, int blue, int rev);
void UniqueMirrorBlend(QImage &image, bool neg, int iteration, int red, int green, int blue, int rev);
void Reverse(QImage &image, bool neg, int iteration, int red, int green, int blue, int rev);
void BlendScanLines(QImage &image, bool neg, int iteration, int red, int green, int blue, int rev);
void UpDown(QImage &image, bool neg, int iteration, int red, int green, int blue, int rev);
void LeftRight(QImage &image, bool neg, unsigned int iteration, unsigned int red, unsigned int green, unsigned int blue, int rev);
void XorMultiBlend(QImage &image, bool neg, unsigned int iteration, unsigned int red, unsigned int green, unsigned int blue, int rev);
void GradientLines(QImage &image, bool neg, unsigned int iteration, unsigned int red, unsigned int green, unsigned int blue, int rev);
void GradientSelf(QImage &image, bool neg, unsigned int iteration, unsigned int red, unsigned int green, unsigned int blue, int rev);
void Side2Side(QImage &image, bool neg, unsigned int iteration, unsigned int red, unsigned int green, unsigned int blue, int rev);
void Top2Bottom(QImage &image, bool neg, unsigned int iteration, unsigned int red, unsigned int green, unsigned int blue, int rev);
void Outward(QImage &image, bool neg, unsigned int iteration, unsigned int red, unsigned int green, unsigned int blue, int rev);
void OutwardSquare(QImage &image, bool neg, unsigned int iteration, unsigned int red, unsigned int green, unsigned int blue, int rev);
void ShiftPixels(QImage &image, bool neg, unsigned int iteration, unsigned int red, unsigned int green, unsigned int blue, int rev);
void ShiftPixelsDown(QImage &image, bool neg, unsigned int iteration, unsigned int red, unsigned int green, unsigned int blue, int rev);
void BlendWithSource(QImage &image, bool neg, unsigned int iteration, unsigned int red, unsigned int green, unsigned int blue, int rev);


#endif

