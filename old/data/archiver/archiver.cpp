
//====================================
// Ascencia Archiver
// .dat data file compiler
// C++11 Windows & MinGW compiler only
//====================================
// (c) Phragware 2019
//====================================

/*
-----------------------------------------------------------------------

This program reads a directory of subdirectories
(each subdirectory CAN ONLY CONTAIN FILES, NO SUB-SUB-DIRECTORIES)
and packs them into a single data file, eg FolderName.dat
read "output dat file composition" below to 
understand how output.dat files are laid out.

Also find below an example program to read .dat files.

-----------------------------------------------------------------------

Notes:

1: This program ignores Thumbs.db files created by Windows Explorer:
they can be in input directories, but won't be compiled into dats.

2: Any input files with no file extension are renamed with .file ext
(once inside dat: original file is not renamed)

3: All input files are renamed so extensions are lower-case
(file name is unchanged, but ORIGINAL FILE IS CHANGED, not just in dat)

-----------------------------------------------------------------------

Build & Run with this buildnrun.bat:
del archiver.exe
g++ archiver.cpp -o archiver.exe
pause
start archiver.exe

-----------------------------------------------------------------------
*/

//============
// VARIABLES
//============
// Comment these out if you want to use defaults
// defaults: ./input/ ./output/ 0xffffff

#define SOURCE_DIRECTORY "..\\gamedata\\"
#define TARGET_DIRECTORY "..\\build\\" 
#define BUFFER_WRITE_SIZE 0xffffff

// SOURCE_DIRECTORY : input: directories to compile:
// TARGET_DIRECTORY : output: where .dats go
// BUFFER_WRITE_SIZE : bigger buffer = more RAM use, but faster

// buffer size notes:
// 0xffff to 0xffffffff
// 0xffff around 4MB RAM usage, but VERY slow
// 0xffffff middle ground, around 160MB RAM usage, moderately fast
// 0xffffffff up to 4.1GB RAM usage, but very fast

/*

example project:

#define SOURCE_DIRECTORY "..\\gamedata\\"
#define TARGET_DIRECTORY "..\\build\\"
#define BUFFER_WRITE_SIZE 0xffffff 

{PROJ}/data/archiver/archiver.exe 	-this exe
{PROJ}/data/gamedata/*				-source folders
{PROJ}/data/build/*.dat				-destination

creates: folder_name.dat

*/

//============================================

/*

OUTPUT DAT FILE COMPOSITION:

HEADER: 14 bytes
{
	TAG: 6 bytes
	{
		0x41 0x53 0x43 0x44 0x41 0x54 //ascii: "ASCDAT"
	}
	BODY_START_ADDRESS: 8 bytes
	{
		uint64_t body_start_address //address of first file body
	}
}

INDEX:
{
	PER FILE:
	{
		1 byte: uint8_t file_name_length = x
		x bytes: char file_name[x] (ascii)
		1 byte: uint8_t file_ext_length = y
		y bytes: char file_ext[y] (ascii)
		8 bytes: uint64_t file_address
		8 bytes: uint64_t file_length
	}
}

BODY:
{
	PER FILE:
	{
		binary file data
	}
}


first byte written is at address 0x00 (index 0, like C array)

----------------------
C++ fstream notes:

std::ios::ate puts read pointer at end of file
std::ifstream::pos_type size; // fstream type to hold size data

//NOTE: pos_type doesnt play well with normal int operations
// you can:
// std::ifstream::pos_type size;
// ... get data into size ...
// uint64_t pos = size; 
// and then operate on that.

file.open(path, std::ios::in | std::ios::binary | std::ios::ate);
size = ifile.tellg();  gets file size
file.seekg(0, std::ios::beg); moves read pointer to start

way: std::ios::beg start, std::ios::cur current, std::ios::end EOF

file.seekg(offset, way); set read pos
file.tellg(); get read pos
file.get(c); get next byte
file.put(c); write next byte
file.write( (char*) &variable , (sizeof)variable ); write variable
file.read( (char*) &variable , (sizeof)variable ); read into variable
----------------------

*/

