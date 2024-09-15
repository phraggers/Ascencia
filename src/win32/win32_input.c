/*============================================================
 * Project: Ascencia
 * File: win32_input.c
 * Author: Phraggers
 * Date: Sat Sep 14 2024
============================================================*/

#include <util/types.h>
#include <util/maths.h>
#include <platform/input.h>
#include <win32/win32_state.h>
#include <win32/win32_api.h>

PL_Input* PL_GetInput(void)
{
    if(!G_win32_state)
    {
        return 0;
    }

    PL_Input *result = &G_win32_state->window.input;
    return result;
}

PL_Mouse* PL_GetMouse(void)
{
    if(!G_win32_state)
    {
        return 0;
    }

    PL_Mouse *result = &PL_GetInput()->mouse;
    return result;
}

PL_Gamepad* PL_GetGamepad(i32 id)
{
    if((!G_win32_state) ||
       (id < 0) ||
       (id > GP_MAX_COUNT-1))
    {
        return 0;
    }

    PL_Gamepad *result = &PL_GetInput()->gamepads[id];
    return result;
}

PL_ButtonState* PL_GetKeyState(PL_KEYCODE key)
{
    if((!G_win32_state) ||
       (key <= KEY_NONE) ||
       (key >= KEY_MAX))
    {
        return 0;
    }

    PL_ButtonState *result = &PL_GetInput()->keyboard[key];
    return result;
}

