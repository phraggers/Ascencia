#=============
# Exolvere
# makefile
#=============

# All
CI=-Ilibs/include/ -Llibs/lib/
CI1=-I../libs/include/ -L../libs/lib/

# Clear
CLEAR_DAT_COMPILER=pushd ..\\data && DEL /Q /F "exolvere_dat_compiler.exe" && popd
CLEAR_SETUP_DATS=pushd ..\\build\\exolvere_setup && DEL /Q /F "*.dat" && popd
CLEAR_EXOLVERE_DATS=pushd ..\\build\\exolvere_main\\data && DEL /Q /F "*.dat" && popd
CLEAR_INSTALLER=pushd ..\\build\\exolvere_installer && DEL /Q /F "exolvere_in.exe" && popd
CLEAR_SETUP=pushd ..\\build\\exolvere_setup && DEL /Q /F "exolvere_su.exe" && popd
CLEAR_EXOLVERE=pushd ..\\build\\exolvere_main\\bin && DEL /Q /F "exolvere.exe" && popd
CLEAR=$(CLEAR_DAT_COMPILER) && $(CLEAR_SETUP_DATS) && $(CLEAR_EXOLVERE_DATS) && $(CLEAR_INSTALLER) && $(CLEAR_SETUP) && $(CLEAR_EXOLVERE)

# Dat_Compiler
DAT_COMPILER_CC=g++ -std=gnu++11 -g
DAT_COMPILER_OBJS=exolvere_dat_compiler.cpp
DAT_COMPILER_ARGS=-w
DAT_COMPILER_LIBS=-lmingw32
DAT_COMPILER_TARGET=../data/exolvere_dat_compiler.exe
DAT_COMPILER=$(DAT_COMPILER_CC) $(DAT_COMPILER_OBJS) $(CI) $(DAT_COMPILER_ARGS) $(DAT_COMPILER_LIBS) -o $(DAT_COMPILER_TARGET)

# Compile_Dats
COMPILE_EXOLVERE=pushd ..\\data && exolvere_dat_compiler.exe M && popd
COMPILE_SETUP=pushd ..\\data && exolvere_dat_compiler.exe S && popd
COMPILE_INSTALLER=pushd ..\\data && exolvere_dat_compiler.exe I && popd
COMPILE=$(COMPILE_EXOLVERE) && $(COMPILE_SETUP) && $(COMPILE_INSTALLER)

# Exolvere
EXOLVERE_CC=g++ -std=c++11 -g
EXOLVERE_OBJS=main.cpp Moligen.cpp Media.cpp Dat.cpp ErrorHandler.cpp Sprite.cpp
EXOLVERE_ARGS=-w -Wl,-subsystem,windows
EXOLVERE_LIBS=-lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lSDL2_net
EXOLVERE_TARGET=../../build/exolvere_main/bin/exolvere.exe
EXOLVERE=pushd exolvere_main && $(EXOLVERE_CC) $(EXOLVERE_OBJS) $(CI1) $(EXOLVERE_ARGS) $(EXOLVERE_LIBS) -o $(EXOLVERE_TARGET) && popd

# Setup
SETUP_CC=g++ -std=c++11 -g
SETUP_OBJS=main.cpp Moligen.cpp Media.cpp Dat.cpp ErrorHandler.cpp Sprite.cpp
SETUP_ARGS=-w -Wl,-subsystem,windows
SETUP_LIBS=-lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lSDL2_net
SETUP_TARGET=../../build/exolvere_setup/exolvere_su.exe
SETUP=pushd exolvere_setup && $(SETUP_CC) $(SETUP_OBJS) $(CI1) $(SETUP_ARGS) $(SETUP_LIBS) -o $(SETUP_TARGET) && popd

# Installer
INSTALLER_CC=g++ -std=gnu++11 -g
INSTALLER_OBJS=exolvere_installer.cpp
INSTALLER_ARGS=-w
INSTALLER_LIBS=-lmingw32
INSTALLER_TARGET=../../build/exolvere_installer/exolvere_in.exe
INSTALLER=pushd exolvere_installer && $(INSTALLER_CC) $(INSTALLER_OBJS) $(CI1) $(INSTALLER_ARGS) $(INSTALLER_LIBS) -o $(INSTALLER_TARGET) && popd

#==============
# COMPILATION
#==============

# BUILD
all: clear dat_compiler compile exolvere setup installer

clear:
	$(CLEAR)

dat_compiler:
	$(DAT_COMPILER)

compile:
	$(COMPILE)

exolvere:
	$(EXOLVERE)

setup:
	$(SETUP)

installer:
	$(INSTALLER)