/*
//======================================
// EXAMPLE DAT READER PROGRAM
// to read .dats
// datreader.cpp
//======================================

// Build & Run with this buildnrun.bat:
// del datreader.exe
// g++ datreader.cpp -o datreader.exe
// pause
// start datreader.exe

#include <iostream>
#include <fstream>
#include <string>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

struct AMFile
{
    // name, data
    std::map<std::string, std::string> Files;
};

struct AMDat
{
    // extension, file[name] = data
    std::map<std::string, AMFile> Exts;
};

struct DatManager
{
    // DatManager::Dats[datname].Exts[ext].Files[filename] = data string
    std::map<std::string, AMDat> Dats;
	
	// LoadFile: returns 0 on success, 1 on error,
	// Loads char* data from specified file inside .dat
	// into Dats map above, as std::string
	int LoadFile(std::string _dat, std::string _file)
	{
		if(!_dat.length())
		{
			std::cout << "Error: invalid dat name" << std::endl;
			return 1;
		}

		if(!_file.length())
		{
			std::cout << "Error: invalid file name" << std::endl;
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
			std::cout << "Error: Failed to open dat" << std::endl;
			return 1;
		}

		// check headtag
		std::string buffer;
		for(int i=0; i<6; i++) buffer.push_back(ifile.get());

		if(buffer != "ASCDAT")
		{
			std::cout << "Error: invalid header tag" << std::endl;
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
			std::cout << "Error: file not found in dat" << std::endl;
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
};

int main()
{
	// dat: "example.dat", file: "file.txt"
	
	DatManager DatMan;
	
	int error = DatMan.LoadFile("example", "file.txt");
	
	std::string FileData = Dats["example"].Exts["txt"].Files["file"];
	
	// ... do stuff ...
	
	return error;
}

//*/

//====================
// Ascencia Archiver
//====================

#ifndef SOURCE_DIRECTORY
#define SOURCE_DIRECTORY "input\\"
#endif
#ifndef TARGET_DIRECTORY
#define TARGET_DIRECTORY "output\\"
#endif
#ifndef BUFFER_WRITE_SIZE
#define BUFFER_WRITE_SIZE 0xffffff
#endif

//C Headers
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

// C++ Storage
#include <array>
#include <vector>
#include <map>

// C++ Streams
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>

// C++ Other
#include <string>
#include <memory>
#include <iterator>

struct file 
{
	std::string name;
	std::string ext; 
	uint64_t address; 
	uint64_t length; 
};

struct directory 
{ 
	std::map<std::string,file> files; 
};

std::map<std::string, directory> directories;

