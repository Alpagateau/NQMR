#include <nds.h>
#include "nqme_engine.hpp"
#include "nqmt.hpp"
#include "gameplay.hpp"

// ffmpeg -i input.wav -ar 22050 -ac 1 -f u8 -map_metadata -1 output.raw

#define STATE_MAIN_MENU 1
#define STATE_GAMEPLAY  3

int main( void ) {

	u8 state = STATE_GAMEPLAY;

	videoSetMode(MODE_0_3D);
	videoSetModeSub(MODE_0_2D);

	vramSetPrimaryBanks(
		VRAM_A_MAIN_BG, 
		VRAM_B_MAIN_SPRITE, 
		VRAM_C_SUB_BG,
		VRAM_D_SUB_SPRITE
  );
	
	NE_TextureSystemReset(0, 0, NE_VRAM_AB);
	consoleDebugInit(DebugDevice_NOCASH);
	printf("==================\n");
  printf("= INITIALISATION =\n");
	NQME::InitNQME();
	printf("==================\n");

	//TODO Sprite Pools
	//TODO Material Pools (or preload a handful)

	Gameplay gameplay;
	gameplay.Start();

	while(1)
	{
	  NE_WaitForVBL((NE_UpdateFlags)0);

		//Main Menu
		if(state == STATE_MAIN_MENU)
		{
			//NE_MainScreenSetOnBottom();
		}

		//Main Gameplay
		if(state == STATE_GAMEPLAY){
			gameplay.Update();
		}
		NQME::UpdateGraphics();
		mmStreamUpdate();
	}

	gameplay.Cleanup();
	return 0;
}