PL_KEYCODE Win32_Keycode(u32 key, i64 lparam)
{
    switch(key)
    {
        case 0x1b: return KEY_ESC;
        case 0x2a: return KEY_PRTSC;
        case 0x91: return KEY_SCRLK;
        case 0x13: return KEY_PAUSE;
        case 0x90: return KEY_NUM;
        case 0x2d: return KEY_INS;
        case 0x2e: return KEY_DEL;
        case 0x24: return KEY_HOME;
        case 0x23: return KEY_END;
        case 0x21: return KEY_PGUP;
        case 0x22: return KEY_PGDN;
        
        case 0x70: return KEY_F1;
        case 0x71: return KEY_F2;
        case 0x72: return KEY_F3;
        case 0x73: return KEY_F4;
        case 0x74: return KEY_F5;
        case 0x75: return KEY_F6;
        case 0x76: return KEY_F7;
        case 0x77: return KEY_F8;
        case 0x78: return KEY_F9;
        case 0x79: return KEY_F10;
        case 0x7a: return KEY_F11;
        case 0x7b: return KEY_F12;
        
        case 0x26: return KEY_UP;
        case 0x28: return KEY_DOWN;
        case 0x25: return KEY_LEFT;
        case 0x27: return KEY_RIGHT;

        /* NOTE:
        VK_L* & VK_R* - left and right Alt, Ctrl and Shift virtual keys.
        Used only as parameters to GetAsyncKeyState() and GetKeyState().
        No other API or message will distinguish left and right keys in this way.*/
        
        case 0x14: return KEY_CAPS;
        case 0x08: return KEY_BACKSPACE;
        case 0xa0: return KEY_LSHIFT;
        case 0xa1: return KEY_RSHIFT;
        case 0xa2: return KEY_LCTRL;
        case 0xa3: return KEY_RCTRL;
        case 0x5b: return KEY_LWIN;
        case 0x5c: return KEY_RWIN;
        case 0x5d: return KEY_MENU;
        
        case 0x09: return KEY_TAB;
        case 0xc0: return KEY_TILDE;
        case 0xbd: return KEY_MINUS;
        case 0xbb: return KEY_EQUALS;
        
        case 0x0d:
        {
            if(!(lparam & (1<<24))) return KEY_RETURN;
            else return KEY_KP_RETURN;
        }
        
        case 0x20: return KEY_SPACE;
        case 0xdb: return KEY_LBRACKET;
        case 0xdd: return KEY_RBRACKET;
        case 0xba: return KEY_SEMICOLON;
        case 0xde: return KEY_APOSTROPHE;
        case 0xdc: return KEY_BACKSLASH;
        case 0xbc: return KEY_COMMA;
        case 0xbe: return KEY_PERIOD;
        case 0xbf: return KEY_SLASH;
        
        case 0x41: return KEY_A;
        case 0x42: return KEY_B;
        case 0x43: return KEY_C;
        case 0x44: return KEY_D;
        case 0x45: return KEY_E;
        case 0x46: return KEY_F;
        case 0x47: return KEY_G;
        case 0x48: return KEY_H;
        case 0x49: return KEY_I;
        case 0x4a: return KEY_J;
        case 0x4b: return KEY_K;
        case 0x4c: return KEY_L;
        case 0x4d: return KEY_M;
        case 0x4e: return KEY_N;
        case 0x4f: return KEY_O;
        case 0x50: return KEY_P;
        case 0x51: return KEY_Q;
        case 0x52: return KEY_R;
        case 0x53: return KEY_S;
        case 0x54: return KEY_T;
        case 0x55: return KEY_U;
        case 0x56: return KEY_V;
        case 0x57: return KEY_W;
        case 0x58: return KEY_X;
        case 0x59: return KEY_Y;
        case 0x5a: return KEY_Z;
        
        case 0x31: return KEY_1;
        case 0x32: return KEY_2;
        case 0x33: return KEY_3;
        case 0x34: return KEY_4;
        case 0x35: return KEY_5;
        case 0x36: return KEY_6;
        case 0x37: return KEY_7;
        case 0x38: return KEY_8;
        case 0x39: return KEY_9;
        case 0x30: return KEY_0;
        
        case 0x6f: return KEY_KP_SLASH;
        case 0x6a: return KEY_KP_MULTIPLY;
        case 0x6d: return KEY_KP_MINUS;
        case 0x6b: return KEY_KP_PLUS;
        case 0x6e: return KEY_KP_PERIOD;
        
        case 0x60: return KEY_KP_0;
        case 0x61: return KEY_KP_1;
        case 0x62: return KEY_KP_2;
        case 0x63: return KEY_KP_3;
        case 0x64: return KEY_KP_4;
        case 0x65: return KEY_KP_5;
        case 0x66: return KEY_KP_6;
        case 0x67: return KEY_KP_7;
        case 0x68: return KEY_KP_8;
        case 0x69: return KEY_KP_9;
        
        case 0xad: return KEY_M_MUTE;
        case 0xae: return KEY_M_VOLDOWN;
        case 0xaf: return KEY_M_VOLUP;
        case 0xb3: return KEY_M_PLAYPAUSE;
        case 0xb2: return KEY_M_STOP;
        case 0xb1: return KEY_M_PREV;
        case 0xb0: return KEY_M_NEXT;
        
        default: return KEY_NONE;
    }
}

void Win32_ProcessXInputButton(Win32_XINPUT_GAMEPAD *xpad, PL_Gamepad *gamepad, u16 xbutton, PL_GAMEPADBTNCODE button_code, r64 dt)
{
    if((xpad->buttons & xbutton) != 0)
    {
        gamepad->buttons[button_code].is_down = 1;
        gamepad->buttons[button_code].t_down += dt;
    }
    else
    {
        gamepad->buttons[button_code].is_down = 0;
    }
}

