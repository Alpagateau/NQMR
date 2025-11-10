#ifndef SCORE_MENU_HPP 
#define SCORE_MENU_HPP 

#include "nqme_engine.hpp"
#include "nqmt.hpp"

class ScoreMenu : public NQME::Scene 
{
public:
  explicit ScoreMenu(
    NQME::Sprite2D *pool,
    NQME::SceneManager *scm) : Scene(pool,scm) {};
  void Start() override;
  void Update() override;
  void Cleanup() override;
};

#endif
