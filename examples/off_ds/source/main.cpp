#define ARM9
#include <nds.h>
#include <cstdio>
#include "nqme_engine.hpp"
#include "gfx/test_maze_png.h"

int main()
{
    videoSetMode(MODE_0_2D);
    vramSetPrimaryBanks(VRAM_A_MAIN_BG, VRAM_B_LCD, VRAM_C_LCD, VRAM_D_LCD);

   
    //struct tileset ts = 

    consoleDemoInit();
    int frame = 0;
    while(1)
    {
        frame++;
        swiWaitForVBlank();
        consoleClear();
        printf("Hello World\n");
    }
    return 0;
}
