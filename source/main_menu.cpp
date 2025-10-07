#include "main_menu.hpp"

extern NQME::BGHeader title_screen_bg;
extern GameData game_data;
extern SaveData save_data;

NQME::Theme my_theme;

void MainMenu::Start()
{
  NE_MainScreenSetOnBottom();
  state = MAIN_SCREEN;
  delta = 0;
  loading_out = false;
  background_mat = NE_MaterialCreate();
  background_pal = NE_PaletteCreate();

  background_sprite.dimensions = {256, 196};
  background_sprite.transform.position = {0,0};
  
  NE_SpriteSetMaterial(
    background_sprite.sprite,
    background_mat
  );

  if(!splash_screen)
  {
    NE_MaterialTexLoadGRF(
      background_mat,
      background_pal,
      (NE_TextureFlags)0,
      "models/splash_screen1_png.grf"
    );
    for(int i = 0; i < 80; i++)
    {
      background_sprite.Draw();
      NQME::UpdateGraphics();
	    NE_WaitForVBL((NE_UpdateFlags)0);
    }
    splash_screen = true;    
    NE_MaterialDelete(background_mat);
    NE_PaletteDelete(background_pal);
  }

  background_mat = NE_MaterialCreate();
  background_pal = NE_PaletteCreate();
  NE_SpriteSetMaterial(
    background_sprite.sprite,
    background_mat
  );



  NQME::SetBackgroundSub(title_screen_bg);
  NQME::SetBackgroundPaletteSub((void*)new_title_pngPal, new_title_pngPalLen);

  //Loading textures
  NE_MaterialTexLoadGRF(
    background_mat,
    background_pal,
    (NE_TextureFlags)0,
    "models/new_menu_background_png.grf"
    //"models/256_tiny_png.grf"
  ); 

  NE_RichTextInit(0);
  NE_RichTextMetadataLoadFAT(0, "fonts/graphiti.fnt");
  NE_RichTextMaterialLoadGRF(0, "fonts/graphiti_0_png.grf");

  NE_RichTextPriorityReset(); 
  my_theme = NQME::NewTheme("models/new_button_map2_png.grf");

  //start_button.label.text = "START";
  strcpy(start_button.label.text, "START");
  start_button.label.channel = 0;
  start_button.position = {80, 25};
  start_button.txt_offset = {5, -2};
  start_button.margin = {10, 0};
  start_button.is_visible = true;
  start_button.SetTheme(&my_theme);
  start_button.selected_current = true;
  
  //credits_button.label.text = "CREDITS";
  strcpy(credits_button.label.text, "CREDITS");
  credits_button.label.channel = 0;
  credits_button.txt_offset = {5, -2};
  credits_button.margin = {10, 0};
  credits_button.position = {80, 60};
  credits_button.is_visible = true;
  credits_button.SetTheme(&my_theme);
  credits_button.selected_current = false;
  
  start_button.down = &credits_button;
  credits_button.up = &start_button;

  //Load Song List
  
  std::string line;
  std::ifstream list_file("songlist.txt");
  for(int i = 0; i < NUMBER_OF_SONGS; i++)
  {
    std::getline(list_file, line);
    printf("[SONG]%s\n", line.c_str()); 
    available_songs[i] = line; 
  }
  list_file.close();

  for(int i = 0; i < 5; i++)
  {
    strcpy(selection[i].label.text, available_songs[i].c_str());
    selection[i].position = {10 + max_delta, 35 * i};
    selection[i].is_visible = true;
    selection[i].label.channel = 0;
    selection[i].SetTheme(&my_theme);
    selection[i].txt_offset = {5, -2};
    selection[i].margin = {10, 0};
    selection[i].up   = (i == 0) ? &selection[4] : &selection[i-1];
    selection[i].down = (i == 4) ? &selection[0] : &selection[i+1];
  }

  NQME::LoadSong("songs/ouverture.wav.raw");
	NQME::PlayStream();
}

void MainMenu::Update()
{
  if(state == TO_MAIN)
  {

    if(delta > 0){
      delta-=20;
      start_button.position.x -= delta;
      credits_button.position.x -= delta;
      
      for(int i = 0; i  < 5; i++)
      {
        selection[i].position.x -= delta;
        selection[i].Draw(); 
        selection[i].position.x += delta;
      }

      start_button.Draw();
      credits_button.Draw();
       
      start_button.position.x += delta;
      credits_button.position.x += delta;
    }else {
      state = MAIN_SCREEN; 
    }
  }
  if(state == LVL_SELECT)
  {
    NQME::UpdateInputs();
    for(int i = 0; i < 5; i++)
    {
      selection[i].position.x -= delta;
      if(selection[i].IsClicked())
      {
        loading_out = true;
        //game_data.music_name = selection[i].label.text;
        game_data.song_idx = i;
        strcpy(game_data.music_name, selection[i].label.text);
        sm->SwitchTo(1);
      }
      
      selection[i].Draw();
      selection[i].UpdateSelected();
      selection[i].position.x += delta;
    }
    if(NQME::JustPressed(KEY_B) && !loading_out)
    {
      state = TO_MAIN; 
      for(int i = 0; i < 5; i++){
        selection[i].selected_current = false; 
      }
      start_button.selected_current = true;
      start_button.is_visible = true;
      credits_button.is_visible = true;

    }
  }
  if(state == TO_SELECT)
  {
    if(delta < max_delta){
      delta+=20;
      start_button.position.x -= delta;
      credits_button.position.x -= delta;
      
      for(int i = 0; i  < 5; i++)
      {
        selection[i].position.x -= delta;
        selection[i].Draw(); 
        selection[i].position.x += delta;
      }

      start_button.Draw();
      credits_button.Draw();
       
      start_button.position.x += delta;
      credits_button.position.x += delta;
    }else {
      state = LVL_SELECT;
      start_button.is_visible = false;
      credits_button.is_visible = false;
      for(int i = 0; i  < 5; i++)
      {
        selection[i].is_visible = true;
      }
    }
  }
  if(state == MAIN_SCREEN){
    NQME::UpdateInputs();
    if(start_button.IsClicked())
    {
      state = TO_SELECT;
      start_button.selected_current = false;
      credits_button.selected_current = false;
      selection[0].selected_current = true;
      for(int i = 0; i < 5; i++)
      {
        selection[i].is_visible = true;
      }
    }
    
    start_button.Draw();
    credits_button.Draw(); 
    start_button.UpdateSelected();
    credits_button.UpdateSelected();
    for(int i = 0; i < 5; i++)
    {
      selection[i].Draw();
    }
  }
  background_sprite.Draw();
}

void MainMenu::Cleanup()
{ 
  NE_MaterialDelete(my_theme.material);
  NE_PaletteDelete(my_theme.palette);
  NE_MaterialDelete(background_mat);
}
