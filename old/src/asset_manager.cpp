//====================================
//          ASCENCIA CLIENT
//          asset_manager.cpp
//====================================
//        (c) Phragware 2019
//====================================

#define ASC_ASSETMANAGER_SYSTEMS
#include <systems.h>
#undef ASC_ASSETMANAGER_SYSTEMS

//============
// LoadDat
//============
int AssetManager::LoadDat(std::string _dat)
{
    if(!_dat.length())
    {
        Error.Set(0, "asset_manager/LoadDat", "invalid dat name", _dat);
        return 1;
    }

    std::string datpath = ASC_BASEPATH;
    datpath += "data\\";
    datpath += _dat;
    datpath += ".dat";

    // check file exists
    std::ifstream ifile;
    ifile.open(datpath.c_str(), std::ios::in | std::ios::binary);

    if(!ifile.is_open())
    {
        Error.Set(0, "asset_manager/LoadFile", "Failed to open dat file", datpath);
        return 1;
    }

    // check headtag
    std::string buffer;
    for(int i=0; i<6; i++) buffer.push_back(ifile.get());

    if(buffer != "ASCDAT")
    {
        Error.Set(0, "asset_manager/LoadFile", "datfile invalid headtag", datpath);
        ifile.close();
        return 1;
    }

    // get index
    uint64_t body_start_address = 0;
    ifile.seekg(6, std::ios::beg); // skip headtag
    ifile.read((char *)&body_start_address, sizeof(body_start_address));

    std::vector<std::string> index;
    std::string filename;
    std::string ext;
    uint64_t address;
    uint64_t length;
    uint8_t namelength;
    uint8_t extlength;

    while(ifile.tellg() < body_start_address)
    {
        if(filename.length()>0) filename.clear();
        if(ext.length()>0) ext.clear();
        address = 0;
        length = 0;
        namelength = 0;
        extlength = 0;

        ifile.read((char *)&namelength, sizeof(uint8_t));

        for(int i=0; i<namelength; i++)
        {
            char c;
            ifile.read((char *)&c, sizeof(char));
            filename.push_back(c);
        }

        ifile.read((char *)&extlength, sizeof(uint8_t));

        for(int i=0; i<extlength; i++)
        {
            char c;
            ifile.read((char *)&c, sizeof(char));
            ext.push_back(c);
        }

        ifile.read((char *)&address, sizeof(uint64_t));
        ifile.read((char *)&length, sizeof(uint64_t));

        std::string file_ne;
        file_ne = filename;
        file_ne += ".";
        file_ne += ext;
        index.push_back(file_ne);
    }

    ifile.close();

    for(int i=0; i<index.size(); i++)
    {
        if(LoadFile(_dat, index[i])) return 1;
    }

    return 0;
}

