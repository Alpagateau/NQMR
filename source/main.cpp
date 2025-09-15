//#define NE_DEBUG
#include <nds.h>
#include "nqme_engine.hpp"
#include "nqmt.hpp"

//Scenes
#include "main_menu.hpp"
#include "gameplay.hpp"

// ffmpeg -i input.wav -ar 22050 -ac 1 -f u8 -map_metadata -1 output.raw

NQME::Sprite2D sprite_pool[SPRITE_POOL_SIZE];
//NE_Material* player_mat;
//NE_Palette*  player_pal;

NQME::BGHeader title_screen_bg 
	   {
		   .tiles = (void*)title_pngTiles,
		   .tileSize = title_pngTilesLen,
		   .map = (void*)title_pngMap,
		   .mapSize = title_pngMapLen,
	   };

void debug_print(const char* s)
{
  printf("[DEBUG] : %s\n", s);
}

int main( void ) {

	videoSetMode(MODE_0_3D);
	videoSetModeSub(MODE_0_2D);
  
  NE_Init3D();
  
	NE_TextureSystemReset(3, 3, NE_VRAM_AB);
	consoleDebugInit(DebugDevice_NOCASH);
	printf("==================\n");
  printf("= INITIALISATION =\n");
	NQME::InitNQME();
	printf("==================\n");
	NQME::listDir();
  chdir("./models");
  printf("=====================\n");
  NQME::listDir();
  printf("======================\n");
  chdir("..");  

  NE_DebugSetHandler(debug_print);

  //player_mat = NE_MaterialCreate();
  //player_pal = NE_PaletteCreate();

 /* NE_MaterialTexLoadGRF(
    player_mat,
    player_pal,
    (NE_TextureFlags)0,
    //"models/menu_background_png.grf"
    "models/256_tiny_png.grf"
  ); 
  */



	//TODO Material Pools (or preload a handful)
	NQME::SceneManager scene_manager;

	MainMenu main_menu(sprite_pool, &scene_manager);
	Gameplay gameplay(sprite_pool, &scene_manager);

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
    fflush(stdout);
	}

	return 0;
}
