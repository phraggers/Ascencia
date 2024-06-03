/*============================================================
 * Project: Ascencia
 * File: ascencia.cpp
 * Author: Phraggers
 * Date: Sun May 12 2024
============================================================*/

#include <platform/application.h>
#include <SDL3/SDL_main.h>

int main(int argc, char **argv)
{
    if(!ASC_Init()) return -1;
    if(!ASC_Run()) return -1;
    ASC_Quit();
    return 0;
}