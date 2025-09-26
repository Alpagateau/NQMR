#include "gameplay.hpp"

extern NQME::BGHeader title_screen_bg;
extern GameData game_data;

void Gameplay::Start()
{

  frame = 0;
  printf("Gameplay settup \n");
  NQME::StopStream();
  printf("Setting up background\n");
  NQME::SetBackgroundSub(title_screen_bg);
	NQME::SetBackgroundPaletteSub((void*)title_pngPal, title_pngPalLen);
  NQME::SetSpritePalette((void*)arrows_pngPal, arrows_pngPalLen);
  
  printf("Loading ArrowHeader\n");
  ArrowHeader.Load((void*)arrows_pngTiles, arrows_pngTilesLen, SQ32_256);

  printf("Creating materials\n");
  player_mat = NE_MaterialCreate();
	player_pal = NE_PaletteCreate();
  printf("Loading global spritesheet\n");
  int small = NE_MaterialTexLoadGRF(player_mat, 
                        player_pal, 
                        NE_TEXTURE_COLOR0_TRANSPARENT,
                        //(NE_TextureFlags)0,
                        "models/squeezed_256_png.grf");
  if(small != 1 )
  {
    printf("Couldnt load small txt, error code : %d\n", small);
    int big = NE_MaterialTexLoadGRF(player_mat,
                                    player_pal, 
                                    (NE_TextureFlags)0, 
                                    "models/spritesheet_small_png.grf");
    if(big != 1) printf("Neither the big one...\n");
    printf("===============\n");
  }


  printf("Loading arrow material\n");
  
  NE_SpriteSetMaterial(player.sprite, player_mat);
  NE_SpriteSetMaterial(road1.sprite, player_mat);
  NE_SpriteSetMaterial(road2.sprite, player_mat);
  printf("Creating camera\n");
  camera = NE_CameraCreate();

  NE_CameraSet(camera,
            -8, 0, 0,  // Position
             0, 0, 0,  // Look at
             0, 1, 0); // Up direction
  NQME::UseCamera(camera);
  NE_LightSet(0, NE_White, -0.5, -0.5, -0.5);
  
  printf("Seeting some pointers\n");
  arrow_sprites = sprite_pool;
  printf("Setting up the props");
  for(int i = 0; i < PROPS_BUFFER_SIZE; i++)
  {
    NE_SpriteSetMaterial(props[i].sprite, player_mat);
    props[i].centering = SPT_DM;
    props[i].transform.position.y = 0;
    props[i].transform.position.x = 20;
    props[i].index = 1;
  }
  printf("Setting target arrows \n");
	for(int i = 0; i < 4; i++)
	{
    target_arrows[i].uv_position.x = arrws_rect[i+1].x;
		target_arrows[i].uv_position.y = arrws_rect[i+1].y;
		target_arrows[i].anchor = (Vector2i){16, 16};
		target_arrows[i].transform.position = {X_Positions[i+1],  16};
		target_arrows[i].dimensions = {32, 32};
		NE_SpriteSetMaterial(target_arrows[i].sprite, player_mat);
  }
  NE_RichTextInit(1);
  NE_RichTextMetadataLoadFAT(1, "fonts/graphiti.fnt");
  NE_RichTextMaterialLoadGRF(1, "fonts/graphiti_0_png.grf"); 
	
  printf("Setting up movement arrows\n");

  for(int i = 0; i < EVENT_BUFFER_SIZE; i++)
 	{
   	arrws[i] = (NQME::event){0};
		arrow_sprites[i].SetHeader(ArrowHeader);
		arrow_sprites[i].anchor = (Vector2i){16, 16};
    arrow_sprites[i].visible = true;
		arrow_sprites[i].Update();

    top_arrows[i].dimensions = {32, 32};
    top_arrows[i].anchor.x = 16;
    top_arrows[i].anchor.y =  16;
    top_arrows[i].uv_position.x = arrow_frame.x;
    top_arrows[i].uv_position.y = arrow_frame.y;
    NE_SpriteSetMaterial(top_arrows[i].sprite, player_mat);
  }

  printf("Setting up Player Chatacter\n");
  player.dimensions = {64, 128};
  player.centering = SPT_DM;
  player.transform.position = (Vector2i){127, 160};
  player_animation.sprite = &player;
  player_animation.Play(&idle);

  printf("Setting up river\n");
  road1.dimensions = {256, 256};
  road2.dimensions = {256, 256};
  road1.uv_position = {0, 256};
  road2.uv_position = {0, 256};
  road1.uv_dimensions = {256, 256};
  road2.uv_dimensions = {256, 256};

  player.index = 1;
  road1.index = 3;
  road2.index = 3;
  NE_MainScreenSetOnTop(); 

  char song_path[100] = "songs/";
  strcat(song_path, game_data.music_name);
  strcat(song_path, ".wav.raw");
  //song_path = ("songs/" + game_data.music_name + ".wav.raw");
  printf("(%s)", song_path);
  NQME::LoadSong(song_path);
	NQME::PlayStream();

  song_path[0] = 0;
  strcat(song_path, "bms/");
  strcat(song_path, game_data.music_name);
  strcat(song_path, ".bbm");
  eh.Init(
    song_path,
    EVENT_BUFFER_SIZE, 
    arrws);
  eh.grace = 64;	

  game_data.pts = 0;
  score_text.channel = 0;
  accuracy_text.channel = 0;
  strcpy(accuracy_text.text, "");
  strcpy(score_text.text, "0"); 
  score_text.position = {127, 170};
  score_text.centering = TEXT_CENTER;
  accuracy_text.position = {100, 100};
  accuracy_text.centering = TEXT_CENTER;
}

