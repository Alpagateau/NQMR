#include "notquite.hpp"

u16 *arrow_sprite;

#define OFFSET 32

int SetArrowSprite(SpriteHeader sh)
{
    arrow_sprite = oamAllocateGfx(&oamMain, SpriteSize_32x32, SpriteColorFormat_256Color);
    //Load the palette first 
	dmaCopy(sh.pal, SPRITE_PALETTE, sh.palSize);
	//Load the tile data 
	dmaCopy(sh.tiles, arrow_sprite, sh.tileSize);
    return 0;
}

int SetArrow(Arrow ar)
{
    if(ar.position.y > -48 && ar.position.y < 192 && ar.isValid){
        oamSet(
            &oamMain,       //Screen 
            ar.id,          //Sprite ID
            ar.position.x, ar.position.y, // Position
            0,              //Priority
            0,              //Alpha
            SpriteSize_32x32, SpriteColorFormat_256Color, //Format and color
            arrow_sprite + (ar.type * 128 * 4), //Pointer to the tiles
            -1,             // affine
            false,          // extended
            false,          // hide
            false, false,   // flips
            false           // mosaic
        );
    }
    return 0;
}

// Beatmap thingy

#define BBM_BUFFER_SIZE 16

FILE *bbmFile = NULL;
BMNote bmBuffer[BBM_BUFFER_SIZE] = {0};
int buffer_head = 0;

int loadBeatMap(const char* path)
{
    bbmFile = fopen(path, "rb");
    if(bbmFile == NULL)
        return 1;

    BMNote temp_n;
    int ctr = 0;
    while(fread(&temp_n, sizeof(BMNote), 1, bbmFile) && ctr < BBM_BUFFER_SIZE)
    {
        bmBuffer[ctr] = temp_n;
        //printf("NOTE : %d : %d ms for %d ms\n", temp_n.type, temp_n.start, temp_n.duration);
        ctr++;
    }
    // printf("First part loaded with %d\n", ctr);
    buffer_head = ctr%BBM_BUFFER_SIZE;
    while(ctr < BBM_BUFFER_SIZE)
    {
        bmBuffer[ctr] = (BMNote){0, -1, 0};
        ctr++;
    }
    printf("beatmap loaded\n");
    return 0;
}

int readNextBeat(int i)
{
    BMNote temp_n;
    if(fread(&temp_n, sizeof(BMNote), 1, bbmFile) != 0)
    {
        bmBuffer[i] = temp_n;
        return 0;
    }
    else 
    {
        bmBuffer[i] = (BMNote){0, -1, 0};
        return 1;
    }
    //buffer_head = (i+1)%BBM_BUFFER_SIZE;
    return 0;
}

int updateBeatBuffer()
{
    int min_delta = 64;
    for(int i = 0; i < BBM_BUFFER_SIZE; i++)
    {
        int delta = NQMT::getTime() - bmBuffer[i].start;
        if(delta >= min_delta)
        {
            printf("Updated arrow %d\n", i);
            readNextBeat(i);
        }
    }
    return 0;
}

void fromBeatmap(Arrow *arrows, int size)
{
    int m = size;
    if(BBM_BUFFER_SIZE < m)
        m = BBM_BUFFER_SIZE;
    
    for(int i = 0; i < m; i++)
    {
        int delta = (bmBuffer[i].start - NQMT::getTime());
        arrows[i].type = bmBuffer[i].type;
        arrows[i].position.x = 25 + bmBuffer[i].type * 50;
        arrows[i].position.y =  (delta / 8);
        arrows[i].isValid    = bmBuffer[i].start >= 0;
        if(arrows[i].position.y < 192){
            //printf("Aw #%d, [%d,%d] : %d\n", i, arrows[i].position.x, arrows[i].position.y, arrows[i].isValid);
            if(i == 0)
                printf("delta : %d\n", delta);
        }
    }
}

u16* getArrowOffset()
{
    return arrow_sprite;
}