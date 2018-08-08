//===================
// Exolvere
// ErrorHandler.cpp
//===================
#include "Headers.h"
#include "ErrorHandler.h"

//======
// init
//======
void ErrorHandler::init()
{

    std::string system0datLocation;
    std::string system0datContents;

    system0datLocation = Globals::appBasePath;

    for(int i = 0; i < 4; i++)
    {
        system0datLocation.pop_back();
    }
    system0datLocation += "data/";

    /*
    TODO: currently reading system0.dat as if it was JUST errorCodes.csv but it isn't. Need to first get errorCodes.csv into a string and then use:

    for (int i = 0; i != ERROR-CODES-CSV.end(); i++)
    {
        CODE currently in ifile.get, below.
    }

    Then rework the readErrorCode function so it doesn't need to read system0.dat

    Also try and make errorCodesMap static global for all error objects.
    (it's currently static, don't know if it'll work or not.)

    */
/*
    std::ifstream ifile;
    std::string fileLocation;
    std::string systemfilestring;

    fileLocation = "../data/";
    fileLocation += "system0.dat";

    ifile.open(fileLocation.c_str(), std::ifstream::in | std::ifstream::binary);
    if(ifile.is_open())
    {
        char inputchar;
        while(ifile.get(inputchar))
        {
            // get system0.dat to systemfilestring
            systemfilestring.push_back(inputchar);
        }
    }

    // selector: 0: errorNo, 1: humanReadable
    int selector = 0;
    unsigned int errorCode = 0;
    bool read = false;
    std::string buffer;

    //ifile.open(fileLocation.c_str(), std::ifstream::in | std::ifstream::binary);
    if(ifile.is_open())
    {
        char inputchar;
        while(ifile.get(inputchar))
        {
            if(inputchar == ',' || inputchar == '\n')
            {
                read = false;
            }
            else
            {
                read = true;
            }

            // read errorCode to buffer
            if(read && selector == 0)
            {
                buffer.push_back(inputchar);
            }

            // after reading errorCode, convert to str and clear buffer
            if(inputchar == ',' && selector == 0)
            {
                errorCode = stoi(buffer);
                buffer.clear();
            }

            // read human readable error string
            if(read && selector == 1)
            {
                errorCodesCSV[errorCode].push_back(inputchar);
            }

            if(inputchar == ',')
            {
                selector++;
            }

            if(inputchar == '\n')
            {
                selector = 0;
            }
        }
        ifile.close();
    }*/
}

//================
// readErrorCode
//================
std::string ErrorHandler::readErrorCode(unsigned int errorCode, std::string pErrorMessage)
{
    std::string errorMessage = pErrorMessage;
    std::stringstream ss;
    ss << errorCode;
    errorMessage += ss.str();
    errorMessage += "\n";

    std::string ErrorCodesStr;
    std::ifstream ifile;
    std::string fileLocation = "../data/system0.dat";

    ifile.open(fileLocation.c_str(), std::ifstream::in | std::ifstream::binary);

    if(ifile.is_open())
    {
        char inputchar;
        while(ifile.get(inputchar))
        {
            ErrorCodesStr.push_back(inputchar);
        }
        ifile.close();
    }

    std::string startPosStr = "__SEP__ErrorCodes.csv__SEP__";
    size_t startPos = ErrorCodesStr.find(startPosStr)+startPosStr.length();
    size_t endPos = ErrorCodesStr.find("__SEP__", startPos);
    std::string errorCodes = ErrorCodesStr.substr(startPos, endPos-startPos);

    std::stringstream ss2(errorCodes);
    std::string errorCodeLine;
    unsigned int lineNumber = 1;

    while(std::getline(ss2, errorCodeLine, '\n'))
    {
        if(lineNumber == errorCode)
        {
            break;
        }
        else
        {
            lineNumber++;
        }
    }

    int eraseCount = 1;

    if(errorCode < 10)
    eraseCount = 2;
    else if(errorCode < 100)
    eraseCount = 3;
    else if(errorCode < 1000)
    eraseCount = 4;

    errorCodeLine.erase(0, eraseCount);

    errorMessage += "\n";
    errorMessage += errorCodeLine;

    return errorMessage;
}

//=========
// warning
//=========
void ErrorHandler::warning(unsigned int errorCode, std::string errorString)
{
    std::string errorMessage;
    errorMessage = "A non-fatal error has occured!\nError Code: ";

    errorMessage = readErrorCode(errorCode, errorMessage);

    if(errorString != "0")
    {
        errorMessage += errorString;
    }

    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, "Warning", errorMessage.c_str(), NULL);
}

//=======
// fatal
//=======
void ErrorHandler::fatal(unsigned int errorCode, std::string errorString)
{
    std::string errorMessage;
    errorMessage = "A fatal error has occured!\nError Code: ";

    errorMessage = readErrorCode(errorCode, errorMessage);

    if(errorString != "0" && errorString.length() > 0 && errorMessage.length() > 0)
    {
        errorMessage += errorString;
    }

    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Fatal Error", errorMessage.c_str(), NULL);

    exit(errorCode);
}
