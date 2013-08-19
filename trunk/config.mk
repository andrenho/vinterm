#################################
#                               #
#  CONFIGURE YOUR CHOICES HERE  #
#                               #
#################################

# vinterm version
VERSION = 1.0.0

# add debugging info
DEBUG = yes
DUMA = no

# paths
PREFIX = /usr/local
MANPREFIX = ${PREFIX}/share/man
VINTERMPREFIX = ${PREFIX}/share/vinterm

# basic flags
CXXFLAGS = -DVERSION=\"${VERSION}\" -DDATADIR=\"${VINTERMPREFIX}\" -pedantic -Wall -I. -I/usr/include -std=c++0x
LDFLAGS = -L/usr/lib -lutil

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
