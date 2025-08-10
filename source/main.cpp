#include <nds.h>
#include "nqme_engine.hpp"
#include "nqmt.hpp"

//Scenes
#include "main_menu.hpp"
#include "gameplay.hpp"

// ffmpeg -i input.wav -ar 22050 -ac 1 -f u8 -map_metadata -1 output.raw

NQME::Sprite2D sprite_pool[SPRITE_POOL_SIZE];
NQME::NitroSprite nitro_pool[SPRITE_POOL_SIZE];

NQME::BGHeader title_screen_bg 
	   {
		   .tiles = (void*)title_pngTiles,
		   .tileSize = title_pngTilesLen,
		   .map = (void*)title_pngMap,
		   .mapSize = title_pngMapLen,
	   };

int main( void ) {

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
	NQME::listDir();

	//TODO Material Pools (or preload a handful)
	NQME::SceneManager scene_manager;

	MainMenu main_menu(sprite_pool, nitro_pool, &scene_manager);
	Gameplay gameplay(sprite_pool, nitro_pool, &scene_manager);

	NQME::Scene *scenes[] = {
		&main_menu,
		&gameplay
	};

	scene_manager.scenes = scenes;
	
	scene_manager.current = MAIN_MENU;
	scene_manager.Start();

	while(1)
	{
	  NE_WaitForVBL((NE_UpdateFlags)0);
		scene_manager.Update();
		NQME::UpdateGraphics();
		mmStreamUpdate();
	}

	return 0;
}
