#ifndef NQME_COLOR_HPP
#define NQME_COLOR_HPP

#define COLOR_BLACK 0b0000000000000000
#define COLOR_WHITE 0b0111111111111111
#define COLOR_RED   0b0000000000011111
#define COLOR_GREEN 0b0000001111100000
#define COLOR_BLUE  0b0111110000000000
#define COLOR_GRAY  0b0110001100011000

typedef struct NQMPalette 
{
    int len = 0;
    short *data;
};

NQMPalette new_runtime_palette(int size);
NQMPalette make_palette_runtime(const short unsigned int*, int size);
short color_rgb(unsigned char r, unsigned char g, unsigned char b);

#endif
