/*
 
 Alpha Flame Filter
 written by Jared Bruni
 http://github.com/lostjared
 
 License: GPL
 */
#include<QtCore>
#include<QWidget>
#include<QDialog>
#include<QLabel>
#include<QMenu>
#include<QMenuBar>
#include<QPushButton>
#include<QComboBox>
#include<QSlider>
#include<QStatusBar>
#include<QLineEdit>
#include<QMainWindow>
#include<QMessageBox>
#include<QFileDialog>
#include<QComboBox>
#include<QPainter>
#include<QClipboard>
#include<cmath>
#include<iostream>
#include<vector>
#include<algorithm>

#include "alphaflame.h"

unsigned char *pixelAt(const QImage &image, int x, int y, unsigned int &value) {
    value = image.pixel(x,y);
    unsigned char *ptr = (unsigned char *)&value;
    //++ptr;
    return ptr;
}

void ApplyOptions(unsigned char *buffer, bool negated, int red, int green, int blue, int reverse) {
    buffer[2] += red;
    buffer[1] += green;
    buffer[0] += blue;
    
    if(negated == true) {
        buffer[0] = ~buffer[0];
        buffer[1] = ~buffer[1];
        buffer[2] = ~buffer[2];
    }
    
    unsigned char buf[3];
    buf[0] = buffer[0];
    buf[1] = buffer[1];
    buf[2] = buffer[2];
    
    switch(reverse) {
        case 0://normal
            break;
        case 1:
            buffer[0] = buf[2];
            buffer[1] = buf[1];
            buffer[2] = buf[0];
            break;
        case 2:
            buffer[0] = buf[1];
            buffer[1] = buf[2];
            buffer[2] = buf[0];
            break;
        case 3:
            buffer[0] = buf[2];
            buffer[1] = buf[0];
            buffer[2] = buf[1];
            break;
        case 4:
            buffer[0] = buf[1];
            buffer[1] = buf[0];
            buffer[2] = buf[2];
            break;
    }
}

void GlitchSort(QImage &image, bool neg, int iteration, int red, int green, int blue, int rev) {
    int w = image.width();
    int h = image.height();
    static std::vector<unsigned int> v;
    v.reserve(w);
    
    for(int z = 0; z < h; ++z) {
        for(int i = 0; i < w; ++i) {
            unsigned int value = 0;
            pixelAt(image, i, z, value);
            v.push_back(value);
        }
        std::sort(v.begin(), v.end());
        for(int i = 0; i < w; ++i) {
            
            unsigned int blend = 0;
            unsigned char *value = (unsigned char*)&v[i];
            unsigned char *pixel = pixelAt(image, i, z, blend);
            unsigned char rgb[3];
            rgb[2] = pixel[0] + (iteration)*value[0];
            rgb[1] = pixel[1] + (iteration)*value[1];
            rgb[0] = pixel[2] + (iteration)*value[2];
            ApplyOptions(rgb, neg, red, green, blue, rev);
            QRgb rgbvalue = qRgb(rgb[0], rgb[1], rgb[2]);
            image.setPixel(i, z, rgbvalue);
        }
        v.erase(v.begin(), v.end());
    }
}

struct Pixel {
    unsigned char rgb[3];
};

unsigned int color = 0;

bool operator<(const Pixel &p1, const Pixel &p2) {
    return (p1.rgb[color] > p2.rgb[color]);
}

void GlitchSortByChannel(QImage &image, bool neg, int iteration, int red, int green, int blue, int rev) {
    int w = image.width();
    int h = image.height();
    static std::vector<Pixel> v;
    v.reserve(w);
    
    for(int z = 0; z < h; ++z) {
        for(int i = 0; i < w; ++i) {
            unsigned int value = 0;
            unsigned char *rgb = pixelAt(image, i, z, value);
            Pixel p;
            p.rgb[0] = rgb[0];
            p.rgb[1] = rgb[1];
            p.rgb[2] = rgb[2];
            v.push_back(p);
        }
        std::sort(v.begin(), v.end());
        ++color;
        if(color >= 3) color = 0;
        for(int i = 0; i < w; ++i) {
            
            unsigned int blend = 0;
            unsigned char *value = (unsigned char*)&v[i].rgb[0];
            unsigned char *pixel = pixelAt(image, i, z, blend);
            unsigned char rgb[3];
            rgb[2] = pixel[0] + (iteration)*value[0];
            rgb[1] = pixel[1] + (iteration)*value[1];
            rgb[0] = pixel[2] + (iteration)*value[2];
            ApplyOptions(rgb, neg, red, green, blue, rev);
            QRgb rgbvalue = qRgb(rgb[0], rgb[1], rgb[2]);
            image.setPixel(i, z, rgbvalue);
        }
        v.erase(v.begin(), v.end());
    }
}

void GlitchSortVertical(QImage &image, bool neg, int iteration, int red, int green, int blue, int rev) {
    int w = image.width();
    int h = image.height();
    static std::vector<unsigned int> v;
    v.reserve(w);
    
    for(int i = 0; i < w; ++i) {
        for(int z = 0; z < h; ++z) {
            unsigned int value = 0;
            pixelAt(image, i, z, value);
            v.push_back(value);
        }
        std::sort(v.begin(), v.end());
        for(int z = 0; z < h; ++z) {
            unsigned int blend = 0;
            unsigned char *value = (unsigned char*)&v[i];
            unsigned char *pixel = pixelAt(image, i, z, blend);
            unsigned char rgb[3];
            rgb[2] = pixel[0] + (iteration)*value[0];
            rgb[1] = pixel[1] + (iteration)*value[1];
            rgb[0] = pixel[2] + (iteration)*value[2];
            ApplyOptions(rgb, neg, red, green, blue, rev);
            QRgb rgbvalue = qRgb(rgb[0], rgb[1], rgb[2]);
            image.setPixel(i, z, rgbvalue);
        }
        v.erase(v.begin(), v.end());
    }
}



