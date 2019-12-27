//================================
//          ASCENCIA CLIENT
//         client_helpers.h
//================================
//        (c) Phragware 2019
//================================

#ifndef ASC_CLIENTHELPERS_H_
#define ASC_CLIENTHELPERS_H_
#include <headers.h>

time_t GetTime
(int *_year, int *_month, int *_day, int *_hour, int *_minute, int *_second);

std::string exec
(const char *_cmd);

void SeparateString
(std::string _in, char _sep, std::vector<std::string> &_vecstr);

std::string GetDirectoryList
(std::string _path);

std::string GetFileList
(std::string _path);

#endif