/*============================================================
 * Project: Ascencia
 * File: config.h
 * Author: Phraggers
 * Date: Sun May 12 2024
============================================================*/

#ifdef ASC_HEAD

/* Global Defines */
#define AUDIO_SFX_BUFFERS 0xff

/* Non-Configurables */
#define CONFIG_APP_ORG "Phragware"
#define CONFIG_APP_NAME "Ascencia"
#define CONFIG_APP_VER_MAJ 0
#define CONFIG_APP_VER_MIN 1
#define CONFIG_APP_VER_REV 2
#define CONFIG_GL_VER_MAJ 4
#define CONFIG_GL_VER_MIN 5

/* Config File Default Settings */
#define CONFIG_DEFAULT_WINDOW_WIDTH 960
#define CONFIG_DEFAULT_WINDOW_HEIGHT 540
#define CONFIG_DEFAULT_FULLSCREEN 0
#define CONFIG_DEFAULT_VSYNC 1
#define CONFIG_DEFAULT_FPS 60
#define CONFIG_DEFAULT_GL_MULTISAMPLING 4
#define CONFIG_DEFAULT_SERVER_IP_A 127
#define CONFIG_DEFAULT_SERVER_IP_B 0
#define CONFIG_DEFAULT_SERVER_IP_C 0
#define CONFIG_DEFAULT_SERVER_IP_D 1
#define CONFIG_DEFAULT_SERVER_PORT 32000

/* Keybind Modifiers (not configurable) */
#define ASC_KEYBIND_MOD_LSHIFT (1<<0)
#define ASC_KEYBIND_MOD_LCTRL (1<<1)
#define ASC_KEYBIND_MOD_LALT (1<<2)
#define ASC_KEYBIND_MOD_RSHIFT (1<<3)
#define ASC_KEYBIND_MOD_RCTRL (1<<4)
#define ASC_KEYBIND_MOD_RALT (1<<5)
#define ASC_KEYBIND_MOD_SHIFT (1<<6) // either shift
#define ASC_KEYBIND_MOD_CTRL (1<<7) // either ctrl
#define ASC_KEYBIND_MOD_ALT (1<<8) // either alt

typedef enum
{
    ASC_KEYBIND_DOWNTICK,
    ASC_KEYBIND_UPTICK,
    ASC_KEYBIND_ISDOWN
} ASC_KEYBIND_KEYSTATE;

typedef struct
{
    SDL_Scancode key;
    ASC_KEYBIND_KEYSTATE state;
    u16 modifiers;
} ASC_Keybind;

typedef struct
{
    ASC_Keybind AppQuit;
    ASC_Keybind AppFullscreen[2];
} ASC_Keybinds;

typedef struct
{
    char app_org[32];
    char app_name[32];
    i32 app_ver[3];
    i32 gl_ver[2];

    // Config File Variables
    i32 window_width;
    i32 window_height;
    b8 fullscreen;
    b8 vsync;
    i32 fps;
    i32 gl_multisampling;
    u8 server_ip[4];
    u16 server_port;

    ASC_Keybinds keybinds;

    char base_path[256];
    char pref_path[256];
} ASC_Config;

#define GCONFIGPATH_LEN 512
static cstr gConfigPath = 0;
static cstr gKeybindsPath = 0;

static bool ASC_ConfigInit(int argc, char **argv);
static void ASC_ConfigQuit(void);
static void ASC_ConfigKeybindsInit(void);
static bool ASC_LoadConfigFile(bool defFullscreen, bool defVsync);
static bool ASC_SaveConfigFile(void);
static bool ASC_LoadKeybindsFile(void);
static bool ASC_SaveKeybindsFile(void);

#endif

/*=============================================================================*/

#ifdef ASC_IMPL

