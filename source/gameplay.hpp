extern "C"{
#include "gfx/title_png.h"
#include "gfx/arrows_png.h"
#include "gfx/tiny_fat_luigi_png.h"
}

#include "nqme_engine.hpp"
#include "nqmt.hpp"

#define EVENT_BUFFER_SIZE 10
#define NUM_ARROWS 32


class Gameplay: public NQME::Scene
{
public:
   int X_Positions[5]       = {-32, 32, 94, 168, 232};
	int arrws_offsets[5]     = {  0,  0,  8,  16,  24};
	u16 controls[4] = {KEY_LEFT, KEY_UP, KEY_X, KEY_A};

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
  u8 fading = 198;

  NQME::EventHandler eh;

   //Define animations
  Vector2i idle_frames[4] = {{0, 0}, {64, 0}, {128, 0}, {192, 0} };
	NQME::NSAnimation idle = (NQME::NSAnimation)
                            {
                                .size = 4,
                                .fpf = 8,
                                .frames = idle_frames 
                            };
    
  Vector2i ollie_frames[4] = {{0, 128}, {64, 128}, {128, 128}, {192, 128} };
	NQME::NSAnimation ollie = (NQME::NSAnimation)
                            {
                                .size = 4,
                                .fpf = 8,
                                .frames = ollie_frames 
                            }; 
    
  Vector2i kick_frames[4] = {{256, 0}, {256 + 64, 0}, {256 + 128, 0}, {256 + 192, 0} };
	NQME::NSAnimation kick = (NQME::NSAnimation)
                            {
                                .size = 4,
                                .fpf = 8,
                                .frames = kick_frames 
                            };
    
  Vector2i shove_frames[4] = {{256, 128}, {256 + 64, 128}, {256 + 128, 128}, {256 + 192, 128} };
  NQME::NSAnimation shove = (NQME::NSAnimation)
                        	{
                            	.size = 4,
                              .fpf = 8,
                              .frames = shove_frames 
                          };
  int frame = 0;

  explicit Gameplay(
      NQME::Sprite2D     *pool,
      NQME::SceneManager *scm) : Scene(pool, scm) {};
  void Start() override;
  void Update() override;
  void Cleanup() override;
};
