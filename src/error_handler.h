//====================================
//          ASCENCIA CLIENT
//          error_handler.h
//====================================
//        (c) Phragware 2019
//====================================

#ifndef ASC_ERRORHANDLER_H_
#define ASC_ERRORHANDLER_H_

#include <headers.h>

struct ErrorHandler
{
    ErrorHandler();
    void Set(bool _fatal, std::string _location, std::string _log_message, std::string _log_additional = "an error occurred");
    void CheckErrorLogExists();
    void CheckForErrors();
    std::string GetGLErrorString();

    std::string SDLError;
    std::string OpenGLError;
};

#endif // ASC_ERRORHANDLER_H_