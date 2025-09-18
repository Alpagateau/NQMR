#include "gameplay.hpp"

extern NQME::BGHeader title_screen_bg;
extern GameData game_data;

void Gameplay::Start()
{

  printf("Gameplay settup \n");
  NQME::stopStream();
  printf("Setting up background\n");
  NQME::SetBackgroundSub(title_screen_bg);
	NQME::SetBackgroundPaletteSub((void*)title_pngPal, title_pngPalLen);
  NQME::SetSpritePalette((void*)arrows_pngPal, arrows_pngPalLen);
  
  printf("Loading ArrowHeader\n");
  ArrowHeader.Load((void*)arrows_pngTiles, arrows_pngTilesLen, SQ32_256);

  printf("Creating materials\n");
  player_mat = NE_MaterialCreate();
	player_pal = NE_PaletteCreate();
  player_mat = NE_MaterialCreate();
	player_pal  = NE_PaletteCreate();

  printf("Loading player spritesheet\n");
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
  
  printf("Setting target arrows \n");
	for(int i = 0; i < 4; i++)
	{
    target_arrows[i].uv_position.x = arrws_rect[i+1].x;
		target_arrows[i].uv_position.y = arrws_rect[i+1].y;
		target_arrows[i].anchor = (Vector2i){16, 16};
		target_arrows[i].transform.position = {X_Positions[i+1],  16};
		target_arrows[i].dimensions = {32, 32};
		NE_SpriteSetMaterial(target_arrows[i].sprite, player_mat);
	  //NE_SpriteSetMaterial(target_arrows[i].sprite, player_mat);
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

  player.index = 1;
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
  strcpy(score_text.text, "0"); 
  score_text.position = {127, 170};
  score_text.centering = TEXT_CENTER;
}

void Gameplay::Update()
{
  frame++;
  NQME::UpdateInputs(); 
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
      //printf("[DEBUG] Angle : %ld\n", top_arrows[i].transform.angle);
      int d = DistForKey(i+1, eh, 15);
			if(d >= 0)
			{
        int pts = PtsForDist(d);
        game_data.pts += pts;
        if(pts > 40){
          player_animation.Play_then(
            &kick, 
            &idle);
        }
        else if(pts >= 10)
        {
          player_animation.Play_then(
            &shove,
            &idle
          );
        }
        else
        {
          player_animation.Play_then(
            &ollie,
            &idle
          );
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
  if(eh.Ended())
    sm->SwitchTo(0);
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
}
