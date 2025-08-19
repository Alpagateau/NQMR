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

  background_sprite.dimensions = {256, 196};
  background_sprite.transform.position = {0,0};
  //NE_RichTextPrioritySet(2); NE_RichTextPrioritySet(2); 
  NE_RichTextInit(0);
  NE_RichTextMetadataLoadFAT(0, "fonts/graphiti.fnt");
  NE_RichTextMaterialLoadGRF(0, "fonts/graphiti_0_png.grf");
  
  NE_RichTextInit(1);
  NE_RichTextMetadataLoadFAT(1, "fonts/graphiti.fnt");
  NE_RichTextMaterialLoadGRF(1, "fonts/graphiti_0_png.grf");
  
  my_theme = NQME::NewTheme("models/button_map_png.grf");

  start_button.label.text = "START";
  start_button.label.channel = 0;
  start_button.position = {80, 25};
  start_button.txt_offset = {5, 0};
  start_button.margin = {10, 0};
  start_button.SetTheme(&my_theme);
  start_button.selected_current = true;
  
  credits_button.label.text = "CREDITS";
  credits_button.label.channel = 0;
  credits_button.txt_offset = {5, 0};
  credits_button.margin = {10, 0};
  credits_button.position = {80, 60};
  credits_button.SetTheme(&my_theme);
  credits_button.selected_current = false;
  
  start_button.down = &credits_button;
  credits_button.up = &start_button;

  //Load Song List
  
  char *line = NULL;
  size_t len;
  FILE *list_file = fopen("songlist.txt", "r");
  for(int i = 0; i < NUMBER_OF_SONGS; i++)
  {
    getline( &line, &len, list_file );
    printf("[SONG]%s\n", line); 
    available_songs[i] = line;
    for(int j = 0; j < available_songs[i].size();j++)
    {
      if(available_songs[i][j] == '\n') available_songs[i][j] = '_';
    }
    //available_songs.push_back(str.substr(0, str.size() -1));
  }
  fclose(list_file);

  for(int i = 0; i < 5; i++)
  {
    selection[i].label.text = available_songs[i];
    selection[i].position = {10 + max_delta, 35 * i};
    selection[i].is_visible = true;
    selection[i].label.channel = 0;
    selection[i].SetTheme(&my_theme);
    selection[i].txt_offset = {5, 0};
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
      if(selection[i].IsClicked())
      {
        loading_out = true; 
      }
      selection[i].position.x -= delta;
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
    if(fading > 0)
      fading--;
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
      //sm->SwitchTo(1);
    }
    
    start_button.Draw();
    credits_button.Draw(); 
    start_button.UpdateSelected();
    credits_button.UpdateSelected();
    for(int i = 0; i < 5; i++)
    {
      //selection[i].position.x -= delta;
      selection[i].Draw();
      //selection[i].UpdateSelected();
      //selection[i].position.x += delta;
    }
  }
  HiText.text = std::to_string(fading); 
  HiText.Draw();
  
  if(loading_out && fading < 198)
    fading++;

  if(loading_out && fading == 198)
  {
    sm->SwitchTo(1);
  }
  NQME::SetFade(fading);

  background_sprite.Draw();
}

void MainMenu::Cleanup()
{ 
  NE_RichTextEnd(0);
  NE_MaterialDelete(my_theme.material);
  NE_PaletteDelete(my_theme.palette);
  NE_MaterialDelete(background_mat);
}