void Win32_ProcessInput(r64 dt)
{
    PL_ButtonState *keyboard = &PL_GetInput()->keyboard[0];

    for(int key_index = 0;
        key_index < KEY_MAX;
        key_index++)
    {
        if(keyboard[key_index].is_down)
        {
            if(!keyboard[key_index].was_down)
            {
                keyboard[key_index].down_tick = 1;
            }
            keyboard[key_index].t_down += dt;
        }
        else if(keyboard[key_index].was_down)
        {
            keyboard[key_index].up_tick = 1;
        }
    }

    PL_ButtonState *buttons = &PL_GetInput()->mouse.buttons[0];

    for(int mouse_btn_index = 0;
        mouse_btn_index < MB_MAX;
        mouse_btn_index++)
    {
        if(buttons[mouse_btn_index].is_down)
        {
            if(!buttons[mouse_btn_index].was_down)
            {
                buttons[mouse_btn_index].down_tick = 1;
            }
            buttons[mouse_btn_index].t_down += dt;
        }
        else if(buttons[mouse_btn_index].was_down)
        {
            buttons[mouse_btn_index].up_tick = 1;
        }
    }

    PL_Gamepad *gamepads = &PL_GetInput()->gamepads[0];

    for(int gamepad_index = 0;
        gamepad_index < GP_MAX_COUNT;
        gamepad_index++)
    {
        if(gamepads[gamepad_index].is_connected)
        {
            for(int gamepad_btn_index = 0;
                gamepad_btn_index < GP_MAX;
                gamepad_btn_index++)
            {
                if(gamepads[gamepad_index].buttons[gamepad_btn_index].is_down)
                {
                    if(!gamepads[gamepad_index].buttons[gamepad_btn_index].was_down)
                    {
                        gamepads[gamepad_index].buttons[gamepad_btn_index].down_tick = 1;
                    }
                    gamepads[gamepad_index].buttons[gamepad_btn_index].t_down += dt;
                }
                else if(gamepads[gamepad_index].buttons[gamepad_btn_index].was_down)
                {
                    gamepads[gamepad_index].buttons[gamepad_btn_index].up_tick = 1;
                }
            }
        }
    }

    for(int gamepad_index = 0;
        gamepad_index < GP_MAX_COUNT;
        gamepad_index++)
    {
        Win32_XINPUT_STATE xpadState; //PL.c 2887
        
        if(XINPUT_API.XInputGetState((u32)gamepad_index, &xpadState) == 0L)
        {
            Win32_XINPUT_GAMEPAD *xpad = &xpadState.gamepad;
            PL_Gamepad *gamepad = &gamepads[gamepad_index];
            
            gamepad->is_connected = 1;
            
            Win32_ProcessXInputButton(xpad, gamepad, 0x1000, GP_A, dt);
            Win32_ProcessXInputButton(xpad, gamepad, 0x2000, GP_B, dt);
            Win32_ProcessXInputButton(xpad, gamepad, 0x4000, GP_X, dt);
            Win32_ProcessXInputButton(xpad, gamepad, 0x8000, GP_Y, dt);
            Win32_ProcessXInputButton(xpad, gamepad, 0x0010, GP_START, dt);
            Win32_ProcessXInputButton(xpad, gamepad, 0x0020, GP_BACK, dt);
            Win32_ProcessXInputButton(xpad, gamepad, 0x0100, GP_LSHOULDER, dt);
            Win32_ProcessXInputButton(xpad, gamepad, 0x0200, GP_RSHOULDER, dt);
            Win32_ProcessXInputButton(xpad, gamepad, 0x0001, GP_UP, dt);
            Win32_ProcessXInputButton(xpad, gamepad, 0x0002, GP_DOWN, dt);
            Win32_ProcessXInputButton(xpad, gamepad, 0x0004, GP_LEFT, dt);
            Win32_ProcessXInputButton(xpad, gamepad, 0x0008, GP_RIGHT, dt);
            Win32_ProcessXInputButton(xpad, gamepad, 0x0040, GP_LSTICK, dt);
            Win32_ProcessXInputButton(xpad, gamepad, 0x0080, GP_RSTICK, dt);
            
            gamepad->left_trigger = ((r32)xpad->lt / 255.0f);
            gamepad->right_trigger = ((r32)xpad->rt / 255.0f);
            
            if(xpad->lx < 0) gamepad->left_stick.x = norm32(xpad->lx, 0, 32768);
            else gamepad->left_stick.x = norm32(xpad->lx, 0, 32767);
            if(xpad->ly < 0) gamepad->left_stick.y = norm32(xpad->ly, 0, 32768);
            else gamepad->left_stick.y = norm32(xpad->ly, 0, 32767);
            if(xpad->rx < 0) gamepad->right_stick.x = norm32(xpad->rx, 0, 32768);
            else gamepad->right_stick.x = norm32(xpad->rx, 0, 32767);
            if(xpad->ry < 0) gamepad->right_stick.y = norm32(xpad->ry, 0, 32768);
            else gamepad->right_stick.y = norm32(xpad->ry, 0, 32767);
            
            if(gamepad->left_vib > 1.0f) gamepad->left_vib = 1.0f;
            else if(gamepad->left_vib < 0.0f) gamepad->left_vib = 0.0f;
            if(gamepad->right_vib > 1.0f) gamepad->right_vib = 1.0f;
            else if(gamepad->right_vib < 0.0f) gamepad->right_vib = 0.0f;
            
            Win32_XINPUT_VIBRATION vib;
            vib.left = (u16)(65535.0f * gamepad->left_vib);
            vib.right = (u16)(65535.0f * gamepad->right_vib);
            XINPUT_API.XInputSetState(gamepad_index, &vib);
        }
        
        else
        {
            if(gamepads[gamepad_index].is_connected)
            {
                memset(&gamepads[gamepad_index], 0, sizeof(PL_Gamepad));
            }
        }
    }
}

