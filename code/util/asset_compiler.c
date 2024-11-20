/* ==============================================================
   Project: Ascencia
   File:    asset_compiler.c
   Author:  Phraggers
   Date:    20-11-2024
   ============================================================== */

#include <util/lz77.c>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

int main(int argc, char **argv)
{
    printf("\nASSET_COMPILER: %s\n", argv[0]);

    if(argc < 3 || argc > 3)
    {
        printf("ERROR: Invalid arg count: %d\n", argc);
        return -1;
    }

    printf("INPUT:  %s\nOUTPUT: %s\n", argv[1], argv[2]);

    return 0;
}
