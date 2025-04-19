// Ascencia
// platform/config.cpp

#include <ascencia/platform/core.h>

cConfig::cConfig()
{
	IsConfigNew = 0;
}

bool cConfig::Init()
{
	const char* SDLBasePath = SDL_GetBasePath();
	BasePath = std::string(SDLBasePath);

	SavePath = std::string(SDLBasePath);
	SavePath.append("Save");
	std::filesystem::create_directory(SavePath);

	ConfigFilePath = std::string(SavePath);
	if(SDLBasePath[strlen(SDLBasePath)] == '/')	ConfigFilePath.append("/Config.bin");
	else ConfigFilePath.append("\\Config.bin");

	std::filesystem::path ConfigPath = std::filesystem::path(ConfigFilePath);
	if (std::filesystem::exists(ConfigPath))
	{
		IsConfigNew = 0;
		Load();
	}
	else
	{
		IsConfigNew = 1;
		Save();
	}

	return 1;
}

bool cConfig::Save()
{
	sUserConfig* UserConfig = new sUserConfig;
	memset(UserConfig, 0, sizeof(sUserConfig));
	memcpy(&UserConfig->Version, &Core->State.Version, sizeof(sAppVersion));
	memcpy(&UserConfig->LogState, &Core->Log.State, sizeof(sLogState));
	memcpy(&UserConfig->WindowState, &Core->Window.State, sizeof(sWindowState));

	FILE* file = 0;
	file = fopen(ConfigFilePath.c_str(), "wb");
	if (!file)
	{
		std::stringstream ss;
		ss << "Failed to open " << ConfigFilePath << " for writing";
		LOG_ERROR("Config::Save", ss.str());
		delete UserConfig;
		return 0;
	}
	else
	{
		std::stringstream ss;
		ss << "Writing Config to " << ConfigFilePath << " ... ";
		LOG_INFO("Config::Save", ss.str());
	}
	size_t bytes = fwrite(UserConfig, 1, sizeof(sUserConfig), file);
	fclose(file);

	if (bytes != sizeof(sUserConfig))
	{
		std::stringstream ss;
		ss << "Error writing Config : expected " << sizeof(sUserConfig) << " bytes, actual: "
			<< bytes << " bytes written";
		LOG_ERROR("Config::Save", ss.str());
		delete UserConfig;
		return 0;
	}
	else
	{
		std::stringstream ss;
		ss << "Wrote " << bytes << " bytes";
		LOG_INFO("Config::Save", ss.str());
		delete UserConfig;
		return 1;
	}
}

bool cConfig::Load()
{
	sUserConfig* UserConfig = new sUserConfig;
	memset(UserConfig, 0, sizeof(sUserConfig));

	FILE* file = 0;
	file = fopen(ConfigFilePath.c_str(), "rb");
	if (!file)
	{
		std::stringstream ss;
		ss << "Failed to open " << ConfigFilePath << " for reading";
		LOG_ERROR("Config::Load", ss.str());
		delete UserConfig;
		return 0;
	}
	else
	{
		std::stringstream ss;
		ss << "Reading Config from " << ConfigFilePath << " ... ";
		LOG_INFO("Config::Load", ss.str());
	}

	size_t bytes = fread(UserConfig, 1, sizeof(sUserConfig), file);
	fclose(file);

	if (bytes != sizeof(sUserConfig))
	{
		std::stringstream ss;
		ss << "Error reading Config : expected " << sizeof(sUserConfig) << " bytes, actual: "
			<< bytes << " bytes read";
		LOG_ERROR("Config::Load", ss.str());
		delete UserConfig;
		return 0;
	}

	//TODO handle version mismatch

	memcpy(&Core->Log.State, &UserConfig->LogState, sizeof(sLogState));
	memcpy(&Core->Window.State, &UserConfig->WindowState, sizeof(sWindowState));

	std::stringstream ss;
	ss << "Loaded Config (" << bytes << " bytes)";
	LOG_INFO("Config::Load", ss.str());
	delete UserConfig;
	return 1;
}

std::string CFG_DevStageToString(eVersionDevStage stage)
{
	switch (stage)
	{
	case DEVSTAGE_PREALPHA:
	{
		return std::string("pre-alpha");
	} break;
	case DEVSTAGE_ALPHA:
	{
		return std::string("alpha");
	} break;
	case DEVSTAGE_BETA:
	{
		return std::string("beta");
	} break;
	case DEVSTAGE_PRERELEASE:
	{
		return std::string("pre-release");
	} break;
	case DEVSTAGE_RELEASE:
	{
		return std::string("release");
	} break;
	default:
	{
		return std::string(" ");
	} break;
	}
}
