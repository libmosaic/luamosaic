# luamosaic's makefile

src = luamosaic.c
SHARED_OBJ = mosaic.so
PIC_OBJ = mosaic.os

CC = gcc
LINKS = $(shell pkg-config --cflags lua5.2 mosaic mosaic_stream_io)
CFLAGS = -Wall -g -O2 $(LINKS)

PIC_CFLAGS = $(CFLAGS) -fPIC
SHARED_CFLAGS = $(CFLAGS) -shared

all : PIC
	$(CC) -o $(SHARED_OBJ) $(SHARED_CFLAGS) $(PIC_OBJ)

PIC : $(src)
	$(CC) -o $(PIC_OBJ) -c $(PIC_CFLAGS) $<

clean :
	$(RM) *~ $(SHARED_OBJECT) $(PIC_OBJ)