void UniqueMirrorBlend(QImage &image, bool neg, int iteration, int red, int green, int blue, int rev) {
    int w = image.width();
    int h = image.height();
    
    QImage copyof = image;
    
    for(int z =  1; z < h-1; ++z) {
        for(int i = 1; i < w-1; ++i) {
            unsigned int blend = 0, mir_1 = 0, mir_2 = 0, mir_3 = 0;;
            unsigned char *pixel = pixelAt(image, i, z, blend);
            
            unsigned char *pixelA = pixelAt(copyof, (w-i), (h-z), mir_1);
            unsigned char *pixelB = pixelAt(copyof, (w-i), z, mir_2);
            unsigned char *pixelC = pixelAt(copyof, i, (h-z), mir_3);
            
            unsigned char rgb[3];
            rgb[2] = pixel[0]+(pixelA[0]*(iteration*0.1));
            rgb[1] = pixel[1]+(pixelB[1]*(iteration*0.1));
            rgb[0] = pixel[2]+(pixelC[2]*(iteration*0.1));

            ApplyOptions(rgb, neg, red, green, blue, rev);
            QRgb rgbvalue = qRgb(rgb[0], rgb[1], rgb[2]);
            image.setPixel(i, z, rgbvalue);
        }
    }
}

void Reverse(QImage &image, bool neg, int iteration, int red, int green, int blue, int rev) {
	const int w = image.width(), h = image.height();
    QImage copyOf = image;
    for(int z = 0; z < h; ++z) {
        for(int i = 1; i < w-1; ++i) {
            unsigned int p1;
            unsigned char *pix1;
            pix1 = pixelAt(copyOf, (w-i), z, p1);
            unsigned char rgb[3];
            rgb[0] = pix1[0]*(1+(0.1*iteration));
            rgb[1] = pix1[1]*(1+(0.1*iteration));
            rgb[2] = pix1[2]*(1+(0.1*iteration));
            ApplyOptions(rgb, neg, red, green, blue, rev);
            QRgb rgbvalue = qRgb(rgb[2], rgb[1], rgb[0]);
            image.setPixel(i, z, rgbvalue);
        }
    }
}

void BlendScanLines(QImage &image, bool neg, int iteration, int red, int green, int blue, int rev) {
    unsigned int w = image.width();// frame width
    unsigned int h = image.height();// frame height
    static unsigned int cnt = 0;
    for(unsigned int z = 0; z < h; ++z) {
        int r = rand()%255;
        for(unsigned int i = 0; i < w; ++i) {
            unsigned int p1;
            unsigned char *pixel;
            pixel = pixelAt(image, i, z, p1);
            pixel[cnt] += r*iteration;
            ++r;
            ApplyOptions(pixel, neg, red, green, blue, rev);
            QRgb rgbvalue = qRgb(pixel[2], pixel[1], pixel[0]);
            image.setPixel(i, z, rgbvalue);
            ++cnt;
            if(cnt > 2) {
                cnt = 0;
            }
        }
    }
}

void UpDown(QImage &image, bool neg, int iteration, int red, int green, int blue, int rev) {
    unsigned int w = image.width();// frame width
    unsigned int h = image.height();// frame height
    double pos = iteration;
    double alpha = 1.0;
    bool order = true;
    
    for(unsigned int i = 0; i < w; ++i) {
        if(order == true) {
            order = false;
            for(unsigned int z = 0; z < h; ++z) {
                unsigned int p1;
                unsigned char *pixel;
                pixel = pixelAt(image, i, z, p1);
                
                for(unsigned int q = 0; q < 3; ++q)
                    pixel[q] = alpha+(pixel[q]*pos);
                
                ApplyOptions(pixel, neg, red, green, blue, rev);
                QRgb rgbvalue = qRgb(pixel[2], pixel[1], pixel[0]);
                image.setPixel(i, z, rgbvalue);
                
            }
            alpha += 0.1;
        } else {
            order = true;
            for(unsigned int z = h-1; z > 1; --z) {
                unsigned int p1;
                unsigned char *pixel;
                pixel = pixelAt(image, i, z, p1);
                
                for(unsigned int q = 0; q < 3; ++q)
                    pixel[q] = alpha-(pixel[q]*pos);
                
                ApplyOptions(pixel, neg, red, green, blue, rev);
                QRgb rgbvalue = qRgb(pixel[2], pixel[1], pixel[0]);
                image.setPixel(i, z, rgbvalue);
            }
            
            alpha += 0.1;
        }
    }
}

