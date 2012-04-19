# vinterm version
VERSION = 0.3.1

# system libraries
SDL = yes
X11 = no

# add debugging info
DEBUG = yes
DUMA = no

# Customize below to fit your system

# paths
PREFIX = /usr/local
MANPREFIX = ${PREFIX}/share/man
VINTERMPREFIX = ${PREFIX}/share/vinterm

# basic flags
CXXFLAGS = -DVERSION=\"${VERSION}\" -DDATADIR=\"${VINTERMPREFIX}\" -pedantic -Wall -I. -I/usr/include -std=c++0x
LDFLAGS = -L/usr/lib -lutil

# SDL libraries
ifeq (${SDL},yes)
  CXXFLAGS += `sdl-config --cflags` -D_SDL
  LDFLAGS += `sdl-config --libs`
endif

# X11 libraries
ifeq (${X11},yes)
  CXXFLAGS += -I/usr/X11R6/include -D_X11
  LDFLAGS += -L/usr/X11R6/lib -lX11 -lutil
endif

# Duma libraries
ifeq (${DUMA}, yes)
  LDFLAGS += -lduma
endif

ifeq (${DEBUG},yes)
  # debug flags
  CXXFLAGS += -g -O0 -DDEBUG
  LDFLAGS += -g
else
  # production flags
  CXXFLAGS += -Os
  LDFLAGS += -s
endif

# compiler and linker
CC = g++