void Gameplay::Update()
{
  frame++;
  NQME::UpdateInputs(); 
  road1.Draw();
  road2.Draw();
  road1.transform.position.y += 1;
  road2.transform.position.y += 1;

  for(int i = 0; i < PROPS_BUFFER_SIZE; i++)
  {
    //printf("[Prop Pos] %d\n", props[i].transform.position.y);
    if(props[i].transform.position.y == 0)
    {
      if(randInt()%101 == 0)
      {
        props[i].transform.position.y++;
        Rect *r = NULL;
        int ra = randInt();
        if(ra%3 == 0) r = &bench;
        if(ra%3 == 1) r = &bin;
        if(ra%3 == 2) r = &barrier; 
        
        props[i].uv_position.x = r->x;
        props[i].uv_position.y = r->y;
        props[i].uv_dimensions.x = r->w;
        props[i].uv_dimensions.y = r->h;
        props[i].dimensions.x = r->w;
        props[i].dimensions.y = r->h;

        if(randInt()%2 == 0)
        {
          props[i].transform.position.x = 236;
          props[i].dimensions.x *= -1;
        }
        else
        {
          props[i].transform.position.x = 20;
        }
      }
    }
    else
    {
      props[i].transform.position.y++;
      //props[i].index++;
      if(props[i].transform.position.y >= 256) 
      {
        props[i].transform.position.y = 0;
        props[i].index = 2;
      }
    }  
    props[i].Draw();
  }
  
  if(road1.transform.position.y >= 0 ) 
  {
    road1.transform.position.y = -256;
    road2.transform.position.y = 0;
  }
  for(int i = 0; i < EVENT_BUFFER_SIZE; i++)
  {
    int chnl = arrws[i].channel;
		arrow_sprites[i]._SetPosition(	
		  X_Positions[chnl],
			(-1 * (frame - arrws[i].time_start) * SPEED_MULT) + 16 - SCREEN_HEIGHT - SCREEN_GAP
		);

    top_arrows[i].transform.position.x = X_Positions[chnl];
    top_arrows[i].transform.position.y = (-1 * (frame - arrws[i].time_start) * SPEED_MULT) + 16;
    //top_arrows[i].uv_position = {0, 32 * (chnl - 1)};
    top_arrows[i].tint = arrws_col[chnl];
		//top_arrows[i].transform.angle = ar
    top_arrows[i].uv_position.x = arrws_rect[chnl].x;
		top_arrows[i].uv_position.y = arrws_rect[chnl].y;
    top_arrows[i].Draw();
    arrow_sprites[i].offset = arrws_offset[chnl];
		arrow_sprites[i].Update();
  }

  for(int i = 0; i < 4; i++)
	{
    if(NQME::Pressed(controls[i]))
    {
      target_arrows[i].transform.scale = 0.8;
    }
    else 
    {
      target_arrows[i].transform.scale = 1;
    }
		if(NQME::JustPressed(controls[i]))
		{
      int d = DistForKey(i+1, eh, 15);
			if(d >= 0)
			{
        int pts = PtsForDist(d);
        game_data.pts += pts;
        for(int i = 0; i < 4; i++)
        {
          if(d <= lvls[i].min_score)
          {
            player_animation.Play_then(
              lvls[i].animation,
              &idle
            ); 
            //printf("Level %d\n", i);
            break;
          }
        }
        
			}
		}		
		target_arrows[i].Draw();
	}

  eh.Update(frame);
  player_animation.Update(); 
  //score_text.text = std::to_string(game_data.pts);
  sprintf(score_text.text, "%d", game_data.pts);
  score_text.Draw();
  accuracy_text.Draw();
  if(accuracy_cooldown > 0)
  {
    accuracy_cooldown--;
    if(accuracy_cooldown == 0)
    {
      strcpy(accuracy_text.text, "");
    }
  }
  if(eh.Ended())
    sm->SwitchTo(2);
}

void Gameplay::Cleanup()
{
  //NE_MaterialDelete(player_mat);
  NE_MaterialDelete(player_mat);
  //NE_PaletteDelete(player_pal);
  NE_PaletteDelete(player_pal);

  for(int i = 0; i < EVENT_BUFFER_SIZE; i++)
 	{
    arrow_sprites[i].visible = false;
		arrow_sprites[i].Update();
  }
  eh.Close();
  NQME::StopStream();
  NQME::CloseStream();
}
