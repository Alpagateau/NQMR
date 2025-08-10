#include "main_menu.hpp"

extern NQME::BGHeader title_screen_bg;

void MainMenu::Start()
{
  NE_MainScreenSetOnBottom();

  NQME::SetBackgroundSub(title_screen_bg);
  NQME::SetBackgroundPaletteSub((void*)title_pngPal, title_pngPalLen);

  background_mat = NE_MaterialCreate();
  background_pal = NE_PaletteCreate();

  NE_MaterialTexLoadGRF(background_mat, background_pal, (NE_TextureFlags)0, "models/menu_background_png.grf");
  
  NE_SpriteSetMaterial(
    background_sprite.sprite,
    background_mat
  );

  background_sprite.dimensions = {256, 236};
  background_sprite.transform.position = {0,0};

  NE_RichTextInit(0);
  NE_RichTextMetadataLoadFAT(0, "fonts/graphiti.fnt");
  NE_RichTextMaterialLoadGRF(0, "fonts/graphiti_0_png.grf");

  NQME::Theme my_theme = NQME::NewTheme("models/button_map_png.grf");

  btn.label.text = "This is a Button";
  btn.label.channel = 0;
  btn.position = {100, 50};
  btn.txt_offset = {5, 0};
  btn.SetTheme(&my_theme);

  HiText.text = "Hello Pierre";
  HiText.position = {0, 0};

  NQME::LoadSong("songs/ouverture.wav.raw");
	NQME::PlayStream();
}

void MainMenu::Update()
{
  NQME::UpdateInputs();
  if(NQME::Pressed(KEY_A))
  {
    printf("A pressed\n");
    sm->SwitchTo(1);
  }
  if(NQME::JustPressed(KEY_B))
  {
    printf("B pressed\n");
  }
  background_sprite.Draw();
  HiText.Draw();
  btn.Draw();
}

void MainMenu::Cleanup()
{
  NE_RichTextEnd(0);
}
