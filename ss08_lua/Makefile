CC = gcc
CFLAGS = -llua -lglut

ss08: ss08.c ss08_render.c ss08.h
	$(CC) -lglut -llua -o ss08 ss08_render.c ss08.c

clean:
	rm ss08
	rm ss08_render
	rm *~ *.o