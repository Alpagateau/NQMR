#pragma once
#ifndef NQMT_GFX_H 
#define NQMT_GFX_H

#include <nds.h>
#include "nqmt_math.hpp"

//===========
//= G O A L =
//===========

/*

-> Top Screen    : 2D
-> Bottom Screen : 2D

*/

namespace NQMT{

//Sprite types 
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

#define SCREEN_WIDTH 256
#define SCREEN_HEIGHT 192

struct BGHeader
{
    void *tiles;
    u32 tileSize;
    void *map;
    u32 mapSize;
};

struct SpriteSetting
{
    SpriteSize ss;
    SpriteColorFormat scf;
};

SpriteSetting decodeSS(u8 spr);

class SpriteHeader
{
public:
    
    void *tiles;
    u32 tileSize;
    u16 *addr;
    u8 type;
    SpriteHeader(
        void *tiles,
        u32 tileSize, 
        u8 type);
    ~SpriteHeader();
    u8 GetWidth();
    u8 GetHeight();
};

class Sprite2D
{
public:
    Sprite2D(u8 _id);
    Sprite2D(u8 _id,SpriteHeader &h);
    Sprite2D SetHeader(SpriteHeader &h);
    void Update();
    SpriteHeader* header;
    bool visible;
    Vector2i position;
    u16 offset;
    u16 palIndex;
    u8 id;
private:
    bool needRedraw;
    Vector2i oldPos;
};


//Initialize Backgrounds
//Currently only allows for  BgType_Text8bpp, BgSize_T_256x256 settings
int InitBG();
int SetBackground(BGHeader header);
int SetBackgroundPalette(void* source, u32 size);
int SetSpritePalette(void* source, u32 size);
int InitSprites();

int InitGfx();

}
#endif