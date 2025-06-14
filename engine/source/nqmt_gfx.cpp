#include "nqmt_gfx.hpp"

int bg;

int InitBG()
{
    bg = bgInit(0, BgType_Text8bpp, BgSize_T_256x256, 0, 1);
    return 0;
}

int SetBackground(BGHeader header)
{
	dmaCopy(header.tiles, bgGetGfxPtr(bg), header.tileSize);
	dmaCopy(header.map, bgGetMapPtr(bg), header.mapSize);
    return 0;
}

int InitSprites()
{
    oamInit(&oamMain, SpriteMapping_1D_128, false);
    return 0;
}

int InitGfx()
{
    InitBG();
    InitSprites();
    return 0;
}