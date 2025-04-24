# =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= #
# =                               = #
# =        A S C E N C I A        = #
# =                               = #
# =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= #
# = github.com/phraggers/Ascencia = #
# =      (C) 2025 Phragware       = #
# =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= #
# makefile

# makefile auto variables:
# $@ : target
# $? : prereqs newer than target
# $^ : all prereqs
# $< : first prereq

#=============================================#

PROJECT_NAME := Ascencia

#enable debug code (-DDEBUG, else -DNDEBUG)
BUILD_DEBUG := 1

#compiler optimizations on/off
BUILD_OPTIMIZED := 1

#=============================================#

# Configuration

CFG_PLATFORM :=
CFG_ARCH :=

ifeq ($(OS),Windows_NT)
	CFG_PLATFORM := WIN32
    ifeq ($(PROCESSOR_ARCHITEW6432),AMD64)
        CFG_ARCH := x64
    else
        ifeq ($(PROCESSOR_ARCHITECTURE),AMD64)
			CFG_ARCH := x64
        endif
        ifeq ($(PROCESSOR_ARCHITECTURE),x86)
            CFG_ARCH := x86
        endif
    endif
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        CFG_PLATFORM := LINUX
    endif
    ifeq ($(UNAME_S),Darwin)
        CFG_PLATFORM := MACOS
    endif
    UNAME_P := $(shell uname -p)
    ifeq ($(UNAME_P),x86_64)
        CFG_ARCH := x64
    endif
    ifneq ($(filter %86,$(UNAME_P)),)
        CFG_ARCH := x86
    endif
    ifneq ($(filter arm%,$(UNAME_P)),)
        CFG_ARCH := ARM
    endif
endif

PROJECT_DIR := $(dir $(realpath $(lastword $(MAKEFILE_LIST))))
BUILD_DIR := $(PROJECT_DIR)build/

MKDIR_BUILD :=
ifeq ($(CFG_PLATFORM),WIN32)
	MKDIR_BUILD := $(subst /,\, $(BUILD_DIR))
else
	MKDIR_BUILD = $(BUILD_DIR)
endif

CORE_INPUT := $(PROJECT_DIR)src/core/base/entry.c
ifeq ($(CFG_PLATFORM),WIN32)
	CORE_INPUT += $(PROJECT_DIR)src/core/platform/win32/win32_platform.c
endif
ifeq ($(CFG_PLATFORM),LINUX)
	CORE_INPUT += $(PROJECT_DIR)src/core/platform/linux/linux_platform.c
endif
ifeq ($(CFG_PLATFORM),MACOS)
	CORE_INPUT += $(PROJECT_DIR)src/core/platform/macos/macos_platform.c
endif

CORE_OUTPUT := $(BUILD_DIR)$(PROJECT_NAME)

CORE_DEFINES := -D$(CFG_PLATFORM)
ifeq ($(BUILD_DEBUG),1)
	CORE_DEFINES += -DDEBUG
else
	CORE_DEFINES += -DNDEBUG
endif

COMPILER := clang

COMPILER_FLAGS := -I$(PROJECT_DIR)src/
ifeq ($(BUILD_DEBUG),1)
	COMPILER_FLAGS += -g3 -Wall -Wextra -Wconversion -Wno-sign-conversion -Wdouble-promotion -Wno-unused-parameter -Wno-unused-function -fsanitize=undefined -fsanitize-trap
    ifeq ($(CFG_PLATFORM),WIN32)
		COMPILER_FLAGS += -mconsole
    endif
else
    ifeq ($(CFG_PLATFORM),WIN32)
		COMPILER_FLAGS += -mwindows
    endif
endif
ifeq ($(BUILD_OPTIMIZED),1)
	COMPILER_FLAGS += -O3
else
	COMPILER_FLAGS += -O0
endif

LINKER_FLAGS := -static
ifeq ($(CFG_PLATFORM),WIN32)
	LINKER_FLAGS += -lkernel32 -luser32
endif
ifeq ($(CFG_PLATFORM),LINUX)
endif
ifeq ($(CFG_PLATFORM),MACOS)
endif

PRINT_BUILD_INFO := $(PROJECT_NAME)
ifeq ($(BUILD_DEBUG),1)
	PRINT_BUILD_INFO += Config[DEBUG]
else
	PRINT_BUILD_INFO += Config[RELEASE]
endif
ifeq ($(BUILD_OPTIMIZED),1)
	PRINT_BUILD_INFO += Optimizations[ON]
else
	PRINT_BUILD_INFO += Optimizations[OFF]
endif

all: $(CORE_OUTPUT)
	@echo $(PRINT_BUILD_INFO)

$(CORE_OUTPUT): $(CORE_INPUT) | $(BUILD_DIR)
	$(COMPILER) $^ $(COMPILER_FLAGS) $(CORE_DEFINES) $(LINKER_FLAGS) -o $@

$(BUILD_DIR):
	mkdir $(MKDIR_BUILD)

clean:

debug:
	gdb -i=mi $(BUILD_DIR)$(PROJECT_NAME)

.PHONY: all
