extern "C"{
#include "title.h"
#include "arrows.h"
#include "tiny_fat_luigi.h"
}

#include "nqmt_engine.hpp"
#define printf(args...) fprintf(stderr, ##args)
#include <nds.h>

#define NUM_ARROWS 32
#define TEST_BUFFER_SIZE 6

// ffmpeg -i input.wav -ar 22050 -ac 1 -f u8 -map_metadata -1 output.raw

typedef struct {
    int placeholder;
} SceneData;

int main( void ) {

	int X_Positions[5]   = {-32, 32, 94, 168, 232};
	int arrws_offsets[5] = {  0,  0,  8,  16,  24};

	videoSetMode(MODE_0_3D);
	videoSetModeSub(MODE_0_2D);

	vramSetPrimaryBanks(
		VRAM_A_MAIN_BG, 
		VRAM_B_MAIN_SPRITE, 
		VRAM_C_SUB_BG,
		VRAM_D_SUB_SPRITE);

	//consoleDemoInit();
	consoleDebugInit(DebugDevice_NOCASH);
	printf("==================\n");
  	printf("= INITIALISATION =\n");
	NQMT::InitNQMT();
	printf("==================\n");
  	printf("Size of event : %d", sizeof(NQMT::event));
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
	NQMT::event arrws[TEST_BUFFER_SIZE];

	NQMT::Sprite2D target_arrows[4];

	for(int i = 0; i < 4; i++)
	{
		printf("ta %d, %u\n", i,target_arrows[i].id);
		target_arrows[i].SetHeader(ArrowHeader);
		target_arrows[i].offset = arrws_offsets[i+1];
		target_arrows[i].anchor = (Vector2i){16, 16};
		target_arrows[i]._SetPosition(X_Positions[i+1],  16);
		target_arrows[i].Update();
	}

	for(int i = 0; i < 100; i++)
  	{
    	swiWaitForVBlank();
  	}

  	for(int i = 0; i < TEST_BUFFER_SIZE; i++)
 	{
   		arrws[i] = (NQMT::event){0};
		arrow_sprites[i].SetHeader(ArrowHeader);
		arrow_sprites[i].anchor = (Vector2i){16, 16};
		arrow_sprites[i].Update();
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
	int bg_colour = 13 << 10;
	
	// red cpu usage
	int cpu_colour = 31;

	bool is_playing = true;
	while(1)
	{
		swiWaitForVBlank();
		NQMT::UpdateInputs();
		consoleClear();
		//printf("Frame : %d\n", frame);
		
		for(int i = 0; i < TEST_BUFFER_SIZE; i ++)
    	{
			arrow_sprites[i]._SetPosition(
				
				X_Positions[arrws[i].channel],
				(-1 * (frame - arrws[i].time_start)) + 16
			);
			arrow_sprites[i].offset = arrws_offsets[arrws[i].channel];
			arrow_sprites[i].Update();
    	}

		frame++;
		if(NQMT::isButtonJustPressed( KEY_A ))
		{
			//fatlugi.position.x += 10;
			is_playing = !is_playing;
			if(is_playing)
				NQMT::PlayStream();
			else 
				NQMT::stopStream();
		}
		fatlugi.Update();
	  	
		eh.Update(frame);
		mmStreamUpdate();
		oamUpdate(&oamSub);
	}
	
	return 0;
}