void LeftRight(QImage &image, bool neg, unsigned int iteration, unsigned int red, unsigned int green, unsigned int blue, int rev) {
    unsigned int w = image.width();// frame width
    unsigned int h = image.height();// frame height
    double pos = iteration;
    double alpha = 1.0;
    bool order = true;
    for(unsigned int z = 0; z < h; ++z) {
        if(order == true) {
            order = false;
            for(unsigned int i = 0; i < w; ++i) {
                unsigned int p1;
                unsigned char *pixel;
                pixel = pixelAt(image, i, z, p1);
                for(unsigned int q = 0; q < 3; ++q)
                    pixel[q] = alpha+(pixel[q]*pos);
                
                ApplyOptions(pixel, neg, red, green, blue, rev);
                QRgb rgbvalue = qRgb(pixel[2], pixel[1], pixel[0]);
                image.setPixel(i, z, rgbvalue);
            }
            alpha += 0.1;
        } else {
            order = true;
            for(unsigned int i = w-1; i > 1; --i) {
                unsigned int p1;
                unsigned char *pixel;
                pixel = pixelAt(image, i, z, p1);
                for(unsigned int q = 0; q < 3; ++q)
                    pixel[q] = alpha-(pixel[q]*pos);
                
                ApplyOptions(pixel, neg, red, green, blue, rev);
                QRgb rgbvalue = qRgb(pixel[2], pixel[1], pixel[0]);
                image.setPixel(i, z, rgbvalue);
            }
            
            alpha += 0.1;
        }
    }
}


void XorMultiBlend(QImage &image, bool neg, unsigned int iteration, unsigned int red, unsigned int green, unsigned int blue, int rev) {
    int w = image.width();// frame width
    int h = image.height();// frame height
    double pos = iteration;
    double s[3] = { pos, -pos, pos };
    for(int y = h-1; y > 0; --y) {
        for(int x = w-1; x > 0; --x) {
            unsigned int p1;
            unsigned char *pixel;
            pixel = pixelAt(image, x, y, p1);
            pixel[0] = (pixel[0]^(int)s[0])*iteration;
            pixel[1] = (pixel[1]^(int)s[1])*iteration;
            pixel[2] = (pixel[2]^(int)s[2])*iteration;
            ApplyOptions(pixel, neg, red, green, blue, rev);
            QRgb rgbvalue = qRgb(pixel[2], pixel[1], pixel[0]);
            image.setPixel(x, y, rgbvalue);
        }
    }
}

void GradientLines(QImage &image, bool neg, unsigned int iteration, unsigned int red, unsigned int green, unsigned int blue, int rev) {
    int w = image.width();// frame width
    int h = image.height();// frame height
    static unsigned int count = 0, index = 0;
    count = iteration;
    for(int z = 0; z < h; ++z) {
        for(int i = 0; i < w; ++i) {
            unsigned int p1;
            unsigned char *pixel;
            pixel = pixelAt(image, i, z, p1);
            pixel[index] += count;
            ++count;
            if(count >= 255) {
                count = 0;
                ++index;
                if(index > 2) index = 0;
            }
            ApplyOptions(pixel, neg, red, green, blue, rev);
            QRgb rgbvalue = qRgb(pixel[2], pixel[1], pixel[0]);
            image.setPixel(i, z, rgbvalue);
        }
    }
}

void GradientSelf(QImage &image, bool neg, unsigned int iteration, unsigned int red, unsigned int green, unsigned int blue, int rev) {
    int w = image.width();// frame width
    int h = image.height();// frame height
    static unsigned int count = 0, index = 0;
    double pos = iteration;
    for(int z = 0; z < h; ++z) {
        for(int i = 0; i < w; ++i) {
            unsigned int p1;
            unsigned char *pixel;
            pixel = pixelAt(image, i, z, p1);
            pixel[index] = (pixel[index]*pos)+count;
            ++count;
            if(count >= 255) {
                count = 0;
            }
            ApplyOptions(pixel, neg, red, green, blue, rev);
            QRgb rgbvalue = qRgb(pixel[2], pixel[1], pixel[0]);
            image.setPixel(i, z, rgbvalue);
        }
        ++index;
        if(index > 2) index = 0;
    }
}

void Side2Side(QImage &image, bool neg, unsigned int iteration, unsigned int red, unsigned int green, unsigned int blue, int rev) {
    int w = image.width();// frame width
    int h = image.height();// frame height
    double pos = iteration;
    for(int z = 0; z < h; ++z) {
        double total[3] = {0};
        for(int i = 0; i < w; ++i) {
            unsigned int p1;
            unsigned char *pixel;
            pixel = pixelAt(image, i, z, p1);
            total[0] += (pixel[0]/2);
            total[1] += (pixel[1]/2);
            total[2] += (pixel[2]/2);
            pixel[0] = pixel[0] + (total[0]*pos)*0.01;
            pixel[1] = pixel[1] + (total[1]*pos)*0.01;
            pixel[2] = pixel[2] + (total[2]*pos)*0.01;
            ApplyOptions(pixel, neg, red, green, blue, rev);
            QRgb rgbvalue = qRgb(pixel[2], pixel[1], pixel[0]);
            image.setPixel(i, z, rgbvalue);
        }
    }
}

void Top2Bottom(QImage &image, bool neg, unsigned int iteration, unsigned int red, unsigned int green, unsigned int blue, int rev) {
    int w = image.width();// frame width
    int h = image.height();// frame height
    double pos = iteration;
    for(int i = 0; i < w; ++i) {
        double total[3] = {0};
        for(int z = 0; z < h; ++z) {
            unsigned int p1;
            unsigned char *pixel;
            pixel = pixelAt(image, i, z, p1);
            total[0] += (pixel[0]/2);
            total[1] += (pixel[1]/2);
            total[2] += (pixel[2]/2);
            pixel[0] = pixel[0] + (total[0]*pos)*0.01;
            pixel[1] = pixel[1] + (total[1]*pos)*0.01;
            pixel[2] = pixel[2] + (total[2]*pos)*0.01;
            ApplyOptions(pixel, neg, red, green, blue, rev);
            QRgb rgbvalue = qRgb(pixel[2], pixel[1], pixel[0]);
            image.setPixel(i, z, rgbvalue);
        }
    }
}

