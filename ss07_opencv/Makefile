CC = g++
CFLAGS = -Wall -O2 `pkg-config --cflags opencv` `pkg-config --libs opencv`

all: startup binarize pickbycolor kernel kernel_blur

startup: startup.c
	$(CC) $(CFLAGS) -o startup startup.c
binarize: binarize.c
	$(CC) $(CFLAGS) -o binarize binarize.c
pickbycolor: pickbycolor.c
	$(CC) $(CFLAGS) -o pickbycolor pickbycolor.c
kernel: kernel.c
	$(CC) $(CFLAGS) -o kernel kernel.c
kernel_blur: kernel_blur.c
	$(CC) $(CFLAGS) -o kernel_blur kernel_blur.c

clean:
	rm -rf startup binarize pickbycolor kernel kernel_blur *.o *~