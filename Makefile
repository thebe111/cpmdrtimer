CC = gcc
CFLAGS = -Wall -Werror -Wextra
PKGCFG = `pkg-config --cflags --libs libnotify`

cpmdrtimer: main.c
	${CC} ${CFLAGS} $< -o $@ ${PKGCFG}
