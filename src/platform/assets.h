/*============================================================
 * Project: Ascencia
 * File: assets.h
 * Author: Phraggers
 * Date: Thu May 30 2024
============================================================*/

#ifdef ASC_HEAD

struct ASC_VirtualFileHandle
{
    ptr data;
    u64 size;
    u64 position;
};

struct ASC_AssetHeader
{
    char name[256];
    ASC_VirtualFileHandle handle;
};

struct ASC_Assets
{
    u64 totalAllocated;
    ptr index;
    u32 indexCount;
};

static bool ASC_AssetsInit(void);

/** Load file from .dat from data/ directory. Files loaded will be prefixed with datname/
 * e.g. test.dat containing file.txt, will be named "test.dat/file.txt"
 * @param datName: must match filename as it appears in data/ directory
 * @param fileName: filepath from .dat as root. */
static bool ASC_AssetsLoadDatFile(const cstr datName, const cstr fileName);

/* Load all files from .dat. Files loaded will be prefixed with datname/
 * e.g. test.dat containing file.txt, will be named "test.dat/file.txt" */
static bool ASC_AssetsLoadDat(const cstr datName);

/** Load loose file from data/ directory */
static bool ASC_AssetsLoadFile(const cstr fileName);

/** Frees memblock associated with all files loaded from specific dat. */
static bool ASC_AssetsUnloadDat(const cstr datName);

/** Frees memblock of specific file. Can be used to unload single file even if loaded from dat. */
static bool ASC_AssetsUnloadFile(const cstr fileName);

static ASC_VirtualFileHandle *ASC_GetAsset(const cstr fileName);

#endif

/*=============================================================================================*/

#ifdef ASC_IMPL

static bool ASC_AssetsInit(void)
{
    gApp->assets.index = malloc(sizeof(ASC_AssetHeader));
    if(!gApp->assets.index)
    {
        ASC_Error("ASC_AssetsInit: malloc error");
        return 0;
    }

    memset(gApp->assets.index, 0, sizeof(ASC_AssetHeader));

    return 1;
}

