// Ascencia
// platform/config.h

#pragma once

// DEFAULTS

// AppState
#define CFG_APP_NAME "Ascencia"
#define CFG_APP_ORG "Phragware"
#define CFG_APP_VER_DEVSTAGE DEVSTAGE_PREALPHA
#define CFG_APP_VER_MAJ 0
#define CFG_APP_VER_MIN 1

// Log
#define CFG_CONSOLELOGLEVEL LOGLEVEL_DEBUG
#if(BUILD_DEBUG==1)
#define CFG_LOGFILELOGLEVEL LOGLEVEL_INFO
#elif(BUILD_RELEASE==1)
#define CFG_LOGFILELOGLEVEL LOGLEVEL_WARNING
#elif(BUILD_SHIP==1)
#define CFG_LOGFILELOGLEVEL LOGLEVEL_ERROR
#endif

// WindowState
#define CFG_WINDOW_GL_VERSION_MAJ 4
#define CFG_WINDOW_GL_VERSION_MIN 5
#define CFG_WINDOW_X SDL_WINDOWPOS_CENTERED
#define CFG_WINDOW_Y SDL_WINDOWPOS_CENTERED
#define CFG_WINDOW_W 960
#define CFG_WINDOW_H 540
#define CFG_WINDOW_FULLSCREEN 0
#define CFG_WINDOW_VSYNC 1
#define CFG_WINDOW_UPDATEHZ 60

enum eVersionDevStage
{
	DEVSTAGE_NONE = 0,
	DEVSTAGE_PREALPHA,
	DEVSTAGE_ALPHA,
	DEVSTAGE_BETA,
	DEVSTAGE_PRERELEASE,
	DEVSTAGE_RELEASE,
	DEVSTAGE_MAX
};

struct sAppVersion
{
	eVersionDevStage DevStage;
	int Major;
	int Minor;
};

struct sUserConfig
{
	sAppVersion Version;
	sWindowState WindowState;
	sLogState LogState;
};

class cConfig
{
	public:
	std::string BasePath;
	std::string SavePath;
	std::string ConfigFilePath;
	bool IsConfigNew;

	cConfig();
	bool Init(void);
	bool Save(void);
	bool Load(void);
};

std::string CFG_DevStageToString(eVersionDevStage stage);