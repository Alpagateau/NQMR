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
    printf("Scene System Enabled\n");
  }
  
  void SceneManager::SwitchTo(u8 next_scene)
  {
    next = next_scene;
    printf("Going to scene #%d\n", next);
  }

  void SceneManager::Start()
  {
    next = current;
    scenes[current]->Start();
  }

  void SceneManager::Update()
  {
    scenes[current]->Update();
    if(next == current)
      return;
    scenes[current]->Cleanup();
    scenes[next]->Start();
    current = next;
  }
  
}