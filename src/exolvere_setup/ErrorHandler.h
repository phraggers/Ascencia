//=================
// Exolvere
// ErrorHandler.h
//=================
#ifndef _ERRORHANDLER_H_
#define _ERRORHANDLER_H_

struct ErrorHandler
{
  // Functions
  void init();
  std::string readErrorCode(unsigned int errorCode, std::string pErrorMessage);
  void warning(unsigned int errorCode, std::string errorString);
  void fatal(unsigned int errorCode, std::string errorString);

  // Variables
  static std::map<unsigned int, std::string> errorCodesMap;
};

#endif
