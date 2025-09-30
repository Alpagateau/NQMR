extern "C"{
#include "gfx/new_title_png.h"
#include "gfx/arrows_png.h"
}
#include "nqmt.hpp"

class Results: public NQME::Scene
{
public:
  NQME::NitroSprite background_sprite; 
 
  int delta     = 0;
  int max_delta = 255;  
  bool finished = false; 
  int dp = 0;
  
  NE_Material *background_mat;
  NE_Palette *background_pal;
  
  NQME::BasicText song_name;
  NQME::BasicText score_text;
  NQME::BasicText best_score;
  NQME::SimpleButton continue_button;


  explicit Results(
    NQME::Sprite2D *pool,
    NQME::SceneManager *scm) : Scene(pool,scm) {};
  void Start() override;
  void Update() override;
  void Cleanup() override;
};
