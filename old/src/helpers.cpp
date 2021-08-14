//================================
//          ASCENCIA CLIENT
//          helpers.cpp
//================================
//        (c) Phragware 2019
//================================

#include <helpers.h>

//===========
// GetTime
//===========
time_t GetTime(int *_year, int *_month, int *_day, int *_hour, int *_minute, int *_second)
{
    time_t t = time(0);
    struct tm *now = localtime(&t);
    if(_year != NULL) *_year = now->tm_year+1900;
    if(_month != NULL) *_month = now->tm_mon+1;
    if(_day != NULL) *_day = now->tm_mday;
    if(_hour != NULL) *_hour = now->tm_hour;
    if(_minute != NULL) *_minute = now->tm_min;
    if(_second != NULL) *_second = now->tm_sec;
    return t;
}

//=======
// exec
//=======
std::string exec(const char *_cmd)
{
    std::array<char, 128> buffer;
    std::string result;
    std::shared_ptr<FILE> pipe(popen(_cmd, "r"), pclose);

    while(!feof(pipe.get()))
    {
        if(fgets(buffer.data(), 128, pipe.get()) != nullptr)
        result += buffer.data();
    }

    return result;
}

//=================
// SeparateString
//=================
void SeparateString(std::string _in, char _sep, std::vector<std::string> &_vecstr)
{
    std::stringstream ss(_in);
    std::string outstr;

    while(std::getline(ss, outstr, _sep))
    {
        _vecstr.push_back(outstr);
    }
}

//====================
// GetDirectoryList
//====================
std::string GetDirectoryList(std::string _path)
{
    std::string cmd;
    cmd = "dir /b /a:d ";
    cmd += _path;
    return exec(cmd.c_str());
}

//===============
// GetFileList
//===============
std::string GetFileList(std::string _path)
{
    std::string cmd;
    cmd = "dir /b /a:-d ";
    cmd += _path;
    return exec(cmd.c_str());
}