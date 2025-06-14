#ifndef NOTQUITE_H
#define NOTQUITE_H

#include "nqmt_engine.hpp"

#define DOWN  0
#define RIGHT 1
#define UP    2
#define LEFT  3

typedef struct{
    unsigned short id;
    unsigned short type;
    Vector2i position;
    bool isValid;
}Arrow;

#pragma pack(push, 1)
typedef struct 
{
    u8 type;
    s32 start;
    u16 duration;
}BMNote;
#pragma pack(pop)

int SetArrowSprite(NQMT::SpriteHeader sh);

int SetArrow(Arrow ar);

int loadBeatMap(const char* path);

int readNextBeat();

int updateBeatBuffer();

void fromBeatmap(Arrow *arrows, int size);

u16* getArrowOffset();

#endif