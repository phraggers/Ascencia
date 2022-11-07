# Ascencia
- Work in progress sci-fi/fantasy RPG, procedural storytelling
- Still working on engine basics & platform layer, so nothing really to show yet
- (~4500 lines of code so far!)

# Version History

## 0.1.1 patch
- Fixed bug with Log thread getting stuck after SDL_Quit was called

## 0.1.0 Initial platform layer
- Decided to write in Pure C, SDL2, GLAD OpenGL, aiming for cross-platform any 64bit device (sorry win32 folks)
- multithread log system
- complete SDL input system (game controllers, keyboard, mouse)
- my helper functions: Random Generators, String Helpers etc