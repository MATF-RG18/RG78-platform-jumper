PROGRAM = 1
CC      = gcc
CFLAGS  = -g -Wall -I/usr/X11R6/include -I/usr/pkg/include
LDFLAGS = -L/usr/X11R6/lib -L/usr/pkg/lib
LDLIBS  = -lglut -lGLU -lGL

$(PROGRAM): main.o image.o helper.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(PROGRAM) image.o main.o helper.o $(LDLIBS)
	rm main.o image.o helper.o

main.o: main.c
	$(CC) $(CFLAGS) main.c -c $(LDLIBS)
	
image.o: image.c
	$(CC) $(CFLAGS) image.c -c $(LDLIBS)

helper.o: helper.c
	$(CC) $(CFLAGS) helper.c -c $(LDLIBS)
