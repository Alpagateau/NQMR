#define NQMT_DEBUG

#include "nqme_engine.hpp"
#include "nqmt.hpp"
#include <nds.h>

// Scenes
#include "credits.hpp"
#include "gameplay.hpp"
#include "main_menu.hpp"
#include "resultat.hpp"

// ffmpeg -i input.wav -ar 22050 -ac 1 -f u8 -map_metadata -1 output.raw

NQME::Sprite2D sprite_pool[SPRITE_POOL_SIZE];

NQME::BGHeader title_screen_bg{
    .tiles = (void *)new_title_pngTiles,
    .tileSize = new_title_pngTilesLen,
    .map = (void *)new_title_pngMap,
    .mapSize = new_title_pngMapLen,
};

extern SaveData save_data;

void debug_print(const char *s) { printf("[DEBUG] : %s\n", s); }

int main(void) {
  defaultExceptionHandler();

  videoSetMode(MODE_0_3D);
  videoSetModeSub(MODE_0_2D);

  NE_Init3D();

  NE_TextureSystemReset(3, 3, NE_VRAM_AB);
  consoleDebugInit(DebugDevice_NOCASH);
  DEBUG_PRINT("==================\n");
  DEBUG_PRINT("= INITIALISATION =\n");
  NQME::InitNQME();
  DEBUG_PRINT("==================\n");
  NQME::listDir();
  NE_DebugSetHandler(debug_print);

  loadSave();
  saveSave();

  DEBUG_PRINT("Save data : {\n %d,\n %d, \n %d, \n %d, \n %d}\n",
              save_data.scores[0], save_data.scores[1], save_data.scores[2],
              save_data.scores[3], save_data.scores[4]);

  NQME::SceneManager scene_manager;

  MainMenu main_menu(sprite_pool, &scene_manager);
  Gameplay gameplay(sprite_pool, &scene_manager);
  Results results(sprite_pool, &scene_manager);
  Credits credits(sprite_pool, &scene_manager);
  NQME::Scene *scenes[] = {&main_menu, &gameplay, &results, &credits};

  scene_manager.scenes = scenes;
  scene_manager.current = MAIN_MENU;
  scene_manager.Start();

  long long frame_idx = 0;

  while (1) {
    frame_idx++;
    NE_WaitForVBL((NE_UpdateFlags)0);
    scene_manager.Update();
    NQME::UpdateGraphics();
    mmStreamUpdate();
    fflush(stdout);
  }
  return 0;
}
