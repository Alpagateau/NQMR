#include "nqmt_gfx.hpp"


int bg;
namespace NQMT{



SpriteSetting decodeSS(u8 spr)
{
    printf("Decoding : %d\n", spr);
    SpriteSize ss;
    SpriteColorFormat scf;

    switch(spr) {
        // 16-color sprites
        case SQ8_16:        ss = SpriteSize_8x8;     scf = SpriteColorFormat_16Color; break;
        case SQ16_16:       ss = SpriteSize_16x16;   scf = SpriteColorFormat_16Color; break;
        case SQ32_16:       ss = SpriteSize_32x32;   scf = SpriteColorFormat_16Color; break;
        case SQ64_16:       ss = SpriteSize_64x64;   scf = SpriteColorFormat_16Color; break;
        case R8x16_16:      ss = SpriteSize_8x16;    scf = SpriteColorFormat_16Color; break;
        case R8x32_16:      ss = SpriteSize_8x32;    scf = SpriteColorFormat_16Color; break;
        case R16x32_16:     ss = SpriteSize_16x32;   scf = SpriteColorFormat_16Color; break;
        case R32x16_16:     ss = SpriteSize_32x16;   scf = SpriteColorFormat_16Color; break;
        case R32x64_16:     ss = SpriteSize_32x64;   scf = SpriteColorFormat_16Color; break;
        case R64x32_16:     ss = SpriteSize_64x32;   scf = SpriteColorFormat_16Color; break;
        case R32x8_16:      ss = SpriteSize_32x8;    scf = SpriteColorFormat_16Color; break;
        case R16x8_16:      ss = SpriteSize_16x8;    scf = SpriteColorFormat_16Color; break;

        // 256-color sprites
        case SQ8_256:       ss = SpriteSize_8x8;     scf = SpriteColorFormat_256Color; break;
        case SQ16_256:      ss = SpriteSize_16x16;   scf = SpriteColorFormat_256Color; break;
        case SQ32_256:      ss = SpriteSize_32x32;   scf = SpriteColorFormat_256Color; break;
        case SQ64_256:      ss = SpriteSize_64x64;   scf = SpriteColorFormat_256Color; break;
        case R8x16_256:     ss = SpriteSize_8x16;    scf = SpriteColorFormat_256Color; break;
        case R8x32_256:     ss = SpriteSize_8x32;    scf = SpriteColorFormat_256Color; break;
        case R16x32_256:    ss = SpriteSize_16x32;   scf = SpriteColorFormat_256Color; break;
        case R32x16_256:    ss = SpriteSize_32x16;   scf = SpriteColorFormat_256Color; break;
        case R32x64_256:    ss = SpriteSize_32x64;   scf = SpriteColorFormat_256Color; break;
        case R64x32_256:    ss = SpriteSize_64x32;   scf = SpriteColorFormat_256Color; break;
        case R32x8_256:     ss = SpriteSize_32x8;    scf = SpriteColorFormat_256Color; break;
        case R16x8_256:     ss = SpriteSize_16x8;    scf = SpriteColorFormat_256Color; break;

        default:
            ss = SpriteSize_32x32; // default size
            scf = SpriteColorFormat_16Color; // default format
            break;
    }

    printf("Returning %d, %d", ss, scf);
    return (SpriteSetting){ss, scf};
}

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

int SetPalette(void* source, u32 size)
{
    dmaCopy(source, BG_PALETTE, size);
    return 0;
}

Sprite2D::Sprite2D()
{
    position = (Vector2i){0, 0};
}

Sprite2D::Sprite2D(SpriteHeader &h)
{
    header = &h;
    position = (Vector2i){0, 0};
}

Sprite2D Sprite2D::SetHeader(SpriteHeader &h)
{
    header = &h;
    return *this;
}

void Sprite2D::Update()
{
    SpriteSetting s = decodeSS(header->type);
    oamSet(&oamMain,
            id, // Sprite ID (0 to 127)
            position.x , position.y, // X, Y
            0, // Priority
            0, // Palette index
            s.ss, s.scf, // Size, format
            header->addr,  // Graphics offset
            -1, // Affine index (unused in this example)
            false, // Double size for affine sprites
            false, // Hide
            false, false, // H flip, V flip
            false); // Mosaic
}

SpriteHeader::SpriteHeader(  
    void *tiles,
    u32 tileSize,
    void *pal,
    u32 palSize,
    u8 _type
)
{
    SpriteSetting s = decodeSS(_type);
    type = _type;
    addr = oamAllocateGfx(&oamMain, s.ss, s.scf);
    // Copy tiles to the space assigned to this sprite
    dmaCopy(tiles, addr, tileSize);

    // Copy palette to the palette RAM
    dmaCopy(pal, SPRITE_PALETTE, palSize);
}

SpriteHeader::~SpriteHeader()
{
    oamFreeGfx(&oamMain, addr);
}

}