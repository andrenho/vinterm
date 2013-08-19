# system libraries
SDL    = $(shell sdl-config --cflags > /dev/null 2> /dev/null; echo $$?)
AO     = $(shell pkg-config --cflags ao > /dev/null 2> /dev/null; echo $$?)
CONFIG = $(shell pkg-config --cflags libconfig++ > /dev/null 2> /dev/null; echo $$?)

# SDL libraries
ifeq (${SDL},0)
  CXXFLAGS += `sdl-config --cflags` -D_SDL
  LDFLAGS += `sdl-config --libs`
else
  $(error The SDL library was not detected in the system. Please install it and try again)
endif

# AO library
ifeq (${AO},0)
  CXXFLAGS += `pkg-config --cflags ao` -DAO
  LDFLAGS += `pkg-config --libs ao`
else
  $(warning IMPORTANT: The AO library was not detected in the system. The installation will continue but the audible beep in the terminal will not be avaliable)
endif

# X11 libraries
LDFLAGS += -lX11

# libconfig++ library
ifeq (${CONFIG},0)
  CXXFLAGS += `pkg-config --cflags libconfig++` -DLIBCONFIG
  LDFLAGS += `pkg-config --libs libconfig++`
else
  $(warning IMPORTANT: The libconfig++ library was not detected in the system. The installation will continue but the audible beep in the terminal will not be avaliable)
endif
