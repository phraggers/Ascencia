#-------------------
# Ascencia: Origins
# Makefile
#-------------------

# Compiler
CC=g++ -std=c++11

# Objects
OBJS=main.cpp AO_Engine.cpp AO_SDL.cpp AO_Sprite.cpp AO_TTF.cpp

# Build Directory
BDIR=../build/$(OS)/

# Target File
TGT=Ascencia_Origins_$(OS)

# Compiler Includes
INC=-Ilibs/include -Llibs/lib/$(OS)

#-------------
# COMPILATION
#-------------

# Windows
ifeq ($(shell echo "check_quotes"),"check_quotes")
OS=WIN
LIBS=-w -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
endif

# Linux
ifeq ($(shell uname), Linux)
OS=LIN
LIBS=
endif

# MacOS
ifeq ($(shell uname), Darwin)
OS=OSX
LIBS=-framework SDL2 -framework SDL2_image -framework SDL2_ttf -framework SDL2_mixer
endif

# BUILD
all:
	$(CC) $(OBJS) $(INC) $(LIBS) -o $(BDIR)$(TGT)
