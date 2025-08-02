extern "C"{
#include "title.h"
#include "arrows.h"
#include "tiny_fat_luigi.h"
}

#include "nqmt_engine.hpp"
//#define printf(args...) fprintf(stderr, ##args)
#include "nqmt.hpp"
#include <nds.h>

#define NUM_ARROWS 32
#define TEST_BUFFER_SIZE 6

// ffmpeg -i input.wav -ar 22050 -ac 1 -f u8 -map_metadata -1 output.raw

int main( void ) {

	int X_Positions[5]       = {-32, 32, 94, 168, 232};
	int arrws_offsets[5]     = {  0,  0,  8,  16,  24};
  	int ns_arrw_positions[5] = {  0,  0, 32,  64,  96};
	u16 controls[4] = {KEY_LEFT, KEY_UP, KEY_X, KEY_A};


	videoSetMode(MODE_0_3D);
	videoSetModeSub(MODE_0_2D);

	vramSetPrimaryBanks(
		VRAM_A_MAIN_BG, 
		VRAM_B_MAIN_SPRITE, 
		VRAM_C_SUB_BG,
		VRAM_D_SUB_SPRITE);
	
	NE_TextureSystemReset(0, 0, NE_VRAM_AB);
	//consoleDemoInit();
	consoleDebugInit(DebugDevice_NOCASH);
	printf("==================\n");
  	printf("= INITIALISATION =\n");
	NQMT::InitNQMT();
	printf("==================\n");
  	printf("Size of event : %d\n", sizeof(NQMT::event));
	NQMT::listDir();

  	for(int i = 0; i < 100; i++)
  	{
    	swiWaitForVBlank();
  	}
  
	NQMT::BGHeader title_screen_bg 
	{
		.tiles = (void*)titleTiles,
		.tileSize = titleTilesLen,
		.map = (void*)titleMap,
		.mapSize = titleMapLen,
	};
	
	NQMT::SetBackground(title_screen_bg);
	NQMT::SetBackgroundPalette((void*)titlePal, titlePalLen);	
	
	NQMT::SetSpritePalette((void*)arrowsPal, arrowsPalLen);

	NQMT::SpriteHeader ArrowHeader((void*)arrowsTiles, arrowsTilesLen, SQ32_256);
	NQMT::Sprite2D arrow_sprites[TEST_BUFFER_SIZE];
  	NQMT::NitroSprite top_arrows[TEST_BUFFER_SIZE];
	NQMT::event arrws[TEST_BUFFER_SIZE];
  
	NQMT::NitroSprite target_arrows[4];
  	NE_Material *arrows_mat = NE_MaterialCreate();
	NE_Palette *arrows_pal = NE_PaletteCreate();

 
  	NE_MaterialTexLoadFAT(arrows_mat, NE_PAL16, 32, 32*4, NE_TEXTURE_COLOR0_TRANSPARENT, "models/arrows_tex.bin");	
  	NE_PaletteLoadFAT(arrows_pal, "models/arrows_pal.bin", NE_PAL16);
  	NE_MaterialSetPalette(arrows_mat, arrows_pal);
	for(int i = 0; i < 4; i++)
	{
		target_arrows[i].uv_position.y = 32 * i;
		target_arrows[i].anchor = (Vector2i){16, 16};
		target_arrows[i].transform.position = {X_Positions[i+1],  16};
		target_arrows[i].dimensions = {32, 32};
		NE_SpriteSetMaterial(target_arrows[i].sprite, arrows_mat);
	}

  	for(int i = 0; i < TEST_BUFFER_SIZE; i++)
 	{
   		arrws[i] = (NQMT::event){0};
		arrow_sprites[i].SetHeader(ArrowHeader);
		arrow_sprites[i].anchor = (Vector2i){16, 16};
		arrow_sprites[i].Update();
    	//top_arrows[i]; 
    	top_arrows[i].dimensions = {32, 32};
    	top_arrows[i].anchor.x = 16;
    	top_arrows[i].anchor.y =  16;
    	NE_SpriteSetMaterial(top_arrows[i].sprite, arrows_mat);
  	}
	
	NQMT::SpriteHeader fat_luigiH((void*)tiny_fat_luigiTiles, (u32)tiny_fat_luigiTilesLen, SQ64_256);
	NQMT::Sprite2D fatlugi(120, fat_luigiH);
	fatlugi.position.x = 10;
	fatlugi.position.y = 10;
	fatlugi.Update();
	
	NQMT::LoadSong("songs/khali.raw");
	NQMT::PlayStream();
  	
  	NQMT::EventHandler eh( "bms/khali.bbm", TEST_BUFFER_SIZE, arrws);
	eh.grace = 64;
	int frame = 0;
	
	//NQMT::StaticModel tube("models/cylinder.bin");
	//NQMT::StaticModel guy("models/Version1.bin");
 	NE_Material *player_mat = NE_MaterialCreate();
	NE_Palette *player_pal = NE_PaletteCreate();
	NE_MaterialTexLoadFAT(player_mat, NE_PAL256, 512, 256, NE_TEXTURE_COLOR0_TRANSPARENT, "models/spritesheet_small_tex.bin");
  	NE_PaletteLoadFAT(player_pal, "models/spritesheet_small_pal.bin", NE_PAL256);
  	NE_MaterialSetPalette(player_mat, player_pal);

	//NE_MaterialTexLoadFAT(guy_mat, NE_PAL256, 128, 128, (NE_TextureFlags)0, "models/texture1_tex.bin");
	//NE_PaletteLoadFAT(guy_pal, "models/texture1_pal.bin", NE_PAL256);

  	// Set coordinates for the camera
  	NE_Camera *camera = NE_CameraCreate();
  	NE_CameraSet(camera,
               -8, 0, 0,  // Position
                0, 0, 0,  // Look at
                0, 1, 0); // Up direction
	
  	NQMT::UseCamera(camera);
  	NE_LightSet(0, NE_White, -0.5, -0.5, -0.5);
	NQMT::NitroSprite player(64, 128);
  
	NE_SpriteSetMaterial(player.sprite, player_mat);
    //player.transform.scale = 0.3f;
    player.transform.position = (Vector2i){-10, 0};
    Vector2i idle_frames[] = {{0, 0}, {64, 0}, {128, 0}, {192, 0} };

	NQMT::NSAnimation idle = (NQMT::NSAnimation)
                            {
                                .size = 4,
                                .fpf = 8,
                                .frames = idle_frames 
                            };
    
    Vector2i ollie_frames[] = {{0, 128}, {64, 128}, {128, 128}, {192, 128} };
	NQMT::NSAnimation ollie = (NQMT::NSAnimation)
                            {
                                .size = 4,
                                .fpf = 8,
                                .frames = ollie_frames 
                            }; 
    
    Vector2i kick_frames[] = {{256, 0}, {256 + 64, 0}, {256 + 128, 0}, {256 + 192, 0} };
	NQMT::NSAnimation kick = (NQMT::NSAnimation)
                            {
                                .size = 4,
                                .fpf = 8,
                                .frames = kick_frames 
                            };
    
    Vector2i shove_frames[] = {{256, 128}, {256 + 64, 128}, {256 + 128, 128}, {256 + 192, 128} };
	NQMT::NSAnimation shove = (NQMT::NSAnimation)
                        	{
                            	.size = 4,
                                .fpf = 8,
                                .frames = shove_frames 
                            };
	
  	NQMT::AnimatedSprite player_anim(&player);
  	player_anim.Play(&idle);
  	player.index = 1;

  	bool is_playing = true;
	while(1)
	{
	  	NE_WaitForVBL((NE_UpdateFlags)0);
    	NQMT::UpdateInputs(); 
    	for(int i = 0; i < TEST_BUFFER_SIZE; i++)
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
		frame++;
		
		for(int i = 0; i < 4; i++)
		{
			if(NQMT::Pressed(controls[i]))
			{
				if(pointsForKey(i+1, frame) == 0)
				{
					target_arrows[i].transform.scale = 0.8;
				}
			}
			else 
			{
				target_arrows[i].transform.scale = 1.0;
			}
			target_arrows[i].Draw();
			printf("Target %d : %.6f\n", i, target_arrows[i].transform.scale);
		}
		printf("\n================\n");

   		eh.Update(frame);
		mmStreamUpdate();
    	//tube.Draw();
		//guy.Draw();
		//player.Draw();
    	player_anim.Update();
    	NQMT::UpdateGraphics();
	}
	
	return 0;
}