static bool ASC_ConfigInit(int argc, char **argv)
{
    bool noPref = 0;
    bool resetConfig = 0;
    bool resetKeybinds = 0;
    bool noFullscreen = 0;
    bool fullscreen = 0;
    bool vsyncOn = 0;
    bool vsyncOff = 0;

    for(int argIndex = 0; argIndex < argc; argIndex++)
    {
        cstr arg = (cstr)malloc(strlen(argv[argIndex]));
        for(int i=0;i<strlen(argv[argIndex]);i++)
        {
            if(argIndex > 0) arg[i] = (char)tolower(argv[argIndex][i]);
            else arg[i] = argv[argIndex][i];
        }
        free(arg);
        
        ASC_DebugLog("arg[%d] : %s", argIndex, arg);

        if(argIndex > 0)
        {
            if(strstr(arg, "-nopref")) noPref = 1;
            if(strstr(arg, "-resetconfig")) resetConfig = 1;
            if(strstr(arg, "-resetkeybinds")) resetKeybinds = 1;
            if(strstr(arg, "-nofullscreen")) noFullscreen = 1;
            if(strstr(arg, "-fullscreen")) fullscreen = 1;
            if(strstr(arg, "-vsyncon")) vsyncOn = 1;
            if(strstr(arg, "-vsyncoff")) vsyncOff = 1;
        }
    }

    ASC_Config *config = &gApp->config;

    memcpy(config->app_org, CONFIG_APP_ORG, 32);
    memcpy(config->app_name, CONFIG_APP_NAME, 32);
    config->app_ver[0] = CONFIG_APP_VER_MAJ;
    config->app_ver[1] = CONFIG_APP_VER_MIN;
    config->app_ver[2] = CONFIG_APP_VER_REV;
    config->gl_ver[0] = CONFIG_GL_VER_MAJ;
    config->gl_ver[1] = CONFIG_GL_VER_MIN;

    config->window_width = CONFIG_DEFAULT_WINDOW_WIDTH;
    config->window_height = CONFIG_DEFAULT_WINDOW_HEIGHT;
    config->fullscreen = CONFIG_DEFAULT_FULLSCREEN;
    config->vsync = CONFIG_DEFAULT_VSYNC;
    config->fps = CONFIG_DEFAULT_FPS;
    config->gl_multisampling = CONFIG_DEFAULT_GL_MULTISAMPLING;
    config->server_ip[0] = CONFIG_DEFAULT_SERVER_IP_A;
    config->server_ip[1] = CONFIG_DEFAULT_SERVER_IP_B;
    config->server_ip[2] = CONFIG_DEFAULT_SERVER_IP_C;
    config->server_ip[3] = CONFIG_DEFAULT_SERVER_IP_D;
    config->server_port = CONFIG_DEFAULT_SERVER_PORT;

    ASC_ConfigKeybindsInit();

    if(noFullscreen)
    {
        ASC_InfoLog("-nofullscreen : forcing no fullscreen");
        config->fullscreen = 0;
    }

    if(fullscreen)
    {
        ASC_InfoLog("-fullscreen : forcing fullscreen");
        config->fullscreen = 1;
    }

    if(vsyncOn)
    {
        ASC_InfoLog("-vsyncon : forcing vsync on");
        config->vsync = 1;
    }

    if(vsyncOff)
    {
        ASC_InfoLog("-vsyncoff : forcing vsync off");
        config->vsync = 0;
    }

    cstr basepath = SDL_GetBasePath();
    memcpy(config->base_path, basepath, strlen(basepath));
    SDL_free(basepath);

    if(noPref)
    {
        ASC_InfoLog("-nopref : Setting PrefPath to %s", config->base_path);
        memcpy(config->pref_path, config->base_path, strlen(config->base_path));
    }
    else
    {
        cstr prefpath = SDL_GetPrefPath(CONFIG_APP_ORG, CONFIG_APP_NAME);
        memcpy(config->pref_path, prefpath, strlen(prefpath));
        SDL_free(prefpath);
    }

    gConfigPath = (cstr)malloc(GCONFIGPATH_LEN);
    memset(gConfigPath, 0, GCONFIGPATH_LEN);
    strcpy(gConfigPath, config->pref_path);
    strcat(gConfigPath, "config.bin");

    gKeybindsPath = (cstr)malloc(GCONFIGPATH_LEN);
    memset(gKeybindsPath, 0, GCONFIGPATH_LEN);
    strcpy(gKeybindsPath, config->pref_path);
    strcat(gKeybindsPath, "keybinds.bin");

    if(resetConfig)
    {
        ASC_InfoLog("-resetconfig : Resetting config...");
        remove(gConfigPath);
        return ASC_SaveConfigFile();
    }

    if(resetKeybinds)
    {
        ASC_InfoLog("-resetkeybinds : Resetting keybinds...");
        remove(gKeybindsPath);
        return ASC_SaveKeybindsFile();
    }

    bool result = ASC_LoadConfigFile((fullscreen||noFullscreen), (vsyncOn||vsyncOff));
    result = ASC_LoadKeybindsFile() && result;

    return result;
}

