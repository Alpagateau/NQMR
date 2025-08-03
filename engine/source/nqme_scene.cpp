#include "nqme_scene.hpp"

namespace NQME
{
  Scene::Scene(NQME::Sprite2D *pool, NQME::SceneManager *scene_manager)
  {
    sprite_pool = pool;
    sm = scene_manager;
  }

  void Scene::Start()
  {
    return;
  }

  void Scene::Update()
  {
    return;
  }

  void Scene::Cleanup()
  {
    return;
  }


  SceneManager::SceneManager()
  {
  }
  
  void SceneManager::SwitchTo(u8 scene_id)
  {
    next = scene_id;
  }

  void SceneManager::Update()
  {
    scenes[current]->Update();
    if(current == next)
      return;
    scenes[current]->Cleanup();
    scenes[next]->Start();
    current = next;
  }
}