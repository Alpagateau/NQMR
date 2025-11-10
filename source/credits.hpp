extern "C"{
#include "gfx/new_title_png.h"
#include "gfx/arrows_png.h"
}
#include "nqmt.hpp"

struct section_t
{
  char subject[50];
  char person[50];
};

class Credits: public NQME::Scene
{
public:
  NQME::NitroSprite background_sprite; 
 
  NE_Material *background_mat;
  NE_Palette *background_pal;
  
  NQME::BasicText subject;
  NQME::BasicText person;

  int section = 0;
  int timing = 0;

  section_t sections[12] = 
  {
    (section_t){"Sprites et menus", "DeLatrech Inc."},
    (section_t){"Sprites et Personnage", "Martin Nadaud"},
    (section_t){"Gameplay", "Martin Nadaud"},
    (section_t){"Arrangements", "Martin Nadaud"},
    (section_t){"Librairies NDS", "AntonioND"},
    (section_t){"Music des menus", "Justin Hurwitz"},
    (section_t){"Sous Un Ciel", "Kaci et Score"},
    (section_t){"Tous Les Gens", "Spider ZED et Leo Roi"},
    (section_t){"Tout le monde autour", "Jaymee"},
    (section_t){"Trouble du comportement", "Juifboy"},
    (section_t){"Musique des credits", "Jade"},
    (section_t){"Merci d'avoir jouer", "Bon anniversaire"}
  };

  explicit Credits(
    NQME::Sprite2D *pool,
    NQME::SceneManager *scm) : Scene(pool,scm) {};
  void Start() override;
  void Update() override;
  void Cleanup() override;
};
