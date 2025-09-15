extern "C"{
#include "gfx/title_png.h"
#include "gfx/arrows_png.h"
#include "gfx/tiny_fat_luigi_png.h"
}

#include "nqme_engine.hpp"
#include "nqmt.hpp"
//#include "frames.cpp"

#define EVENT_BUFFER_SIZE 10
#define NUM_ARROWS 32


class Gameplay: public NQME::Scene
{
public:
  int X_Positions[5]       = {-32, 32, 94, 168, 232};
	int arrws_offsets[5]     = {  0,  0,  8,  16,  24};
	u16 controls[4] = {KEY_LEFT, KEY_UP, KEY_X, KEY_A};


  NQME::BasicText score_text;
  NQME::Sprite2D *arrow_sprites;
  NQME::NitroSprite top_arrows[EVENT_BUFFER_SIZE];
  NQME::NitroSprite player;
  NQME::AnimatedSprite player_animation;

	NQME::event arrws[EVENT_BUFFER_SIZE];
  
	NQME::NitroSprite target_arrows[4];
   
  NQME::SpriteHeader ArrowHeader;

  NE_Material *arrows_mat;
	NE_Palette *arrows_pal;
  NE_Material *player_mat;
	NE_Palette *player_pal;

  NE_Camera *camera;

  NQME::EventHandler eh;

  Rect anim0[4] = {
    {0, 49, 30, 50},
    {145, 0, 31, 49},
    {176, 0, 31, 49},
    {48, 0, 34, 45}
  };

  Rect anim1[4] = {
    {203, 102, 17, 58},
    {185, 102, 18, 57},
    {220, 102, 17, 58},
    {0, 160, 22, 58}
  };

  Rect anim2[4] = {
    {30, 49, 25, 51},
    {93, 102, 29, 55},
    {100, 160, 25, 59},
    {122, 102, 24, 55}
  };

  Rect anim3[4] = {
    {120, 0, 25, 47},
    {125, 160, 29, 60},
    {154, 160, 22, 75},
    {22, 160, 25, 58}
  };

  Rect anim4[4] = {
    {82, 0, 38, 46},
    {55, 49, 28, 51},
    {207, 0, 29, 49},
    {83, 49, 27, 51}
  };

  Rect anim5[4] = {
    {0, 102, 39, 54},
    {39, 102, 54, 54},
    {162, 49, 29, 53},
    {110, 49, 26, 51}
  };

  Rect anim6[4] = {
    {47, 160, 35, 58},
    {146, 102, 20, 56},
    {166, 102, 19, 56},
    {82, 160, 18, 58}
  };

  Rect anim7[4] = {
    {136, 49, 26, 51},
    {191, 49, 29, 53},
    {0, 0, 48, 36},
    {220, 49, 36, 53}
  };



   //Define animations
  //Vector2i idle_frames[4] = {{0, 0}, {64, 0}, {128, 0}, {192, 0} };
	NQME::NSAnimation idle = (NQME::NSAnimation)
                            {
                                .size = 4,
                                .fpf = 8,
                                .frames = anim0
                            };
    
  //Vector2i ollie_frames[4] = {{0, 128}, {64, 128}, {128, 128}, {192, 128} };
	NQME::NSAnimation ollie = (NQME::NSAnimation)
                            {
                                .size = 4,
                                .fpf = 8,
                                .frames = anim1
                            }; 
    
  //Vector2i kick_frames[4] = {{256, 0}, {256 + 64, 0}, {256 + 128, 0}, {256 + 192, 0} };
	NQME::NSAnimation kick = (NQME::NSAnimation)
                            {
                                .size = 4,
                                .fpf = 8,
                                .frames = anim2
                            };
    
  //Vector2i shove_frames[4] = {{256, 128}, {256 + 64, 128}, {256 + 128, 128}, {256 + 192, 128} };
  NQME::NSAnimation shove = (NQME::NSAnimation)
                        	{
                            	.size = 4,
                              .fpf = 8,
                              .frames = anim3
                          };
  int frame = 0;

  explicit Gameplay(
      NQME::Sprite2D     *pool,
      NQME::SceneManager *scm) : Scene(pool, scm) {};
  void Start() override;
  void Update() override;
  void Cleanup() override;
};
