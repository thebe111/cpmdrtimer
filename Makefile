CC = gcc
CFLAGS = -Wall -Werror -Wextra

cpmdrtimer: main.c
	${CC} ${CFLAGS} $< -o $@
