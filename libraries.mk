# system libraries
SDL2   = $(shell sdl2-config --cflags > /dev/null 2> /dev/null; echo $$?)
AO     = $(shell pkg-config --cflags ao > /dev/null 2> /dev/null; echo $$?)
CONFIG = $(shell pkg-config --cflags libconfig++ > /dev/null 2> /dev/null; echo $$?)

# SDL libraries
ifeq (${SDL2},0)
  CXXFLAGS += `sdl2-config --cflags` -D_SDL2
  LDFLAGS += `sdl2-config --libs`
else
  $(error The SDL2 library was not detected in the system. Please install it and try again)
endif

# AO library
ifeq (${AO},0)
  CXXFLAGS += `pkg-config --cflags ao` -DAO
  LDFLAGS += `pkg-config --libs ao`
else
  $(warning IMPORTANT: The AO library was not detected in the system. The installation will continue but the audible beep in the terminal will not be avaliable)
endif

# libconfig++ library
ifeq (${CONFIG},0)
  CXXFLAGS += `pkg-config --cflags libconfig++` -DLIBCONFIG
  LDFLAGS += `pkg-config --libs libconfig++`
else
  $(warning IMPORTANT: The libconfig++ library was not detected in the system. The installation will continue but the audible beep in the terminal will not be avaliable)
endif

# X11 libraries
LDFLAGS += -lX11
