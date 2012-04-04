# Vintage Terminal
# See LICENSE file for copyright and license details.

include config.mk

SRC = main.cc
OBJ = ${SRC:.cc=.o}
DIST = AUTHORS HACKING LICENSE NEWS README

all: options vinterm

options:
	@echo Vintage Terminal build options:
	@echo "CPPFLAGS = ${CPPFLAGS}"
	@echo "LDFLAGS  = ${LDFLAGS}"
	@echo "CC       = ${CC}"

.cc.o:
	@echo CC $<
	@${CC} -c ${CFLAGS} $<

${OBJ}: config.mk

st: ${OBJ}
	@echo CC -o $@
	@${CC} -o $@ ${OBJ} ${LDFLAGS}

clean:
	@echo cleaning
	@rm -f vinterm ${OBJ} vinterm-${VERSION}.tar.gz

dist: clean
	@echo creating dist tarball
	@mkdir -p vinterm-${VERSION}
	@cp -R Makefile ${DIST} config.mk vinterm.info ${SRC} vinterm-${VERSION}
	@tar -cf vinterm-${VERSION}.tar vinterm-${VERSION}
	@gzip vinterm-${VERSION}.tar
	@rm -rf vinterm-${VERSION}

install: all
	@echo installing executable file to ${DESTDIR}${PREFIX}/bin
	@mkdir -p ${DESTDIR}${PREFIX}/bin
	@cp -f vinterm ${DESTDIR}${PREFIX}/bin
	@chmod 755 ${DESTDIR}${PREFIX}/bin/st
#	@echo installing manual page to ${DESTDIR}${MANPREFIX}/man1
#	@mkdir -p ${DESTDIR}${MANPREFIX}/man1
#	@sed "s/VERSION/${VERSION}/g" < st.1 > ${DESTDIR}${MANPREFIX}/man1/st.1
#	@chmod 644 ${DESTDIR}${MANPREFIX}/man1/st.1
	@tic -s st.info

uninstall:
	@echo removing executable file from ${DESTDIR}${PREFIX}/bin
	@rm -f ${DESTDIR}${PREFIX}/bin/st
#	@echo removing manual page from ${DESTDIR}${MANPREFIX}/man1
#	@rm -f ${DESTDIR}${MANPREFIX}/man1/st.1

.PHONY: all options clean dist install uninstall
