extern "C"{
#include "gfx/title_png.h"
#include "gfx/arrows_png.h"
}
#include <fstream>
#include "nqmt.hpp"

#define MAIN_SCREEN 0
#define LVL_SELECT  1
#define TO_MAIN     2 
#define TO_SELECT   3

#define NUMBER_OF_SONGS 5

class MainMenu: public NQME::Scene
{
public:
  NQME::NitroSprite background_sprite; 
  std::string available_songs[NUMBER_OF_SONGS];
  u8 state = MAIN_SCREEN;
  
  int delta     = 0;
  int max_delta = 255; 
  bool loading_out = false;
   
  NE_Material *background_mat;
  NE_Palette *background_pal;
  
  NQME::Theme my_theme;
  NQME::BasicText score_texts[5];
  NQME::SimpleButton start_button;
  NQME::SimpleButton credits_button;

  NQME::SimpleButton selection[5];

  explicit MainMenu(
    NQME::Sprite2D *pool,
    NQME::SceneManager *scm) : Scene(pool,scm) {};
  void Start() override;
  void Update() override;
  void Cleanup() override;
};
