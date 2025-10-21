extern "C"{
//#include "gfx/title_png.h"
#include "gfx/new_title_png.h"
#include "gfx/allconsumingvoid_background_png.h"
#include "gfx/arrows_png.h"
#include "gfx/anuvverbubbla_8x8_png.h"
}

#include "nqme_engine.hpp"
#include "nqmt.hpp"
//#include "frames.cpp"
//#include <time.h>


#define EVENT_BUFFER_SIZE 20
#define PROPS_BUFFER_SIZE 4
#define NUM_ARROWS 20

#define SPEED_MULT 1

struct accuracy_level 
{
  int min_dist;
  int pts;
  NQME::NSAnimation *animation;
  char *label;
};

void initTimer();
u32 getTimer();

class Gameplay: public NQME::Scene
{
public:

  // Size of a color in bytes
  const size_t size_color = 2;

  u32 beginning;
  u32 ts;
  // Size of a font character in 4 and 8 BPP modes in bytes
  const size_t size_char_4bpp = (8 * 8) / 2;
  const size_t size_char_8bpp = 8 * 8;

  int X_Positions[5]       = {-32, 29, 88, 167, 225};
  int arrws_offset[5]      = {0, 0, 8, 16, 24};
  Rect arrws_rect[5]        = {
    {0, 0, 32, 32},
    {256 - 32, 256 - 64, 32, 32},  // LEFT
    {256 - 32, 256 - 32, 32, 32}, // UP
    {256 - 64, 256 - 64, 32, 32}, // DOWN
    {256 - 64, 256 - 32, 32, 32}, // RIGHT
  };
  u32 arrws_col[5]     = {
    0xFFFFFF, //Black but invisible so who cares
    0b000001111111111, //Yellow
    0b000001111100000, //Green
    0b000000000011111, //Red
    0b111110000000000, //Blue
  };
	u16 controls[4] = {KEY_LEFT | KEY_Y, KEY_UP | KEY_X , KEY_B | KEY_DOWN, KEY_A | KEY_RIGHT};

  NQME::BasicText score_text;
  NQME::BasicText accuracy_text; //accuracy_text
  int accuracy_cooldown = 0;
  NQME::Sprite2D *arrow_sprites;
  NQME::NitroSprite top_arrows[EVENT_BUFFER_SIZE];
  NQME::NitroSprite props[PROPS_BUFFER_SIZE];
  NQME::NitroSprite player;
  NQME::NitroSprite road1;
  NQME::NitroSprite road2;

  NQME::NitroSprite void1;
  NQME::NitroSprite void2;

  NQME::AnimatedSprite player_animation;
  NQME::AnimatedSprite void1_animation;
  NQME::AnimatedSprite void2_animation;

	NQME::event arrws[EVENT_BUFFER_SIZE];
  
	NQME::NitroSprite target_arrows[4];
   
  NQME::SpriteHeader ArrowHeader;

  NE_Material *player_mat;
	NE_Palette *player_pal;

  NE_Material *void_mat;
  NE_Palette *void_pal;

  NE_Camera *camera;

  NQME::EventHandler eh;

  ConsoleFont font_anuvverbubbla= {
    .gfx = anuvverbubbla_8x8_pngTiles,
    .pal = anuvverbubbla_8x8_pngPal,
    .numColors =  (u16)(anuvverbubbla_8x8_pngPalLen / size_color),
    .bpp = 8,
    .asciiOffset = 32,
    .numChars = (u16)(anuvverbubbla_8x8_pngTilesLen / size_char_8bpp),
  }; 

  PrintConsole sub_console1;

/*
idle0 
idle1 
ollie 
kickflip 
shoveit
flip 
push 
sonic
*/

  Rect arrow_frame = {256 -32, 256 - 32, 32, 32};

  Rect bench   = {204,  98, 23, 56};
  Rect bin     = {231, 100, 24, 40};
  Rect barrier = {228, 152, 25, 36};

  Rect idle0_frames[4] = {
    {31, 69, 24, 60},
    {55, 60, 18, 60},
    {50, 0, 20, 60},
    {70, 0, 18, 60},
  };

  Rect idle1_frames[4] = {
    {207, 0, 36, 48},
    {172, 53, 33, 51},
    {158, 108, 31, 52},
    {174, 0, 33, 51},
  };

  Rect ollie_frames[4] = {
    {148, 0, 26, 53},
    {73, 60, 31, 58},
    {24, 0, 26, 61},
    {105, 113, 26, 55},
  };

  Rect kickflip_frames[4] = {
    {207, 48, 26, 49},
    {0, 77, 31, 62},
    {0, 0, 24, 77},
    {57, 120, 27, 59},
  };

  Rect shoveit_frames[4] = {
    {145, 55, 27, 53},
    {0, 199, 31, 57},
    {31, 199, 56, 56},
    {104, 58, 41, 55},
  };

  Rect push_frames[4] = {
    {0, 139, 37, 60},
    {88, 0, 22, 58},
    {84, 118, 21, 58},
    {37, 129, 20, 60},
  };

  Rect sonic_frames[4] = {
    {131, 113, 27, 53},
    {87, 176, 31, 55},
    {118, 168, 50, 53},
    {110, 0, 38, 55},
  };

   //Define animations
  //Vector2i idle_frames[4] = {{0, 0}, {64, 0}, {128, 0}, {192, 0} };
	NQME::NSAnimation idle = (NQME::NSAnimation)
                            {
                                .size = 4,
                                .fpf = 8,
                                .frames = idle0_frames
                            };

  NQME::NSAnimation idle1 = (NQME::NSAnimation)
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
                              .frames = shoveit_frames
                          };
  NQME::NSAnimation push = (NQME::NSAnimation)
                          {
                              .size = 4,
                              .fpf = 8,
                              .frames = push_frames
                          };
  NQME::NSAnimation sonic = (NQME::NSAnimation)
                          {
                              .size = 4,
                              .fpf = 8,
                              .frames = sonic_frames
                          };

  char accuracy_labels[4][25] = 
  {
    "MYSTIQUE",
    "SUPER",
    "BIEN",
    "PAS MAL"
  };

  accuracy_level lvls[4] = {
    {6, 100, &sonic,  accuracy_labels[0]},
    {17, 50, &kick,   accuracy_labels[1]},
    {40, 25, &shove,  accuracy_labels[2]},
    {80, 10, &ollie, accuracy_labels[3]}
 };


  Rect void_frames[4] = 
  {
    {0,  0, 128, 32},
    {0, 32, 128, 32},
    {0, 64, 128, 32},
    {0, 96, 128, 32},
  };

  NQME::NSAnimation void_anim = (NQME::NSAnimation)
    {
      .size = 4,
      .fpf = 9,
      .frames = void_frames,
    };

  int frame = 0;
  int drive = 0; 
  int num_arrows = 0;
  bool is_fast = false;
  explicit Gameplay(
      NQME::Sprite2D     *pool,
      NQME::SceneManager *scm) : Scene(pool, scm) {};
  void Start() override;
  void Update() override;
  void Cleanup() override;
};