void Win32_UpdateInput(void)
{
    PL_ButtonState *keyboard = &PL_GetInput()->keyboard[0];

    for(int keyIndex = 0;
        keyIndex < KEY_MAX;
        keyIndex++)
    {
        if(keyboard[keyIndex].is_down)
        {
            if(!keyboard[keyIndex].was_down)
            {
                keyboard[keyIndex].was_down = 1;
            }
            
            keyboard[keyIndex].down_tick = 0;
        }
        else if(keyboard[keyIndex].was_down)
        {
            keyboard[keyIndex].was_down = 0;
        }
        else
        {
            keyboard[keyIndex].t_down = 0.0f;
        }
        
        keyboard[keyIndex].down_tick = 0;
        keyboard[keyIndex].up_tick = 0;
    }
    
    PL_ButtonState *buttons = &PL_GetMouse()->buttons[0];

    for(int mouseBtnIndex = 0;
        mouseBtnIndex < MB_MAX;
        mouseBtnIndex++)
    {
        if(buttons[mouseBtnIndex].is_down)
        {
            if(!buttons[mouseBtnIndex].was_down)
            {
                buttons[mouseBtnIndex].was_down = 1;
            }
            
            buttons[mouseBtnIndex].down_tick = 0;
        }
        else if(buttons[mouseBtnIndex].was_down)
        {
            buttons[mouseBtnIndex].was_down = 0;
        }
        else
        {
            buttons[mouseBtnIndex].t_down = 0.0f;
        }
        
        buttons[mouseBtnIndex].down_tick = 0;
        buttons[mouseBtnIndex].up_tick = 0;
    }
    
    PL_GetMouse()->wheel = 0;
    
    PL_Gamepad *gamepads = &PL_GetInput()->gamepads[0];

    for(int gamepadIndex = 0;
        gamepadIndex < GP_MAX_COUNT;
        gamepadIndex++)
    {
        if(gamepads[gamepadIndex].is_connected)
        {
            for(int gamepadBtnIndex = 0;
                gamepadBtnIndex < GP_MAX;
                gamepadBtnIndex++)
            {
                if(gamepads[gamepadIndex].buttons[gamepadBtnIndex].is_down)
                {
                    if(!gamepads[gamepadIndex].buttons[gamepadBtnIndex].was_down)
                    {
                        gamepads[gamepadIndex].buttons[gamepadBtnIndex].was_down = 1;
                    }
                    
                    gamepads[gamepadIndex].buttons[gamepadBtnIndex].down_tick = 0;
                }
                else if(gamepads[gamepadIndex].buttons[gamepadBtnIndex].was_down)
                {
                    gamepads[gamepadIndex].buttons[gamepadBtnIndex].was_down = 0;
                }
                else
                {
                    gamepads[gamepadIndex].buttons[gamepadBtnIndex].t_down = 0.0f;
                }
                
                gamepads[gamepadIndex].buttons[gamepadBtnIndex].down_tick = 0;
                gamepads[gamepadIndex].buttons[gamepadBtnIndex].up_tick = 0;
            }
        }
    }
}