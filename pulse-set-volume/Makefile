CFLAGS=$(shell pkg-config --cflags libpulse glib-2.0)
LIBS=$(shell pkg-config --libs libpulse glib-2.0)

all:
	gcc -Wall ${CFLAGS} set-volume.c ${LIBS} -o set-volume
