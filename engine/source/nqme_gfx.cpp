#include "nqme_gfx.hpp"

namespace NQME{

int bg_sub, bg_main;
SpriteAllocater SA;
DrawStack DS;

SpriteSetting decodeSS(u8 spr)
{
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
    return (SpriteSetting){ss, scf};
}

int InitBG()
{
    bg_sub  = bgInitSub(2, BgType_Text4bpp, BgSize_T_256x256, 0, 1);
    //bg_main = bgInit   (0, BgType_Text8bpp, BgSize_T_256x256, 0, 1);
    return 0;
}

int SetBackgroundSub(BGHeader header)
{
	dmaCopy(header.tiles, bgGetGfxPtr(bg_sub), header.tileSize);
	dmaCopy(header.map, bgGetMapPtr(bg_sub), header.mapSize);
    return 0;
}


int SetBackgroundPaletteSub(void* source, u32 size)
{
    //dmaCopy(source, BG_PALETTE, size);
    dmaCopy(source, BG_PALETTE_SUB, size);
    return 0;
}

int SetBackgroundMain(BGHeader header)
{
	dmaCopy(header.tiles, bgGetGfxPtr(bg_sub), header.tileSize);
	dmaCopy(header.map, bgGetMapPtr(bg_sub), header.mapSize);
    return 0;
}

int SetBackgroundPaletteMain(void* source, u32 size)
{
    //dmaCopy(source, BG_PALETTE, size);
    dmaCopy(source, BG_PALETTE_SUB, size);
    return 0;
}

int InitSprites()
{
    SA = SpriteAllocater();
    //oamInit(&oamMain, SpriteMapping_1D_128, false);
    oamInit(&oamSub, SpriteMapping_1D_128, false);
    return 0;
}

int InitGfx()
{
    irqEnable(IRQ_HBLANK);
    irqSet(IRQ_VBLANK, NE_VBLFunc);
    irqSet(IRQ_HBLANK, NE_HBLFunc);

    InitBG();
    InitSprites();
    NE_Init3D();
    return 0;
}

int SetSpritePalette(void *source, u32 size)
{
    dmaCopy(source, SPRITE_PALETTE_SUB, size);
    return 0;
}

Sprite2D::Sprite2D()
{
    position = (Vector2i){0, 0};
    id = SA.regFirst();
    offset = 0;
}

Sprite2D::Sprite2D(u8 _id)
{
    position = (Vector2i){0, 0};
    anchor = (Vector2i){0, 0};
    id = _id;
    SA.set(id, true);
    sa = &SA;
    offset = 0;
}

Sprite2D::Sprite2D(u8 _id,SpriteHeader &h)
{
    header = &h;
    id = _id;
    SA.set(id, true);
    offset = 0;
    position = (Vector2i){0, 0};
    anchor = (Vector2i){0, 0};
}

Sprite2D::~Sprite2D()
{
    SA.free(id);
}

Sprite2D Sprite2D::SetHeader(SpriteHeader &h)
{
    header = &h;
    return *this;
}

void Sprite2D::_SetPosition(int x, int y)
{
    position.x = x;
    position.y = y;
}

void Sprite2D::SetPosition(int x, int y)
{
    position.x = x;
    position.y = y;
    oamSetXY(&oamSub, id, x - anchor.x, y - anchor.y);
    //oamSetXY(&oamMain, id, x - anchor.x, y - anchor.y);
}

void Sprite2D::Update()
{
    
    SpriteSetting s = decodeSS(header->type);
    u8 w = header->GetWidth();
    u8 h = header->GetHeight();
    bool visible = (position.x+w > 0) && (position.x < SCREEN_WIDTH)
            && (position.y+h > 0) && (position.y < SCREEN_HEIGHT);
    //oamSet(&oamMain,
    oamSet(&oamSub,
            id, // Sprite ID (0 to 127)
            position.x - anchor.x, position.y - anchor.y, // X, Y
            0, // Priority
            0, // Palette index
            s.ss, s.scf, // Size, format
            header->addr + ((int)offset * 64),  // Graphics offset
            -1, // Affine index (unused in this example)
            false, // Double size for affine sprites
            !visible, // Hide
            false, false, // H flip, V flip
            false
        ); // Mosaic
}

SpriteHeader::SpriteHeader()
{
}

SpriteHeader::SpriteHeader(  
    void *tiles,
    u32 tileSize,
    u8 _type
)
{
    SpriteSetting s = decodeSS(_type);
    type = _type;
    addr = oamAllocateGfx(&oamSub, s.ss, s.scf);
    // Copy tiles to the space assigned to this sprite
    dmaCopy(tiles, addr, tileSize);
}

SpriteHeader::~SpriteHeader()
{
    oamFreeGfx(&oamSub, addr);
}

u8 SpriteHeader::GetWidth()
{
    u8 output;
    switch(type) {
        // 16-color sprites
        case SQ8_16:        output = 8; break;
        case SQ16_16:       output = 16; break;
        case SQ32_16:       output = 32; break;
        case SQ64_16:       output = 64; break;
        case R8x16_16:      output = 8; break;
        case R8x32_16:      output = 8; break;
        case R16x32_16:     output = 16; break;
        case R32x16_16:     output = 32; break;
        case R32x64_16:     output = 32; break;
        case R64x32_16:     output = 64; break;
        case R32x8_16:      output = 32; break;
        case R16x8_16:      output = 16; break;

        // 256-color sprites
        case SQ8_256:       output = 8; break;
        case SQ16_256:      output = 16; break;
        case SQ32_256:      output = 32; break;
        case SQ64_256:      output = 64; break;
        case R8x16_256:     output = 8; break;
        case R8x32_256:     output = 8; break;
        case R16x32_256:    output = 16; break;
        case R32x16_256:    output = 32; break;
        case R32x64_256:    output = 32; break;
        case R64x32_256:    output = 64; break;
        case R32x8_256:     output = 32; break;
        case R16x8_256:     output = 16; break;

        default:
            output = 64;
            break;
    }
    return output;
}
u8 SpriteHeader::GetHeight()
{
    u8 output;
    switch(type) {
        // 16-color sprites
        case SQ8_16:        output = 8; break;
        case SQ16_16:       output = 16; break;
        case SQ32_16:       output = 32; break;
        case SQ64_16:       output = 64; break;
        case R8x16_16:      output = 16; break;
        case R8x32_16:      output = 32; break;
        case R16x32_16:     output = 32; break;
        case R32x16_16:     output = 16; break;
        case R32x64_16:     output = 64; break;
        case R64x32_16:     output = 32; break;
        case R32x8_16:      output = 8; break;
        case R16x8_16:      output = 8; break;

        // 256-color sprites
        case SQ8_256:       output = 8; break;
        case SQ16_256:      output = 16; break;
        case SQ32_256:      output = 32; break;
        case SQ64_256:      output = 64; break;
        case R8x16_256:     output = 16; break;
        case R8x32_256:     output = 32; break;
        case R16x32_256:    output = 32; break;
        case R32x16_256:    output = 16; break;
        case R32x64_256:    output = 64; break;
        case R64x32_256:    output = 32; break;
        case R32x8_256:     output = 8; break;
        case R16x8_256:     output = 8; break;

        default:
            output = 64;
            break;
    }
    return output;
}

void SpriteHeader::Load(
    void *tiles,
    u32 tileSize, 
    u8 _type)
{
    SpriteSetting s = decodeSS(_type);
    type = _type;
    addr = oamAllocateGfx(&oamSub, s.ss, s.scf);
    // Copy tiles to the space assigned to this sprite
    dmaCopy(tiles, addr, tileSize);
}


SpriteAllocater::SpriteAllocater()
{
    //Sets the mask to 0
    for(int i = 0; i < 32; i++)
    {
        mask[i] = 0;
    }
}

bool SpriteAllocater::get(u8 n)
{
    //Get batch index
    u8 idx = n % 8;
    u8 batch = (n-idx)/8;

    return ((1 << idx) & mask[batch]) != 0;
}

bool SpriteAllocater::set(u8 n, bool v)
{
    u8 idx = n % 8;
    u8 batch = (n-idx)>>3;
    if(v)
    {
        mask[batch] |= (1 << idx);
    }
    else 
    {
        mask[batch] = ~( ~(mask[batch]) | (1 << idx) );
    }
    return true;
}

u8 SpriteAllocater::getFirst()
{
    u8 batch = 0;
    while(mask[batch] == 0xFF)
    {
        batch++;
    }
    u8 idx = 0;
    while((mask[batch] & (1 << idx) ) != 0)
    {
        idx ++;
        if(idx >= 8)
        {
            printf("Error in the get first value");
        }
    }
    //printf("batch : %u | idx : %u\n", batch, idx);
    return (batch << 3) + idx;
}

u8 SpriteAllocater::regFirst()
{
    u8 idx = getFirst();
    set(idx, true);
    return idx;
}

bool SpriteAllocater::free(u8 n)
{
    return set(n, false);
}

StaticModel::StaticModel()
{
  mesh = NE_ModelCreate(NE_Static); 
}

NitroSprite::NitroSprite()
{
  sprite = NE_SpriteCreate();
}

NitroSprite::NitroSprite(int w, int h)
{
  sprite = NE_SpriteCreate();
  dimensions = (Vector2i){w, h};
  NE_SpriteSetSize(sprite, w, h);
}

StaticModel::StaticModel(const char *path)
{
  mesh = NE_ModelCreate(NE_Static);
  NE_ModelLoadStaticMeshFAT(mesh, path);
}

Vector2i NitroSprite::GetOffset()
{
  int w = dimensions.x;
  int h = dimensions.y;
  Vector2i output = {0, 0};
  switch(centering)
  {
    case SPT_TL:
      break;
    case SPT_TM:
      output.x = w/2;
      break;
    case SPT_TR:
      output.x = w;
      break;
    case SPT_ML:
      output.y = (h/2);
      break;
    case SPT_MM:
      output.x = (w/2);
      output.y = (h/2);
      break;
    case SPT_MR:
      output.x = w;
      output.y = (h/2);
      break;
    case SPT_DL:
      output.y = h;
      break;
    case SPT_DM:
      output.y = h;
      output.x = (w/2);
      break;
    case SPT_DR:
      output.y = h;
      output.x = w;
  }
  //printf("[DEBUG] OFFSET : %d, %d\n", output.x, output.y);
  return output;
}

void NitroSprite::Draw()
{
    if(DS.sprite_count >= SPRITE_STACK_SIZE)
    {
      DEBUG_PRINT("Not enough space for nsprites\n");   
      return;
    }
    NE_SpriteSetParams(sprite, 0x1F, DS.sprite_count, tint);
    NE_SpriteSetPriority(sprite, (int)index+1);
    //Vector2i old_pos = transform.position; 
    Vector2i delta = GetOffset();
    Vector2i new_pos = {
      transform.position.x - delta.x - (int)(anchor.x * transform.scale),
      transform.position.y - delta.y - (int)(anchor.y * transform.scale)
    };
    NE_SpriteSetPos(
        sprite, 
        new_pos.x,
        new_pos.y
        //transform.position.x - (int)(anchor.x * transform.scale), 
        //transform.position.y - (int)(anchor.y * transform.scale)
    );
    NE_SpriteSetRot(
      sprite,
      transform.angle
    );
    
    int u = (uv_dimensions.x == 0) ? dimensions.x : uv_dimensions.x; 
    int v = (uv_dimensions.y == 0) ? dimensions.y : uv_dimensions.y;

    NE_SpriteSetMaterialCanvas(
        sprite,
        uv_position.x,
        uv_position.y,
        uv_position.x + u,
        uv_position.y + v
    );

    NE_SpriteSetSize(
      sprite,
      (int)(dimensions.x * transform.scale),
      (int)(dimensions.y * transform.scale)
    );
    DS.sprites[DS.sprite_count] = sprite;
    DS.sprite_count ++;
}

BasicText::BasicText()
{
    channel = 0;
    text[0] = 0;
}

void BasicText::Draw()
{
    if(DS.text_count >= TEXT_STACK_SIZE)
        return;
    DS.texts[DS.text_count] = this;
    DS.text_count++;
}

void StaticModel::Draw()
{
  if(DS.model_count < MODEL_STACK_SIZE)
  {
    NE_ModelSetCoord(
      mesh, 
      transform.position.x,
      transform.position.y,
      transform.position.z
    );
    NE_ModelScale(
      mesh,
      transform.scale.x,
      transform.scale.y,
      transform.scale.z
    );
    NE_ModelSetRot(
      mesh,
      transform.rotation.x,
      transform.rotation.y,
      transform.rotation.z
    );
    
    DS.models[DS.model_count] = mesh;
    DS.model_count++;
  }
}

void UseCamera(NE_Camera *cam)
{
  DS.camera = cam;
}

void UpdateGraphics()
{
  oamUpdate(&oamSub);
  NE_ProcessArg(Draw3DScene, &DS);
}

void Draw3DScene(void *args)
{
  DrawStack *ds = (DrawStack*)args;
  if(ds->camera != nullptr)
    NE_CameraUse(ds->camera);
  
  //3D Models [Not really implemented yet
  for(int i = 0; i < ds->model_count; i++)
  {
    NE_ModelDraw(ds->models[i]); 
  }
  ds->model_count = 0;

  //2D Sprites on main engine
  NE_2DViewInit();
  for(int i = 0; i < ds->sprite_count; i++)
  {
    NE_SpriteDraw(ds->sprites[i]);
  }
  ds->sprite_count = 0;
  NE_RichTextPrioritySet(1);
  for(int i = 0; i < ds->text_count; i++)
  { 
    if(ds->texts[i]->centering == TEXT_DEFAULT)
    {
      if(ds->texts[i]->alpha < 32){
        NE_RichTextRender3D(
          ds->texts[i]->channel,
          ds->texts[i]->text,
          ds->texts[i]->position.x,
          ds->texts[i]->position.y,
          POLY_ALPHA( ds->texts[i]->alpha)
          | POLY_CULL_BACK ,
          2
        );
      }
      else 
      {
        
      }
    }else if(ds->texts[i]->centering == TEXT_CENTER)
    {
      size_t dimx, dimy;
      NE_RichTextRenderDryRun(
        ds->texts[i]->channel,
        ds->texts[i]->text,
        &dimx, &dimy
      );
      NE_RichTextRender3D(
        ds->texts[i]->channel,
        ds->texts[i]->text,
        ds->texts[i]->position.x - (dimx>>1),
        ds->texts[i]->position.y - (dimy>>1)       
      );
    }
  
    NE_RichTextPriorityReset();
  }
  ds->text_count = 0;

  //Screen fading
  NE_PolyFormat(31, 
                SPRITE_STACK_SIZE,
                (NE_LightEnum)0,
                NE_CULL_BACK,
                (NE_OtherFormatEnum)0);
 
  for(int i = 0; i < ds->fading_percent; i++)
  {
    NE_2DDrawQuad(
      (i / 12) * 16,
      (i % 12) * 16,
      ((i / 12) + 1)*16,
      min( ((i % 12) + 1) * 16, SCREEN_HEIGHT-1),
      0,
      0x0000
    );
  }
}

void SetFade(u8 percent)
{
  DS.fading_percent = percent;
}

}
