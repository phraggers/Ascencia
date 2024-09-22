/*============================================================
 * Project: Ascencia
 * File: input.h
 * Author: Phraggers
 * Date: Sat Sep 14 2024
============================================================*/

#ifndef ASCENCIA_INPUT_H
#define ASCENCIA_INPUT_H

#define GP_MAX_COUNT 4

#include <util/types.h>

typedef enum
{
    KEY_NONE = 0,

    KEY_ESC = 1,
    KEY_PRTSC = 2,
    KEY_SCRLK = 3,
    KEY_PAUSE = 4,
    KEY_NUM = 5,
    KEY_INS = 6,
    KEY_DEL = 7,
    KEY_HOME = 8,
    KEY_TAB = '\t', //9
    KEY_RETURN = '\n', //10
    KEY_END = 11,
    KEY_PGUP = 12,
    KEY_PGDN = 13,
    KEY_F1 = 14,
    KEY_F2 = 15,
    KEY_F3 = 16,
    KEY_F4 = 17,
    KEY_F5 = 18,
    KEY_F6 = 19,
    KEY_F7 = 20,
    KEY_F8 = 21,
    KEY_F9 = 22,
    KEY_F10 = 23,
    KEY_F11 = 24,
    KEY_F12 = 25,
    KEY_UP = 26,
    KEY_DOWN = 27,
    KEY_LEFT = 28,
    KEY_RIGHT = 29,
    KEY_CAPS = 30,
    KEY_BACKSPACE = 31,
    KEY_SPACE = ' ', //32
    KEY_LSHIFT = 33,
    KEY_RSHIFT = 34,
    KEY_LCTRL = 35,
    KEY_RCTRL = 36,
    KEY_LALT = 37,
    KEY_RALT = 38,
    KEY_APOSTROPHE = '\'', //39
    KEY_LWIN = 40,
    KEY_RWIN = 41,
    KEY_MENU = 42,
    KEY_KP_SLASH = 43,
    KEY_COMMA = ',', //44
    KEY_MINUS = '-', //45
    KEY_PERIOD = '.', //46
    KEY_SLASH = '/', //47
    KEY_0 = '0', //48
    KEY_1 = '1', //49
    KEY_2 = '2', //50
    KEY_3 = '3', //51
    KEY_4 = '4', //52
    KEY_5 = '5', //53
    KEY_6 = '6', //54
    KEY_7 = '7', //55
    KEY_8 = '8', //56
    KEY_9 = '9', //57
    KEY_KP_MULTIPLY = 58,
    KEY_SEMICOLON = ';', //59
    KEY_KP_MINUS = 60,
    KEY_EQUALS = '=', //61
    KEY_KP_PLUS = 62,
    KEY_KP_RETURN = 63,
    KEY_KP_PERIOD = 64,
    KEY_KP_0 = 65,
    KEY_KP_1 = 66,
    KEY_KP_2 = 67,
    KEY_KP_3 = 68,
    KEY_KP_4 = 69,
    KEY_KP_5 = 70,
    KEY_KP_6 = 71,
    KEY_KP_7 = 72,
    KEY_KP_8 = 73,
    KEY_KP_9 = 74,
    KEY_M_MUTE = 75,
    KEY_M_VOLDOWN = 76,
    KEY_M_VOLUP = 77,
    KEY_M_PLAYPAUSE = 78,
    KEY_M_STOP = 79,
    KEY_M_PREV = 80,
    KEY_M_NEXT = 81,
    KEY_UNUSED82 = 82,
    KEY_UNUSED83 = 83,
    KEY_UNUSED84 = 84,
    KEY_UNUSED85 = 85,
    KEY_UNUSED86 = 86,
    KEY_UNUSED87 = 87,
    KEY_UNUSED88 = 88,
    KEY_UNUSED89 = 89,
    KEY_UNUSED90 = 90,
    KEY_LBRACKET = '[', //91
    KEY_BACKSLASH = '\\', //92
    KEY_RBRACKET = ']', //93
    KEY_UNUSED94 = 94,
    KEY_UNUSED95 = 95,
    KEY_TILDE = '`', //96
    KEY_A = 'a', //97
    KEY_B = 'b', //98
    KEY_C = 'c', //99
    KEY_D = 'd', //100
    KEY_E = 'e', //101
    KEY_F = 'f', //102
    KEY_G = 'g', //103
    KEY_H = 'h', //104
    KEY_I = 'i', //105
    KEY_J = 'j', //106
    KEY_K = 'k', //107
    KEY_L = 'l', //108
    KEY_M = 'm', //109
    KEY_N = 'n', //110
    KEY_O = 'o', //111
    KEY_P = 'p', //112
    KEY_Q = 'q', //113
    KEY_R = 'r', //114
    KEY_S = 's', //115
    KEY_T = 't', //116
    KEY_U = 'u', //117
    KEY_V = 'v', //118
    KEY_W = 'w', //119
    KEY_X = 'x', //120
    KEY_Y = 'y', //121
    KEY_Z = 'z', //122

    //not implemented: browser keys, applaunch keys

    KEY_MAX = 123
} PL_KEYCODE;

typedef enum
{
    MB_NONE = 0,

    MB_LEFT,
    MB_RIGHT,
    MB_MIDDLE,
    MB_X1,
    MB_X2,

    MB_MAX
} PL_MOUSEBTNCODE;

typedef enum
{
    GP_NONE = 0,

    GP_A,
    GP_B,
    GP_X,
    GP_Y,
    GP_START,
    GP_BACK,
    GP_UP,
    GP_DOWN,
    GP_LEFT,
    GP_RIGHT,
    GP_LSHOULDER,
    GP_RSHOULDER,
    GP_LSTICK, //L3
    GP_RSTICK, //R3

    GP_MAX
} PL_GAMEPADBTNCODE;

typedef struct
{
    r64 t_down; // seconds held down
    b8 is_down; // is currently down
    b8 was_down; // was down previous frame
    b8 down_tick; // was first pressed down this frame
    b8 up_tick; // was first released this frame
} PL_ButtonState;

typedef struct
{
    r32 rx,ry; // normalized pos -1 to 1 (0,0 is window centre)
    int px,py; // pixel pos (relative to window: 0,0 is top left)
    PL_ButtonState buttons[MB_MAX];
    int wheel; // <0: down clicks, >0: up clicks
} PL_Mouse;

typedef struct
{
    r32 x, y; // normalized stick state -1 to 1 (0,0: centre)
} PL_GamepadStickState;

typedef struct
{
    b32 is_connected;
    PL_ButtonState buttons[GP_MAX];
    PL_GamepadStickState left_stick, right_stick; // sticks -1.0 to 1.0
    r32 left_trigger, right_trigger; // triggers 0.0 - 1.0
    r32 left_vib, right_vib; // vibration 0.0 - 1.0
} PL_Gamepad;

typedef struct
{
    PL_ButtonState keyboard[KEY_MAX];
    PL_Mouse mouse;
    PL_Gamepad gamepads[GP_MAX_COUNT];
} PL_Input;

// returns handles to the input structs
PL_Input* PL_GetInput(void);
PL_Mouse* PL_GetMouse(void);
PL_Gamepad* PL_GetGamepad(i32 id);
PL_ButtonState* PL_GetKeyState(PL_KEYCODE key);

#endif /* ASCENCIA_INPUT_H */