#ifndef NQME_SCENE_HPP 
#define NQME_SCENE_HPP

#include <nds.h>
#include "nqme_gfx.hpp"

namespace NQME 
{
  class SceneManager;

  class Scene 
  {
  public:
    Sprite2D *sprite_pool;
    SceneManager *sm;
    explicit Scene(NQME::Sprite2D *pool, NQME::SceneManager *scene_manager);
    virtual void Start();
    virtual void Update();
    virtual void Cleanup();
  };

  class SceneManager 
  {
  public:
    SceneManager();
    Scene **scenes;
    u8 current = 0;
    u8 next = 0;
    void SwitchTo(u8 scene_id);
    void Update();
  };
}

#endif