static bool ASC_AssetsLoadDatFile(const cstr datName, const cstr fileName)
{
    if(!datName || !fileName)
    {
        ASC_Error("ASC_AssetsLoadDatFile: invalid parameters");
        return 0;
    }

    struct archive* ar = archive_read_new();
    archive_read_support_filter_all(ar);
    archive_read_support_format_all(ar);

    cstr datPath = (cstr)malloc(512);
    memset(datPath, 0, 512);
    strcpy(datPath, "data/");
    strcat(datPath, datName);

    int ret = archive_read_open_filename(ar, datPath, 10240);
    if(ret != ARCHIVE_OK) 
    {
        ASC_Error("ASC_AssetsLoadDatFile: Failed to open dat: %s", datName);
        archive_read_free(ar);
        free(datPath);
        return 0;
    }

    bool fileFound = 0;

    struct archive_entry* entry;
    while(archive_read_next_header(ar, &entry) == ARCHIVE_OK) 
    {
        if(strcmp(archive_entry_pathname(entry), fileName))
        {
            ASC_DebugLog("ASC_AssetsLoadDatFile: Skipping file (no-match): %s", archive_entry_pathname(entry));
            continue;
        }

        cstr datFileName = (cstr)malloc(512);
        memset(datFileName, 0, 512);
        strcpy(datFileName, datName);
        strcat(datFileName, "/");
        strcat(datFileName, archive_entry_pathname(entry));

        ASC_DebugLog("ASC_AssetsLoadDatFile: Found Dat File: %s", datFileName);

        ASC_AssetHeader *assetHeader = 0;
        assetHeader = (ASC_AssetHeader*)gApp->assets.index;
        bool assetExists = 0;

        for(u32 index = 0; index < gApp->assets.indexCount; index++)
        {
            assetHeader++;
            if(!strcmp(assetHeader->name, datFileName))
            {
                assetExists = 1;
                break;
            }
        }

        if(assetExists)
        {
            ASC_InfoLog("ASC_AssetsLoadDatFile: File already loaded: %s", datFileName);
            free(datFileName);
            break;
        }

        else
        {
            size_t fileSize = archive_entry_size(entry);

            if(fileSize == 0)
            {
                ASC_InfoLog("ASC_AssetsLoadDatFile: File is 0 bytes, skipping: %s", datFileName);
                free(datFileName);
                break;
            }
            
            // Read file contents into memory
            gApp->assets.indexCount++;
            gApp->assets.index = realloc(gApp->assets.index, (sizeof(ASC_AssetHeader) * (gApp->assets.indexCount+1)));
            assetHeader = (ASC_AssetHeader*)gApp->assets.index;
            assetHeader += gApp->assets.indexCount;
            
            memset(assetHeader, 0, sizeof(ASC_AssetHeader));
            strcpy(assetHeader->name, datFileName);
            assetHeader->handle.size = fileSize;
            assetHeader->handle.data = malloc(fileSize);
            assetHeader->handle.position = 0;
            gApp->assets.totalAllocated += assetHeader->handle.size;

            ASC_DebugLog("ASC_AssetsLoadDatFile: New Asset: %s : size %llu bytes", datFileName, fileSize);

            size_t bytesRead;
            while((bytesRead = archive_read_data(ar, assetHeader->handle.data, assetHeader->handle.size)) > 0)
            {
                ASC_InfoLog("ASC_AssetsLoadDatFile: Read New Asset : %s : %llu bytes read", datFileName, bytesRead);
            }

            free(datFileName);
            fileFound = 1;
            break;
        }
    }

    free(datPath);
    archive_read_close(ar);
    archive_read_free(ar);

    if(fileFound) 
    {
        ASC_InfoLog("ASC_Assets: %u assets loaded, %llu bytes.", gApp->assets.indexCount, gApp->assets.totalAllocated);
        return 1;
    }

    ASC_InfoLog("ASC_AssetsLoadDatFile: File was not found: %s", fileName);
    return 0;
}

static bool ASC_AssetsLoadDat(const cstr datName)
{
    if(!datName)
    {
        ASC_Error("ASC_AssetsLoadDat: invalid parameters");
        return 0;
    }

    struct archive* ar = archive_read_new();
    archive_read_support_filter_all(ar);
    archive_read_support_format_all(ar);

    cstr datPath = (cstr)malloc(512);
    memset(datPath, 0, 512);
    strcpy(datPath, "data/");
    strcat(datPath, datName);

    int ret = archive_read_open_filename(ar, datPath, 10240);
    if(ret != ARCHIVE_OK) 
    {
        ASC_Error("ASC_AssetsLoadDat: Failed to open dat: %s", datName);
        archive_read_free(ar);
        free(datPath);
        return 0;
    }

    struct archive_entry* entry;
    while(archive_read_next_header(ar, &entry) == ARCHIVE_OK) 
    {
        cstr datFileName = (cstr)malloc(512);
        memset(datFileName, 0, 512);
        strcpy(datFileName, datName);
        strcat(datFileName, "/");
        strcat(datFileName, archive_entry_pathname(entry));

        ASC_DebugLog("ASC_AssetsLoadDat: Found Dat File: %s", datFileName);

        ASC_AssetHeader *assetHeader = 0;
        assetHeader = (ASC_AssetHeader*)gApp->assets.index;
        bool assetExists = 0;

        for(u32 index = 0; index < gApp->assets.indexCount; index++)
        {
            assetHeader++;
            if(!strcmp(assetHeader->name, datFileName))
            {
                assetExists = 1;
                break;
            }
        }

        if(assetExists)
        {
            ASC_DebugLog("ASC_AssetsLoadDat: File already loaded: %s", datFileName);
            free(datFileName);
            continue;
        }

        else
        {
            size_t fileSize = archive_entry_size(entry);

            if(fileSize == 0)
            {
                ASC_DebugLog("ASC_AssetsLoadDat: File is 0 bytes, skipping: %s", datFileName);
                free(datFileName);
                continue;
            }
            
            // Read file contents into memory
            gApp->assets.indexCount++;
            gApp->assets.index = realloc(gApp->assets.index, (sizeof(ASC_AssetHeader) * (gApp->assets.indexCount+1)));
            assetHeader = (ASC_AssetHeader*)gApp->assets.index;
            assetHeader += gApp->assets.indexCount;
            
            memset(assetHeader, 0, sizeof(ASC_AssetHeader));
            strcpy(assetHeader->name, datFileName);
            assetHeader->handle.size = fileSize;
            assetHeader->handle.data = malloc(fileSize);
            assetHeader->handle.position = 0;
            gApp->assets.totalAllocated += assetHeader->handle.size;

            ASC_DebugLog("ASC_AssetsLoadDat: New Asset: %s : size %llu bytes", datFileName, fileSize);

            size_t bytesRead;
            while((bytesRead = archive_read_data(ar, assetHeader->handle.data, assetHeader->handle.size)) > 0)
            {
                ASC_InfoLog("ASC_AssetsLoadDat: Read New Asset : %s : %llu bytes read", datFileName, bytesRead);
            }

            free(datFileName);
        }
    }

    ASC_InfoLog("ASC_AssetsLoadDat: %s Loaded", datPath);
    free(datPath);
    archive_read_close(ar);
    archive_read_free(ar);

    ASC_InfoLog("ASC_Assets: %u assets loaded, %llu bytes.", gApp->assets.indexCount, gApp->assets.totalAllocated);
    return 1;
}

