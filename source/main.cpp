extern "C"{
#include "title.h"
#include "arrows.h"
#include "tiny_fat_luigi.h"
}
#include "notquite.hpp"
#include "nqmt_engine.hpp"
#include <nds.h>

#define NUM_ARROWS 32
#define TEST_BUFFER_SIZE 6

// ffmpeg -i input.wav -ar 22050 -ac 1 -f u8 -map_metadata -1 output.raw

typedef struct {
    int placeholder;
} SceneData;

int main( void ) {

	int X_Positions[5] =   {-32, 10, 50, 100, 150};
	int arrws_offsets[5] = {  0,  0,  8,  16, 24};

	videoSetMode(MODE_0_2D);
	vramSetBankA(VRAM_A_MAIN_BG);
	vramSetBankB(VRAM_B_MAIN_SPRITE);


	consoleDemoInit();
	printf("==================\n");
  	printf("= INITIALISATION =\n");
	NQMT::InitNQMT();
	printf("==================\n");
  	printf("Size of event : %d", sizeof(NQMT::event));

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
	NQMT::event arrws[TEST_BUFFER_SIZE] = {0};

  	for(int i = 0; i < TEST_BUFFER_SIZE; i++)
 	{
   		arrws[i] = (NQMT::event){0};
		arrow_sprites[i].id = i;
		arrow_sprites[i].SetHeader(ArrowHeader);
		arrow_sprites[i].Update();
  	}

	NQMT::SpriteHeader fat_luigiH((void*)tiny_fat_luigiTiles, (u32)tiny_fat_luigiTilesLen, SQ64_256);
	NQMT::Sprite2D fatlugi(6, fat_luigiH);
	fatlugi.position.x = 10;
	fatlugi.position.y = 10;
	fatlugi.Update();
	NQMT::LoadSong("songs/khali.raw");
	NQMT::PlayStream();
  
  	
  	NQMT::EventHandler eh( "bms/khali.bbm", TEST_BUFFER_SIZE, arrws);
	eh.grace = 64;
	int frame = 0;
	while(1)
	{
		swiWaitForVBlank();
		consoleClear();
		printf("Frame : %d\n", frame);
		for(int i = 0; i < TEST_BUFFER_SIZE; i ++)
    	{
      		printf("event type %u, start %lu, len %u\n",
            	(arrws[i].channel), 
            	(arrws[i].time_start),
            	(arrws[i].duration)
            );
			
			arrow_sprites[i]._SetPosition(
				
				X_Positions[arrws[i].channel],
				-1 * (frame - arrws[i].time_start)
			);
			arrow_sprites[i].offset = arrws_offsets[arrws[i].channel];
			printf("Sprite %d is at [%d, %d]\n", i, arrow_sprites[i].position.x, arrow_sprites[i].position.y);
			arrow_sprites[i].Update();
    	}
		frame++;
		fatlugi.SetPosition(frame % 10, frame % 15);
	  	eh.Update(frame);	
		mmStreamUpdate();
		oamUpdate(&oamMain);
	}
	
	return 0;
}
