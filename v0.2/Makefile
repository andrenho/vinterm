# Vintage Terminal
# See LICENSE file for copyright and license details.

include config.mk

SRC = main.cc			\
      options.cc		\
      terminal/blink.cc		\
      terminal/framebuffer.cc	\
      terminal/pty.cc		\
      terminal/terminal.cc	\
      terminal/vinterm.cc	\
      graphic/chars.cc		\
      graphic/font.cc		\
      graphic/input.cc		\
      graphic/screen.cc		\
      filters/filter_bright.cc  \
      filters/filter_inexact.cc \
      filters/filter_scanline.cc
OBJ = ${SRC:.cc=.o}
DIST = AUTHORS HACKING LICENSE NEWS README INSTALL
DATA = data/vinterm_profile data/vinterm.info data/mda9.bmp

all: options vinterm

options:
	@echo Vintage Terminal build options:
	@echo "CXXFLAGS = ${CXXFLAGS}"
	@echo "LDFLAGS  = ${LDFLAGS}"
	@echo "CC       = ${CC}"

.cc.o:
	@echo CC $<
	@${CC} -c ${CXXFLAGS} -o $@ $<

${OBJ}: config.mk

vinterm: depend ${OBJ} data/vinterm.info
	@echo CC -o $@
	@${CC} -o $@ ${OBJ} ${LDFLAGS}
	@tic -s data/vinterm.info
-include depend

clean:
	@echo cleaning
	@rm -f vinterm ${OBJ} vinterm-${VERSION}.tar.gz depend

dist: clean
	@echo creating dist tarball
	@mkdir -p vinterm-${VERSION}
	@cp --parents config.mk Makefile ${DIST} ${SRC} ${DATA} vinterm-${VERSION}
	@tar -cf vinterm-${VERSION}.tar vinterm-${VERSION}
	@gzip vinterm-${VERSION}.tar
	@rm -rf vinterm-${VERSION}

depend: ${SRC}
	@echo checking dependencies
	@${CC} -MM ${CXXFLAGS} ${SRC} >depend

install: all
	@echo installing executable file to ${DESTDIR}${PREFIX}/bin
	@mkdir -p ${DESTDIR}${PREFIX}/bin
	@cp -f vinterm ${DESTDIR}${PREFIX}/bin
	@chmod 755 ${DESTDIR}${PREFIX}/bin/vinterm
	@echo installing data to ${DESTDIR}${VINTERMPREFIX}
	@mkdir -p ${DESTDIR}${VINTERMPREFIX}
	@cp -f ${DATA} ${DESTDIR}${VINTERMPREFIX}
#	@echo installing manual page to ${DESTDIR}${MANPREFIX}/man1
#	@mkdir -p ${DESTDIR}${MANPREFIX}/man1
#	@sed "s/VERSION/${VERSION}/g" < st.1 > ${DESTDIR}${MANPREFIX}/man1/st.1
#	@chmod 644 ${DESTDIR}${MANPREFIX}/man1/st.1
	@tic -s data/vinterm.info

# TODO - check data
uninstall:
	@echo removing executable file from ${DESTDIR}${PREFIX}/bin
	@rm -f ${DESTDIR}${PREFIX}/bin/st
	@echo removing data from ${DESTDIR}${VINTERMPREFIX}
	@rm -f ${DESTDIR}${VINTERMPREFIX}/*
	@rmdir ${DESTDIR}${VINTERMPREFIX}
#	@echo removing manual page from ${DESTDIR}${MANPREFIX}/man1
#	@rm -f ${DESTDIR}${MANPREFIX}/man1/st.1

.PHONY: all options clean dist install uninstall
