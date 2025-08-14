#include "main_menu.hpp"

extern NQME::BGHeader title_screen_bg;

void MainMenu::Start()
{
  NE_MainScreenSetOnBottom();

  NQME::SetBackgroundSub(title_screen_bg);
  NQME::SetBackgroundPaletteSub((void*)title_pngPal, title_pngPalLen);

  background_mat = NE_MaterialCreate();
  background_pal = NE_PaletteCreate();

  NE_MaterialTexLoadGRF(
    background_mat, 
    background_pal, 
    (NE_TextureFlags)0, 
    //"models/spritesheet_small_png.grf"
    "models/menu_background_png.grf"
  );
  
  NE_SpriteSetMaterial(
    background_sprite.sprite,
    background_mat
  );

  background_sprite.dimensions = {256, 236};
  background_sprite.transform.position = {0,0};

  NE_RichTextInit(0);
  NE_RichTextMetadataLoadFAT(0, "fonts/graphiti.fnt");
  NE_RichTextMaterialLoadGRF(0, "fonts/graphiti_0_png.grf");
   
  my_theme = NQME::NewTheme("models/button_map_png.grf");

  btn.label.text = "START";
  btn.label.channel = 0;
  btn.position = {80, 25};
  btn.txt_offset = {5, 0};
  btn.margin = {10, 0};
  btn.SetTheme(&my_theme);
  btn.selected_current = true;
  
  btn2.label.text = "CREDITS";
  btn2.label.channel = 0;
  btn2.txt_offset = {5, 0};
  btn2.margin = {10, 0};
  btn2.position = {80, 60};
  btn2.SetTheme(&my_theme);
  btn2.selected_current = false;
  
  btn.down = &btn2;
  btn2.up = &btn;
  //HiText.text = "Hello Pierre";
  //HiText.position = {0, 0};

  NQME::LoadSong("songs/ouverture.wav.raw");
	NQME::PlayStream();
}

void MainMenu::Update()
{
  NQME::UpdateInputs();
  if(btn.IsClicked())
  {
    sm->SwitchTo(1);
  }
  background_sprite.Draw();
  //HiText.Draw();
  btn.Draw();
  btn2.Draw();

  btn.UpdateSelected();
  btn2.UpdateSelected();
}

void MainMenu::Cleanup()
{ 
  NE_RichTextEnd(0);
  NE_MaterialDelete(my_theme.material);
  NE_PaletteDelete(my_theme.palette);
  NE_MaterialDelete(background_mat);
}
