#include <nds.h>
#include "nqme_engine.hpp"
#include "nqmt.hpp"
#include "gameplay.hpp"

// ffmpeg -i input.wav -ar 22050 -ac 1 -f u8 -map_metadata -1 output.raw

#define STATE_MAIN_MENU 1
#define STATE_GAMEPLAY  3

NQME::Sprite2D sprite_pool[SPRITE_POOL_SIZE];

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

	//TODO Material Pools (or preload a handful)
	NQME::SceneManager scene_manager;
	Gameplay gameplay(sprite_pool);
	
	NQME::Scene *scene_pool[] = {&gameplay};

	scene_manager.scenes = scene_pool;
	gameplay.Start();

	while(1)
	{
	  NE_WaitForVBL((NE_UpdateFlags)0);
		scene_manager.Update();
		NQME::UpdateGraphics();
		mmStreamUpdate();
	}

	gameplay.Cleanup();
	return 0;
}
