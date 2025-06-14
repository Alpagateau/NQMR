#pragma once
#ifndef NQMT_GFX_H 
#define NQMT_GFX_H

#include <nds.h>

//===========
//= G O A L =
//===========

/*

- Sprite loading from grit integrated AND filesystem.
- Sprites either bitmap or 
- Make sprite rendering easier
Same with backgrounds
*/
//Sprite types 
/*
#define SQ8_16      0
#define SQ16_16     1
#define SQ32_16     2
#define SQ64_16     3
#define R8x16_16    4
#define R8x32_16    5
#define R16x32_16   6
#define R32x16_16   7
#define R32x64_16   8
#define R64x32_16   9
#define R32x8_16   10
#define R16x8_16   11
#define SQ8_256    12
#define SQ16_256   13
#define SQ32_256   14
#define SQ64_256   16
#define R8x16_256  17
#define R8x32_256  18
#define R16x32_256 19
#define R32x16_256 20
#define R32x64_256 21
#define R64x32_256 22
#define R32x8_256  23
#define R16x8_256  24
#define SQ16_256   25
#define SQ32_256   26
#define SQ64_256   27
#define R8x16_256  28
#define R8x32_256  29
#define R16x32_256 30
#define R32x16_256 31
#define R32x64_256 32
#define R64x32_256 33
#define R32x8_256  34
#define R16x8_256  35
*/
typedef struct 
{
    void *tiles;
    u32 tileSize;
    void *map;
    u32 mapSize;
}BGHeader;

typedef struct 
{
    void *tiles;
    u32 tileSize;
    void *pal;
    u32 palSize;
}SpriteHeader;

//Currently only allows for  BgType_Text8bpp, BgSize_T_256x256 settings
int InitBG();
int SetBackground(BGHeader header);

int InitSprites();

int InitGfx();

#endif