static void ASC_ConfigQuit(void)
{
    ASC_SaveConfigFile();
}

static void ASC_ConfigKeybindsInit(void)
{
    ASC_Config *config = &gApp->config;
    config->keybinds.AppQuit = {SDL_SCANCODE_F4, ASC_KEYBIND_DOWNTICK, ASC_KEYBIND_MOD_ALT};
    config->keybinds.AppFullscreen[0] = {SDL_SCANCODE_F11, ASC_KEYBIND_DOWNTICK, 0};
    config->keybinds.AppFullscreen[1] = {SDL_SCANCODE_RETURN, ASC_KEYBIND_DOWNTICK, ASC_KEYBIND_MOD_ALT};
}

static bool ASC_LoadConfigFile(bool defFullscreen, bool defVsync)
{
    ASC_Config *config = &gApp->config;

    if(!gConfigPath)
    {
        ASC_Error("ASC_LoadConfigFile: Config path null");
        return 0;
    }

    FILE *configFile = 0;
    configFile = fopen(gConfigPath, "rb");

    if(!configFile)
    {
        ASC_InfoLog("ASC_LoadConfigFile: Config file does not exist, creating new");
        return ASC_SaveConfigFile();
    }

    { // Read Header
        char headTag[6] = {0};
        fread(headTag, 1, 6, configFile);
        if(strcmp(headTag, "ASCCFG") != 0)
        {
            ASC_Error("ASC_LoadConfigFile: Config file header invalid, overwriting with new");
            fclose(configFile);
            return ASC_SaveConfigFile();
        }

        i32 headVer[3] = {0,0,0};
        fread(headVer, sizeof(i32), 3, configFile);
        if(headVer[0] != config->app_ver[0] || headVer[1] != config->app_ver[1] || headVer[2] != config->app_ver[2])
        {
            ASC_Error("ASC_LoadConfigFile: Config file version mis-match, config version: %d.%d.%d", headVer[0], headVer[1], headVer[2]);
            //TODO decide what to do here. probably do check on major ver, check if ver is later, write code to update config if earlier version, etc.
            // for now, just remake it.
            fclose(configFile);
            remove(gConfigPath);
            return ASC_SaveConfigFile();
        }
    }

    fread(&config->window_width, sizeof(i32), 1, configFile);
    fread(&config->window_height, sizeof(i32), 1, configFile);
    if(!defFullscreen) fread(&config->fullscreen, sizeof(b8), 1, configFile);
    else fseek(configFile, sizeof(b8), SEEK_CUR);
    if(!defVsync) fread(&config->vsync, sizeof(b8), 1, configFile);
    else fseek(configFile, sizeof(b8), SEEK_CUR);
    fread(&config->fps, sizeof(i32), 1, configFile);
    fread(&config->gl_multisampling, sizeof(i32), 1, configFile);
    fread(&config->server_ip, sizeof(u8), 4, configFile);
    fread(&config->server_port, sizeof(u16), 1, configFile);
    
    fclose(configFile);

    ASC_InfoLog("ASC_LoadConfigFile: Loaded config");

    return 1;
}

