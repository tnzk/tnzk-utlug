CC = gcc
CFLAGS = -Wall -llua -lglut

ss08: ss08_lua.o ss08_render.o
	$(CC) $(CFLAGS) -o ss08 ss08_render.o ss08_lua.o

ss08_lua.o: ss08_lua.c ss08.h
	$(CC) -c -Wall ss08_lua.c

ss08_render.o: ss08_render.c ss08.h
	$(CC) -c -Wall ss08_render.c

sample: sample.c ss08_lua.o
	$(CC) $(CFLAGS) -o sample sample.c ss08_lua.o

.PHONY: clean
clean:
	rm -f ss08 ss08_lua.o ss08_render.o sample