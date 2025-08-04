#include "nqmt.hpp"


class MainMenu: public NQME::Scene
{
public:
  explicit MainMenu(NQME::Sprite2D *pool, NQME::SceneManager *scm) : Scene(pool, scm) {};
  void Start() override;
  void Update() override;
  void Cleanup() override;
};