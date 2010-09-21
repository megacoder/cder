# vim: ts=8 sw=8

PREFIX	=${HOME}/opt
BINDIR	=${PREFIX}/bin

CC	=ccache gcc -march=i686
OPT	=-Os
CFLAGS	=${OPT} -Wall -Werror -pedantic -pthread -g
LDFLAGS	=-pthread -g
LDLIBS	=

all::	cder

check:: cder
	./cder

clean::

distclean clobber:: clean

install:: cder
	install -d ${BINDIR}
	install -c cder ${BINDIR}/cder

uninstall::
	${RM} ${BINDIR}/cder
