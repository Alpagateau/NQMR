extern "C"{
#include "nqmt.h"
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
	NQMT::initFS();
	NQMT::listDir();
	initAudio();

	loadSong("songs/khali.raw");
	nqmt_playStream();
	
	int frame = 0;
	while(1)
	{
		swiWaitForVBlank();
		mmStreamUpdate();
		frame++;
		oamUpdate(&oamMain);
	}
	
	return 0;
}
