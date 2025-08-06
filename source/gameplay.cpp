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
  NE_MaterialTexLoadGRF(player_mat, player_pal, NE_TEXTURE_COLOR0_TRANSPARENT, "models/spritesheet_small_png.grf");

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
  NE_SpriteSetMaterial(player.sprite, player_mat);
  player.transform.position = (Vector2i){-10, 0};

  player_animation.sprite = &player;

  player_animation.Play(&idle);

  player.index = 1;

  NQME::LoadSong("songs/khali.raw");
	NQME::PlayStream();

  eh.Init( "bms/khali.bbm", EVENT_BUFFER_SIZE, arrws);
  eh.grace = 64;
  
	NE_MainScreenSetOnTop();
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
			(-1 * (frame - arrws[i].time_start)) + 16
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
		if(NQME::JustPressed(controls[i]))
		{
			if(pointsForKey(i+1, eh) == 0)
			{
				target_arrows[i].transform.scale = 0.8;
			}
		}
		else 
		{
			target_arrows[i].transform.scale = 1.0;
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
