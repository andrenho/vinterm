# Vintage Terminal
# See LICENSE file for copyright and license details.

#
# Please make your configuration changes in the `config.mk` file.
# 

include config.mk
include libraries.mk

SRC = main.cc			\
      options.cc		\
      terminal/blink.cc		\
      terminal/backtrack.cc     \
      terminal/clipboard.cc	\
      terminal/charmatrix.cc	\
      terminal/keyqueue.cc      \
      terminal/mouse.cc         \
      terminal/pty.cc		\
      terminal/terminal.cc	\
      terminal/vinterm.cc	\
      graphic/audio.cc		\
      graphic/font.cc		\
      graphic/fontbmp.cc	\
      graphic/framebuffer.cc    \
      graphic/screen.cc         \
      render/renderer.cc	\
      render/simple.cc
#      render/monochrome.cc      \

OBJ = ${SRC:.cc=.o}
HEADERS = ${SRC:.cc=.h} global.h filters/filter.h terminal/charattr.h \
	render/renderer.h
DIST = AUTHORS HACKING LICENSE NEWS README INSTALL
ICONS = icon/icon_16.png icon/icon_32.png icon/icon_64.png \
	icon/icon_128.png icon/icon.svg icon/icon_22.png icon/icon_48.png
DATA = data/vinterm_profile data/vinterm.info data/850.bmp data/vinterm.desktop \
       data/icon_16.bmp

all: options vinterm

options:
	@echo
	@echo Vintage Terminal build options:
	@echo "CXXFLAGS = ${CXXFLAGS}"
	@echo "LDFLAGS  = ${LDFLAGS}"
	@echo "CC       = ${CC}"

.cc.o:
	@echo CC $<
	@${CC} -c ${CXXFLAGS} -o $@ $<

${OBJ}: config.mk libraries.mk

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
	@cp --parents config.mk libraries.mk vinterm.1 Makefile ${DIST} ${SRC} ${HEADERS} ${DATA} ${ICONS} vinterm-${VERSION}
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
	@echo installing manual page to ${DESTDIR}${MANPREFIX}/man1
	@mkdir -p ${DESTDIR}${MANPREFIX}/man1
	@sed "s/VERSION/${VERSION}/g" < vinterm.1 > ${DESTDIR}${MANPREFIX}/man1/vinterm.1
	@chmod 644 ${DESTDIR}${MANPREFIX}/man1/vinterm.1
	@echo compiling terminfo data
	@tic -s data/vinterm.info
	@echo installing icons
	@cp -f icon/icon_16.png  /usr/share/icons/hicolor/16x16/apps/vinterm.png
	@cp -f icon/icon_22.png  /usr/share/icons/hicolor/22x22/apps/vinterm.png
	@cp -f icon/icon_32.png  /usr/share/icons/hicolor/32x32/apps/vinterm.png
	@cp -f icon/icon_48.png  /usr/share/icons/hicolor/48x48/apps/vinterm.png
	@cp -f icon/icon_64.png  /usr/share/icons/hicolor/64x64/apps/vinterm.png
	@cp -f icon/icon_128.png /usr/share/icons/hicolor/128x128/apps/vinterm.png
	@cp -f icon/icon.svg     /usr/share/icons/hicolor/scalable/apps/vinterm.svg
	@cp -f data/vinterm.desktop /usr/share/applications

# TODO - check data
uninstall:
	@echo removing executable file from ${DESTDIR}${PREFIX}/bin
	@rm -f ${DESTDIR}${PREFIX}/bin/st
	@echo removing data from ${DESTDIR}${VINTERMPREFIX}
	@rm -f ${DESTDIR}${VINTERMPREFIX}/*
	@rmdir ${DESTDIR}${VINTERMPREFIX}
	@echo removing manual page from ${DESTDIR}${MANPREFIX}/man1
	@rm -f ${DESTDIR}${MANPREFIX}/man1/vinterm.1
	@echo removing icons
	@rm -f /usr/share/icons/hicolor/16x16/apps/vinterm.png
	@rm -f /usr/share/icons/hicolor/22x22/apps/vinterm.png
	@rm -f /usr/share/icons/hicolor/32x32/apps/vinterm.png
	@rm -f /usr/share/icons/hicolor/48x48/apps/vinterm.png
	@rm -f /usr/share/icons/hicolor/64x64/apps/vinterm.png
	@rm -f /usr/share/icons/hicolor/128x128/apps/vinterm.png
	@rm -f /usr/share/icons/hicolor/scalable/apps/vinterm.svg
	@rm -f /usr/share/applications/vinterm.desktop

.PHONY: all options clean dist install uninstall
