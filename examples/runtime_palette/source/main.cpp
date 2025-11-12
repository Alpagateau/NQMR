#include <nds.h>
#include <cstdio>
#include "gfx/test_maze_png.h"

int main()
{

    videoSetMode(MODE_0_2D);
    vramSetPrimaryBanks(VRAM_A_MAIN_BG, VRAM_B_LCD, VRAM_C_LCD, VRAM_D_LCD);

    int bg = bgInit(0, BgType_Text8bpp, BgSize_T_256x256, 0,1);

    char* new_palette = (char*)malloc(test_maze_pngPalLen);
    
    for(int i = 0; i < test_maze_pngPalLen; i++)
    {
        new_palette[i] = test_maze_pngPal[i];
    }

    dmaCopy(test_maze_pngTiles, bgGetGfxPtr(bg), test_maze_pngTilesLen);
    dmaCopy(test_maze_pngMap, bgGetMapPtr(bg), test_maze_pngMapLen);
    dmaCopy(new_palette, BG_PALETTE, test_maze_pngPalLen);

    consoleDemoInit();
    int frame = 0;
    while(1)
    {
        frame++;
        swiWaitForVBlank();
        consoleClear();
        printf("Hello World\n");

        if(frame % 120 < 60)
        {
            new_palette[0] = 0xFF;
        }else
        {
            new_palette[0] = 0x0;
        }
        
        for(int i = 0; i < 4; i++)
        {
            printf("color %d : %X\n", i, test_maze_pngPal[i]);
        }
        
        dmaCopy(new_palette, BG_PALETTE, test_maze_pngPalLen);
    }
    return 0;
}