void Outward(QImage &image, bool neg, unsigned int iteration, unsigned int red, unsigned int green, unsigned int blue, int rev) {
    int w = image.width();// frame width
    int h = image.height();// frame height
    double start_pos = iteration;
    double pos = start_pos;
    static double offset[3] = {5, 50, 100};
    pos = start_pos;
    for(int y = h/2; y > 0; --y) {
        for(int x = 0; x < w; ++x) {
            unsigned int p1;
            unsigned char *pixel;
            pixel = pixelAt(image, x, y, p1);
            pixel[0] += (pos*offset[0]);
            pixel[1] += (pos*offset[1]);
            pixel[2] += (pos*offset[2]);
            ApplyOptions(pixel, neg, red, green, blue, rev);
            QRgb rgbvalue = qRgb(pixel[2], pixel[1], pixel[0]);
            image.setPixel(x, y, rgbvalue);
        }
        pos += 0.005;
    }
    pos = start_pos;
    for(int y = h/2+1; y < h; ++y) {
        for(int x = 0; x < w; ++x) {
            unsigned int p1;
            unsigned char *pixel;
            pixel = pixelAt(image, x, y, p1);
            pixel[0] += (pos*offset[0]);
            pixel[1] += (pos*offset[1]);
            pixel[2] += (pos*offset[2]);
            ApplyOptions(pixel, neg, red, green, blue, rev);
            QRgb rgbvalue = qRgb(pixel[2], pixel[1], pixel[0]);
            image.setPixel(x, y, rgbvalue);
        }
        pos += 0.005;
    }
    offset[0] += 12;
    offset[1] += 6;
    offset[2] += 3;
    for(int i = 0; i < 3; ++i) if(offset[i] > 200) offset[i] = 0;
}

void OutwardSquare(QImage &image, bool neg, unsigned int iteration, unsigned int red, unsigned int green, unsigned int blue, int rev) {
    int w = image.width();// frame width
    int h = image.height();// frame height
    int wx = w/2;
    double start_pos = iteration;
    double pos = start_pos;
    static double offset[3] = {5, 50, 100};
    pos = start_pos;
    
    for(int y = h/2; y > 0; --y) {
        for(int x = 0; x < wx; ++x) {
            unsigned int p1;
            unsigned char *pixel;
            pixel = pixelAt(image, x, y, p1);
            pixel[0] += (pos*offset[0]);
            pixel[1] += (pos*offset[1]);
            pixel[2] += (pos*offset[2]);
            ApplyOptions(pixel, neg, red, green, blue, rev);
            QRgb rgbvalue = qRgb(pixel[2], pixel[1], pixel[0]);
            image.setPixel(x, y, rgbvalue);
        }
        pos += 0.005;
    }
    //pos = start_pos;
    for(int y = h/2; y > 0; --y) {
        for(int x = w-1; x > wx-1; --x) {
            unsigned int p1;
            unsigned char *pixel;
            pixel = pixelAt(image, x, y, p1);
            pixel[0] += (pos*offset[0]);
            pixel[1] += (pos*offset[1]);
            pixel[2] += (pos*offset[2]);
            ApplyOptions(pixel, neg, red, green, blue, rev);
            QRgb rgbvalue = qRgb(pixel[2], pixel[1], pixel[0]);
            image.setPixel(x, y, rgbvalue);
        }
        pos += 0.005;
    }
    
   	pos = start_pos;
    for(int y = h/2+1; y < h; ++y) {
        for(int x = 0; x < wx; ++x) {
            unsigned int p1;
            unsigned char *pixel;
            pixel = pixelAt(image, x, y, p1);
            pixel[0] += (pos*offset[0]);
            pixel[1] += (pos*offset[1]);
            pixel[2] += (pos*offset[2]);
            ApplyOptions(pixel, neg, red, green, blue, rev);
            QRgb rgbvalue = qRgb(pixel[2], pixel[1], pixel[0]);
            image.setPixel(x, y, rgbvalue);
        }
        pos += 0.005;
    }
    //pos = start_pos;
    for(int y = h/2+1; y < h; ++y) {
        for(int x = w-1; x > wx-1; --x) {
            unsigned int p1;
            unsigned char *pixel;
            pixel = pixelAt(image, x, y, p1);
            pixel[0] += (pos*offset[0]);
            pixel[1] += (pos*offset[1]);
            pixel[2] += (pos*offset[2]);
            ApplyOptions(pixel, neg, red, green, blue, rev);
            QRgb rgbvalue = qRgb(pixel[2], pixel[1], pixel[0]);
            image.setPixel(x, y, rgbvalue);
        }
        pos += 0.005;
    }
    offset[0] += 12;
    offset[1] += 6;
    offset[2] += 3;
    for(int i = 0; i < 3; ++i) if(offset[i] > 200) offset[i] = 0;
}

