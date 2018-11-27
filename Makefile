PROGRAM = 1
CC      = gcc
CFLAGS  = -g -Wall -I/usr/X11R6/include -I/usr/pkg/include
LDFLAGS = -L/usr/X11R6/lib -L/usr/pkg/lib
LDLIBS  = -lglut -lGLU -lGL

$(PROGRAM): main.o 
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(PROGRAM) main.o $(LDLIBS)
	rm main.o

main.o: main.c
	$(CC) $(CFLAGS) main.c -c $(LDLIBS)