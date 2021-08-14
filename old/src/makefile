##================================
##             ASCENCIA
##             makefile
##================================
##        (c) Phragware 2019
##================================

# Variables ==========================
RELEASE=1 # (1)indev, (2)alpha, (3)beta, (4)release, (0)error
VERSION=001 # 000 error
DEBUG=1 # bool 1 or 0
# auto vars ===========================
ifeq ($(DEBUG),1 )
COMPILER=g++ -std=c++17 -U__STRICT_ANSI__ -DOS_$(OS) -DGLEW_STATIC -DDEBUG_MODE
else
COMPILER=g++ -std=c++17 -U__STRICT_ANSI__ -DOS_$(OS) -DGLEW_STATIC
endif
CLIENT_OBJECTS:=*.cpp
INC=-I../include/ -I../src/ -L../lib/ $(OSINC)
#======================================

CLEAN=@echo. & @echo. & @echo COMPILING: Cleaning... & @echo. & @echo. \
& rd /s /q ..\build
PRE_BUILD=@echo. & @echo. & @echo COMPILING: Pre-Build... & @echo. & @echo. \
& mkdir ..\build\Ascencia \
& mkdir ..\build\Ascencia\data \
& pushd .. && robocopy data\build build\Ascencia\data & popd \
& pushd .. && robocopy data\libs build\Ascencia & popd
POST_BUILD=@echo. & @echo. & @echo COMPILING: Post-Build... & @echo. & @echo. \
& ..\misc\rh\ResourceHacker.exe \
-open ..\build\Ascencia\Ascencia.exe \
-resource ..\data\icons\Ascencia.ico \
-mask ICONGROUP,MAINICON, \
-action addskip \
-save ..\build\Ascencia\Ascencia.exe
FINALIZE=pause
ifeq ($(DEBUG),1)
CONSOLE=-Wl,-Bdynamic
else
CONSOLE=-Wl,-Bdynamic -mwindows
endif
LIBS=-lmingw32 -Wl,-Bstatic -lSDL2main -lSDL2 -lglew32 -lopengl32 -lglu32 $(CONSOLE) -lm -ldinput8 -ldxguid -ldxerr8 -luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lshell32 -lversion -luuid -static-libgcc
DLL_LIBS=-Wl,-Bdynamic -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lSDL2_net
ARGS=-g -s -w
CLIENT_TARGET=../build/Ascencia/Ascencia.exe
ECHO_CLIENT=@echo. & @echo. & @echo COMPILING: Building Client... & @echo. & @echo.

# recipe =============================
all:
# clean
	-$(CLEAN)
# prebuild
	-$(PRE_BUILD)
# client
	-$(ECHO_CLIENT)
	-$(COMPILER) -DG_RELEASE=$(RELEASE) -DG_VERSION=$(VERSION) $(CLIENT_OBJECTS) $(INC) $(LIBS) $(DLL_LIBS) $(ARGS) -o $(CLIENT_TARGET)
# postbuild
	-$(POST_BUILD)
# finalize
	-$(FINALIZE)
# launch
	-$(CLIENT_TARGET)
#======================================