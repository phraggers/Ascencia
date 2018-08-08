#include <iostream>
#include <fstream>
#include <string>

int main()
{    
  std::string file;
  std::string byte;
  char c;
  
  // get filename (NO .ext or replace . with _)
  std::string filename = "backpanel_png";
  
  file = "const char ";
  file += filename;
  file += "[] =\n{\n";
  
  std::ifstream ifile;
  ifile.open("backpanel.png", std::ifstream::in | std::ifstream::binary);
  
  while(ifile.get(c))
    {
      
    }
	
  return 0;
}
