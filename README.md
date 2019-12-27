# Ascencia

Ascencia is a way-over-ambitious-Sandbox-RPG written in C++17, OpenGL & SDL2, as my first proper project.

VERSION
-------
- inDev 1 (2019-12-27)

UPDATE LOG
----------
2019-12-27: indev1: Recently picked this up again, completely re-written everything like 12 times since I last updated the github repository, so here's the latest version with fully functioning datcompiler and SDL2 and OpenGL systems up and running and ready to rock. 
Current systems I'm proud of:
- asset_manager reads .dat files compiled by the archiver
- error_handler manages all application errors and outputs a fancy log in csv format
- gamestate holds all current gamedata and auto loads dat files through asset manager.
- systems holds SDL and OpenGL stuff, up and running nicely.
- versions are a bit simpler this time too, #state #version where state is indev/alpha/beta/release and version is just a number, I don't need ver 1.2.7.5.3.2 etc :)

2018-08-08: v0.15: Gave up on MacOS/Linux, focussing on Windows only. Reworked compilation and file management systems, working on a single .exe installer

2018-06-22: v0.14: Gave up on openGL, maybe next project! For now, DAT system working perfectly!

2018-06-15: v0.13: Implemented new DAT file system, trying to figure out openGL

2018-05-15: v0.12: Switched back to Emacs, playing with new DAT file loading system

2018-03-12: v0.11: NEW NAME: exolvere! Continued code clean-up.

2018-03-11: v0.10: Cleaning up code, rewriting many classes, throwing away useless code etc. Now using VSCode fulltime with gdb debugger integrated. Also for error checking and debugging, I have implemented a proper ErrorHandler class to catch errors, halting the program properly before segfaults and writing ErrorLog.txt. Can definitely expand on this later when I learn better error handling methods.

2018-02-18: v0.9: Implemented audio class, cleaned up some code. I'm loving the button click sounds and main menu music!

2018-02-18: v0.8: Added launcher executable which successfully changes screen resolution and fullscreen mode, writes a config file and then launches the game. Awesome!

2018-02-16: v0.7: Polishing main menu. Got button behaviour working almost as intended, about to start on the game.. :)

2018-02-12: v0.6: More cleaning up, working on Main Menu. Using new functions to make sure certain Sprites remain centered at the correct positions no matter the screen size.

2018-02-11: v0.5: Finished basic engine, added the logo loop and set the appControl switches to control Program Flow.

2018-02-10: v0.4b: More engine classes added, now can start adding assets to test game loop.

2018-02-10: v0.4: Working on Engine related classes and cleaning up code, making it usable and readable. Added Event Handler for user input - should be helpful when it comes to controls in game.

2018-02-08: v0.3: Rewrote the engine for the 139489234th time because I keep learning better ways to put everything together

2018-01-14: v0.2: Finished the basic engine and made the logo and main menu sequences

2018-01-13: v0.1: Scrapped entire project and started again using newly acquired SDL library knowledge! Should actually be able to start making the game now...

TO DO
-----
- todo is commented at the bottom of main.cpp until I start building the game itself (when I'm happy with the basic engine)

CURRENT ISSUES
--------------
- Not going to start this until I decide the game is out of "inDev"
