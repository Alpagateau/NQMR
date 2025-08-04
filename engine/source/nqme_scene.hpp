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
    u8 current = 0;
    u8 next = 0;
    Scene **scenes = nullptr;
    void SwitchTo(u8 next_scene);
    void Start();
    void Update();
  };

}

#endif