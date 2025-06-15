extern "C"{
#include "title.h"
#include "arrows.h"
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

	consoleDemoInit();

	NQMT::initFS(); 
	NQMT::listDir();
	NQMT::initAudio();

	NQMT::InitBG();

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

	test.Update();

	NQMT::loadSong("songs/khali.raw");
	NQMT::nqmt_playStream();
	
	int frame = 0;
	while(1)
	{
		swiWaitForVBlank();
		consoleClear();
		printf("Frame : %d\n", frame);
		mmStreamUpdate();
		oamUpdate(&oamMain);
	}
	
	return 0;
}
