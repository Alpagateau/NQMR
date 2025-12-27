#include "nqme_tile.hpp"

struct tileset load_tileset(
    unsigned int *tiles, unsigned int len
)
{
    struct tileset ts;

    ts.buffer = (unsigned int*)malloc(len);
    ts.len = len/4;
    return ts;
}


struct tile drawTile(
    const unsigned char *data, int len
)
{
    struct tile t;    
    for(int i = 0; i < len; i++)
    {
        t.segments[i >> 4] = ~(~t.segments[i >> 4] |(0xFF << (8 - (2*i)))) | (data[i] << (8 - (2*i)));
    }
    return t;
}
