#include "gameplay.hpp"

extern NQME::BGHeader title_screen_bg;

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
  arrows_mat = NE_MaterialCreate();
	arrows_pal = NE_PaletteCreate();
  player_mat = NE_MaterialCreate();
	player_pal  = NE_PaletteCreate();

  printf("Loading arrow material\n");
  NE_MaterialTexLoadGRF(arrows_mat, arrows_pal, NE_TEXTURE_COLOR0_TRANSPARENT, "models/arrows_png.grf");
  
  printf("Loading player spritesheet\n");
  NE_MaterialTexLoadGRF(player_mat, 
                        player_pal, 
                        (NE_TextureFlags)0, 
                        "models/spritesheet_small_png.grf");
  
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
  //top_arrows = nitro_pool;
  //target_arrows = &(nitro_pool[EVENT_BUFFER_SIZE]);

  printf("Setting target arrows \n");
	for(int i = 0; i < 4; i++)
	{
		target_arrows[i].uv_position.y = 32 * i;
		target_arrows[i].anchor = (Vector2i){16, 16};
		target_arrows[i].transform.position = {X_Positions[i+1],  16};
		target_arrows[i].dimensions = {32, 32};
		NE_SpriteSetMaterial(target_arrows[i].sprite, arrows_mat);
	}

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
    NE_SpriteSetMaterial(top_arrows[i].sprite, arrows_mat);
  }

  printf("Setting up Player Chatacter\n");
  player.dimensions = {64, 128};
  
  player.transform.position = (Vector2i){100, 50};

  player_animation.sprite = &player;

  player_animation.Play(&idle);

  player.index = 1;
  NE_MainScreenSetOnTop();
  
  for(int i = 0; i < 198; i++)
  {
    NE_WaitForVBL((NE_UpdateFlags)0);
    fading--;
    NQME::SetFade(fading);
    player_animation.Update();
    NQME::UpdateGraphics();
  }

  NQME::LoadSong("songs/khali.wav.raw");
	NQME::PlayStream();

  eh.Init( "bms/khali.bbm", EVENT_BUFFER_SIZE, arrws);
  eh.grace = 64;
  
	
}

void Gameplay::Update()
{
  //Happens right after VB
  frame++;
  NQME::UpdateInputs(); 
  for(int i = 0; i < EVENT_BUFFER_SIZE; i++)
  {
		arrow_sprites[i]._SetPosition(	
		  X_Positions[arrws[i].channel],
			(-1 * (frame - arrws[i].time_start)) + 16 - SCREEN_HEIGHT - SCREEN_GAP
		);
    top_arrows[i].transform.position.x = X_Positions[arrws[i].channel];
    top_arrows[i].transform.position.y = (-1 * (frame - arrws[i].time_start)) + 16;
    top_arrows[i].uv_position = {0, 32 * (arrws[i].channel - 1)};
    top_arrows[i].Draw();
		arrow_sprites[i].offset = arrws_offsets[arrws[i].channel];
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
}

void Gameplay::Cleanup()
{
  NE_MaterialDelete(arrows_mat);
  NE_MaterialDelete(player_mat);
  NE_PaletteDelete(arrows_pal);
  NE_PaletteDelete(player_pal);

  for(int i = 0; i < EVENT_BUFFER_SIZE; i++)
 	{
    arrow_sprites[i].visible = false;
		arrow_sprites[i].Update();
  }
}
