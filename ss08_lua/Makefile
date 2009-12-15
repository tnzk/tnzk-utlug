CC = gcc
CFLAGS = -llua -lglut

ss08: ss08.c ss08_render.c
	$(CC) -llua -o ss08 ss08.c
	$(CC) -lglut -o ss08_render ss08_render.c

clean:
	rm ss08
	rm ss08_render
	rm *~ *.o