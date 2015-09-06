# luamosaic's makefile

src = luamosaic.c
SHARED_OBJ = mosaic.so
PIC_OBJ = mosaic.os

# check out if the lua package is named 'lua5.2' or just 'lua', for i've seen both in my life
lualib = $(shell if pkg-config --list-all | grep lua5.2; then echo "lua5.2"; else echo "lua"; fi)

CC = gcc
LINKS = $(shell pkg-config --cflags --libs $(lualib) mosaic mosaic_stream_io mosaic_color)
CFLAGS = -Wall -g -O2 $(LINKS)

PIC_CFLAGS = $(CFLAGS) -fPIC
SHARED_CFLAGS = $(CFLAGS) -shared

all : PIC
	$(CC) -o $(SHARED_OBJ) $(SHARED_CFLAGS) $(PIC_OBJ)

PIC : $(src)
	$(CC) -o $(PIC_OBJ) -c $(PIC_CFLAGS) $<


run :
	@lua teste.lua

clean :
	$(RM) *~ $(SHARED_OBJ) $(PIC_OBJ)
