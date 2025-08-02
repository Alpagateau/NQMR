#ifndef HPP_NQME_ANIM 
#define HPP_NQME_ANIM 

#include "nqme_math.hpp"
#include "nqme_gfx.hpp"

namespace NQME
{

struct NSAnimation 
{
  
  u8 size;
  u8 fpf; //frames(render) per frame(animation) 
  Vector2i *frames;
};

class AnimatedSprite
{
public:
  AnimatedSprite();
  AnimatedSprite(NitroSprite *ssprite);
  NitroSprite *sprite;
  u8 framecount = 0;
  u8 framepos = 0;
  NSAnimation *cur_anim = NULL;
  NSAnimation *nxt_anim = NULL;
  void Play(NSAnimation *anim);
  void Play_then(NSAnimation *anim1, NSAnimation *anim2);
  void Update();
};

}

#endif
