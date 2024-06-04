/*============================================================
 * Project: Ascencia
 * File: ascencia.cpp
 * Author: Phraggers
 * Date: Sun May 12 2024
============================================================*/

/*

    cmdline Args:
    -nopref : doesn't create an AppData or home directory for configs, all saves are at basepath
    -resetconfig : deletes old config and restores defaults
    -resetkeybinds : deletes old keybinds and restores defaults
    -nofullscreen : ignores config setting and forces no fullscreen at startup
    -fullscreen : ignores config setting and forces fullscreen at startup
    -vsyncon : ignore config setting and force vsync on at startup
    -vsyncoff : ignore config setting and force vsync off at startup

*/

#include <platform/application.h>
#include <SDL3/SDL_main.h>

int main(int argc, char **argv)
{
    if(!ASC_Init(argc, argv)) return -1;
    if(!ASC_Run()) return -1;
    ASC_Quit();
    return 0;
}