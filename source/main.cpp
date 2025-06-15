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

	//NQMT::initFS(); 
	//NQMT::listDir();
	
	printf("======================\n");

	NQMT::initAudio();

	NQMT::InitBG();
	NQMT::InitSprites();

	NQMT::BGHeader title_screen_bg 
	{
		.tiles = (void*)titleTiles,
		.tileSize = titleTilesLen,
		.map = (void*)titleMap,
		.mapSize = titleMapLen,
	};
	
	NQMT::SetBackground(title_screen_bg);
	NQMT::SetPalette((void*)titlePal, titlePalLen);	
	
	NQMT::SpriteHeader ArrowHeader((void*)arrowsTiles, arrowsTilesLen, (void*)arrowsPal, arrowsPalLen, SQ32_256);
	NQMT::Sprite2D test;
	test.id = 0;
	test.SetHeader(ArrowHeader);
	test.position.x = 100;
	test.position.y = 100;

	NQMT::SpriteHeader fat_luigiH((void*)tiny_fat_luigiTiles, (u32)tiny_fat_luigiTilesLen, (void*)tiny_fat_luigiPal, tiny_fat_luigiPalLen, SQ64_256);
	NQMT::Sprite2D fatlugi(fat_luigiH);
	fatlugi.id = 1;
	fatlugi.position.x = 10;
	fatlugi.position.y = 10;

	//NQMT::loadSong("songs/khali.raw");
	//NQMT::nqmt_playStream();
	
	int frame = 0;
	while(1)
	{
		swiWaitForVBlank();
		//consoleClear();
		//printf("Frame : %d\n", frame);
		test.Update();
		fatlugi.Update();
		test.position.x = (test.position.x+1)%255;
		mmStreamUpdate();
		oamUpdate(&oamMain);
	}
	
	return 0;
}
