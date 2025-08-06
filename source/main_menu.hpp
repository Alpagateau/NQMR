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

  NE_Material *background_mat;
  NE_Palette *background_pal;

  explicit MainMenu(
    NQME::Sprite2D *pool,
    NQME::NitroSprite *pool2, 
    NQME::SceneManager *scm) : Scene(pool, pool2,scm) {};
  void Start() override;
  void Update() override;
  void Cleanup() override;
};