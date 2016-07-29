CFLAGS=$(shell pkg-config --cflags gtk+-2.0)
LIBS=$(shell pkg-config --libs gtk+-2.0)

all:
	gcc -Wall ${CFLAGS} gtk-test.c ${LIBS} -o gtk-test
