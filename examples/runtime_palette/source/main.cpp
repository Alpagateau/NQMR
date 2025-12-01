#define ARM9
#include <nds.h>
#include <cstdio>
#include "nqme_engine.hpp"
#include "gfx/test_maze_png.h"

int main()
{
    videoSetMode(MODE_0_2D);
    vramSetPrimaryBanks(VRAM_A_MAIN_BG, VRAM_B_LCD, VRAM_C_LCD, VRAM_D_LCD);

    int bg = bgInit(0, BgType_Text8bpp, BgSize_T_256x256, 0,1);
   
    NQMPalette new_palette = make_palette_runtime(test_maze_pngPal, test_maze_pngPalLen); 
    //struct tileset ts = 

    dmaCopy(test_maze_pngTiles, bgGetGfxPtr(bg), test_maze_pngTilesLen);
    dmaCopy(test_maze_pngMap, bgGetMapPtr(bg), test_maze_pngMapLen);
    dmaCopy(new_palette.data, BG_PALETTE, test_maze_pngPalLen);
    new_palette.data[1] = COLOR_GRAY;
    consoleDemoInit();
    int frame = 0;
    while(1)
    {
        frame++;
        swiWaitForVBlank();
        consoleClear();
        //printf("Hello World\n");

        if(frame % 120 < 60)
        {
            new_palette.data[0] = color_rgb(0xFF, 0xFF, 0x00);
        }else
        {
            new_palette.data[0] = 0x0;
        }
        
        for(int i = 0; i < 4; i++)
        {
            printf("color %d : %X\n", i, new_palette.data[i]);
        }
        
        dmaCopy(new_palette.data, BG_PALETTE, test_maze_pngPalLen);
    }
    return 0;
}
