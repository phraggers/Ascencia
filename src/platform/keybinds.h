/*============================================================
 * Project: Ascencia
 * File: keybinds.h
 * Author: Phraggers
 * Date: Mon Sep 23 2024
============================================================*/

#ifndef ASCENCIA_KEYBINDS_H
#define ASCENCIA_KEYBINDS_H

#include <platform/input.h>

typedef enum
{
    KBMOD_NONE = 0,

    KBMOD_LSHIFT = (1<<0),
    KBMOD_LCTRL = (1<<1),
    KBMOD_LALT = (1<<2),

    KBMOD_RSHIFT = (1<<3),
    KBMOD_RCTRL = (1<<4),
    KBMOD_RALT = (1<<5),

    KBMOD_SHIFT = (KBMOD_LSHIFT & KBMOD_RSHIFT),
    KBMOD_CTRL = (KBMOD_LCTRL & KBMOD_RCTRL),
    KBMOD_ALT = (KBMOD_LALT & KBMOD_RCTRL)

} PL_KBMOD; //Key Bind Modifier

typedef enum
{
    KBKS_NONE = 0, // unset
    KBKS_ONDOWN, // on down tick
    KBKS_ONUP, // on up tick
    KBKS_HELD // while held
} PL_KBKS; //Key Bind Key State

typedef struct
{
    PL_KEYCODE key;
    PL_KBKS state;
    PL_KBMOD mod;
} PL_Keybind;

typedef struct
{
    PL_Keybind Quit;
    PL_Keybind Fullscreen1;
    PL_Keybind Fullscreen2;
} PL_Keybindings;

bool PL_KeybindsInit(cstr keybinds_path);
bool PL_KeybindsSave(void);
bool PL_KeybindsLoad(void);
cstr PL_GetKeybindsPath(void);
bool PL_SetKeybindsPath(cstr path);
PL_Keybindings *PL_GetKeybindings(void);
PL_Keybind PL_KeybindNew(PL_KEYCODE key, PL_KBKS state, PL_KBMOD mod);
bool PL_SetKeybind(PL_Keybind *dst, PL_Keybind src);
bool PL_ProcessKeybind(PL_Keybind *keybind);
void PL_ProcessKeybinds(void);

#endif /* ASCENCIA_KEYBINDS_H */