/*============================================================
 * Project: Ascencia
 * File: win32_keybinds.c
 * Author: Phraggers
 * Date: Mon Sep 23 2024
============================================================*/

#include <util/string_helpers.h>
#include <platform/keybinds.h>
#include <platform/alloc.h>
#include <platform/config.h>
#include <win32/win32_state.h>

#define KEYBINDS_FILENAME "keybinds.asccfg"

local void KeybindsInit(void)
{
    PL_SetKeybind(&PL_GetKeybindings()->Quit, PL_KeybindNew(KEY_F4, KBKS_ONDOWN, KBMOD_LALT));
    PL_SetKeybind(&PL_GetKeybindings()->Fullscreen1, PL_KeybindNew(KEY_F11, KBKS_ONDOWN, 0));
    PL_SetKeybind(&PL_GetKeybindings()->Fullscreen2, PL_KeybindNew(KEY_RETURN, KBKS_ONDOWN, KBMOD_ALT));
}

bool PL_KeybindsInit(cstr keybinds_path)
{
    Assert(G_win32_state != 0);

    KeybindsInit();

    if(PL_GetKeybindsPath() != 0)
    {
        PL_Log(LOG_WARN, "KeybindsInit: keybinds path already set");
        return 0;
    }

    if(!PL_SetKeybindsPath(keybinds_path))
    {
        PL_Log(LOG_ERROR, "KeybindsInit: failed to set keybinds path");
        return 0;
    }

    PL_Log(LOG_DEBUG, "KeybindsInit success");
    return 1;
}

bool PL_KeybindsSave(void)
{
    if((!G_win32_state) || 
       (!strlen(G_win32_state->keybinds_path)))
    {
        PL_Log(LOG_DEBUG, "KeybindsSave: invalid parameters");
        return 0;
    }

    FILE *keybinds_file = fopen(PL_GetKeybindsPath(), "wb");
    if(keybinds_file)
    {
        ptr keybinds_data = PL_Alloc0(sizeof(PL_Version) + sizeof(PL_Keybindings));
        if(!keybinds_data)
        {
            PL_Log(LOG_FATAL, "KeybindsSave: malloc error");
            return 0;
        }

        {
            PL_Version *version_ptr = 0;
            PL_Keybindings *keybinds_ptr = 0;
            u8 *bp = (u8*)keybinds_data;
            bp += sizeof(PL_Version);
            version_ptr = (PL_Version*)keybinds_data;
            keybinds_ptr = (PL_Keybindings*)bp;
            memcpy(version_ptr, &G_win32_state->config.ascencia_version, sizeof(PL_Version));
            memcpy(keybinds_ptr, &G_win32_state->keybinds, sizeof(PL_Keybindings));
        }

        u64 bytes_written = fwrite(keybinds_data, 1, sizeof(PL_Version) + sizeof(PL_Keybindings), keybinds_file);
        fclose(keybinds_file);

        if((sizeof(PL_Version) + sizeof(PL_Keybindings)) != bytes_written)
        {
            cstr size1 = PL_String_New();
            PL_String_ShortFileSize(size1, sizeof(PL_Version) + sizeof(PL_Keybindings));

            cstr size2 = PL_String_New();
            PL_String_ShortFileSize(size2, bytes_written);

            PL_Log(LOG_ERROR, "KeybindsSave: bytes written mis-match writing %s (expected %s : wrote %s)", PL_GetKeybindsPath(), size1, size2);
            PL_Free(size1);
            PL_Free(size2);
            return 0;
        }
        else
        {
            cstr size = PL_String_New();
            PL_String_ShortFileSize(size, bytes_written);
            PL_Log(LOG_INFO, "KeybindsSave: saved %s (%s)", PL_GetKeybindsPath(), size);
            PL_Free(size);
            return 1;
        }
    }
    else
    {
        PL_Log(LOG_ERROR, "KeybindsSave: failed to open keybinds file for saving: %s", PL_GetKeybindsPath());
        return 0;
    }
}

