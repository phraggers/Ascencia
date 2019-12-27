//====================================
//          ASCENCIA CLIENT
//          error_handler.cpp
//====================================
//        (c) Phragware 2019
//====================================

#define ASC_ERRORHANDLER_SYSTEMS
#include <systems.h>
#undef ASC_ERRORHANDLER_SYSTEMS

//================================
//    Constructor
//================================
ErrorHandler::ErrorHandler()
{
    SDLError = SDL_GetError();
    OpenGLError = "No error";
}

// log csv: DATE TIME APP MAJVER MINVER FATAL STATE LOCATION MESSAGE INFO

//=================
// Set
//=================
void ErrorHandler::Set(bool _fatal, std::string _location, std::string _log_message, std::string _log_additional)
{
    CheckErrorLogExists();

    std::string log_string;
    std::string log_csv;

    // get date/time
    time_t t = time(0);
    struct tm *now = localtime(&t);
    int day = now->tm_mday;
    int month = now->tm_mon+1;
    int year = now->tm_year+1900;
    int hour = now->tm_hour;
    int minute = now->tm_min;
    int second = now->tm_sec;

    {
        std::ostringstream o;
        o << year;
        log_string = o.str();
        log_csv = o.str();
    }

    log_string += '-';
    log_csv += '-';

    {
        std::ostringstream o;
        o << month;
        log_string += o.str();
        log_csv += o.str();
    }

    log_string += '-';
    log_csv += '-';

    {
        std::ostringstream o;
        o << day;
        log_string += o.str();
        log_csv += o.str();
    }

    log_string += ' ';
    log_csv += ',';

    {
        std::ostringstream o;
        o << hour;
        log_string += o.str();
        log_csv += o.str();
    }

    log_string += ':';
    log_csv += ':';

    {
        std::ostringstream o;
        o << minute;
        log_string += o.str();
        log_csv += o.str();
    }

    log_string += ':';
    log_csv += ':';

    {
        std::ostringstream o;
        o << second;
        log_string += o.str();
        log_csv += o.str();
    }

    log_string += " ";
    log_string += ASC_APPNAME;
    log_string += " ";
    log_csv += ',';
    log_csv += ASC_APPNAME;
    log_csv += ',';

    if(ASC_RELEASE == 1) { log_string += "i"; log_csv += "indev"; }
    if(ASC_RELEASE == 2) { log_string += "a"; log_csv += "alpha"; }
    if(ASC_RELEASE == 3) { log_string += "b"; log_csv += "beta"; }
    if(ASC_RELEASE == 4) { log_string += "r"; log_csv += "release"; }

    log_csv += ',';

    {
        std::ostringstream o;
        o << ASC_VERSION;
        log_string += o.str();
        log_csv += o.str();
    }

    log_string += ":\n";
    log_csv += ',';

    if(_fatal)
    {
        log_string += "(Fatal Error)";
        log_csv += "FATAL";
    }

    else
    {
        log_string += "(Warning)";
        log_csv += "WARNING";
    }

    log_string += " APPSTATE=[";
    log_csv += ',';

    {
        std::ostringstream o;
        o << ASC_APPSTATE << "/" << ASC_SUBSTATE;
        log_string += o.str();
        log_csv += o.str();
        log_csv += ',';
    }

    log_string += "] @ ";
    log_string += _location;
    log_string += ":\n";
    log_string += _log_message;
    log_string += ":\n";
    log_string += _log_additional;

    // ensure no commas inside strings
    {
        std::string location = _location;
        std::string log_message = _log_message;
        std::string log_additional = _log_additional;

        for(int i=0; i<location.length(); i++)
        if(location[i]==',') location[i] = '-';
        for(int i=0; i<log_message.length(); i++)
        if(log_message[i]==',') log_message[i] = '-';
        for(int i=0; i<log_additional.length(); i++)
        if(log_additional[i]==',') log_additional[i] = '-';

        log_csv += location;
        log_csv += ',';
        log_csv += log_message;
        log_csv += ',';
        log_csv += log_additional;
        log_csv += '\n';
    }

    std::string error_log_path = ASC_PREFPATH;
    error_log_path += "ErrorLog.csv";

    log_string += "\nError Log written to:\n";
    log_string += error_log_path;

    // write to error log
    FILE *fpo = fopen(error_log_path.c_str(), "a");
    fprintf(fpo, "%s", log_csv.c_str());
    fclose(fpo);

    if(_fatal)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Fatal Error!", log_string.c_str(), NULL);
        Game.SetState("quit");
    }

    else if(ASC_DEBUGMODE)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Debug: Warning", log_string.c_str(), NULL);
    }

    std::cerr << "Error: " << log_string << std::endl;
    return;
}

//================================
//    CheckErrorLog
//================================
void ErrorHandler::CheckErrorLogExists()
{
    // log csv: DATE TIME APP MAJVER MINVER FATAL STATE LOCATION MESSAGE INFO

    std::string error_log_path = ASC_PREFPATH;
    error_log_path += "ErrorLog.csv";

    {
        std::ifstream ifile;
        ifile.open(error_log_path.c_str());
        if(ifile.is_open())
        {
            // file exists, return
            ifile.close();
            return;
        }
    }

    std::ofstream ofile;
    ofile.open(error_log_path.c_str(), std::ios::out | std::ios::binary);
    ofile << "DATE,TIME,APP,MAJVER,MINVER,TYPE,STATE,LOCATION,MESSAGE,INFO\n";
    ofile.close();
    return;
}

//================================
//    CheckForErrors
//================================
void ErrorHandler::CheckForErrors()
{
    // SDL Errors
    if(SDLError != SDL_GetError())
    {
        SDLError = SDL_GetError();
        Set(0, "SDLMain", "SDL Error", SDL_GetError());
    }

    // OpenGL Errors
    if(OpenGLError != GetGLErrorString())
    {
        OpenGLError = GetGLErrorString();
        Set(0, "OpenGLMain", "OpenGL Error", GetGLErrorString());
    }
}

//================================
//    GetGLErrorString
//================================
std::string ErrorHandler::GetGLErrorString()
{
    switch (glGetError())
    {
        case GL_NO_ERROR:          return "No error";
        case GL_INVALID_ENUM:      return "Invalid enum";
        case GL_INVALID_VALUE:     return "Invalid value";
        case GL_INVALID_OPERATION: return "Invalid operation";
        case GL_STACK_OVERFLOW:    return "Stack overflow";
        case GL_STACK_UNDERFLOW:   return "Stack underflow";
        case GL_OUT_OF_MEMORY:     return "Out of memory";
        default:                   return "Unknown error";
    }
}