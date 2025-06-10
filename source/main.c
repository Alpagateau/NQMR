#include "nqmt_engine.h"
#include <nds.h>
#include "nqmt.h"
#include "arrows.h"
#include "notquite.h"

#define NUM_ARROWS 32

// ffmpeg -i input.wav -ar 22050 -ac 1 -f u8 -map_metadata -1 output.raw

int main( void ) {
	videoSetMode(MODE_0_2D);

	vramSetBankA(VRAM_A_MAIN_BG);
	vramSetBankB(VRAM_B_MAIN_SPRITE);

	consoleDemoInit();

	initFS();
	InitGfx();
	listDir();
	initAudio();

	BGHeader title_bg = {
		.tiles = nqmtTiles,
		.tileSize = nqmtTilesLen,
		.map = nqmtMap,
		.mapSize = nqmtMapLen
	};

	SetBackground(title_bg);
	dmaCopy(nqmtPal, BG_PALETTE, nqmtPalLen);

	SpriteHeader arrowSprite = {
		.tiles = arrowsTiles,
		.tileSize = arrowsTilesLen,
		.pal = arrowsPal,
		.palSize = arrowsPalLen
	};

	SetArrowSprite(arrowSprite);
	Arrow arrow_buffer[NUM_ARROWS] = {0};

	//Load arrows
	for(int i = 0; i < NUM_ARROWS; ++i)
	{
		arrow_buffer[i] = (Arrow){i, i%4, (Vector2i){(i%4)*50, i*80}, false};
		SetArrow(arrow_buffer[i]);
	}

	// Draw target arrows
	u16* arw_ptr = getArrowOffset();
	oamSet(
		&oamMain, NUM_ARROWS + 0,
		0, 0,
		0, 0, SpriteSize_32x32, SpriteColorFormat_256Color,
		arw_ptr + 4 * 128 * 4,
		-1,
		false,
		false,
		false,
		false,
		false
	);
	
	loadSong("songs/khali.raw");
	loadBeatMap("bms/khali.bbm");
	nqmt_playStream();
	fromBeatmap(arrow_buffer, NUM_ARROWS);
	int frame = 0;
	while(1)
	{
		frame++;
		swiWaitForVBlank();
		mmStreamUpdate();
		fromBeatmap(arrow_buffer, NUM_ARROWS);
		updateBeatBuffer();
		for(int i = 0; i < NUM_ARROWS; ++i)
		{
			//arrow_buffer[i].position.y--;
			SetArrow(arrow_buffer[i]);
		}
		oamUpdate(&oamMain);
        //printf("Time is : %d\n", getTime());
		if(frame % 1000 == 0){
			consoleClear();
		}
	}
	
	return 0;
}
