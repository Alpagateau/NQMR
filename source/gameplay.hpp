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

/*
  idle0 : 4, 5, 6, 7
  ollie : 8, 9, 10, 11
  kickflip : 12, 13, 14, 15
  flip : 12, 16, 17, 18, 19 
  shoveit : 19, 20, 21, 22
  push : 24, 25, 26, 27
  sonic : 28, 29, 30, 31
  idle1 : 1, 3, 2, 0
*/

  Rect idle1_frames[4] = {
    {145, 0, 31, 49}, //1
    {0, 49, 30, 50},  //0
    {176, 0, 31, 49}, //2
    {48, 0, 34, 45}   //3
  };

  Rect ollie_frames[4] = {
    {203, 102, 17, 58}, //4
    {185, 102, 18, 57}, //5
    {220, 102, 17, 58}, //6
    {0, 160, 22, 58}    //7
  };

  Rect kickflip_frames[4] = {
    {30, 49, 25, 51},   //8
    {93, 102, 29, 55},  //9
    {100, 160, 25, 59}, //10
    {122, 102, 24, 55}  //11
  };

  Rect flip_frames[4] = {
    {120, 0, 25, 47},  //12
    {125, 160, 29, 60},//13
    {154, 160, 22, 75},//14
    {22, 160, 25, 58}  //15
  };

  Rect anim4[4] = {
    {82, 0, 38, 46},  //16
    {55, 49, 28, 51}, //17
    {207, 0, 29, 49}, //18
    {83, 49, 27, 51}  //19
  };

  Rect anim5[4] = {
    {0, 102, 39, 54}, //20
    {39, 102, 54, 54},//21
    {162, 49, 29, 53},//22
    {110, 49, 26, 51} //23
  };

  Rect anim6[4] = {
    {47, 160, 35, 58}, //24
    {146, 102, 20, 56},//25
    {166, 102, 19, 56},//26
    {82, 160, 18, 58}  //27
  };

  Rect anim7[4] = {
    {136, 49, 26, 51}, //28
    {191, 49, 29, 53}, //29
    {0, 0, 48, 36},    //30
    {220, 49, 36, 53}  //31
  };



   //Define animations
  //Vector2i idle_frames[4] = {{0, 0}, {64, 0}, {128, 0}, {192, 0} };
	NQME::NSAnimation idle = (NQME::NSAnimation)
                            {
                                .size = 4,
                                .fpf = 8,
                                .frames = idle1_frames
                            };
    
  //Vector2i ollie_frames[4] = {{0, 128}, {64, 128}, {128, 128}, {192, 128} };
	NQME::NSAnimation ollie = (NQME::NSAnimation)
                            {
                                .size = 4,
                                .fpf = 8,
                                .frames = ollie_frames
                            }; 
    
  //Vector2i kick_frames[4] = {{256, 0}, {256 + 64, 0}, {256 + 128, 0}, {256 + 192, 0} };
	NQME::NSAnimation kick = (NQME::NSAnimation)
                            {
                                .size = 4,
                                .fpf = 8,
                                .frames = kickflip_frames
                            };
    
  //Vector2i shove_frames[4] = {{256, 128}, {256 + 64, 128}, {256 + 128, 128}, {256 + 192, 128} };
  NQME::NSAnimation shove = (NQME::NSAnimation)
                        	{
                            	.size = 4,
                              .fpf = 8,
                              .frames = flip_frames
                          };
  int frame = 0;

  explicit Gameplay(
      NQME::Sprite2D     *pool,
      NQME::SceneManager *scm) : Scene(pool, scm) {};
  void Start() override;
  void Update() override;
  void Cleanup() override;
};
