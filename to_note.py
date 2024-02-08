#!/usr/bin/env python3
import re

RE_NOTE = re.compile("([A-G]#?)([0-8]?)")
BASE_FREQ = 128_000 / 8
MAX_TICKS = 100
MAX_LETTERS = ord("z") - ord("a")
RATIO = MAX_LETTERS / MAX_TICKS
NOTES = "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"
A4 = 440


def main(line: str, default_octave: int = 4) -> str:
    data = ""
    for match in RE_NOTE.finditer(line):
        note, raw_octave = match.groups()
        freq = note_to_freq(note, int(raw_octave or default_octave))
        data += freq_to_char(freq)
    return data


def note_to_freq(note: str, octave: int) -> float:
    n = NOTES.index(note) + 12 * (octave - 1) + 4
    return A4 * 2 ** ((n - 49) / 12)


def freq_to_char(freq: float) -> str:
    ticks = BASE_FREQ / freq
    assert ticks < MAX_TICKS, f"{ticks} > {MAX_TICKS}"
    letter_index = round(ticks * RATIO)
    return chr(letter_index + ord("a"))


if __name__ == "__main__":
    from sys import argv, stderr

    if len(argv) <= 1:
        print("Not argument provided!", file=stderr)
        exit(1)
    result = main(argv[1])
    print(result)
    exit(0)
