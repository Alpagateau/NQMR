#ifndef NQME_GFX_H 
#define NQME_GFX_H

#include <nds.h>
//Let's try this 
#include <string>
#include "nqme_math.hpp"
#include "nqme_debug.hpp"
#include <NEMain.h>

//===========
//= G O A L =
//===========

/*

-> Top Screen    : 3D
-> Bottom Screen : 2D

*/

namespace NQME{


#define MODEL_STACK_SIZE  10
#define SPRITE_STACK_SIZE 20
#define TEXT_STACK_SIZE    5
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

// 2D Rendering

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
    SpriteHeader();
    SpriteHeader(
        void *tiles,
        u32 tileSize, 
        u8 type);
    ~SpriteHeader();
    u8 GetWidth();
    u8 GetHeight();
    void Load(
        void *tiles,
        u32 tileSize, 
        u8 type);
};

class SpriteAllocater
{
private:
    
    u8 getFirst();
public:
    SpriteAllocater();
    u8 regFirst();
    u8 mask[32]; //256 bits bitmask TO MOVE TO PRIVATE
    bool free(u8 n);
    bool get(u8 n);
    bool set(u8 n, bool v);
};


class Sprite2D
{
public:
    Sprite2D();
    Sprite2D(u8 _id);
    Sprite2D(u8 _id, SpriteHeader &h);
    ~Sprite2D();
    Sprite2D SetHeader(SpriteHeader &h);
    void _SetPosition(int x, int y);
    void SetPosition(int x, int y);
    void Update();
    SpriteHeader* header;
    bool visible;
    Vector2i position;
    Vector2i anchor;
    u16 offset;
    u16 palIndex;
    u8 id;
    SpriteAllocater* sa;
private:
//    u8 screen = SUB_SCREEN;
    bool needRedraw;
    Vector2i oldPos;
};

//3D Engine

struct Transform 
{
  Vector3f position = (Vector3f){0.0f, 0.0f, 0.0f};
  Vector3f rotation = (Vector3f){0.0f, 0.0f, 0.0f};
  Vector3f scale    = (Vector3f){1.0f, 1.0f, 1.0f};
};

struct Transform2D 
{
  Vector2i position = (Vector2i){0, 0};
  float scale = 1.0f;
  float angle = 0.0f;
};

class StaticModel 
{
public:
  Transform transform;
  NE_Model *mesh; 
  StaticModel();
  StaticModel(const char *path);
  void Draw();
};

class NitroSprite 
{
public:
  Vector2i dimensions = {64, 64};
  Transform2D transform;
  Vector2i uv_position = (Vector2i){0, 0};   
  Vector2i uv_dimensions = (Vector2i){0,0};
  Vector2i anchor = (Vector2i){0,0};
  u8 index = 0;
  NE_Sprite *sprite;
  NitroSprite();
  NitroSprite(int w, int h);
  void Draw();
};

class BasicText
{
public:
  std::string text;
  Vector2i position;
  u8 channel;
  BasicText();
  void Draw();
};

struct DrawStack 
{
  NE_Camera *camera = nullptr; 
  NE_Model  *models[MODEL_STACK_SIZE];
  NE_Sprite *sprites[SPRITE_STACK_SIZE];
  BasicText *texts[TEXT_STACK_SIZE];
  int model_count;
  int sprite_count;
  int text_count;
};

void Draw3DScene(void* args);

//Initialize Backgrounds
//Currently only allows for  BgType_Text8bpp, BgSize_T_256x256 settings
int InitBG();
int SetBackgroundSub(BGHeader header);
int SetBackgroundPaletteSub(void* source, u32 size);
int SetBackgroundMain(BGHeader header);
int SetBackgroundPaletteMain(void* source, u32 size);
int SetSpritePalette(void* source, u32 size);
int InitSprites();

int InitGfx();
void UseCamera(NE_Camera *cam);
void UpdateGraphics();
void Draw3DScene(void *args);

}

#endif
