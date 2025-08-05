#include "main_menu.hpp"

extern NQME::BGHeader title_screen_bg;

void MainMenu::Start()
{
  NE_MainScreenSetOnBottom();

  //NE_RichTextInit(0);
  //NE_RichTextMetadataLoadFAT(0, "fonts/font.fnt");
  //NE_RichTextMaterialLoadGRF(0, "fonts/font_16_png.grf");

  NQME::SetBackgroundSub(title_screen_bg);
  NQME::SetBackgroundPaletteSub((void*)titlePal, titlePalLen);

  background_mat = NE_MaterialCreate();
  background_pal = NE_PaletteCreate();

  NE_MaterialTexLoadFAT(
    background_mat,
    NE_PAL4,
    256, 256,
    (NE_TextureFlags)0,
    "models/menu_background_tex.bin"
  );

  NE_PaletteLoadFAT(
    background_pal,
    "models/menu_background_pal.bin",
    NE_PAL4
  );
  NE_MaterialSetPalette(background_mat, background_pal);

  NE_SpriteSetMaterial(
    background_sprite.sprite,
    background_mat
  );

  background_sprite.dimensions = {256, 236};
  background_sprite.transform.position = {0,0};

  NQME::LoadSong("songs/ouverture.raw");
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
  background_sprite.Draw();
}

void MainMenu::Cleanup()
{
  NE_RichTextEnd(0);
}