void ShiftPixels(QImage &image, bool neg, unsigned int iteration, unsigned int red, unsigned int green, unsigned int blue, int rev) {
    int offset = iteration;
    int w = image.width();// frame width
    int h = image.height();// frame height
    
    for(int z = 0; z < h; ++z) {
        int start = 0;
        for(int i = offset; i < w && start < w; ++i) {
            unsigned int p1;
            unsigned char *pixel;
            pixel = pixelAt(image, i, z, p1);
            unsigned int p2;
            unsigned char *source;
            source = pixelAt(image, start, z, p2);
            pixel[0] += source[0];
            pixel[1] += source[1];
            pixel[2] += source[2];
            ++start;
            // swap colors
            ApplyOptions(pixel, neg, red, green, blue, rev);
            QRgb rgbvalue = qRgb(pixel[2], pixel[1], pixel[0]);
            image.setPixel(i, z, rgbvalue);
        }
        for(int i = 0; i < offset-1 && start < w; ++i) {
            unsigned int p1;
            unsigned char *pixel;
            pixel = pixelAt(image, i, z, p1);
            unsigned int p2;
            unsigned char *source;
            source = pixelAt(image, start, z, p2);
            pixel[0] += source[0];
            pixel[1] += source[1];
            pixel[2] += source[2];
            ++start;
            ApplyOptions(pixel, neg, red, green, blue, rev);
            QRgb rgbvalue = qRgb(pixel[2], pixel[1], pixel[0]);
            image.setPixel(i, z, rgbvalue);
        }
    }
}

void ShiftPixelsDown(QImage &image, bool neg, unsigned int iteration, unsigned int red, unsigned int green, unsigned int blue, int rev) {
    int offset = iteration;
    int w = image.width();// frame width
    int h = image.height();// frame height
    double pos = iteration;
    for(int i = 0; i < w; ++i) {
        int start = 0;
        for(int z = offset; z < h && start < h; ++z) {
            unsigned int p1;
            unsigned char *pixel;
            pixel = pixelAt(image, i, z, p1);
            unsigned int p2;
            unsigned char *source;
            source = pixelAt(image, start, z, p2);
            pixel[0] += source[0]*pos;
            pixel[1] += source[1]*pos;
            pixel[2] += source[2]*pos;
            ++start;
            // swap colors
            ApplyOptions(pixel, neg, red, green, blue, rev);
            QRgb rgbvalue = qRgb(pixel[2], pixel[1], pixel[0]);
            image.setPixel(i, z, rgbvalue);
        }
        for(int z = 0; z < offset-1 && start < h; ++z) {
            unsigned int p1;
            unsigned char *pixel;
            pixel = pixelAt(image, i, z, p1);
            unsigned int p2;
            unsigned char *source;
            source = pixelAt(image, start, z, p2);
            pixel[0] += source[0]*pos;
            pixel[1] += source[1]*pos;
            pixel[2] += source[2]*pos;
            ++start;
            ApplyOptions(pixel, neg, red, green, blue, rev);
            QRgb rgbvalue = qRgb(pixel[2], pixel[1], pixel[0]);
            image.setPixel(i, z, rgbvalue);
        }
    }
}



void BlendWithSource(QImage &image, bool neg, unsigned int iteration, unsigned int red, unsigned int green, unsigned int blue, int rev) {
    int w = image.width();// frame width
    int h = image.height();// frame height
    for(int z = 0; z < h; ++z) {
        for(int i = 0; i < w; ++i) {
            unsigned int p1;
            unsigned char *pixel;
            pixel = pixelAt(image, i, z, p1);
            unsigned int p2;
            unsigned char *color2;
            color2 = pixelAt(start_image, i, z, p2);
           
            for(int q = 0; q < 3; ++q)
                pixel[q] = color2[q]+(pixel[q]*(0.5+(0.01*iteration)));// multiply
            
            ApplyOptions(pixel, neg, red, green, blue, rev);
            QRgb rgbvalue = qRgb(pixel[2], pixel[1], pixel[0]);
            image.setPixel(i, z, rgbvalue);
        }
    }
}

void alphaFlame(QImage &image, bool neg, unsigned int red, unsigned int green, unsigned int blue, int rev, int filter_num, int iteration) {
    
    switch(filter_num) {
        case 36:
            GlitchSort(image, neg, iteration, red, green, blue, rev);
            return;
        case 37:
            GlitchSortVertical(image, neg, iteration, red, green, blue, rev);
            return;
        case 38:
            GlitchSortByChannel(image, neg, iteration, red, green, blue, rev);
            return;
        case 39:
            UniqueMirrorBlend(image, neg, iteration, red, green, blue, rev);
            return;
        case 40:
            Reverse(image, neg, iteration, red, green, blue, rev);
            return;
        case 41:
            BlendScanLines(image, neg, iteration, red, green, blue, rev);
            return;
        case 42:
            UpDown(image, neg, iteration, red, green, blue, rev);
            return;
        case 43:
            LeftRight(image, neg, iteration, red, green, blue, rev);
            return;
        case 44:
            XorMultiBlend(image, neg, iteration, red, green, blue, rev);
            return;
        case 45:
            GradientLines(image, neg, iteration, red, green, blue, rev);
            return;
        case 46:
            GradientSelf(image, neg, iteration, red, green, blue, rev);
            return;
        case 47:
            Side2Side(image, neg, iteration, red, green, blue, rev);
            return;
        case 48:
            Top2Bottom(image, neg, iteration, red, green, blue, rev);
            return;
        case 49:
            Outward(image, neg, iteration, red, green, blue, rev);
            return;
        case 50:
            OutwardSquare(image, neg, iteration, red, green, blue, rev);
            return;
        case 51:
            ShiftPixels(image, neg, iteration, red, green, blue, rev);
            return;
        case 52:
            ShiftPixelsDown(image, neg, iteration, red, green, blue, rev);
            return;
        case 53:
            BlendWithSource(image, neg, iteration, red, green, blue, rev);
            return;
    }
    
    static double count = 1.0;
    unsigned int randomNumber = 1;
    for(int i = 0; i < image.width(); ++i) {
        for(int z = 0; z < image.height(); ++z) {
            unsigned int value = 0;
            unsigned char *buffer = pixelAt(image, i, z, value);
            changePixel(filter_num, image, i, z, buffer, iteration, &count, rev, neg, red, green, blue, randomNumber);
            QRgb val = qRgb(buffer[2], buffer[1], buffer[0]);
           	image.setPixel(i, z, val);
        }
    }
    std::cout << "Applied Filter #: " << filter_num << "\n";
}


