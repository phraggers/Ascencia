//====================================
//          ASCENCIA CLIENT
//          asset_manager.h
//====================================
//        (c) Phragware 2019
//====================================

#ifndef ASC_ASSETMANAGER_H_
#define ASC_ASSETMANAGER_H_

#include <headers.h>

struct AMFile
{
    // name, data
    std::map<std::string, std::string> Files;
};

struct AMDat
{
    // extension, file[name] = data
    std::map<std::string, AMFile> Exts;
};

struct AssetManager
{
    int LoadDat(std::string _dat);
    int LoadFile(std::string _dat, std::string _file);
    void UnloadDat(std::string _dat);
    void UnloadFile(std::string _dat, std::string _file);

    // Assets.Dats[datname].Exts[ext].Files[filename] = data string
    std::map<std::string, AMDat> Dats;
};

#endif // ASC_ASSETMANAGER_H_