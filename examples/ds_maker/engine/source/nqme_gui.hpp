#ifndef NQME_GUI_HPP
#define NQME_GUI_HPP 

#include <nds.h>
#include "nqme_gfx.hpp"
#include "nqme_input.hpp"

#define len(n) sizeof(n)/sizeof(n[0])

namespace NQME 
{
  struct Theme
  {
    NE_Material *material;
    NE_Palette *palette;
    //NitroSprite *cursor;
  };

  struct Rectangle
  {
    Vector2i pos;
    Vector2i size;
  };

  Theme NewTheme(const char *path);
  
  class Selectable 
  {
  public: 
    bool selected_current = false;
    bool selected_before = false;
    Selectable *up = nullptr;
    Selectable *down = nullptr;
    Selectable *left = nullptr;
    Selectable *right = nullptr;
    Theme *theme;

    void UpdateSelected();
  };

  class SimpleButton : public Selectable
  {
  public:
    BasicText label;
    NitroSprite panel;
    Vector2i position;
    Vector2i margin;
    Vector2i txt_offset;
    bool is_visible = true;

    SimpleButton();
    void SetTheme(Theme *t);

    void Update();
    void Draw();
    bool IsClicked();
  private:
    u8 rect_index;
  };

  u8 getBestPanel(Vector2i dim);
}

#endif
