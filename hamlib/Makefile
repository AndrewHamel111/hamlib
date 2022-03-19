.PHONY: all clean

SUBDIRS	= utility particles ui drawing
SUBDIRS := $(addprefix src/, $(SUBDIRS))

SOURCE_FILES = $(wildcard src/utility/*.c)
SOURCE_FILES += $(wildcard src/particles/*.c)
SOURCE_FILES += $(wildcard src/ui/*.c)
SOURCE_FILES += $(wildcard src/drawing/*.c)

RAYLIB_VERSION     ?= 3.8.0
PLATFORM           ?= PLATFORM_DESKTOP
DESTDIR ?= /usr/local
HAMLIB_PATH = C:/Users/thelu/Documents/Projects/hamlib/hamlib
RAYLIB_INSTALL_PATH ?= $(DESTDIR)/lib
RAYLIB_H_INSTALL_PATH ?= $(DESTDIR)/include
RAYLIB_LIBTYPE        ?= STATIC
BUILD_MODE            ?= DEBUG
USE_EXTERNAL_GLFW     ?= FALSE
USE_WAYLAND_DISPLAY   ?= FALSE
export PATH             := C:\raylib\mingw\bin;$(PATH)

# Determine PLATFORM_OS in case PLATFORM_DESKTOP selected
ifeq ($(PLATFORM),PLATFORM_DESKTOP)
    ifeq ($(OS),Windows_NT)
        PLATFORM_OS=WINDOWS
    else
        UNAMEOS=$(shell uname)
        ifeq ($(UNAMEOS),Linux)
            PLATFORM_OS=LINUX
        endif
        ifeq ($(UNAMEOS),Darwin)
            PLATFORM_OS=OSX
        endif
    endif
endif

ifeq ($(PLATFORM),PLATFORM_DESKTOP)
    ifeq ($(PLATFORM_OS),LINUX)
        RAYLIB_PATH    = /home/thelu/raylib/raylib
		HAMLIB_PATH	= /home/thelu/projects/hamlib/hamlib
    endif
    ifeq ($(PLATFORM_OS),WINDOWS)
		RAYLIB_PATH 	= C:\raylib\raylib
    endif
    ifeq ($(PLATFORM_OS),OSX)
    	RAYLIB_PATH = /usr/local/raylib
		HAMLIB_PATH = /Users/hamel111/Documents/GitHub/hamlib/hamlib
    endif
endif

ifeq ($(PLATFORM),PLATFORM_WEB)
    # Emscripten required variables
    EMSDK_PATH         ?= C:/emsdk
    EMSCRIPTEN_PATH    ?= $(EMSDK_PATH)/upstream/emscripten
    CLANG_PATH          = $(EMSDK_PATH)/upstream/bin
    PYTHON_PATH         = $(EMSDK_PATH)/python/3.9.2-1_64bit
    NODE_PATH           = $(EMSDK_PATH)/node/14.15.5_64bit/bin
    export PATH         = $(EMSDK_PATH);$(EMSCRIPTEN_PATH);$(CLANG_PATH);$(NODE_PATH);$(PYTHON_PATH);C:\raylib\MinGW\bin:$$(PATH)
endif

RAYLIB_RELEASE_PATH 	?= $(RAYLIB_PATH)/src
EXAMPLE_RUNTIME_PATH   ?= $(RAYLIB_RELEASE_PATH)

CC = gcc

ifeq ($(PLATFORM),PLATFORM_WEB)
    CC = emcc
endif

MAKE = make

ifeq ($(PLATFORM),PLATFORM_DESKTOP)
    ifeq ($(PLATFORM_OS),WINDOWS)
        MAKE = mingw32-make
    endif
endif

# Define compiler flags:
CFLAGS += -Wall -std=c99 -D_DEFAULT_SOURCE -Wno-missing-braces -Wunused-result
CFLAGS += -Wextra -Wmissing-prototypes -Wno-varargs
ifeq ($(BUILD_MODE), DEBUG)
	CFLAGS += -g
endif
# -Wstrict-prototypes is the worst

ifeq ($(PLATFORM),PLATFORM_DESKTOP)
    ifeq ($(PLATFORM_OS),LINUX)
        ifeq ($(RAYLIB_LIBTYPE),STATIC)
            CFLAGS += -D_DEFAULT_SOURCE
        endif
        ifeq ($(RAYLIB_LIBTYPE),SHARED)
            # Explicitly enable runtime link to libraylib.so
            CFLAGS += -Wl,-rpath,$(EXAMPLE_RUNTIME_PATH)
        endif
    endif
endif
ifeq ($(PLATFORM),PLATFORM_WEB)
    CFLAGS += -s USE_GLFW=3 -s ASYNCIFY -s TOTAL_MEMORY=67108864 -s FORCE_FILESYSTEM=1 --preload-file $(dir $<)resources@resources
    CFLAGS += --shell-file $(RAYLIB_PATH)/src/shell.html
    EXT = .html
endif

INCLUDE_PATHS = -I. -I$(RAYLIB_PATH)/src -I$(RAYLIB_PATH)/src/external -I$(RAYLIB_PATH)/src/extras

ifeq ($(PLATFORM),PLATFORM_DESKTOP)
    ifeq ($(PLATFORM_OS),LINUX)
        INCLUDE_PATHS += -I$(RAYLIB_H_INSTALL_PATH)
    endif
endif

INCLUDE_PATHS += -I$(HAMLIB_PATH)/include

# Define library paths containing required libs.
LDFLAGS = -L. -L$(RAYLIB_RELEASE_PATH) -L$(RAYLIB_PATH)/src

ifeq ($(PLATFORM),PLATFORM_DESKTOP)
    ifeq ($(PLATFORM_OS),WINDOWS)
        LDFLAGS += $(RAYLIB_PATH)/src/raylib.rc.data
        ifeq ($(SHOW_CONSOLE), FALSE)
            LDFLAGS += -Wl,--subsystem,windows
        endif
    endif
    ifeq ($(PLATFORM_OS),LINUX)
        LDFLAGS = -L. -L$(RAYLIB_INSTALL_PATH) -L$(RAYLIB_RELEASE_PATH) -L$(RAYLIB_PATH)
    endif
endif

# if you want to link libraries (libname.so or libname.a), use the -lname
ifeq ($(PLATFORM),PLATFORM_DESKTOP)
    ifeq ($(PLATFORM_OS),WINDOWS)
        LDLIBS = -lraylib -lopengl32 -lgdi32 -lwinmm -static -lpthread
    endif
    ifeq ($(PLATFORM_OS),LINUX)
        LDLIBS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
        # NOTE: It seems additional libraries are not required any more, latest GLFW just dlopen them
        #LDLIBS += -lXrandr -lXinerama -lXi -lXxf86vm -lXcursor
        ifeq ($(RAYLIB_LIBTYPE),SHARED)
            LDLIBS += -lc
        endif
    endif
    ifeq ($(PLATFORM_OS),OSX)
        # NOTE: Required packages: libopenal-dev libegl1-mesa-dev
        LDLIBS = -lraylib -framework OpenGL -framework Cocoa -framework IOKit -framework CoreAudio -framework CoreVideo
    endif
    ifeq ($(USE_EXTERNAL_GLFW),TRUE)
        # NOTE: It could require additional packages installed: libglfw3-dev
        LDLIBS += -lglfw
    endif
endif
ifeq ($(PLATFORM),PLATFORM_WEB)
    # Libraries for web (HTML5) compiling
    LDLIBS = $(RAYLIB_RELEASE_PATH)/libraylib.a
endif

# Define all object files required
# OBJS = $(addsuffix /%.o, $(SUBDIRS))
OBJS = $(patsubst %.c, %.o, $(SOURCE_FILES))
MAKEFILE_PARAMS = hamlib.a

# Default target entry
all: 
	$(MAKE) $(MAKEFILE_PARAMS)

hamlib.a: $(OBJS)
	ar -cvq libhamlib.a $(OBJS)

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS) $(INCLUDE_PATHS) -D$(PLATFORM) $(EXTRAPARAMS)

# Clean everything
clean:
ifeq ($(PLATFORM),PLATFORM_DESKTOP)
    ifeq ($(PLATFORM_OS),WINDOWS)
		rm $(OBJS) libhamlib.a
    endif
    ifeq ($(PLATFORM_OS),LINUX)
		find . -type f -executable -delete
		rm -fv $(OBJS)
    endif
    ifeq ($(PLATFORM_OS),OSX)
		find . -type f -perm +ugo+x -delete
		rm -f $(OBJS) libhamlib.a
    endif
endif
ifeq ($(PLATFORM),PLATFORM_WEB)
	del *.o *.html *.js
endif
	@echo Cleaning done

