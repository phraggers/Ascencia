//==============
// Exolvere
// DATCreator
//==============
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <vector>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <array>
#include <sstream>
#include <map>

// dat file content:
// __SEP__filename.ext__SEP__file-content(__SEP__EOF)

//============================================
// exec (credit: Stack Overflow user "waqas")
//============================================
std::string exec(const char *cmd)
{
  std::array<char, 128> buffer;
  std::string result;
  std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);

  while(!feof(pipe.get()))
    {
      if(fgets(buffer.data(), 128, pipe.get()) != nullptr)
        result += buffer.data();
    }

  return result;
}

int main(int argc, char *argv[])
{
    bool correctUsage = false;
    char setupSelector = 0;

    if(argc < 1)
    {
       goto A_FAIL;
    }

    for(int i=0; i<argc; i++)
    {
        if(*argv[i] == 'S')
        {
            correctUsage = true;
            setupSelector = 'S';

            // setup
            std::cout << "Compiling Setup DAT files..." << std::endl;

        }

        if(*argv[i] == 'M')
        {
            correctUsage = true;
            setupSelector = 'M';

            // exolvere main
            std::cout << "Compiling Exolvere Main DAT files..." << std::endl;
        }

        if(*argv[i] == 'I')
        {
            correctUsage = true;
            setupSelector = 'I';

            // installer
            std::cout << "Compiling Installer files..." << std::endl;
        }
    }

    if(!correctUsage)
    {
        A_FAIL:
        std::cout << "\n\nexolvere_dat_compiler USAGE ERROR!\n" << std::endl;
        std::cout << "Usage: exolvere_dat_compiler.exe [S/M/I]\nS = setup\nM = exolvere main\nI = installer\n" << std::endl;
        return 1;
    }

    std::string separator = "__SEP__";
    std::string dirlist = "";
    std::string coutdatfilename = "";

    if(setupSelector == 'S')
    {
        dirlist = exec("dir exolvere_setup /A:D /B");
    }

    if(setupSelector == 'M')
    {
        dirlist = exec("dir exolvere_main /A:D /B");
    }

    if(setupSelector == 'I')
    {
        dirlist = "exolvere_setup ";
    }

    dirlist.pop_back();

    std::stringstream ss(dirlist);
    std::string datDirectoryName;

    // while per directory/dat
    while(std::getline(ss,datDirectoryName,'\n'))
    {
        std::string datDirectoryDirList;
        std::string execCommand;
        std::string inputFilePrePath;

        if(setupSelector == 'S')
        {
            execCommand = "dir exolvere_setup\\";
            inputFilePrePath = "exolvere_setup/";
        }

        if(setupSelector == 'M')
        {
            execCommand = "dir exolvere_main\\";
            inputFilePrePath = "exolvere_main/";
        }

        if(setupSelector == 'I')
        {
            execCommand = "dir ..\\build\\";
            inputFilePrePath = "../build/";
        }

        execCommand += datDirectoryName;
        execCommand += " /B";
        dirlist = exec(execCommand.c_str());
        dirlist.pop_back();

        std::stringstream ss2(dirlist);
        std::string fileName;
        inputFilePrePath += datDirectoryName;
        inputFilePrePath += "/";

        // while per file
        while(std::getline(ss2,fileName,'\n'))
        {
          std::string inputFilePath = inputFilePrePath;
          std::string fileContents;
          std::ifstream inputfile;
          inputFilePath += fileName;
          inputfile.open(inputFilePath.c_str(), std::ifstream::in | std::ifstream::binary);
          char inputchar;
          while(inputfile.get(inputchar))
          {
              fileContents.push_back(inputchar);
          }
          inputfile.close();

          // add file contents to dat
          std::string datFilePath;

          if(setupSelector == 'S')
          {
              datFilePath = "../build/exolvere_setup/";
          }

          if(setupSelector == 'M')
          {
              datFilePath = "../build/exolvere_main/data/";
          }

          if(setupSelector == 'I')
          {
              datFilePath = "../build/exolvere_installer/";
          }

          datFilePath += datDirectoryName;
          datFilePath += ".dat";

          if(datFilePath != coutdatfilename)
          {
              std::cout << "Writing to " << datFilePath << std::endl;
              coutdatfilename = datFilePath;
          }

          std::ofstream ofile;
          ofile.open(datFilePath.c_str(), std::ofstream::out | std::ofstream::binary | std::ofstream::app);
          ofile << separator << fileName << separator << fileContents;
          ofile.close();

      }

      // add final separator to dat
      std::string datFilePath;

      if(setupSelector == 'S')
      {
          datFilePath = "../build/exolvere_setup/";
      }

      if(setupSelector == 'M')
      {
          datFilePath = "../build/exolvere_main/data/";
      }

      if(setupSelector == 'I')
      {
          datFilePath = "../build/exolvere_installer/";
      }

      datFilePath += datDirectoryName;
      datFilePath += ".dat";

      std::ofstream ofile2;
      ofile2.open(datFilePath.c_str(), std::ofstream::out | std::ofstream::binary | std::ofstream::app);
      ofile2 << separator;
      ofile2.close();
  }

  return 0;
}