# --
# Copyright (c) 2018, Lukasz Marcin Podkalicki <lpodkalicki@gmail.com>
# https://github.com/lpodkalicki/attiny-onewire-library/blob/master/examples/attiny13/Makefile
# --

STEP=250
NAME=sos
MUSIC=afbccdeaz

MCU=attiny13
FUSE_L=0x3B
FUSE_H=0xFF
F_CPU=128000
CC=avr-gcc
LD=avr-ld
OBJCOPY=avr-objcopy
SIZE=avr-size
AVRDUDE=avrdude
CFLAGS =-std=c99 -Wall -g -Os -Iinclude
CFLAGS+=-flto -fno-reorder-blocks
CFLAGS+=-mmcu=${MCU} -DF_CPU=${F_CPU}
CFLAGS+=-DNAME=${NAME} -DSTEP=${STEP} -DMUSIC=${MUSIC}
TARGET=main

SRCS = src/main.c src/morse.c

all:
	${CC} ${CFLAGS} -o ${TARGET}.o ${SRCS}
	${LD} -o ${TARGET}.elf ${TARGET}.o
	${OBJCOPY} -j .text -j .data -O ihex ${TARGET}.o ${TARGET}.hex
	${SIZE} -C --mcu=${MCU} ${TARGET}.elf

flash:
	${AVRDUDE} -p ${MCU} -c usbasp -U flash:w:${TARGET}.hex:i -F -P usb

fuse:
	$(AVRDUDE) -p ${MCU} -c usbasp -U hfuse:w:${FUSE_H}:m -U lfuse:w:${FUSE_L}:m

clean:
	rm -f *.c~ *.o *.elf *.hex