std::string exec(const char *_cmd)
{
	std::cout << "CMD: " << _cmd << std::endl;
	
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

void SeparateString(std::string _in, char _sep, std::vector<std::string> &_vecstr)
{
    std::stringstream ss(_in);
    std::string outstr;

    while(std::getline(ss, outstr, _sep))
    {
        _vecstr.push_back(outstr);
    }
}

std::string GetDirectoryList(std::string _path)
{
    std::string cmd;
    cmd = "dir /b /a:d ";
    cmd += _path;
    return exec(cmd.c_str());
}

std::string GetFileList(std::string _path)
{
    std::string cmd;
    cmd = "dir /b /a:-d ";
    cmd += _path;
    return exec(cmd.c_str());
}

void SetLowercaseExtensions()
{
	std::vector<std::string> input_directories;
	SeparateString(GetDirectoryList(SOURCE_DIRECTORY), '\n', input_directories);
	
	for(int i=0; i<input_directories.size(); i++)
	{
		std::vector<std::string> input_files;
		std::string path = SOURCE_DIRECTORY;
        path += input_directories[i];
        SeparateString(GetFileList(path), '\n', input_files);
		
		for(int j=0; j<input_files.size(); j++)
		{
			std::string filename;
			std::string fileext;
			bool read_ext = false;
			
			// remove Thumbs.db from input list
			{
				std::vector<std::string>::iterator it = input_files.begin();
				while(it != input_files.end())
				{
					if((*it) == "Thumbs.db")
					{
						it = input_files.erase(it);
					}
					else
					{
						it++;
					}
				}
			}
			
			for(int k=0; k<input_files[j].length(); k++)
			{
				if(input_files[j][k] == '.') {read_ext = true; continue;}
				if(!read_ext) filename.push_back(input_files[j][k]);
				if(read_ext) fileext.push_back(tolower(input_files[j][k]));
			}
			
			std::string cmd = "rename ";
			cmd += SOURCE_DIRECTORY;
			cmd += input_directories[i];
			if(cmd.back() != '\\') cmd.push_back('\\');
			cmd += input_files[j];
			cmd += " ";
			cmd += filename;
			if(fileext.length())
			{
				cmd += '.';
				cmd += fileext;
			}
			
			exec(cmd.c_str());
		}
	}
}

int main()
{
	std::cout << "\n=============================\nAscenciaArchiver \n=============================" << std::endl;

	std::cout << "deleting existing dats..." << std::endl;
	// delete existing dats
	{
		std::string cmd = "rd /s /q ";
		cmd += TARGET_DIRECTORY;
		exec(cmd.c_str());
		std::string cmd2 = "mkdir ";
		cmd2 += TARGET_DIRECTORY;
		exec(cmd2.c_str());
	}
	std::cout << "DONE" << std::endl;
	
	std::cout << "changing source files to lowercase extensions..." << std::endl;
	SetLowercaseExtensions();
	std::cout << "DONE" << std::endl;
	
	std::vector<std::string> input_directories;
	SeparateString(GetDirectoryList(SOURCE_DIRECTORY), '\n', input_directories);
	
	// per directory
	for(int i=0; i<input_directories.size(); i++)
	{		
		std::cout << "\n============================= \nwriting new dat: " << input_directories[i] << ".dat\n" << std::endl;
		std::vector<std::string> input_files;
        {
            std::string path = SOURCE_DIRECTORY;
            path += input_directories[i];
            SeparateString(GetFileList(path), '\n', input_files);
			
			// remove Thumbs.db from input list
			{
				std::vector<std::string>::iterator it = input_files.begin();
				while(it != input_files.end())
				{
					if((*it) == "Thumbs.db")
					{
						it = input_files.erase(it);
					}
					else
					{
						it++;
					}
				}
			}
        }
		
		// per file (get size)
		for(int j=0; j<input_files.size(); j++)
		{	
			std::cout << "get size: " << input_files[j] << "..." << std::flush;	
			std::ifstream ifile;
            std::string ifilepath = SOURCE_DIRECTORY;
            ifilepath += input_directories[i];
            if(ifilepath.back() != '\\') ifilepath.push_back('\\');
            ifilepath += input_files[j];
            ifile.open(ifilepath, std::ios::in | std::ios::binary | std::ios::ate);
			std::ifstream::pos_type file_size;
			file_size = ifile.tellg();
			directories[input_directories[i]].files[input_files[j]].length = file_size;
            ifile.close();
			std::cout << "done" << std::endl;
			
			// get file name + ext
			bool read_ext = false;
			for(int k=0; k<input_files[j].length(); k++)
			{
				if(input_files[j][k] == '.') {read_ext = true; continue;}
				if(!read_ext) directories[input_directories[i]].files[input_files[j]].name.push_back(input_files[j][k]);
				if(read_ext) directories[input_directories[i]].files[input_files[j]].ext.push_back(input_files[j][k]);
			}
			// if file has no ext use default filename.file
			if(!directories[input_directories[i]].files[input_files[j]].ext.length()) directories[input_directories[i]].files[input_files[j]].ext = "file";
		}
		
		// get header size		
		uint64_t dat_address_pointer;
		uint64_t headersize = 6+8; // 6 headtag + 8 start address
		
		for(int j=0; j<input_files.size(); j++)
		{
			headersize += 18; // 8 address, 8 length, 1 name length, 1 ext length
			headersize += directories[input_directories[i]].files[input_files[j]].name.length();
			headersize += directories[input_directories[i]].files[input_files[j]].ext.length();
		}
		
		dat_address_pointer = headersize;
		
		// set addresses
		// header + current dat_address_pointer, add file, update dat_address_pointer etc
		for(int j=0; j<input_files.size(); j++)
		{
			directories[input_directories[i]].files[input_files[j]].address = dat_address_pointer;
			dat_address_pointer += directories[input_directories[i]].files[input_files[j]].length;
		}
		
		std::string datpath = TARGET_DIRECTORY;
		datpath += input_directories[i];
		datpath += ".dat";
		
		// write header
		{
			std::cout << "writing header: " << input_directories[i] << ".dat" << std::endl;
			
			std::ofstream ofile;
			ofile.open(datpath.c_str(), std::ios::out | std::ios::binary);
			
			// headtag
			uint8_t headtag[6] = {0x41, 0x53, 0x43, 0x44, 0x41, 0x54};
			ofile.write( (char *) &(headtag), (sizeof(uint8_t)*6) );
			
			// body start address (same as address of file 0)
			ofile.write( (char *) &(directories[input_directories[i]].files[input_files[0]].address), 
			sizeof(directories[input_directories[i]].files[input_files[0]].address) );
			
			// file index
			for(int j=0; j<input_files.size(); j++)
			{			
				// filename length
				uint8_t namelength = (uint8_t)directories[input_directories[i]].files[input_files[j]].name.length();
				ofile.write((char *)&namelength, sizeof(namelength));
			
				// filename
				for(int k=0; k<namelength; k++)
				{
					char ch = directories[input_directories[i]].files[input_files[j]].name[k];
					ofile.write((char *)&directories[input_directories[i]].files[input_files[j]].name[k], sizeof(char));
				}
			
				// ext length
				uint8_t extlength = (uint8_t)directories[input_directories[i]].files[input_files[j]].ext.length();
				ofile.write((char *)&extlength, sizeof(extlength));
			
				// ext
				for(int k=0; k<extlength; k++)
				{
					char ch = directories[input_directories[i]].files[input_files[j]].ext[k];
					ofile.write((char *)&directories[input_directories[i]].files[input_files[j]].ext[k], sizeof(char));
				}
			
				// address
				ofile.write((char *)&directories[input_directories[i]].files[input_files[j]].address, 
				sizeof(directories[input_directories[i]].files[input_files[j]].address));
			
				// length
				ofile.write((char *)&directories[input_directories[i]].files[input_files[j]].length, 
				sizeof(directories[input_directories[i]].files[input_files[j]].length));
			}
		
			ofile.close();
		}
		
		// data buffer struct
		struct datas
		{
			std::vector<uint8_t> data; // uint8 buffer
			std::string currentfilepath; // current file
	
			void output() // write to file
			{
				if(data.size()>0) // output uint8 buffer
				{
					std::ofstream ofile;
					ofile.open(currentfilepath.c_str(), std::ofstream::out | std::ofstream::binary | std::ofstream::app);
					ofile.write((const char*)data.data(), data.size());
					ofile.close();
					data.clear();
				}
			}
	
			void write(std::string filepath, uint8_t b) // add uint8 to buffer
			{
				if(filepath != currentfilepath)
				{
					if(currentfilepath.length()>0) output();
					currentfilepath = filepath;
				}
		
				data.push_back(b);
				if(data.size() >= BUFFER_WRITE_SIZE) output();
			}
		} data;
		
		// write dat file body
		for(int j=0; j<input_files.size(); j++)
		{
			uint64_t file_bytes_written = 0;
			std::cout << "writing body: " << input_files[j] << std::flush;
			
			std::string datpath = TARGET_DIRECTORY;
			datpath += input_directories[i];
			datpath += ".dat";
			
			uint64_t file_pos = 0;
			uint64_t read_pos = 0;
			bool read_done = false;
			
			while(!read_done)
			{
				read_done = true;
				read_pos = 0;
				
				std::ifstream ifile;
				std::string ifilepath = SOURCE_DIRECTORY;
				ifilepath += input_directories[i];
				if(ifilepath.back() != '\\') ifilepath.push_back('\\');
				ifilepath += input_files[j];
				ifile.open(ifilepath, std::ios::in | std::ios::binary);
				
				char c;
				std::string cbuffer;
			
				// get file bytes
				while(ifile.get(c))
				{
					if(read_pos == file_pos)
					{
						cbuffer.push_back(c);
						file_pos++;
						file_bytes_written++;
					}
					
					read_pos++;
					
					if(cbuffer.length()>BUFFER_WRITE_SIZE)
					{
						if(directories[input_directories[i]].files[input_files[j]].length <= file_pos)
						{
							break; // break if cbuffer == BUFFER_WRITE_SIZE AND seek == EOF 
						}
						
						read_pos = 0;
						read_done = false;
						break;
					}
				}

				ifile.close();
				
				double filesize = directories[input_directories[i]].files[input_files[j]].length;
				double written = file_bytes_written;
				int percent = (double)((double)written/(double)filesize) * 100;
				
				std::cout << "\r                                    " 
				<< "\rwriting body: " << input_files[j] << " : " << percent << "%" << std::flush;
				
				std::vector<uint8_t> v(cbuffer.begin(), cbuffer.end());
				cbuffer.clear();
				for(int k=0; k<v.size(); k++) data.write(datpath, v[k]);
			}
			
			std::cout << std::endl;
		}

		data.output();
		std::cout << "\n" << input_directories[i] << ".dat written\n=============================" << std::endl;
		
	}
	
	std::cout << "\n=============================\nAscenciaArchiver completed \n=============================" << std::endl;
	system("pause");
	
	return 0;
}