//===========
// LoadFile
//===========
int AssetManager::LoadFile(std::string _dat, std::string _file)
{
    if(!_dat.length())
    {
        std::ostringstream o;
        o << "dat: " << _dat << " file: " << _file;
        Error.Set(0, "asset_manager/LoadFile", "invalid dat name", o.str());
        return 1;
    }

    if(!_file.length())
    {
        std::ostringstream o;
        o << "dat: " << _dat << " file: " << _file;
        Error.Set(0, "asset_manager/LoadFile", "invalid file name", o.str());
        return 1;
    }

    // separate name/ext
    std::string _file_name;
    std::string _file_ext;
    {
        bool read_ext = 0;
        for(int i=0; i<_file.length(); i++)
        {
            if(_file[i]=='.') read_ext = 1;
            else
            {
                if(!read_ext) _file_name.push_back(_file[i]);
                else _file_ext.push_back(_file[i]);
            }
        }
    }

    if(Dats.count(_dat))
    if(Dats[_dat].Exts.count(_file_ext))
    if(Dats[_dat].Exts[_file_ext].Files.count(_file_name)) return 1;

    std::string datpath = ASC_BASEPATH;
    datpath += "data\\";
    datpath += _dat;
    datpath += ".dat";

    // check file exists
    std::ifstream ifile;
    ifile.open(datpath.c_str(), std::ios::in | std::ios::binary);

    if(!ifile.is_open())
    {
        Error.Set(0, "asset_manager/LoadFile", "Failed to open dat", _dat);
        return 1;
    }

    // check headtag
    std::string buffer;
    for(int i=0; i<6; i++) buffer.push_back(ifile.get());

    if(buffer != "ASCDAT")
    {
        Error.Set(0, "asset_manager/LoadFile", "datfile invalid headtag", _dat);
        ifile.close();
        return 1;
    }

    // get index
    uint64_t body_start_address = 0;
    ifile.seekg(6, std::ios::beg); // skip headtag
    ifile.read((char *)&body_start_address, sizeof(body_start_address));

    uint8_t namelength;
    std::string filename;
    uint8_t extlength;
    std::string ext;
    uint64_t address;
    uint64_t length;
    bool file_found = 0;

    while(ifile.tellg() < body_start_address)
    {
        if(filename.length()>0) filename.clear();
        if(ext.length()>0) ext.clear();
        address = 0;
        length = 0;
        namelength = 0;
        extlength = 0;

        ifile.read((char *)&namelength, sizeof(uint8_t));

        for(int i=0; i<namelength; i++)
        {
            char c;
            ifile.read((char *)&c, sizeof(char));
            filename.push_back(c);
        }

        ifile.read((char *)&extlength, sizeof(uint8_t));

        for(int i=0; i<extlength; i++)
        {
            char c;
            ifile.read((char *)&c, sizeof(char));
            ext.push_back(c);
        }

        ifile.read((char *)&address, sizeof(uint64_t));
        ifile.read((char *)&length, sizeof(uint64_t));

        if(filename == _file_name && ext == _file_ext)
        {
            file_found = 1;
            break;
        }
    }

    if(!file_found)
    {
        std::ostringstream o;
        o << "dat: " << _dat << " file: " << _file;
        Error.Set(0, "asset_manager/LoadFile", "file not found in dat", o.str());
        ifile.close();
        return 1;
    }

    // load file into Dats
    ifile.seekg(0, std::ios::end);
    uint64_t eof = ifile.tellg();
    ifile.seekg(address, std::ios::beg);
    uint64_t fp = ifile.tellg();

    while(fp <= (address+length) || fp <= eof)
    {
        char c;
        ifile.get(c);
        Dats[_dat].Exts[ext].Files[filename].push_back(c);
        fp = ifile.tellg();
    }

    ifile.close();

    return 0;
}

//================================
//    UnloadDat
//================================
void AssetManager::UnloadDat(std::string _dat)
{
    if(!_dat.length())
    {
        Error.Set(0, "asset_manager/UnloadDat", "invalid dat name", _dat);
        return;
    }

    if(!Dats.size()) return;
    if(!Dats.count(_dat)) return;

    Dats.erase(_dat);
}

//================================
//    UnloadFile
//================================
void AssetManager::UnloadFile(std::string _dat, std::string _file)
{
    if(!_dat.length())
    {
        std::ostringstream o;
        o << "dat: " << _dat << " file: " << _file;
        Error.Set(0, "asset_manager/UnloadFile", "invalid dat name", o.str());
        return;
    }

    if(!_file.length())
    {
        std::ostringstream o;
        o << "dat: " << _dat << " file: " << _file;
        Error.Set(0, "asset_manager/UnloadFile", "invalid file name", o.str());
        return;
    }

    // separate name/ext
    std::string _file_name;
    std::string _file_ext;
    {
        bool read_ext = 0;
        for(int i=0; i<_file.length(); i++)
        {
            if(_file[i]=='.') read_ext = 1;
            else
            {
                if(!read_ext) _file_name.push_back(_file[i]);
                else _file_ext.push_back(_file[i]);
            }
        }
    }

    if(!Dats.size()) return;
    if(!Dats.count(_dat)) return;
    if(!Dats[_dat].Exts.size()) return;
    if(!Dats[_dat].Exts[_file_ext].Files.size()) return;
    if(!Dats[_dat].Exts[_file_ext].Files.count(_file_name)) return;

    Dats[_dat].Exts[_file_ext].Files.erase(_file_name);
}