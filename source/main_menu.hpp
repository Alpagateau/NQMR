extern "C"{
#include "gfx/title_png.h"
#include "gfx/arrows_png.h"
#include "gfx/tiny_fat_luigi_png.h"
}
#include "nqmt.hpp"

class MainMenu: public NQME::Scene
{
public:
  NQME::NitroSprite background_sprite;
  //NQME::NitroSprite cursor;

  NE_Material *background_mat;
  NE_Palette *background_pal;
  
  NQME::Theme my_theme;
  NQME::BasicText HiText;
  NQME::SimpleButton btn;
  NQME::SimpleButton btn2;

  explicit MainMenu(
    NQME::Sprite2D *pool,
    NQME::SceneManager *scm) : Scene(pool,scm) {};
  void Start() override;
  void Update() override;
  void Cleanup() override;
};
