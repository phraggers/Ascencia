//============
// Exolvere
// Media.cpp
//============
#include "Headers.h"
#include "Media.h"

//================
// Loading Screen
//================
void Media::loadingscreen()
{
  std::string system0str;
  std::ifstream ifile;
  std::string fileLocation = "../data/system0.dat";

  ifile.open(fileLocation.c_str(), std::ifstream::in | std::ifstream::binary);

  if(ifile.is_open())
    {
      char inputchar;
      while(ifile.get(inputchar))
	{
	  system0str.push_back(inputchar);
	}
      ifile.close();
    }

  else
    {
      ErrorHandler error;
      error.fatal(9, "0");
    }

  std::string startPosStr = "__SEP__loading.png__SEP__";
  size_t startPos = system0str.find(startPosStr)+startPosStr.length();
  size_t endPos = system0str.find("__SEP__", startPos);
  std::string loadingPng = system0str.substr(startPos, endPos-startPos);

  Sprite loadingscreen(loadingPng);

  loadingscreen.draw();
  SDL_RenderPresent(Globals::mRenderer);
}

//===========
// Load DAT
//===========
void Media::loadDat(std::string fileName)
{
  std::string fileLocation;
  fileLocation = "../data/";
  fileLocation += fileName;

  std::ifstream ifile;
  ifile.open(fileLocation.c_str(), std::ifstream::in | std::ifstream::binary);

  if(ifile.fail())
    {
			ErrorHandler error;
      error.fatal(10,fileLocation);
    }

  char inputchar;
  std::string buffer;
  std::string mapname;
  std::string mapstring;
  char mapread = 0; //0: start, 1: mapname, 2:mapstring
  int counter = 0;

  loadingscreen();

  while(ifile.get(inputchar))
    {
      buffer.push_back(inputchar);

      if(counter == 0)
	{
	  if(inputchar == '_')
	    counter = 1;
	}

      else if(counter == 1)
	{
	  if(inputchar == '_')
	    counter = 2;
	  else
	    counter = 0;
	}

      else if(counter == 2)
	{
	  if(inputchar == 'S')
	    counter = 3;
	  else
	    counter = 0;
	}

      else if(counter == 3)
	{
	  if(inputchar == 'E')
	    counter = 4;
	  else
	    counter = 0;
	}

      else if(counter == 4)
	{
	  if(inputchar == 'P')
	    counter = 5;
	  else
	    counter = 0;
	}

      else if(counter == 5)
	{
	  if(inputchar == '_')
	    counter = 6;
	  else
	    counter = 0;
	}

      else if(counter == 6)
	{
	  if(inputchar == '_')
	    counter = 7;
	  else
	    counter = 0;
	}

      if(counter == 7)
	{
	  // Remove __SEP__ from buffer
	  for(int i = 0; i < 7; i++)
	    buffer.pop_back();

	  // Reset Counter for next __SEP__
	  counter = 0;

	  // start
	  if(mapread == 0)
	    {
	      buffer.clear();
	      mapread = 1;
	    }

	  // Name
	  else if(mapread == 1)
	    {
	      mapname = buffer;
	      buffer.clear();
	      mapread = 2;
	    }

	  // Contents
	  else if(mapread == 2)
	    {
	      mapstring = buffer;

	      copyToMap(fileName, mapname, mapstring);

	      buffer.clear();
	      mapname.clear();
	      mapstring.clear();

	      mapread = 1;
	    }
	}
    }

  // Cleanup
  buffer.clear();
  mapname.clear();
  mapstring.clear();

  ifile.close();

}

//===========
// copyToMap
//===========
void Media::copyToMap(std::string datname, std::string mapname, std::string mapstring)
{
  if(mapname.find(".ttf") != std::string::npos)
    {
      datMap[datname].fonts[mapname] = mapstring;
    }

  if(mapname.find(".png") != std::string::npos)
    {
      datMap[datname].sprites[mapname] = mapstring;
    }

  if(mapname.find(".mp3") != std::string::npos)
    {
      datMap[datname].music[mapname] = mapstring;
    }

  if(mapname.find(".wav") != std::string::npos)
    {
      datMap[datname].chunks[mapname] = mapstring;
    }
}

