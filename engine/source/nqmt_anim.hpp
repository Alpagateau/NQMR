#ifndef HPP_NQMT_ANIM 
#define HPP_NQMT_ANIM 

#include "nqmt_math.hpp"
#include "nqmt_gfx.hpp"

namespace NQMT
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
