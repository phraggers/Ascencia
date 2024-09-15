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
    
    KEY_ESC, // escape
    KEY_PRTSC, // print screen
    KEY_SCRLK, // scroll lock
    KEY_PAUSE, // pause
    KEY_NUM, // num lock
    KEY_INS, // insert
    KEY_DEL, // delete
    KEY_HOME, // home
    KEY_END, // end
    KEY_PGUP, // page up
    KEY_PGDN, // page down
        
    KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_F6,
    KEY_F7, KEY_F8, KEY_F9, KEY_F10, KEY_F11, KEY_F12,
        
    KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT,
        
    KEY_CAPS, KEY_BACKSPACE, 
    KEY_LSHIFT, KEY_RSHIFT, KEY_LCTRL, KEY_RCTRL,
    KEY_LALT, KEY_RALT,
    KEY_LWIN, // left windows key
    KEY_RWIN, // right windows key
    KEY_MENU, // key usually right of RALT or RWIN
    // usually same as right-click contextual menu at mouse pos 0,0
        
    KEY_TAB = '\t',         KEY_TILDE = '`', 
    KEY_MINUS = '-',        KEY_EQUALS = '=',
    KEY_RETURN = '\n',      KEY_SPACE = ' ', 
    KEY_LBRACKET = '[',     KEY_RBRACKET = ']',
    KEY_SEMICOLON = ';',    KEY_APOSTROPHE = '\'', 
    KEY_BACKSLASH = '\\',   KEY_COMMA = ',', 
    KEY_PERIOD = '.',       KEY_SLASH = '/',
        
    KEY_A = 'a', KEY_B = 'b', KEY_C = 'c', KEY_D = 'd', 
    KEY_E = 'e', KEY_F = 'f', KEY_G = 'g', KEY_H = 'h', 
    KEY_I = 'i', KEY_J = 'j', KEY_K = 'k', KEY_L = 'l', 
    KEY_M = 'm', KEY_N = 'n', KEY_O = 'o', KEY_P = 'p', 
    KEY_Q = 'q', KEY_R = 'r', KEY_S = 's', KEY_T = 't',
    KEY_U = 'u', KEY_V = 'v', KEY_W = 'w', KEY_X = 'x', 
    KEY_Y = 'y', KEY_Z = 'z',
        
    KEY_1 = '1', KEY_2 = '2', KEY_3 = '3', KEY_4 = '4',
    KEY_5 = '5', KEY_6 = '6', KEY_7 = '7', KEY_8 = '8',
    KEY_9 = '9', KEY_0 = '0',
    
    KEY_KP_SLASH, // keypad /
    KEY_KP_MULTIPLY,  // keypad *
    KEY_KP_MINUS, // keypad -
    KEY_KP_PLUS, // keypad +
    KEY_KP_RETURN,
    KEY_KP_PERIOD,
    
    KEY_KP_0, KEY_KP_1, KEY_KP_2, KEY_KP_3, KEY_KP_4, 
    KEY_KP_5, KEY_KP_6, KEY_KP_7, KEY_KP_8, KEY_KP_9,
    
    // media keys
    KEY_M_MUTE, KEY_M_VOLDOWN, KEY_M_VOLUP,
    KEY_M_PLAYPAUSE, KEY_M_STOP,
    KEY_M_PREV, KEY_M_NEXT,
    
    //not implemented: browser keys, applaunch keys
    
    KEY_MAX
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