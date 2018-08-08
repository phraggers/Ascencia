# Choose compiler:
CC=g++

# Build Directory:
BUILD=../build/

# Objects
OBJS=main.cpp AOsys.cpp Print.cpp Player.cpp

# Target Filename
FILENAME=Ascencia_Origins

# Detect OS
ifeq ($(shell echo "check_quotes"),"check_quotes")
EXT=EXE
endif

ifeq ($(shell uname), Linux)
EXT=.lin
endif

ifeq ($(shell uname), Darwin)
EXT=.osx
endif

# Target : Dependancy
all:
	$(CC) -std=c++11 $(OBJS) -o $(BUILD)$(FILENAME)$(EXT)
