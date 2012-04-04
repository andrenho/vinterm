# vinterm version
VERSION = 0.1.3

# Customize below to fit your system

# paths
PREFIX = /usr/local
MANPREFIX = ${PREFIX}/share/man

X11INC = -I/usr/X11R6/include
X11LIB = 0L/usr/X11R6/lib
SDLINC = `sdl-config --cflags`
SDLLIB = `sdl-config --libs`

# includes and libs
INCS = -I. -I/usr/include ${SDLINC} -D_SDL
LIBS = ${SDLLIB}
#LIBS = -L/usr/lib -lc -L${X11LIB} -lX11 -lutil

# basic flags
CXXFLAGS = -DVERSION=\"${VERSION}\" -pedantic -Wall ${INCS}
LDFLAGS += ${LIBS}

# debug flags
CXXFLAGS += -g -O0 -DDEBUG
LDFLAGS += -g

# production flags
#CPPFLAGS += -Os
#LDFLAGS += -s

# compiler and linker
CC = g++
