#ifndef NQME_SCENE_HPP 
#define NQME_SCENE_HPP

#include <nds.h>

namespace NQME 
{

  class Scene 
  {
  public:
    virtual void Start();
    virtual void Update();
    virtual void Cleanup();
  };

}

#endif