#include "nqmt_anim.hpp"

namespace NQMT 
{

AnimatedSprite::AnimatedSprite(NitroSprite *ssprite)
{
  sprite = ssprite;
  framepos = 0;
  framecount = 0;
}

void AnimatedSprite::Play(NSAnimation *anim)
{
  framepos = 0;
  framecount = 0;
  cur_anim = anim;
  nxt_anim = NULL;
}

void AnimatedSprite::Play_then(NSAnimation *anim1, NSAnimation *anim2)
{
  framepos = 0;
  framecount = 0;
  cur_anim = anim1;
  nxt_anim = anim2;
}

void AnimatedSprite::Update()
{
  if(cur_anim == NULL)
    return;
  
  sprite->uv_position = cur_anim->frames[framepos];
  framecount++;
  if(framecount >= cur_anim->fpf)
  {
    framecount = 0;
    framepos++;
    if(framepos >= cur_anim->size)
    {
      framepos = 0;
      if(nxt_anim != NULL)
      {
        cur_anim = nxt_anim;
        nxt_anim = NULL;
      }
    }
  }
  sprite->Draw();
}

}
