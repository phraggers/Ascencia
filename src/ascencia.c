
/*============================================================
 * Project: Ascencia [DLL]
 * File: ascencia.c
 * Author: Phraggers
 * Date: Sat Sep 07 2024
============================================================*/

/*
:: Note on code style:
:: PL_x : platform layer defined for each platform
:: ASC_x : Ascencia game code
:: STD_x : stdlib functions
:: WIN_x : windows/MSVC code
:: - Functions which can fail are bool type, returns success state
:: - most memory blocks are zero initialized
:: - Platform code has entry point, loads game code
:: - game code can call PL for certain data/functions
:: - stdlib is separated from PL and ASC, injected by DLL wrapper
*/

#include <util/types.h>
#include <util/platform.h>

void placeholder(void)
{
    return;
}