#ifndef NQME_TILE_HPP
#define NQME_TILE_HPP

#include <cstdlib>

struct tileset
{
    unsigned int len;
    unsigned int *buffer;
};

struct tile {
    unsigned int segments[16];
};

struct tileset load_tileset(
    unsigned int *tiles, unsigned int len
);

struct tile drawTile(
    const unsigned char *data, int len
);

#endif
