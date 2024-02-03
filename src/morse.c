#include "morse.h"
// #include <avr/pgmspace.h>
// const uint8_t NEXT_CHAR[26] PROGMEM = { 0 };
// const uint8_t NEXT_SIGNAL[26] PROGMEM = { 0 };

bool decode_morse_char(char c, uint8_t * morse_signals) {
  uint8_t index;
  uint8_t reverse_index;
  uint8_t reversed_signals[8];

  if (c == '\0' || c == ' ') {
    morse_signals[0] = MEDIUM_GAP;
    morse_signals[1] = 0;
    return true;
  }

  for(index = 0; index < 8;) {
    uint8_t signal;
    switch (c) {
      case 't': c = EOT; signal = DASH; break;
      case 'e': c = EOT; signal = DOT; break;
      case 'm': c = 't'; signal = DASH; break;
      case 'n': c = 't'; signal = DOT; break;
      case 'a': c = 'e'; signal = DASH; break;
      case 'i': c = 'e'; signal = DOT; break;
      case 'o': c = 'm'; signal = DASH; break;
      case 'g': c = 'm'; signal = DOT; break;
      case 'k': c = 'n'; signal = DASH; break;
      case 'd': c = 'n'; signal = DOT; break;
      case 'w': c = 'a'; signal = DASH; break;
      case 'r': c = 'a'; signal = DOT; break;
      case 'u': c = 'i'; signal = DASH; break;
      case 's': c = 'i'; signal = DOT; break;
      case 'q': c = 'g'; signal = DASH; break;
      case 'z': c = 'g'; signal = DOT; break;
      case 'y': c = 'k'; signal = DASH; break;
      case 'c': c = 'k'; signal = DOT; break;
      case 'x': c = 'd'; signal = DASH; break;
      case 'b': c = 'd'; signal = DOT; break;
      case 'j': c = 'w'; signal = DASH; break;
      case 'p': c = 'w'; signal = DOT; break;
      case 'l': c = 'r'; signal = DOT; break;
      case 'f': c = 'u'; signal = DOT; break;
      case 'v': c = 's'; signal = DASH; break;
      case 'h': c = 's'; signal = DOT; break;
      default: return false;
    }
    if (c == EOT) {
      reversed_signals[index++] = signal;
      break;
    } else {
      reversed_signals[index++] = GAP;
      reversed_signals[index++] = signal;
    }
  }
  for(reverse_index = 0; reverse_index <= index; reverse_index++) {
    morse_signals[reverse_index] = reversed_signals[index - reverse_index];
  }
  morse_signals[reverse_index++] = SHORT_GAP;
  morse_signals[reverse_index++] = 0;
  return true;
}
