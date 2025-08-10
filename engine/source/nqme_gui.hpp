#ifndef NQME_GUI_HPP
#define NQME_GUI_HPP 

#include <nds.h>
#include "nqme_gfx.hpp"

#define len(n) sizeof(n)/sizeof(n[0])

namespace NQME 
{

  struct Theme
  {
    NE_Material *material;
    NE_Palette *palette;
  };

  struct Rectangle
  {
    Vector2i pos;
    Vector2i size;
  };

  Theme NewTheme(const char *path);

  class SimpleButton 
  {
  public:
    BasicText label;
    NitroSprite panel;
    Vector2i position;
    Vector2i margin;
    Vector2i txt_offset;
    Theme *theme;

    bool is_selected = false;
    bool was_selected = false;
    bool is_clicked = false;

    SimpleButton();
    void SetTheme(Theme *t);

    void Update();
    void Draw();
  private:
    u8 rect_index;
  };

  u8 getBestPanel(Vector2i dim);
}

#endif