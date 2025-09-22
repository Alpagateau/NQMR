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
  label.text[0] = 0;
  theme = nullptr;
}

void SimpleButton::SetTheme(Theme *t)
{
  theme = t;
  NE_SpriteSetMaterial(panel.sprite, theme->material);
}

bool SimpleButton::IsClicked()
{
  if(!is_visible)
    return false;

  if (selected_current && selected_before && JustPressed(KEY_A))
    return true;

  Vector2i p = getTouchPosition();
  //printf("Touched : %d, %d\n", p.x, p.y);
  if(p.x < position.x || p.y < position.y)
    return false;

  if(p.x > position.x + panel.dimensions.x || p.y > position.y + panel.dimensions.y)
    return false;

  return true;
}

void SimpleButton::Draw()
{
  if(theme == nullptr)
    return;
  
  if(!is_visible)
    return;

  size_t dimx;
  size_t dimy;
  NE_RichTextRenderDryRun(
    label.channel, 
    label.text,
    &dimx, &dimy);
  dimx += margin.x;
  dimy += margin.y;
  rect_index = getBestPanel({(int)dimx, (int)dimy });

  panel.uv_position = panel_sizes[rect_index].pos;
  panel.uv_dimensions = panel_sizes[rect_index].size;
  panel.dimensions = panel_sizes[rect_index].size;

  Vector2i centering = { 
    (panel.dimensions.x - (int)dimx) >>2 , 
    (panel.dimensions.y - (int)dimy) >> 2
  };
  if(selected_current)
    panel.uv_position.y += 128;
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
    if(panel_sizes[i].size.x >= dim.x && 
      panel_sizes[i].size.y >= dim.y)
    {
      int d = (abs(panel_sizes[i].size.x - dim.x) + 
        abs( panel_sizes[i].size.y - dim.y) );
      if(d < delta)
      {
        delta = d;
        index = i;
      }
    }
  }
  return index;
}

void Selectable::UpdateSelected()
{
  if(selected_current && selected_before)
  {
    if(JustPressed(KEY_UP) && up != nullptr)
    {
      selected_current = false;
      up->selected_current = true;
      //printf("UP SELECTED");
    }
    else if(JustPressed(KEY_DOWN) && down != nullptr)
    {
      selected_current = false;
      down->selected_current = true;
    }
    else if(JustPressed(KEY_LEFT) && left != nullptr)
    {
      selected_current = false;
      left->selected_current = true;
    }
    else if(JustPressed(KEY_RIGHT) && right != nullptr)
    {
      selected_current = false;
      up->selected_current = true;
    }
   
  }
  selected_before = selected_current;
}

}