static bool ASC_SaveConfigFile(void)
{
    ASC_Config *config = &gApp->config;

    if(!gConfigPath)
    {
        ASC_Error("ASC_SaveConfigFile: Config path null");
        return 0;
    }

    FILE *configFile = 0;
    configFile = fopen(gConfigPath, "wb");

    if(!configFile)
    {
        ASC_Error("ASC_SaveConfigFile: Failed to open Config file for writing: %s", gConfigPath);
        return 0;
    }

    fwrite("ASCCFG", 1, 6, configFile);
    fwrite(&config->app_ver, sizeof(i32), 3, configFile);
    fwrite(&config->window_width, sizeof(i32), 1, configFile);
    fwrite(&config->window_height, sizeof(i32), 1, configFile);
    fwrite(&config->fullscreen, sizeof(b8), 1, configFile);
    fwrite(&config->vsync, sizeof(b8), 1, configFile);
    fwrite(&config->fps, sizeof(i32), 1, configFile);
    fwrite(&config->gl_multisampling, sizeof(i32), 1, configFile);
    fwrite(&config->server_ip, sizeof(u8), 4, configFile);
    fwrite(&config->server_port, sizeof(u16), 1, configFile);
    
    fclose(configFile);

    ASC_InfoLog("ASC_SaveConfigFile: Saved %s", gConfigPath);

    return 1;
}

static bool ASC_LoadKeybindsFile(void)
{
    ASC_Config *config = &gApp->config;

    if(!gKeybindsPath)
    {
        ASC_Error("ASC_LoadKeybindsFile: Keybinds path null");
        return 0;
    }

    FILE *keybindsFile = 0;
    keybindsFile = fopen(gKeybindsPath, "rb");

    if(!keybindsFile)
    {
        ASC_InfoLog("ASC_LoadKeybindsFile: Keybinds file does not exist, creating new");
        return ASC_SaveKeybindsFile();
    }

    { // Read Header
        char headTag[6] = {0};
        fread(headTag, 1, 6, keybindsFile);
        if(strcmp(headTag, "ASCCFG") != 0)
        {
            ASC_Error("ASC_LoadKeybindsFile: Keybinds file header invalid, overwriting with new");
            fclose(keybindsFile);
            return ASC_SaveKeybindsFile();
        }

        i32 headVer[3] = {0,0,0};
        fread(headVer, sizeof(i32), 3, keybindsFile);
        if(headVer[0] != config->app_ver[0] || headVer[1] != config->app_ver[1] || headVer[2] != config->app_ver[2])
        {
            ASC_Error("ASC_LoadKeybindsFile: Keybinds file version mis-match, keybinds version: %d.%d.%d", headVer[0], headVer[1], headVer[2]);
            //TODO decide what to do here. probably do check on major ver, check if ver is later, write code to update config if earlier version, etc.
            // for now, just remake it.
            fclose(keybindsFile);
            remove(gKeybindsPath);
            return ASC_SaveKeybindsFile();
        }
    }

    fread(&config->keybinds, sizeof(ASC_Keybinds), 1, keybindsFile);
    
    fclose(keybindsFile);

    ASC_InfoLog("ASC_LoadKeybindsFile: Loaded keybinds");

    return 1;
}

static bool ASC_SaveKeybindsFile(void)
{
    ASC_Config *config = &gApp->config;

    if(!gKeybindsPath)
    {
        ASC_Error("ASC_SaveKeybindsFile: Keybinds path null");
        return 0;
    }

    FILE *keybindsFile = 0;
    keybindsFile = fopen(gKeybindsPath, "wb");

    if(!keybindsFile)
    {
        ASC_Error("ASC_SaveKeybindsFile: Failed to open Keybinds file for writing: %s", gKeybindsPath);
        return 0;
    }

    fwrite("ASCCFG", 1, 6, keybindsFile);
    fwrite(&config->app_ver, sizeof(i32), 3, keybindsFile);
    fwrite(&config->keybinds, sizeof(ASC_Keybinds), 1, keybindsFile);
    
    fclose(keybindsFile);

    ASC_InfoLog("ASC_SaveKeybindsFile: Saved %s", gKeybindsPath);

    return 1;
}

#endif //CONFIG_H_