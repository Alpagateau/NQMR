#include "nqme_color.hpp"
#include <cstdlib>

short color_rgb(unsigned char r, unsigned char g, unsigned char b)
{
    short col = 0; 
    col = (r >> 3) | ((g >> 3) << 5) | ((b >> 3) << 10);
    return col;
}

NQMPalette new_runtime_palette(int size)
{
    NQMPalette pal = {.len = size, .data = NULL};
    pal.data = (short*)malloc(
        sizeof(short) * size
    );
    return pal;
}

NQMPalette make_palette_runtime(const short unsigned int *source, int size)
{
    NQMPalette pal = {.len = size, .data = NULL};
    pal.data = (short*)malloc(
        sizeof(short) * size
    );
    //int* temp = (int*)pal.data;
    for(int i = 0; i < size; i++)
    {
        pal.data[i] = source[i];
        //temp[(i*2)] = short(source[i] & 0x0000FFFF);
    }
    return pal;
}
