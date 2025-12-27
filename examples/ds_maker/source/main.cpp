#include "nds/arm9/background.h"
#include "nds/arm9/video.h"
#include "nds/dma.h"
#define ARM9
#include "gfx/world_tileset_png.h"
//#include "nqme_engine.hpp"
#include <cstdio>
#include <nds.h>

int main() {
  videoSetMode(MODE_0_2D);
//  vramSetPrimaryBanks(VRAM_A_MAIN_BG, VRAM_B_LCD, VRAM_C_LCD, VRAM_D_LCD);

  vramSetBankA(VRAM_A_MAIN_BG);
  int bg_main = bgInit(0, BgType_Text8bpp , BgSize_T_256x256, 0, 1);

  dmaCopy(world_tileset_pngTiles, bgGetGfxPtr(bg_main), world_tileset_pngTilesLen);
  dmaCopy(world_tileset_pngMap, bgGetMapPtr(bg_main), world_tileset_pngMapLen);

  dmaCopy(world_tileset_pngPal, BG_PALETTE, world_tileset_pngPalLen);
  
  //  consoleDemoInit();
 // int frame = 0;
  while (1) {
  //  frame++;
    swiWaitForVBlank();
   // consoleClear();
   // printf("Hello World\n");
  }
  return 0;
}
