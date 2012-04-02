bin_PROGRAMS = vinterm
vinterm_SOURCES = main.cc					\
		  chars.h		chars.cc		\
		  console.h		console.cc		\
		  font.h		font.cc			\
		  options.h		options.cc		\
		  screen.h		screen.cc		\
		  terminal.h		terminal.cc		\
		  vt100.h               vt100.cc                \
		  						\
		  filter.h					\
		  filter_bright.h	filter_bright.cc	\
		  filter_inexact.h	filter_inexact.cc	\
		  filter_scanline.h	filter_scanline.cc
vinterm_LDADD = -lutil

AM_CPPFLAGS += -DDATADIR=\"$(datadir)/@PACKAGE@\"
