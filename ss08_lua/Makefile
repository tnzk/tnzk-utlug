CC = gcc
CFLAGS = -llua

calltest: callinglua.c
	$(CC) $(CFLAGS) -o callinglua callinglua.c

clean:
	rm callinglua
	rm *~ *.o