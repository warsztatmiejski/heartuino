#!/usr/bin/env python3
import re

RE_NOTE = re.compile("([A-G]#?)([0-8]?)")
BASE_FREQ = 128_000 / 8
MAX_TICKS = 255
NOTES = "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"
A4 = 440


def main(line: str, default_octave: int = 4) -> str:
    for match in RE_NOTE.finditer(line):
        note, raw_octave = match.groups()
        freq = note_to_freq(note, int(raw_octave or default_octave))
        yield freq_to_ticks(freq)


def note_to_freq(note: str, octave: int) -> float:
    n = NOTES.index(note) + 12 * (octave - 1) + 4
    return A4 * 2 ** ((n - 49) / 12)


def freq_to_ticks(freq: float) -> int:
    ticks = BASE_FREQ / freq - 1
    assert ticks <= MAX_TICKS
    return round(ticks)


if __name__ == "__main__":
    from sys import argv, stderr
    if len(argv) <= 1:
        print("Not argument provided!", file=stderr)
        exit(1)
    result = list(main(argv[1]))
    pretty_result = ", ".join(f"0x{x:02x}" for x in result)
    print("{", pretty_result, "}", sep='')
    exit(0)
