// Ascencia
// ascencia/platform/config.cpp

#include <ascencia/platform/application.h>

bool sConfig::Init()
{
	const char* base_path = SDL_GetBasePath();
	BasePath = std::string(base_path);

	SavePath = std::string(base_path);
	SavePath.append("Save");
	std::filesystem::create_directory(SavePath);

	ConfigFilePath = std::string(SavePath);
	if(base_path[strlen(base_path)] == '/')	ConfigFilePath.append("/Config.bin");
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

bool sConfig::Save()
{
	sUserConfig* UserConfig = new sUserConfig;
	memset(UserConfig, 0, sizeof(sUserConfig));
	memcpy(&UserConfig->Version, &App->State.Version, sizeof(sAppVersion));
	memcpy(&UserConfig->WindowState, &App->Window.State, sizeof(sWindowState));

	FILE* file = 0;
	file = fopen(ConfigFilePath.c_str(), "wb");
	if (!file)
	{
		std::cerr << "Config: Failed to open " << ConfigFilePath << " for writing" << std::endl;
		delete UserConfig;
		return 0;
	}
	else
	{
		std::cout << "Config: Writing Config to " << ConfigFilePath << " ... " << std::endl;
	}
	size_t bytes = fwrite(UserConfig, 1, sizeof(sUserConfig), file);
	fclose(file);

	if (bytes != sizeof(sUserConfig))
	{
		std::cerr << "Config: Error writing Config : expected " << sizeof(sUserConfig) << " bytes, actual: "
			<< bytes << " bytes written" << std::endl;
		delete UserConfig;
		return 0;
	}
	else
	{
		std::cout << "Config: Wrote " << bytes << " bytes" << std::endl;
		delete UserConfig;
		return 1;
	}
}

bool sConfig::Load()
{
	sUserConfig* UserConfig = new sUserConfig;
	memset(UserConfig, 0, sizeof(sUserConfig));

	FILE* file = 0;
	file = fopen(ConfigFilePath.c_str(), "rb");
	if (!file)
	{
		std::cerr << "Config: Failed to open " << ConfigFilePath << " for reading" << std::endl;
		delete UserConfig;
		return 0;
	}
	else
	{
		std::cout << "Config: Reading Config from " << ConfigFilePath << " ... " << std::endl;
	}

	size_t bytes = fread(UserConfig, 1, sizeof(sUserConfig), file);
	fclose(file);

	if (bytes != sizeof(sUserConfig))
	{
		std::cerr << "Config: Error reading Config : expected " << sizeof(sUserConfig) << " bytes, actual: "
			<< bytes << " bytes read" << std::endl;
		delete UserConfig;
		return 0;
	}

	//TODO handle version mismatch

	memcpy(&App->Window.State, &UserConfig->WindowState, sizeof(sWindowState));

	std::cout << "Config: Loaded Config (" << bytes << " bytes)" << std::endl;
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