static bool ASC_AssetsLoadFile(const cstr fileName)
{
    if(!fileName)
    {
        ASC_Error("ASC_AssetsLoadFile: Invalid parameters");
        return 0;
    }

    ASC_AssetHeader *assetHeader = 0;
    assetHeader = (ASC_AssetHeader*)gApp->assets.index;

    for(u32 index = 0; index < gApp->assets.indexCount; index++)
    {
        assetHeader++;
        if(!strcmp(assetHeader->name, fileName))
        {
            ASC_InfoLog("ASC_AssetsLoadFile: Asset already loaded: %s", fileName);
            return 0;
        }
    }

    cstr filePath = (cstr)malloc(512);
    memset(filePath, 0, 512);
    strcpy(filePath, "data/");
    strcat(filePath, fileName);
    FILE *assetFile = 0;
    assetFile = fopen(filePath, "rb");

    if(!assetFile)
    {
        ASC_InfoLog("ASC_AssetsLoadFile: Couldn't open file: %s", filePath);
        free(filePath);
        return 0;
    }

    free(filePath);

    u64 fileSize = 0;
    fseek(assetFile, 0, SEEK_END);
    fileSize = (u64)ftell(assetFile);
    fseek(assetFile, 0, SEEK_SET);

    if(!fileSize)
    {
        ASC_InfoLog("ASC_AssetsLoadFile: File is 0 bytes: %s", fileName);
        fclose(assetFile);
        return 0;
    }

    // Read file contents into memory
    gApp->assets.indexCount++;
    gApp->assets.index = realloc(gApp->assets.index, (sizeof(ASC_AssetHeader) * (gApp->assets.indexCount+1)));
    assetHeader = (ASC_AssetHeader*)gApp->assets.index;
    assetHeader += gApp->assets.indexCount;
    
    memset(assetHeader, 0, sizeof(ASC_AssetHeader));
    strcpy(assetHeader->name, fileName);
    assetHeader->handle.size = fileSize;
    assetHeader->handle.data = malloc(fileSize);
    assetHeader->handle.position = 0;
    gApp->assets.totalAllocated += assetHeader->handle.size;

    ASC_DebugLog("ASC_AssetsLoadFile: New Asset: %s : size %llu bytes", fileName, fileSize);

    size_t bytesRead = fread(assetHeader->handle.data, 1, assetHeader->handle.size, assetFile);
    ASC_InfoLog("ASC_AssetsLoadFile: Read New Asset : %s : %llu bytes read", fileName, bytesRead);

    if(assetHeader->handle.size != bytesRead)
    {
        ASC_InfoLog("ASC_AssetsLoadFile: Possible error reading file: %s, expected %llu bytes, read %llu bytes.", fileName, assetHeader->handle.size, bytesRead);
    }

    fclose(assetFile);
    ASC_InfoLog("ASC_Assets: %u assets loaded, %llu bytes.", gApp->assets.indexCount, gApp->assets.totalAllocated);

    return 1;
}

