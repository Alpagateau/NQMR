extern "C"{
#include "title.h"
#include "arrows.h"
#include "tiny_fat_luigi.h"
}
#include "notquite.hpp"
#include "nqmt_engine.hpp"
#include <nds.h>

#define NUM_ARROWS 32

// ffmpeg -i input.wav -ar 22050 -ac 1 -f u8 -map_metadata -1 output.raw

typedef struct {
    int placeholder;
} SceneData;

int main( void ) {
	videoSetMode(MODE_0_2D);
	vramSetBankA(VRAM_A_MAIN_BG);
	vramSetBankB(VRAM_B_MAIN_SPRITE);


	consoleDemoInit();
	printf("==== INITIALISATION ====\n");
	NQMT::InitNQMT();
	printf("========================\n");

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
	NQMT::Sprite2D arrow1(0, ArrowHeader);
	arrow1.SetPosition(100, 100);

	NQMT::Sprite2D arrow2(2, ArrowHeader);
	arrow2.SetPosition(132, 100);

	NQMT::SpriteHeader fat_luigiH((void*)tiny_fat_luigiTiles, (u32)tiny_fat_luigiTilesLen, SQ64_256);
	NQMT::Sprite2D fatlugi(1, fat_luigiH);
	fatlugi.position.x = 10;
	fatlugi.position.y = 10;

	NQMT::LoadSong("songs/khali.raw");
	NQMT::PlayStream();
	
	int frame = 0;
	while(1)
	{
		swiWaitForVBlank();
		//consoleClear();
		//printf("Frame : %d\n", frame);
		arrow1.Update();
		arrow2.Update();
		frame++;
		if(frame%60 == 0)
		{
			arrow2.offset++;
			arrow2.offset %= 100;
		}
		fatlugi.Update();
		arrow1.position.y--;
		if(arrow1.position.y <= -32)
		{
			printf("Changing position : %d\n", arrow1.position.y);
			arrow1.position.y = 256;
		}
		mmStreamUpdate();
		oamUpdate(&oamMain);
	}
	
	return 0;
}