bool PL_KeybindsLoad(void)
{
    if((!G_win32_state) || 
       (!strlen(G_win32_state->keybinds_path)))
    {
        PL_Log(LOG_DEBUG, "KeybindsLoad: invalid parameters");
        return 0;
    }

    FILE *keybinds_file = fopen(PL_GetKeybindsPath(), "rb");
    if(keybinds_file)
    {
        fseek(keybinds_file, 0, SEEK_END);
        u64 size = (u64)ftell(keybinds_file);
        fseek(keybinds_file, 0, SEEK_SET);

        if(size != (sizeof(PL_Version) + sizeof(PL_Keybindings)))
        {
            cstr size1 = PL_String_New();
            PL_String_ShortFileSize(size1, sizeof(PL_Version) + sizeof(PL_Keybindings));

            cstr size2 = PL_String_New();
            PL_String_ShortFileSize(size2, size);

            PL_Log(LOG_ERROR, "KeybindsLoad: keybinds file size mis-match: %s (expected %s, read %s)", PL_GetKeybindsPath(), size1, size2);

            PL_Free(size1);
            PL_Free(size2);
            fclose(keybinds_file);
            return 0;
        }

        ptr loaded_keybinds_data = PL_Alloc0(sizeof(PL_Version) + sizeof(PL_Keybindings));
        if(!loaded_keybinds_data)
        {
            fclose(keybinds_file);
            PL_Log(LOG_FATAL, "KeybindsLoad: malloc error");
            return 0;
        }

        u64 bytes_read = fread((ptr)loaded_keybinds_data, 1, size, keybinds_file);
        fclose(keybinds_file);

        if(bytes_read != (sizeof(PL_Version) + sizeof(PL_Keybindings)))
        {
            PL_Free(loaded_keybinds_data);

            cstr size1 = PL_String_New();
            PL_String_ShortFileSize(size1, bytes_read);

            cstr size2 = PL_String_New();
            PL_String_ShortFileSize(size2, sizeof(PL_Keybindings));

            PL_Log(LOG_ERROR, "KeybindsLoad: failed to read keybinds file: %s (read %s, expected %s)", PL_GetKeybindsPath(), size1, size2);
            PL_Free(size1);
            PL_Free(size2);

            return 0;
        }

        PL_Version* loaded_version = (PL_Version*)loaded_keybinds_data;
        PL_Keybindings *loaded_keybinds = 0;
        {
            u8 *bp = (u8*)loaded_keybinds_data;
            bp += sizeof(PL_Version);
            loaded_keybinds = (PL_Keybindings*)bp;
        }

        u32 *current_version = (u32*)(&G_win32_state->config.ascencia_version);
        u32 *read_version = (u32*)(&loaded_version);
        if(*current_version != *read_version)
        {
            PL_Free(loaded_keybinds_data);
            PL_Log(LOG_ERROR, "KeybindsLoad: keybinds version mis-match");
            return 0;
        }

        memcpy((ptr)(&G_win32_state->keybinds), (ptr)(loaded_keybinds), sizeof(PL_Keybindings));
        PL_Free(loaded_keybinds);

        cstr sizestr = PL_String_New();
        PL_String_ShortFileSize(sizestr, sizeof(PL_Keybindings));
        PL_Log(LOG_INFO, "KeybindsLoad: loaded keybinds: %s (%s)", PL_GetKeybindsPath(), sizestr);
        PL_Free(sizestr);
        return 1;
    }
    else
    {
        PL_Log(LOG_ERROR, "KeybindsSave: failed to open keybinds file for reading: %s", PL_GetKeybindsPath());
        return 0;
    }
}

cstr PL_GetKeybindsPath(void)
{
    if((!G_win32_state) ||
       (strlen(G_win32_state->keybinds_path) == 0))
    {
        return 0;
    }

    return G_win32_state->keybinds_path;
}

bool PL_SetKeybindsPath(cstr path)
{
    if((!G_win32_state) || (!path))
    {
        PL_Log(LOG_DEBUG, "SetKeybindsPath: invalid parameters");
        return 0;
    }

    if(strlen(G_win32_state->keybinds_path) > 0)
    {
        PL_Log(LOG_DEBUG, "SetKeybindsPath: existing string found, clearing");
        memset((ptr)G_win32_state->keybinds_path, 0, STRING_LEN);
    }

    if(!WINAPI.PathFileExistsA(path))
    {
        PL_Log(LOG_DEBUG, "SetKeybindsPath: directory doesn't exist");
        b32 result = WINAPI.CreateDirectoryA(path, 0);
        if(result)
        {
            PL_Log(LOG_INFO, "SetKeybindsPath: created config directory: %s", path);
        }
        else
        {
            PL_Log(LOG_ERROR, "SetKeybindsPath: failed to create config directory: %s", path);
            return 0;
        }
    }

    strcpy(G_win32_state->keybinds_path, path);
    if(G_win32_state->keybinds_path[strlen(G_win32_state->keybinds_path)-1] != '\\')
    {
        G_win32_state->keybinds_path[strlen(G_win32_state->keybinds_path)] = '\\';
    }
    strcat(G_win32_state->keybinds_path, KEYBINDS_FILENAME);

    if(!WINAPI.PathFileExistsA(PL_GetKeybindsPath()))
    {
        PL_Log(LOG_DEBUG, "SetKeybindsPath: keybinds file doesn't exist");
        
        if(!PL_KeybindsSave())
        {
            PL_Log(LOG_ERROR, "SetKeybindsPath: failed to save keybinds: %s", PL_GetKeybindsPath());
            return 0;
        }
        else
        {
            PL_Log(LOG_DEBUG, "SetKeybindsPath: keybinds file saved");
            return 1;
        }
    }

    if(!PL_KeybindsLoad())
    {
        PL_Log(LOG_WARN, "SetKeybindsPath: failed to load keybinds: %s", PL_GetKeybindsPath());
        PL_Log(LOG_DEBUG, "SetKeybindsPath: Deleting erroneous keybinds file");
        WINAPI.DeleteFileA(PL_GetKeybindsPath());

        if(!PL_KeybindsSave())
        {
            PL_Log(LOG_ERROR, "SetKeybindsPath: failed to save keybinds file: %s", PL_GetKeybindsPath());
            return 0;
        }
        else
        {
            PL_Log(LOG_DEBUG, "SetKeybindsPath: keybinds file saved");
            return 1;
        }
    }
    else
    {
        PL_Log(LOG_DEBUG, "SetKeybindsPath: keybinds file loaded");
        return 1;
    }
}

