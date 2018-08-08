//======================
// Exolvere
// MoligenMainLoop.hpp
//======================

void Moligen::mainLoop()
{
  while(!gl.appQuit)
    {
      loopStart();

      switch(gl.appControl)
	{

	case CS_BEGIN:
	  {
	    gl.appControl = CS_LAUNCHER_INIT;
	  } break;

	case CS_LAUNCHER_INIT:
	  {
	    media.loadDat("fonts.dat");
	    media.loadDat("launcher.dat");

	    media.spriteNew("launcher_backpanel", "launcher.dat", "backpanel.png");
	    media.spriteNew("launcher_title", "launcher.dat", "header.png");

	    media.spriteResize("launcher_backpanel", 0, 0, gl.appWidth, gl.appHeight);
	    media.spriteResize("launcher_title", POS_CENTERED, POS_TOP_1, POS_SOURCE, POS_SOURCE);

	    gl.appControl = CS_LAUNCHER_LOOP;
	  } break;

	case CS_LAUNCHER_LOOP:
	  {
	    media.spriteDraw("launcher_backpanel");
	    media.spriteDraw("launcher_title");
	  } break;
	}

      loopEnd();
    }
}
