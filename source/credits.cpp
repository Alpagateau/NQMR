#include "credits.hpp"
#include "nds/system.h"
#include "nqme_gfx.hpp"

extern NQME::BGHeader title_screen_bg;
extern GameData game_data;
extern SaveData save_data;

extern NQME::Theme my_theme;

void Credits::Start()
{
  section = -1;
  timing = 0;
  NE_MainScreenSetOnBottom();

  NQME::SetBackgroundSub(title_screen_bg);
  NQME::SetBackgroundPaletteSub(
    (void*)new_title_pngPal, 
    new_title_pngPalLen);

  background_mat = NE_MaterialCreate();
  background_pal = NE_PaletteCreate();

  NE_MaterialTexLoadGRF(
    background_mat,
    background_pal,
    (NE_TextureFlags)0,
    "models/new_menu_background_png.grf"
  );
  
  NE_SpriteSetMaterial(
    background_sprite.sprite,
    background_mat
  );

  background_sprite.dimensions = {256, 196};
  background_sprite.transform.position = {0,0};
  
  my_theme = NQME::NewTheme(
    "models/new_button_map2_png.grf"
  );
  
  subject.position.y = (SCREEN_HEIGHT / 2) - 12;
  subject.position.x = SCREEN_WIDTH / 2;
  subject.centering = TEXT_CENTER;
  person.position.y = (SCREEN_HEIGHT / 2) + 12;
  person.position.x = SCREEN_WIDTH / 2;
  person.centering = TEXT_CENTER;
  
  NQME::CloseStream(); 
  NQME::LoadSong("songs/mode_d_emploi.wav.raw");
	NQME::PlayStream();
}

void Credits::Update()
{
  NQME::UpdateInputs();
  
  timing--;
  if(timing <= 0) 
  { 
    section++;
    strcpy(
      subject.text, 
      sections[section].subject
    );
    strcpy(
      person.text, 
      sections[section].person
    );
    timing = 200;
  } 

  if (section > 11) {
    sm->SwitchTo(0);
  }
  
  subject.Draw();
  if(timing < 120)
    person.Draw();
  background_sprite.Draw();  
}

void Credits::Cleanup()
{ 
  if(game_data.pts > save_data.scores[game_data.song_idx])
  {
    save_data.scores[game_data.song_idx] = game_data.pts;
    saveSave();
  }
  
  NE_MaterialDelete(my_theme.material);
  NE_PaletteDelete(my_theme.palette);
  NE_MaterialDelete(background_mat);
}