PL_Keybindings *PL_GetKeybindings(void)
{
    if(!G_win32_state)
    {
        return 0;
    }

    PL_Keybindings *result = &G_win32_state->keybinds;
    return result;
}

PL_Keybind PL_KeybindNew(PL_KEYCODE key, PL_KBKS state, PL_KBMOD mod)
{
    PL_Log(LOG_DEBUG, "NewKeybind: key[%d] state[%d] mod[%d]", key, state, mod);
    PL_Keybind result = {key, state, mod};
    return result;
}

bool PL_SetKeybind(PL_Keybind *dst, PL_Keybind src)
{
    if(dst)
    {
        PL_Log(LOG_INFO, "SetKeybind: key[%d] state[%d] mod[%d]", src.key, src.state, src.mod);
        memcpy(dst, &src, sizeof(PL_Keybind));
        return 1;
    }
    else
    {
        PL_Log(LOG_DEBUG, "SetKeybind: invalid pointer");
        return 0;
    }
}

bool PL_ProcessKeybind(PL_Keybind *keybind)
{
    bool result = 0;

    if(keybind->state == KBKS_ONDOWN && 
    PL_GetKeyState(keybind->key)->down_tick)
    {
        result = 1;
    }
    else if(keybind->state == KBKS_ONUP && 
    PL_GetKeyState(keybind->key)->up_tick)
    {
        result = 1;
    }
    else if(keybind->state == KBKS_HELD &&
    PL_GetKeyState(keybind->key)->is_down)
    {
        result = 1;
    }

    if(!result) 
    {
        return 0;
    }

    if(keybind->mod == 0)
    {
        return 1;
    }

    u8 lshift, rshift, lctrl, rctrl, lalt, ralt, shift, ctrl, alt;
    lshift = rshift = lctrl = rctrl = lalt = ralt = shift = ctrl = alt = 0;
    if((keybind->mod & KBMOD_LSHIFT) != 0) lshift = 1;
    if((keybind->mod & KBMOD_RSHIFT) != 0) rshift = 1;
    if((keybind->mod & KBMOD_LCTRL) != 0) lctrl = 1;
    if((keybind->mod & KBMOD_RCTRL) != 0) rctrl = 1;
    if((keybind->mod & KBMOD_LALT) != 0) lalt = 1;
    if((keybind->mod & KBMOD_RALT) != 0) ralt = 1;
    if((keybind->mod & KBMOD_SHIFT) != 0) shift = 1;
    if((keybind->mod & KBMOD_CTRL) != 0) ctrl = 1;
    if((keybind->mod & KBMOD_ALT) != 0) alt = 1;

    if(lshift && PL_GetKeyState(KEY_LSHIFT)->is_down) lshift++;
    if(rshift && PL_GetKeyState(KEY_RSHIFT)->is_down) rshift++;
    if(lctrl && PL_GetKeyState(KEY_LCTRL)->is_down) lctrl++;
    if(rctrl && PL_GetKeyState(KEY_RCTRL)->is_down) rctrl++;
    if(lalt && PL_GetKeyState(KEY_LALT)->is_down) lalt++;
    if(ralt && PL_GetKeyState(KEY_RALT)->is_down) ralt++;
    if(shift && (PL_GetKeyState(KEY_LSHIFT)->is_down || PL_GetKeyState(KEY_RSHIFT)->is_down)) shift++;
    if(ctrl && (PL_GetKeyState(KEY_LCTRL)->is_down || PL_GetKeyState(KEY_RCTRL)->is_down)) ctrl++;
    if(alt && (PL_GetKeyState(KEY_LALT)->is_down || PL_GetKeyState(KEY_RALT)->is_down)) alt++;

    if(lshift==1 || rshift==1 || lctrl==1 || rctrl==1 || lalt==1 || ralt==1 || shift==1 || ctrl==1 || alt==1) return 0;
    
    return 1;
}

void PL_ProcessKeybinds(void)
{
    if(PL_ProcessKeybind(&PL_GetKeybindings()->Quit))
    {
        G_win32_state->running = 0;
    }

    if(PL_ProcessKeybind(&PL_GetKeybindings()->Fullscreen1) ||
       PL_ProcessKeybind(&PL_GetKeybindings()->Fullscreen2))
    {
        PL_ToggleWindowFullscreen();
    }
}