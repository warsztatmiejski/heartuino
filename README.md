# What

- avr-gcc
- avrdude
- usbasp programator

# How

- First time: `make fuse`
- Compile & deploy `make STEP=200 NAME=marek "MUSIC=$(cat notes/FILE.txt)" && make flash`

# Makefile parameters

- `STEP` in ms
- `MUSIC` - notes in format `A`, `A#`, `A4`