//============
// unloadDat
//============
void Media::unloadDat(std::string datName)
{
  loadingscreen();

  if(datMap.erase(datName) == 0)
    {
			ErrorHandler error;
			error.fatal(10, datName);
    }
}

//===========
// spriteNew
//===========
void Media::spriteNew(std::string id, std::string dat, std::string file)
{
  if(sprites.find(id) == sprites.end())
    {
      sprites[id] = new Sprite(datMap[dat].sprites[file]);
    }

  else
    {
			ErrorHandler error;
			error.fatal(12, id);
    }
}

//============
// spriteDraw
//============
void Media::spriteDraw(std::string id)
{
  if(sprites.find(id) != sprites.end())
    {
      sprites[id]->draw();
    }

  else
    {
      //TODO error handling: that sprite ID doesn't exist
    }
}

//===============
// spriteResize
//===============
void Media::spriteResize(std::string id, int x, int y, int w, int h)
{
  Globals gl;

  if(sprites.find(id) != sprites.end())
    {
      int left = gl.appWidth/10;
      int top = gl.appHeight/10;

      // x position
      if(x == POS_NO_CHANGE)
	{
	  sprites[id]->drect.x = sprites[id]->drect.x;
	}

      else if(x == POS_SOURCE)
	{
	  sprites[id]->drect.x = sprites[id]->source.x;
	}

      else if(x == POS_CENTERED)
	{
	  sprites[id]->drect.x = (gl.appWidth/2) - (sprites[id]->drect.w/2);
	}

      else if(x == POS_LEFT_0)
	{
	  sprites[id]->drect.x = 0;
	}

      else if(x == POS_LEFT_1)
	{
	  sprites[id]->drect.x = left*1;
	}

      else if(x == POS_LEFT_2)
	{
	  sprites[id]->drect.x = left*2;
	}

      else if(x == POS_LEFT_3)
	{
	  sprites[id]->drect.x = left*3;
	}

      else if(x == POS_LEFT_4)
	{
	  sprites[id]->drect.x = left*4;
	}

      else if(x == POS_RIGHT_0)
	{
	  sprites[id]->drect.x = (gl.appWidth - sprites[id]->drect.w);
	}

      else if(x == POS_RIGHT_1)
	{
	  sprites[id]->drect.x = left*9;
	  if(sprites[id]->drect.x + sprites[id]->drect.w > gl.appWidth)
	    {
	      sprites[id]->drect.x = (gl.appWidth - sprites[id]->drect.w);
	    }
	}

      else if(x == POS_RIGHT_2)
	{
	  sprites[id]->drect.x = left*8;
	  if(sprites[id]->drect.x + sprites[id]->drect.w > gl.appWidth)
	    {
	      sprites[id]->drect.x = (gl.appWidth - sprites[id]->drect.w);
	    }
	}

      else if(x == POS_RIGHT_3)
	{
	  sprites[id]->drect.x = left*7;
	  if(sprites[id]->drect.x + sprites[id]->drect.w > gl.appWidth)
	    {
	      sprites[id]->drect.x = (gl.appWidth - sprites[id]->drect.w);
	    }
	}

      else if(x == POS_RIGHT_4)
	{
	  sprites[id]->drect.x = left*6;
	  if(sprites[id]->drect.x + sprites[id]->drect.w > gl.appWidth)
	    {
	      sprites[id]->drect.x = (gl.appWidth - sprites[id]->drect.w);
	    }
	}

      else
	{
	 sprites[id]->drect.x = x;
	}

      // y position
      if(y == POS_NO_CHANGE)
	{
	  sprites[id]->drect.y = sprites[id]->drect.y;
	}

      else if(y == POS_SOURCE)
	{
	  sprites[id]->drect.y = sprites[id]->source.y;
	}

      else if(y == POS_CENTERED)
	{
	  sprites[id]->drect.y = (gl.appHeight/2) - (sprites[id]->drect.y/2);
	}

      else if(y == POS_TOP_0)
	{
	  sprites[id]->drect.y = 0;
	}

      else if(y == POS_TOP_1)
	{
	  sprites[id]->drect.y = top*1;
	}

      else if(y == POS_TOP_2)
	{
	  sprites[id]->drect.y = top*2;
	}

      else if(y == POS_TOP_3)
	{
	  sprites[id]->drect.y = top*3;
	}

      else if(y == POS_TOP_4)
	{
	  sprites[id]->drect.y = top*4;
	}

      else if(y == POS_BOTTOM_0)
	{
	  sprites[id]->drect.y = (gl.appHeight - sprites[id]->drect.h);
	}

      else if(y == POS_BOTTOM_1)
	{
	  sprites[id]->drect.y = top*9;
	  if(sprites[id]->drect.y + sprites[id]->drect.h > gl.appHeight)
	    {
	      sprites[id]->drect.y = (gl.appHeight - sprites[id]->drect.h);
	    }
	}

      else if(y == POS_BOTTOM_2)
	{
	  sprites[id]->drect.y = top*8;
	  if(sprites[id]->drect.y + sprites[id]->drect.h > gl.appHeight)
	    {
	      sprites[id]->drect.y = (gl.appHeight - sprites[id]->drect.h);
	    }
	}

      else if(y == POS_BOTTOM_3)
	{
	  sprites[id]->drect.y = top*7;
	  if(sprites[id]->drect.y + sprites[id]->drect.h > gl.appHeight)
	    {
	      sprites[id]->drect.y = (gl.appHeight - sprites[id]->drect.h);
	    }
	}

      else if(y == POS_BOTTOM_4)
	{
	  sprites[id]->drect.y = top*6;
	  if(sprites[id]->drect.y + sprites[id]->drect.h > gl.appHeight)
	    {
	      sprites[id]->drect.y = (gl.appHeight - sprites[id]->drect.h);
	    }
	}

      else
	{
	  sprites[id]->drect.y = y;
	}

      // w position
      if(w == POS_NO_CHANGE)
	{
	  sprites[id]->drect.w = sprites[id]->drect.w;
	}

      else if(w == POS_SOURCE)
	{
	  sprites[id]->drect.w = sprites[id]->source.w;
	}

      else if(w == POS_CENTERED)
	{
	  //TODO think about how to code width centered
	  sprites[id]->drect.w = sprites[id]->source.w;
	}

      else
	{
	  sprites[id]->drect.w = w;
	}

      // h position
      if(h == POS_NO_CHANGE)
	{
	  sprites[id]->drect.h = sprites[id]->drect.h;
	}

      else if(h == POS_SOURCE)
	{
	  sprites[id]->drect.h = sprites[id]->source.h;
	}

      else if(h == POS_CENTERED)
	{
	  //TODO think about how to code height centered
	  sprites[id]->drect.h = sprites[id]->source.h;
	}

      else
	{
	  sprites[id]->drect.h = h;
	}
    }

  else
    {
      //TODO error handling: that sprite ID doesn't exist
    }
}

//=============
// spriteCrop
//=============
void Media::spriteCrop(std::string id, int x, int y, int w, int h)
{
  if(sprites.find(id) != sprites.end())
    {
      sprites[id]->srect.x = x;
      sprites[id]->srect.y = y;
      sprites[id]->srect.w = w;
      sprites[id]->srect.h = h;
    }

  else
    {
      //TODO error handling: that sprite ID doesn't exist
    }
}

//==============
// spriteDelete
//==============
void Media::spriteDelete(std::string id)
{
  if(sprites.find(id) != sprites.end())
    {
      delete sprites[id];
      sprites[id] = NULL;
      sprites.erase(id);
    }

  else
    {
      //TODO error handling: that sprite ID doesn't exist
    }
}
