//#define NE_DEBUG
#include <nds.h>
#include "nqme_engine.hpp"
#include "nqmt.hpp"

//Scenes
#include "main_menu.hpp"
#include "gameplay.hpp"
#include "resultat.hpp"

// ffmpeg -i input.wav -ar 22050 -ac 1 -f u8 -map_metadata -1 output.raw

NQME::Sprite2D sprite_pool[SPRITE_POOL_SIZE];

NQME::BGHeader title_screen_bg 
	   {
		   .tiles = (void*)title_pngTiles,
		   .tileSize = title_pngTilesLen,
		   .map = (void*)title_pngMap,
		   .mapSize = title_pngMapLen,
	   };

extern SaveData save_data;

void debug_print(const char* s)
{
  printf("[DEBUG] : %s\n", s);
}

int main( void ) {
  defaultExceptionHandler();


	videoSetMode(MODE_0_3D);
	videoSetModeSub(MODE_0_2D);
  
  NE_Init3D();
  
	NE_TextureSystemReset(3, 3, NE_VRAM_AB);
	consoleDebugInit(DebugDevice_NOCASH);
  //consoleDemoInit();
  //consoleInit(NULL, 1, BgType_Text4bpp, BgSize_T_256x256, 22, 3, false, true);
  //consoleDebugInit(DebugDevice_CONSOLE);
	printf("==================\n");
  printf("= INITIALISATION =\n");
	NQME::InitNQME();
	printf("==================\n");
	NQME::listDir();
  //chdir("./models");
  //printf("=====================\n");
  //NQME::listDir();
  //printf("=====================\n");
  //chdir("..");

  NE_DebugSetHandler(debug_print);

  //Save System
   
  //Do some mem tests
  //printf("[DEBUG] Eeprom Size : %ld\n", cardEepromGetSize());
  loadSave();
  saveSave();

  printf("[DEBUG] Save data : {\n %d,\n %d, \n %d, \n %d, \n %d}\n", 
         save_data.scores[0],
         save_data.scores[1],
         save_data.scores[2],
         save_data.scores[3],
         save_data.scores[4]
         );

	//TODO Material Pools (or preload a handful)
   
  NQME::SceneManager scene_manager;

	MainMenu main_menu(sprite_pool, &scene_manager);
	Gameplay gameplay(sprite_pool, &scene_manager);
  Results results(sprite_pool, &scene_manager);

	NQME::Scene *scenes[] = {
		&main_menu,
		&gameplay,
    &results
	};

	scene_manager.scenes = scenes;
  	
  printf("End of initialisation\n");
  printf("Press A to resume");
  while(true)
  {
    NQME::UpdateInputs();
    if(NQME::JustPressed(KEY_A))
    {consoleClear();break;}
    swiWaitForVBlank();
  }


	scene_manager.current = MAIN_MENU;
	scene_manager.Start();

	while(1)
	{
	  NE_WaitForVBL((NE_UpdateFlags)0);
		scene_manager.Update();
		NQME::UpdateGraphics();
		mmStreamUpdate();
    fflush(stdout);
	}

	return 0;
}
