# --
# Copyright (c) 2018, Lukasz Marcin Podkalicki <lpodkalicki@gmail.com>
# https://github.com/lpodkalicki/attiny-onewire-library/blob/master/examples/attiny13/Makefile
# --

STEP=250
NAME=sos
MUSIC=G4 G4 G4 A4 A4 A4 B4 G3 G3 G3 A3 A3 A3 B3

MCU=attiny13
FUSE_L=0x3B
FUSE_H=0xFF
F_CPU=128000
CC=avr-gcc
LD=avr-ld
OBJCOPY=avr-objcopy
SIZE=avr-size
AVRDUDE=avrdude
MAIN_CFLAGS=-std=c99 -Wall -g -Os -Iinclude -flto -fno-reorder-blocks
CFLAGS =$(MAIN_CFLAGS)
CFLAGS+=-mmcu=${MCU} -DF_CPU=${F_CPU}
CFLAGS+=-DNAME=${NAME} -DSTEP=${STEP} -DMUSIC="${shell ./to_note.py "${MUSIC}"}"
CFLAGS_TESTS=$(MAIN_CFLAGS) ${shell pkg-config --cflags --libs check}
TARGET=main

SRCS = src/main.c src/morse.c
SRC_TESTS = src/morse.c test/morse.c

.PHONY: all test flash fuse clean

all:
	${CC} ${CFLAGS} -o ${TARGET}.o ${SRCS}
	${LD} -o ${TARGET}.elf ${TARGET}.o
	${OBJCOPY} -j .text -j .data -O ihex ${TARGET}.o ${TARGET}.hex
	${SIZE} -C --mcu=${MCU} ${TARGET}.elf

test:
	gcc -o test.o ${SRC_TESTS} ${CFLAGS_TESTS}
	./test.o

flash:
	${AVRDUDE} -p ${MCU} -c usbasp -U flash:w:${TARGET}.hex:i -F -P usb

fuse:
	$(AVRDUDE) -p ${MCU} -c usbasp -U hfuse:w:${FUSE_H}:m -U lfuse:w:${FUSE_L}:m

clean:
	rm -f *.c~ *.o *.elf *.hex
