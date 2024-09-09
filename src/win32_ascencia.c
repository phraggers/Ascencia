
/*============================================================
 * Project: Ascencia [EXE]
 * File: win32_ascencia.c
 * Author: Phraggers
 * Date: Sat Sep 07 2024
============================================================*/

#include <util/types.h>
#define ASCENCIA_WIN32_INSTANCE
#include <win32/shared.h>

bool WIN_Entry(void);
int WinMain(ptr a, ptr b, cstr c, int d) { return WIN_Entry()?0:-1; }
int main(int ac, char **av) { return WIN_Entry()?0:-1; }

bool WIN_Entry(void)
{
    if(!WIN_InterfaceInit(G_win32_data))
    {
        return 0;
    }

    if(!WIN_StdLibInit(G_stdlib_interface))
    {
        return 0;
    }

    WIN_SetBasePath();

    STD_strcpy(G_win32_data->pref_path, PL_GetBasePath());
    STD_strcat(G_win32_data->pref_path, "pref\\");
    STD_puts(G_win32_data->base_path);
    STD_puts(G_win32_data->pref_path);

    return 1;
}