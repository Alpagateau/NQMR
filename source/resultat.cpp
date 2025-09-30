#include "resultat.hpp"

extern NQME::BGHeader title_screen_bg;
extern GameData game_data;
extern SaveData save_data;

extern NQME::Theme my_theme;

void Results::Start()
{
  NE_MainScreenSetOnBottom();
  
  delta = 0;
  NQME::SetBackgroundSub(title_screen_bg);
  NQME::SetBackgroundPaletteSub((void*)new_title_pngPal, new_title_pngPalLen);

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
  
  my_theme = NQME::NewTheme("models/new_button_map_png.grf");

  strcpy(song_name.text, game_data.music_name);
  sprintf(score_text.text, "SCORE : %d", 0);
  sprintf(best_score.text, "BEST  : %d", 
            save_data.scores[game_data.song_idx]
          );

  strcpy(continue_button.label.text, "CONTINUE");
  continue_button.label.channel = 0;
  continue_button.position = {100, 120};
  continue_button.txt_offset = {5, 0};
  continue_button.margin = {10, 0};
  continue_button.is_visible = true;
  continue_button.SetTheme(&my_theme);
  continue_button.selected_current = true;
  continue_button.selected_before = true;

  score_text.position.y = 32;
  best_score.position.y = 64;

  NQME::CloseStream(); 
  NQME::LoadSong("songs/ouverture.wav.raw");
	NQME::PlayStream();
}

void Results::Update()
{
  NQME::UpdateInputs();
  sprintf(score_text.text, "SCORE : %d", dp);
  if(finished)
  {
    sprintf(best_score.text, "BEST  : %d", 
            save_data.scores[game_data.song_idx]
          );
    best_score.Draw();
  }
  if(!finished) dp+=9;
  if(dp >= game_data.pts) 
  {
    finished = true;
    dp = game_data.pts;
  }
  song_name.Draw();
  score_text.Draw();
  continue_button.Draw();
  background_sprite.Draw();

  if(continue_button.IsClicked())
  {
    if(game_data.pts > save_data.scores[game_data.song_idx])
    {
      save_data.scores[game_data.song_idx] = game_data.pts;
      saveSave();
    }

    sm->SwitchTo(0);

  }
}

void Results::Cleanup()
{ 
  //NE_RichTextEnd(0);
  NE_MaterialDelete(my_theme.material);
  NE_PaletteDelete(my_theme.palette);
  NE_MaterialDelete(background_mat);
}
