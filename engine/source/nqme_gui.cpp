#include "nqme_gui.hpp"

namespace NQME 
{

//Button Texture Positions and sizes
//Refer to texture
const Rectangle panel_sizes[] ={
  {{  0,   0}, {  64, 32}},
  {{ 64,   0}, { 160, 32}},
  {{  0,  32}, {  96, 32}},
  {{ 96,  32}, { 144, 32}},
  {{  0,  64}, {  80, 32}},
  {{ 80,  64}, { 112, 32}},
  {{  0,  96}, { 128, 32}}
};

SimpleButton::SimpleButton()
{
  label.text = "";
  theme = nullptr;
}

void SimpleButton::SetTheme(Theme *t)
{
  theme = t;
  NE_SpriteSetMaterial(panel.sprite, theme->material);
}

void SimpleButton::Draw()
{
  if(theme == nullptr)
    return;
  
  size_t dimx;
  size_t dimy;
  NE_RichTextRenderDryRun(label.channel, label.text.c_str(), &dimx, &dimy);
  rect_index = getBestPanel({(int)dimx, (int)dimy});

  panel.uv_position = panel_sizes[rect_index].pos;
  panel.uv_dimensions = panel_sizes[rect_index].size;
  panel.dimensions = panel_sizes[rect_index].size;

  Vector2i centering = { (panel.dimensions.x - dimx)>>2 , (panel.dimensions.y - dimy) >> 2};

  panel.transform.position = position;
  panel.Draw();
  label.position = position + centering + txt_offset;
  label.Draw();
}

void SimpleButton::Update()
{
}

Theme NewTheme(const char *path)
{
  Theme output;
  output.material = NE_MaterialCreate();
  output.palette = NE_PaletteCreate();

  NE_MaterialTexLoadGRF(output.material, output.palette, (NE_TextureFlags)0, path);
  return output;
}

u8 getBestPanel(Vector2i dim)
{
  u8 index = 0;
  int delta = 1000;
  u8 l = len(panel_sizes);
  for(int i = 0; i < l; i++)
  {
    if(panel_sizes[i].size.x >= dim.x && panel_sizes[i].size.y >= dim.y)
    {
      int d = (abs( panel_sizes[i].size.x - dim.x) + abs( panel_sizes[i].size.y - dim.y) );
      if(d < delta)
      {
        delta = d;
        index = i;
      }
    }
  }
  return index;
}
}