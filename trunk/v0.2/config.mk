# vinterm version
VERSION = 0.1.3

# Customize below to fit your system

# paths
PREFIX = /usr/local
MANPREFIX = ${PREFIX}/share/man

X11INC = /usr/X11R6/include
X11LIB = /usr/X11R6/lib
SDLINC = `sdl-config --cflags`
SDLLIB = `sdl-config --libs`

# includes and libs
INCS = -Isrc -I/usr/include -I${SDLINC}
LIBS = ${SDLLIB}
#LIBS = -L/usr/lib -lc -L${X11LIB} -lX11 -lutil

# basic flags
CPPFLAGS = -DVERSION=\"${VERSION}\" -pedantic -Wall
LDFLAGS += ${LIBS}

# debug flags
CPPFLAGS += -g -O0 -DDEBUG
LDFLAGS += -g

# production flags
#CPPFLAGS += -Os
#LDFLAGS += -s

# compiler and linker
CC ?= gcc