void changePixel(unsigned int current_filterx, QImage &full_buffer, int i, int z, unsigned char *buffer, double pos, double *count, int reverse, bool negated, unsigned int red, unsigned int green, unsigned int blue, unsigned int randomNumber) {
    
    int width = full_buffer.width();
    int height = full_buffer.height();
    
    
    switch(current_filterx) {
        case 0:
        {
            double value = pos;
            buffer[0] = (unsigned char) value*buffer[0];
            buffer[1] = (unsigned char) value*buffer[1];
            buffer[2] = (unsigned char) value*buffer[2];
        }
            break;
        case 1:
        {
            
            double value = pos;
            buffer[0] = (unsigned char) value*buffer[0];
            buffer[1] = (unsigned char) (-value)*buffer[1];
            buffer[2] = (unsigned char) value*buffer[2];
        }
            break;
        case 2:
        {
            buffer[0] += buffer[0]*-pos;
            buffer[1] += buffer[1]*pos;
            buffer[2] += buffer[2]*-pos;
        }
            break;
        case 3:
        {
            int current_pos = pos*0.2f;
            buffer[0] = (i*current_pos)+buffer[0];
            buffer[2] = (z*current_pos)+buffer[2];
            buffer[1] = (current_pos*buffer[1]);
        }
            break;
        case 4:
        {
            int current_pos = pos*0.2f;
            buffer[0] = (z*current_pos)+buffer[0];
            buffer[1] = (i*current_pos)+buffer[1];
            buffer[2] = ((i+z)*current_pos)+buffer[2];
        }
            break;
        case 5:
        {
            int current_pos = pos*0.2f;
            buffer[0] = -(z*current_pos)+buffer[0];
            buffer[1] = -(i*current_pos)+buffer[1];
            buffer[2] = -((i+z)*current_pos)+buffer[2];
        }
            break;
            
        case 6:
        {
            int zq = z+1, iq = i+1;
            if(zq > height-1 || iq > width-1) return;
            unsigned int val;
            unsigned char *temp = pixelAt(full_buffer, iq, zq, val);
            buffer[0] += (i*pos)+temp[0];
            buffer[1] += (z*pos)+temp[1];
            buffer[2] += (i/(z+1))+temp[2];
        }
            break;
        case 7:
        {
            unsigned char colv[4], cola[4];
            colv[0] = buffer[0];
            colv[1] = buffer[1];
            colv[2] = buffer[2];
            cola[0] = buffer[2];
            cola[1] = buffer[1];
            cola[2] = buffer[0];
            unsigned int alpha = (int)pos;
            unsigned int red_values[] = { static_cast<unsigned int>(colv[0]+cola[2]), static_cast<unsigned int>(colv[1]+cola[1]), static_cast<unsigned int>(colv[2]+cola[0]), 0 };
            unsigned int green_values[] = { static_cast<unsigned int>(colv[2]+cola[0]), static_cast<unsigned int>(colv[1]+cola[1]), static_cast<unsigned int>(colv[0]+cola[2]), 0 };
            unsigned int blue_values[] = { static_cast<unsigned int>(colv[1]+cola[1]), static_cast<unsigned int>(colv[0]+cola[2]), static_cast<unsigned int>(colv[2]+cola[0]), 0 };
            unsigned char R = 0,G = 0,B = 0;
            for(unsigned int iq = 0; iq <= 2; ++iq) {
                R += red_values[iq];
                R /= 3;
                G += green_values[iq];
                G /= 3;
                B += blue_values[iq];
                B /= 3;
            }
            buffer[0] += alpha*R;
            buffer[1] += alpha*G;
            buffer[2] += alpha*B;
            
        }
            break;
        case 8:
        {
            unsigned char colv[4], cola[4];
            
            colv[0] = buffer[0];
            colv[1] = buffer[1];
            colv[2] = buffer[2];
            int iq = (width-i-1);
            int zq = (height-z-1);
            unsigned int value = 0;
            unsigned char *t = pixelAt(full_buffer, iq, zq, value);
            //cv::Vec3b &t = full_buffer.at<cv::Vec3b>(zq, iq);
            cola[0] = t[0];
            cola[1] = t[1];
            cola[2] = t[2];
            unsigned int alpha = (int)pos;
            unsigned int red_values[] = { static_cast<unsigned int>(colv[0]+cola[2]), static_cast<unsigned int>(colv[1]+cola[1]), static_cast<unsigned int>(colv[2]+cola[0]), 0 };
            unsigned int green_values[] = { static_cast<unsigned int>(colv[2]+cola[0]), static_cast<unsigned int>(colv[1]+cola[1]), static_cast<unsigned int>(colv[0]+cola[2]), 0 };
            unsigned int blue_values[] = { static_cast<unsigned int>(colv[1]+cola[1]), static_cast<unsigned int>(colv[0]+cola[2]), static_cast<unsigned int>(colv[2]+cola[0]), 0 };
            unsigned char R = 0,G = 0,B = 0;
            for(unsigned int iq = 0; iq <= 2; ++iq) {
                R += red_values[iq];
                R /= 3;
                G += green_values[iq];
                G /= 3;
                B += blue_values[iq];
                B /= 3;
            }
            buffer[0] += alpha*R;
            buffer[1] += alpha*G;
            buffer[2] += alpha*B;
        }
            break;
        case 9:
        {
            double alpha = pos;
            unsigned char colorz[3][3];
            colorz[0][0] = buffer[0];
            colorz[0][1] = buffer[1];
            colorz[0][2] = buffer[2];
            int total_r = colorz[0][0] +colorz[0][1]+colorz[0][2];
            total_r /= 3;
            total_r *= alpha;
            int iq = i+1;
            if(iq > width-1) return;
            int zq = z;
            if(zq > height-1) return;
            unsigned int value = 0;
            unsigned char *temp = pixelAt(full_buffer, iq, zq, value);
            //cv::Vec3b &temp = full_buffer.at<cv::Vec3b>(zq, iq);
            colorz[1][0] = temp[0];
            colorz[1][1] = temp[1];
            colorz[1][2] = temp[2];
            int total_g = colorz[1][0]+colorz[1][1]+colorz[1][2];
            total_g /= 3;
            total_g *= alpha;
            buffer[0] = (unsigned char)total_r;
            buffer[1] = (unsigned char)total_g;
            buffer[2] = (unsigned char)total_r+total_g*alpha;
            
        }
            break;
        case 10:
        {
            buffer[0] = ((i+z)*pos)/(i+z+1)+buffer[0]*pos;
            buffer[1] += ((i*pos)/(z+1))+buffer[1];
            buffer[2] += ((z*pos)/(i+1))+buffer[2];
        }
            break;
        case 11:
        {
            buffer[0] += (buffer[2]+(i*pos))/(pos+1);
            buffer[1] += (buffer[1]+(z*pos))/(pos+1);
            buffer[2] += buffer[0];
        }
            break;
        case 12:
        {
            buffer[0] += (i/(z+1))*pos+buffer[0];
            buffer[1] += (z/(i+1))*pos+buffer[1];
            buffer[2] += ((i+z)/(pos+1)+buffer[2]);
        }
            break;
        case 13:
        {
            buffer[0] += (pos*(i/(pos+1))+buffer[2]);
            buffer[1] += (pos*(z/(pos+1))+buffer[1]);
            buffer[2] += (pos*((i*z)/(pos+1)+buffer[0]));
        }
            break;
        case 14:
        {
            buffer[0] = ((i+z)*pos)/(i+z+1)+buffer[0]*pos;
            buffer[1] += (buffer[1]+(z*pos))/(pos+1);
            buffer[2] += ((i+z)/(pos+1)+buffer[2]);
        }
            break;
        case 15:
        {
            buffer[0] = (i%(z+1))*pos+buffer[0];
            buffer[1] = (z%(i+1))*pos+buffer[1];
            buffer[2] = (i+z%((int)pos+1))+buffer[2];
        }
            break;
        case 16:
        {
            unsigned int r = 0;
            r = (buffer[0]+buffer[1]+buffer[2])/3;
            buffer[0] += pos*r;
            buffer[1] += -(pos*r);
            buffer[2] += pos*r;
        }
            break;
        case 17:
        {
            unsigned long r = 0;;
            r = (buffer[0]+buffer[1]+buffer[2])/(pos+1);
            buffer[0] += r*pos;
            r = (buffer[0]+buffer[1]+buffer[2])/3;
            buffer[1] += r*pos;
            r = (buffer[0]+buffer[1]+buffer[2])/5;
            buffer[2] += r*pos;
        }
            break;
        case 18:
        {
            buffer[0] += 1+(sinf(pos))*z;
            buffer[1] += 1+(cosf(pos))*i;
            buffer[2] += (buffer[0]+buffer[1]+buffer[2])/3;
        }
            break;
        case 19:
        {
            buffer[0] += (buffer[2]-i)*((((int)pos+1)%15)+2);
            buffer[1] += (buffer[1]-z)*((((int)pos+1)%15)+2);
            buffer[2] += buffer[0]-(i+z)*((((int)pos+1)%15)+2);
        }
            break;
        case 20:
        {
            buffer[0] += (buffer[0]+buffer[1]-buffer[2])/3*pos;
            buffer[1] -= (buffer[0]-buffer[1]+buffer[2])/6*pos;
            buffer[2] += (buffer[0]-buffer[1]-buffer[2])/9*pos;
        }
            break;
        case 21:
        {
            int iq = i, zq = z+1;
            if(zq > height-2) return;
            unsigned int val1 = 0, val2 = 0;
            unsigned char *temp = pixelAt(full_buffer, iq, zq, val1);
            //cv::Vec3b &temp = full_buffer.at<cv::Vec3b>(zq, iq);
            zq = z+2;
            if(zq > height-2) return;

            unsigned char *temp2 = pixelAt(full_buffer, iq, zq, val2);
            //cv::Vec3b &temp2 = full_buffer.at<cv::Vec3b>(zq, iq);
            int ir, ig, ib;
            ir = buffer[0]+temp[0]-temp2[0];
            ig = buffer[1]-temp[1]+temp2[1];
            ib = buffer[2]-temp[2]-temp2[2];
            if(z%2 == 0) {
                if(i%2 == 0) {
                    buffer[0] = ir+(0.5*pos);
                    buffer[1] = ig+(0.5*pos);
                    buffer[2] = ib+(0.5*pos);
                } else {
                    buffer[0] = ir+(1.5*pos);
                    buffer[1] = ig+(1.5*pos);
                    buffer[2] = ib+(1.5*pos);
                }
            } else {
                if(i%2 == 0) {
                    buffer[0] += ir+(0.1*pos);
                    buffer[1] += ig+(0.1*pos);
                    buffer[2] += ib+(0.1*pos);
                } else {
                    buffer[0] -= ir+(i*pos);
                    buffer[1] -= ig+(z*pos);
                    buffer[2] -= ib+(0.1*pos);
                }
            }
        }
            break;
        case 22:
        {
            if((i%2) == 0) {
                if((z%2) == 0) {
                    buffer[0] = 1-pos*buffer[0];
                    buffer[2] = (i+z)*pos;
                } else {
                    buffer[0] = pos*buffer[0]-z;
                    buffer[2] = (i-z)*pos;
                }
            } else {
                if((z%2) == 0) {
                    buffer[0] = pos*buffer[0]-i;
                    buffer[2] = (i-z)*pos;
                } else {
                    buffer[0] = pos*buffer[0]-z;
                    buffer[2] = (i+z)*pos;
                }
            }
        }
            break;
        case 23:
        {
            buffer[0] = buffer[0]+buffer[1]*2+pos;
            buffer[1] = buffer[1]+buffer[0]*2+pos;
            buffer[2] = buffer[2]+buffer[0]+pos;
            
        }
            break;
        case 24:
        {
            buffer[0] += buffer[2]+pos;
            buffer[1] += buffer[1]+pos;
            buffer[2] += buffer[0]+pos;
        }
            break;
        case 25:
        {
            buffer[0] += (buffer[2]*pos);
            buffer[1] += (buffer[0]*pos);
            buffer[2] += (buffer[1]*pos);
        }
            break;
        case 26:
        {
            buffer[0] += (buffer[2]*pos)+i;
            buffer[1] += (buffer[0]*pos)+z;
            buffer[2] += (buffer[1]*pos)+i-z;
        }
            break;
        case 27:
        {
            buffer[0] = (-buffer[2])+z;
            buffer[1] = (-buffer[0])+i;
            buffer[2] = (-buffer[1])+pos;
        }
            break;
        case 28:
        {
            buffer[0] = buffer[2]+(1+(i*z)/pos);
            buffer[1] = buffer[1]+(1+(i*z)/pos);
            buffer[2] = buffer[0]+(1+(i*z)/pos);
        }
            break;
        case 29:
        {
            int iq = i, zq = z+1;
            if(zq > height-2) return;
            
            unsigned int val1 = 0, val2 = 0, val3 = 0, val4 = 0;
            //cv::Vec3b &temp = full_buffer.at<cv::Vec3b>(zq, iq);
            unsigned char *temp = pixelAt(full_buffer, iq, zq, val1);
            
            zq = z+2;
            if(zq > height-2) return;
            //cv::Vec3b &temp2 = full_buffer.at<cv::Vec3b>(zq, iq);
            unsigned char *temp2 = pixelAt(full_buffer, iq, zq, val2);
            zq = z+3;
            if(zq > height-2) return;
            
            unsigned char *temp3 = pixelAt(full_buffer, iq, zq, val3);
            //cv::Vec3b &temp3 = full_buffer.at<cv::Vec3b>(zq, iq);
            zq = z+4;
            if(zq > height-2) return;
            
            unsigned char *temp4 = pixelAt(full_buffer, iq, zq, val4);
            //cv::Vec3b &temp4 = full_buffer.at<cv::Vec3b>(zq, iq);
            
            unsigned char col[4];
            
            col[0] = (temp[0]+temp2[0]+temp3[0]+temp4[0])/4;
            col[1] = (temp[1]+temp2[1]+temp3[1]+temp4[1])/4;
            col[2] = (temp[2]+temp2[2]+temp3[2]+temp4[2])/4;
            
            buffer[0] = col[0]*pos;
            buffer[1] = col[1]*pos;
            buffer[2] = col[2]*pos;
            
        }
            break;
        case 30:
        {
            
            double rad = 100.0;
            double degree = 0.01*pos;
            int x = (int)rad * cos(degree);
            int y = (int)rad * sin(degree);
            int z = (int)rad * tanf((double)degree);
            buffer[0] = buffer[0]+x;
            buffer[2] = buffer[1]+y;
            buffer[1] = buffer[1]+z;
            
        }
            break;
        case 31:
        {
            int average= (buffer[0]+buffer[1]+buffer[2]+1)/3;
            buffer[0] += buffer[2]+average*(pos);
            buffer[1] += buffer[0]+average*(pos);
            buffer[2] += buffer[1]+average*(pos);
        }
            break;
        case 32:
        {
            unsigned char value = 0;
            value  = (~buffer[0] + ~buffer[1] + ~buffer[2]);
            buffer[0] += (value*pos)/4;
            buffer[1] += value*pos;
        }
            break;
        case 33:
        {
            
            
            buffer[0] += *count*pos;
            buffer[1] += *count*pos;
            buffer[2] += *count*pos;
            
            *count += 0.00001f;
            if(*count > 255) *count = 0;
            
            
            
        }
            break;
        case 34:
        {
            buffer[0] += pos*(randomNumber+pos);
            buffer[1] += pos*(randomNumber+z);
            buffer[2] += pos*(randomNumber+i);
        }
            break;
        case 35:
        {
            buffer[0] += *count *z;
            buffer[1] += *count *pos;
            buffer[2] += *count *z;
            
            *count += 0.0000001f;
            
            
        }
            break;
        case 36:
        {
            buffer[0] += sinf(M_PI+pos)*pos;
            buffer[1] += cosf(M_PI+pos)*pos;
            buffer[2] += tanf(M_PI+pos)*pos;
        }
            break;
    }
    
    ApplyOptions(buffer, negated, red, green, blue, reverse);
}
