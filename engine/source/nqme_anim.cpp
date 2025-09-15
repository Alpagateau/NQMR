#include "nqme_anim.hpp"

namespace NQME
{

AnimatedSprite::AnimatedSprite(){}

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
  
  sprite->uv_position.x = cur_anim->frames[framepos].x;
  sprite->uv_position.y = cur_anim->frames[framepos].y;
  sprite->uv_dimensions.x = cur_anim->frames[framepos].w;
  sprite->uv_dimensions.y = cur_anim->frames[framepos].h;
  sprite->dimensions.x = cur_anim->frames[framepos].w;
  sprite->dimensions.y = cur_anim->frames[framepos].h;

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
