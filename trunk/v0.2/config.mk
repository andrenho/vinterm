# vinterm version
VERSION = 0.1.3

# system libraries
SDL = no
CURSES = yes
X11 = no

# add debugging info
DEBUG = yes

# Customize below to fit your system

# paths
PREFIX = /usr/local
MANPREFIX = ${PREFIX}/share/man

# basic flags
CXXFLAGS = -std=c++0x -DVERSION=\"${VERSION}\" -pedantic -Wall -I. -I/usr/include
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

# curses
ifeq (${CURSES},yes)
  CXXFLAGS += -D_CURSES
  LDFLAGS += -lncurses
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
