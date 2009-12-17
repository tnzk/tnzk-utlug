CC = gcc
CFLAGS = -llua -lglut

ss08: ss08_lua.o ss08_render.o
	$(CC) $(CFLAGS) -o ss08 ss08_render.o ss08_lua.o

ss08_lua.o: ss08_lua.c
	$(CC) -c ss08_lua.c

ss08_render.o: ss08_render.c
	$(CC) -c ss08_render.c

.PHONY: clean
clean:
	rm -f ss08 ss08_lua.o ss08_render.o