static bool ASC_AssetsUnloadDat(const cstr datName)
{
    if(!datName)
    {
        ASC_Error("ASC_AssetsUnloadDat: Invalid parameters");
        return 0;
    }

    ASC_AssetHeader *assetHeader = 0;
    assetHeader = (ASC_AssetHeader*)gApp->assets.index;

    for(u32 index = 0; index < gApp->assets.indexCount+1; index++)
    {
        assetHeader++;
        
        if(strstr(assetHeader->name, datName))
        {
            ASC_AssetsUnloadFile(assetHeader->name);
            assetHeader = (ASC_AssetHeader*)gApp->assets.index;
            index = 0;
        }
    }

    ASC_InfoLog("ASC_Assets: %u assets loaded, %llu bytes.", gApp->assets.indexCount, gApp->assets.totalAllocated);
    return 1;
}

static bool ASC_AssetsUnloadFile(const cstr fileName)
{
    if(!fileName)
    {
        ASC_Error("ASC_AssetsUnloadFile: Invalid parameters");
        return 0;
    }

    ASC_AssetHeader *assetHeader = 0;
    assetHeader = (ASC_AssetHeader*)gApp->assets.index;
    bool assetExists = 0;
    
    for(u32 index = 0; index < gApp->assets.indexCount; index++)
    {
        assetHeader++;
        if(!strcmp(assetHeader->name, fileName))
        {
            assetExists = 1;
            break;
        }
    }

    if(!assetExists)
    {
        ASC_InfoLog("ASC_AssetsUnloadFile: File not loaded: %s", fileName);
        return 0;
    }

    free(assetHeader->handle.data);
    gApp->assets.totalAllocated -= assetHeader->handle.size;
    memset(assetHeader, 0, sizeof(ASC_AssetHeader));

    // shift down remaining index cards
    while((assetHeader+1) < ((ASC_AssetHeader*)gApp->assets.index)+(gApp->assets.indexCount+1))
    {
        ASC_AssetHeader *next = assetHeader+1;
        memcpy(assetHeader++, next, sizeof(ASC_AssetHeader));
    }

    gApp->assets.indexCount--;
    gApp->assets.index = realloc(gApp->assets.index, (sizeof(ASC_AssetHeader) * (gApp->assets.indexCount+1)));

    ASC_InfoLog("ASC_AssetsUnloadFile: File Unloaded: %s", fileName);
    ASC_InfoLog("ASC_Assets: %u assets loaded, %llu bytes.", gApp->assets.indexCount, gApp->assets.totalAllocated);
    return 1;
}

static ASC_VirtualFileHandle *ASC_GetAsset(const cstr fileName)
{
    ASC_AssetHeader *assetHeader = 0;
    assetHeader = (ASC_AssetHeader*)gApp->assets.index;
    bool assetExists = 0;
    
    for(u32 index = 0; index < gApp->assets.indexCount; index++)
    {
        assetHeader++;
        if(!strcmp(assetHeader->name, fileName))
        {
            assetExists = 1;
            break;
        }
    }
    
    if(assetExists)
    {
        ASC_DebugLog("ASC_GetAsset: File found: %s", fileName);
        return &assetHeader->handle;
    }

    ASC_DebugLog("ASC_GetAsset: File not found: %s", fileName);
    return 0;
}

#endif