# Ascencia makefile

ifeq ($(OS),Windows_NT)
	build.bat
	exit
endif

SOURCES = src/ascencia_linux.c $(wildcard src/linux/*.c) $(wildcard src/util/*.c)

all:
	if [ -d "./build" ]; then rm -rf build; fi
	mkdir build
	gcc $(SOURCES) -o build/ascencia -Ofast -Isrc -lm
